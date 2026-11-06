"""Reject full-main ownership conflicts without changing genuine prior owners.

Each case uses a fresh process, actual native constructors and isolated VMU
images. No game, SDK, filesystem, fiber or System constructor is substituted.
"""
import argparse
import ctypes as C
import faulthandler
import hashlib
import json
import subprocess
import sys
from pathlib import Path

from native_packet_test_support import bind
from native_present_test_support import Info as PresentInfo
from native_texture_test_support import Info as TextureInfo
from verify_native_aica import Options as AicaOptions
from verify_native_card_lifecycle import Card
from verify_native_device_init import Options as DeviceOptions, Info as DeviceInfo
from verify_native_exit_logo import ExitInfo
from verify_native_frame_wait import Snapshot as WaitInfo
from verify_native_game_sound import Options as SoundOptions, Info as SoundInfo
from verify_native_gdfs import Info as GDFSInfo
from verify_native_input_backend import Info as InputInfo, Source, Sample
from verify_native_main_boot import BootInfo
from verify_native_rtc import Info as RTCInfo
from verify_native_sdk_heap import Info as HeapInfo
from verify_native_sound_clock import Options as ClockOptions, Info as ClockInfo
from verify_native_system import Options as SystemOptions, Info as SystemInfo, CORE
from verify_native_vmu_fs import FiberInfo
from verify_native_vmu_lcd import StateInfo, Screen, LCDInfo
from verify_native_vmu_store import Info as StoreInfo

I, U, Q, P, S = C.c_int32, C.c_uint32, C.c_uint64, C.c_void_p, C.c_size_t
CASES = ('preexisting_card', 'preexisting_system', 'preexisting_texture')


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def read(path):
    return json.loads(Path(path).read_bytes())


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace', 'build', 'track', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--case', choices=CASES)
    args = parser.parse_args()
    workspace, build, out = (getattr(args, name).resolve()
                             for name in ('workspace', 'build', 'output'))
    assert not out.exists() and out.is_relative_to(workspace / 'work/sonic-adventure')
    assert not out.is_relative_to(build), 'never alter a qualified build'
    metadata = read(build / 'build_verification.json')
    for name, digest in metadata['authored_source_sha256'].items():
        assert sha(workspace / name) == digest, name
    dll_keys = ('frames_DLL', 'present_DLL', 'GPU_DLL', 'System_DLL', 'game_sound_DLL',
                'main_boot_DLL', 'device_DLL', 'SDK_heap_DLL', 'RTC_DLL',
                'GDFS_DLL', 'authored_exit_DLL')
    dll_hashes = {name: sha(metadata[name]) for name in dll_keys}
    for name, digest in dll_hashes.items():
        assert digest == metadata[name + '_sha256'], name
    out.mkdir()
    if not args.case:
        cases = {}
        for case in CASES:
            command = [sys.executable, '-B', str(Path(__file__).resolve())]
            for name in ('workspace', 'build', 'track'):
                command += ['--' + name, str(getattr(args, name).resolve())]
            command += ['--case', case, '--output', str(out / case)]
            run = subprocess.run(command, capture_output=True, timeout=120,
                                 creationflags=subprocess.CREATE_NO_WINDOW)
            (out / (case + '.log')).write_bytes(run.stdout + run.stderr)
            assert run.returncode == 0, (case, run.returncode,
                (run.stdout + run.stderr).decode(errors='replace')[-6000:])
            cases[case] = read(out / case / 'result.json')
            cases[case]['actual_process_exit_code'] = run.returncode
            print(case, cases[case]['checks'], 'checks', flush=True)
        report = dict(
            status='actual_preexisting_card_System_and_texture_ownership_preserved',
            checks=sum(case['checks'] for case in cases.values()), cases=cases,
            build_verification_sha256=sha(build / 'build_verification.json'),
            verifier_sha256=sha(__file__), qualified_DLLs_sha256=dll_hashes,
            real_existing_owners_and_fresh_VMU_images=True,
            game_or_SDK_constructors_replaced=False,
            terminal_global_card_ownership_verified=False,
            complete_gameplay_verified=False, playable=False, overall_completion_percent=10)
        (out / 'verification.json').write_text(json.dumps(report, indent=2) + '\n')
        print(report['status'], report['checks'], 'checks', flush=True)
        return

    fault = (out / 'native_fault.txt').open('w')
    faulthandler.enable(file=fault, all_threads=True)
    modules = {key: C.CDLL(metadata[key]) for key in dll_keys}
    frame, system, boot = (modules[key] for key in ('frames_DLL', 'System_DLL', 'main_boot_DLL'))
    present, sound = (modules[key] for key in ('present_DLL', 'game_sound_DLL'))
    checks, observations, keep = [], {}, []

    def check(condition, label):
        assert condition, (args.case, label)
        checks.append(label)
        (out / 'progress.json').write_text(json.dumps(
            dict(case=args.case, checks=len(checks), assertions=checks), indent=2) + '\n')

    def snapshot(module, function, kind):
        value = kind()
        rc = bind(module, function, I, P)(C.byref(value))
        return rc, bytes(value).hex()

    def actual_snapshot(module, function, kind):
        value = kind()
        check(bind(module, function, I, P)(C.byref(value)) == 0, function + ' readback')
        return value

    for name, arguments, types in (
            ('jp_queue_initialize', (8 << 20,), (S,)),
            ('jp_frames_initialize', (8, 128, 32 << 20), (U, U, S)),
            ('jp_textures_initialize', (32, 1024, 32 << 20), (U, U, S)),
            ('jp_render_state_initialize', (128, 1 << 20), (U, S))):
        check(bind(frame, name, I, *types)(*arguments) == 0, 'actual borrowed ' + name)
    texture = C.addressof((C.c_ubyte * 0x40000).in_dll(frame, 'texture_buffer'))
    check(bind(frame, 'jp_render_state_register_buffer', I, P, S, U)(
        texture, 0x40000, 4096) == 0, 'register actual aligned original buffer')
    media = out / 'media'
    media.mkdir()
    check(bind(frame, 'jp_vmu_store_initialize', I, C.c_wchar_p)(str(media)) == 0,
          'real isolated VMU store')
    check(bind(frame, 'jp_vmu_store_create', I, U)(0) == 0 and
          bind(frame, 'jp_vmu_store_open', I, U)(0) == 0, 'fresh actual VMU image and exclusive lock')
    check(bind(frame, 'jp_lcd_initialize', I, U)(1) == 0, 'actual borrowed LCD endpoint')
    image_before = sha(media / 'VMU_A1.VMU')

    @Source
    def controlled_source(context, samples):
        C.memset(samples, 0, 4 * C.sizeof(Sample))
        samples[0] = Sample(1, 0, 0, 0, 0, 0, 0, 0)
        return 1

    keep.append(controlled_source)
    track = C.create_unicode_buffer(str(args.track.resolve()))
    keep.append(track)
    options = SystemOptions(C.sizeof(SystemOptions), 0,
        DeviceOptions(640, 480, 0, 0, 5000, 0, 64 << 20), C.addressof(track),
        2352, 3, 5000, 0, C.cast(controlled_source, P).value, None, None, None)
    check(bind(system, 'jp_system_configure', I, P)(C.byref(options)) == 0,
          'configure actual independent System')
    gdfs_handle, texture_info, texture_word, texture_payload = None, None, None, None
    if args.case == 'preexisting_card':
        bind(frame, 'card_init', None)()
        last = bind(frame, 'buGetLastError', I, I)
        for _ in range(10000):
            bind(frame, 'buExecServer', None)()
            if last(0) != -1:
                break
        card = Card()
        check(bind(frame, 'jp_card_state_read', I, U, P)(0, C.byref(card)) == 0 and
              card.initialized == card.mounted == 1 and card.error == 0,
              'preexisting genuine backend completed real mount')
        check(card.work > 0xffffffff and bind(frame, 'jp_card_selected_work_matches', I)() == 1,
              'preexisting backend uses actual original native-width work')
        fibers = actual_snapshot(frame, 'jp_rf_snapshot', FiberInfo)
        check(fibers.servers == fibers.fibers == 1 and fibers.status == 0,
              'preexisting real card fiber and server live')
    elif args.case == 'preexisting_system':
        check(bind(system, 'jp_system_initialize', I, I, I, I)(1, 0, 1) == 0,
              'independent caller genuinely initialized READY System')
        state = actual_snapshot(system, 'jp_system_snapshot', SystemInfo)
        check(state.phase == 3 and state.resources == CORE and state.window != 0,
              'independent READY System owns exact real core resources')
        gdfs_handle = bind(modules['GDFS_DLL'], 'gdFsOpen', P, C.c_char_p, P)(b'SET0001S.BIN', None)
        check(bool(gdfs_handle), 'independent caller retains a genuine open GDFS handle')
    else:
        texture_info = TextureInfo()
        surface = texture_info.texsurface
        surface.Type, surface.BitDepth, surface.PixelFormat = 0x10000, 1, 0x08000000
        surface.nWidth = surface.nHeight = 8
        surface.TextureSize, surface.fSurfaceFlags = 128, 4
        texture_payload = bytes((index * 37 + 11) & 255 for index in range(128))
        source_pixels = C.create_string_buffer(texture_payload)
        keep.extend((texture_info, source_pixels))
        base_allocated = bind(frame, 'jp_textures_allocated_bytes', S)()
        check(bind(frame, 'jp_texture_copy_surface', I, C.POINTER(TextureInfo), P, S)(
            C.byref(texture_info), source_pixels, len(texture_payload)) == 0,
            'prior caller genuinely copies controlled 8x8 texture into native owner')
        texture_word = bind(frame, 'jp_texture_surface_word', U, P)(C.byref(surface))
        check(bool(texture_word) and surface.pSurface > 0xffffffff and surface.pSurface % 32 == 0,
              'real owned texture has valid native-width aligned storage and live handle')
        check(bind(frame, 'jp_textures_live_surfaces', U)() == 1 and
              bind(frame, 'jp_textures_allocated_bytes', S)() == base_allocated + 128,
              'actual texture counters include exactly the prior caller payload')
        check(C.string_at(surface.pSurface, 128) == texture_payload,
              'actual native allocation contains the exact controlled texture payload')

    sound_options = SoundOptions(C.sizeof(SoundOptions), 0, 2 << 20, 5000,
        AicaOptions(32, 1, 16384, 40, 5000, 1, 1, 0), None)
    clock_options = ClockOptions(C.sizeof(ClockOptions), 441, 8192, 5,
                                None, None, 0, None, None)
    check(bind(boot, 'jp_main_boot_configure', I, P, P)(
        C.byref(sound_options), C.byref(clock_options)) == 0, 'configure genuine full-main bridge')
    I.in_dll(frame, 'dat_8c78b4d8').value = 29
    I.in_dll(frame, 'dat_8c78b4dc').value = -41
    U.in_dll(frame, '_rnext').value = 0x13579bdf
    C.memset(C.addressof((C.c_ubyte * 8192).in_dll(frame, 'matrix_stack')), 0xa7, 8192)

    def owners():
        result = {}
        for key, function, kind in (
                ('System_DLL', 'jp_system_snapshot', SystemInfo),
                ('device_DLL', 'jp_device_snapshot', DeviceInfo),
                ('SDK_heap_DLL', 'jp_sdk_heap_snapshot', HeapInfo),
                ('RTC_DLL', 'jp_rtc_snapshot', RTCInfo),
                ('GDFS_DLL', 'jp_gdfs_snapshot', GDFSInfo),
                ('frames_DLL', 'jp_input_snapshot', InputInfo),
                ('frames_DLL', 'jp_rf_snapshot', FiberInfo),
                ('frames_DLL', 'jp_vmu_store_snapshot', StoreInfo),
                ('frames_DLL', 'jp_vmu_state_read', StateInfo),
                ('present_DLL', 'jp_present_snapshot', PresentInfo),
                ('present_DLL', 'jp_wait_snapshot', WaitInfo),
                ('game_sound_DLL', 'jp_sound_snapshot', SoundInfo),
                ('game_sound_DLL', 'jp_sound_clock_snapshot', ClockInfo),
                ('authored_exit_DLL', 'jp_exit_snapshot', ExitInfo)):
            result[function] = snapshot(modules[key], function, kind)
        for drive in range(8):
            card = Card()
            rc = bind(frame, 'jp_card_state_read', I, U, P)(drive, C.byref(card))
            result['card_' + str(drive)] = (rc, bytes(card).hex())
            pixels, lcd = (C.c_ubyte * 192)(), LCDInfo()
            rc = bind(frame, 'jp_lcd_read', I, U, P, S, P)(drive, pixels, 192, C.byref(lcd))
            result['lcd_' + str(drive)] = (rc, bytes(lcd).hex(), bytes(pixels).hex())
        result['render_statuses'] = {name: bind(frame, name, I)() for name in (
            'jp_queue_status', 'jp_render_state_status', 'jp_render_state_check_guards',
            'jp_textures_status', 'jp_print_status', 'jp_card_selected_work_matches')}
        result['main_sentinels'] = {
            'd8': I.in_dll(frame, 'dat_8c78b4d8').value,
            'dc': I.in_dll(frame, 'dat_8c78b4dc').value,
            'random': U.in_dll(frame, '_rnext').value,
            'pointers': {name: P.in_dll(frame, name).value for name in (
                '_nj_texture_buffer_st', '_nj_vertex_buf_', '_nj_shape_buf_',
                '_nj_base_matrix_ptr_', '_nj_current_matrix_ptr_', '_nj_view_', '_nj_texmemlist')},
            'per_info': list((P * 4).in_dll(frame, 'per_info')),
            'buffer_hashes': {name: hashlib.sha256(bytes((C.c_ubyte * size).in_dll(frame, name))).hexdigest()
                              for name, size in (('matrix_stack', 8192), ('texture_buffer', 0x40000),
                                                 ('shape_buffer', 6144 * 4), ('screen', 20), ('view', 120))}}
        result['VMU_image_sha256'] = sha(media / 'VMU_A1.VMU')
        if texture_info is not None:
            pointer = bind(frame, 'jp_texture_surface_data', P, U)(texture_word)
            byte_count = bind(frame, 'jp_texture_surface_bytes', S, U)(texture_word)
            result['preexisting_texture'] = {
                'descriptor': bytes(texture_info).hex(), 'handle': texture_word,
                'owned': bind(frame, 'jp_texture_surface_owned', I, P)(C.byref(texture_info.texsurface)),
                'pointer': pointer, 'bytes': byte_count,
                'payload_sha256': hashlib.sha256(C.string_at(pointer, byte_count)).hexdigest()
                                  if pointer and byte_count <= 128 else None,
                'live_surfaces': bind(frame, 'jp_textures_live_surfaces', U)(),
                'allocated_bytes': bind(frame, 'jp_textures_allocated_bytes', S)(),
                'record_count': I.in_dll(frame, '_nj_tex_count').value,
                'current_texture': U.in_dll(frame, '_nj_current_texture').value,
                'status': bind(frame, 'jp_textures_status', I)()}
        return result

    before = owners()
    (out / 'before.json').write_text(json.dumps(before, indent=2) + '\n')
    expected = {'preexisting_card': 7, 'preexisting_system': 2, 'preexisting_texture': 6}[args.case]
    check(bind(boot, 'jp_main_boot_initialize', I)() == expected,
          'full-main constructor rejected actual ownership conflict')
    after_rejection = owners()
    for name in before:
        check(after_rejection[name] == before[name], 'rejected initialization preserves ' + name)
    state = actual_snapshot(boot, 'jp_main_boot_snapshot', BootInfo)
    check(state.starts == state.completions == state.sound_calls == state.card_owned == 0,
          'no original constructor, sound call, completion or card acquisition occurred')
    check(bind(boot, 'jp_main_boot_shutdown', I, U)(5000) == 0,
          'unacquired bridge shutdown succeeds without touching prior owners')
    after_shutdown = owners()
    for name in before:
        check(after_shutdown[name] == before[name], 'bridge shutdown preserves ' + name)
    state = actual_snapshot(boot, 'jp_main_boot_snapshot', BootInfo)
    check(state.starts == state.completions == state.sound_calls == state.card_owned == 0,
          'rejected lifetime still has no original parent or sound activity')
    (out / 'after_rejection.json').write_text(json.dumps(after_rejection, indent=2) + '\n')
    (out / 'after_shutdown.json').write_text(json.dumps(after_shutdown, indent=2) + '\n')
    observations['initialize_result'] = expected
    observations['unchanged_snapshot_groups'] = list(before)

    if args.case == 'preexisting_card':
        check(bind(frame, 'buGetLastError', I, I)(0) == 0,
              'original caller still observes successful real mounted backend')
        check(bind(frame, 'buExit', I)() == 0, 'original card owner can close its preserved real backend')
        card = Card()
        check(bind(frame, 'jp_card_state_read', I, U, P)(0, C.byref(card)) == 0 and not card.initialized,
              'caller buExit genuinely retires backend')
        fibers = actual_snapshot(frame, 'jp_rf_snapshot', FiberInfo)
        check(fibers.servers == fibers.fibers == 0, 'caller cleanup retires actual card fiber and server')
        check(bind(system, 'jp_system_shutdown', I)() == 0, 'original caller retires its configured System')
    elif args.case == 'preexisting_system':
        data = (C.c_ubyte * 2048)()
        check(bind(modules['GDFS_DLL'], 'gdFsRead', I, P, I, P)(gdfs_handle, 1, data) == 0,
              'preserved independent System still reads through genuine GDFS handle')
        check(hashlib.sha256(bytes(data[:64])).hexdigest() ==
              '16abab341fb7f370e27e4dadcf81766dd0dfd0ae64469477bb2cf6614938b2af',
              'preserved actual media read matches independent pinned sector prefix')
        bind(modules['GDFS_DLL'], 'gdFsClose', None, P)(gdfs_handle)
        check(bind(system, 'jp_system_shutdown', I)() == 0,
              'independent caller can retire its still-owned real System')
        state = actual_snapshot(system, 'jp_system_snapshot', SystemInfo)
        check(state.resources == 0 and state.window == 0 and state.cleanups == 1,
              'only independent caller performed the one genuine core cleanup')
    else:
        check(bytes(texture_info).hex() == before['preexisting_texture']['descriptor'] and
              C.string_at(texture_info.texsurface.pSurface, 128) == texture_payload,
              'original caller retains unchanged descriptor and usable controlled texture')
        check(bind(frame, 'kmFreeTexture', U, P)(C.byref(texture_info.texsurface)) == 0,
              'original caller genuinely frees its preserved native surface')
        check(not texture_info.texsurface.pSurface and not texture_info.texsurface.pVirtual and
              not texture_info.texsurface.pPhysical and
              bind(frame, 'jp_textures_live_surfaces', U)() == 0 and
              bind(frame, 'jp_textures_allocated_bytes', S)() == base_allocated and
              bind(frame, 'jp_textures_status', I)() == 0,
              'only original free clears descriptor pointers and restores exact native counters')
        check(bind(system, 'jp_system_shutdown', I)() == 0,
              'original caller retires configured System after preserving its texture')
    check(sha(media / 'VMU_A1.VMU') == image_before, 'conflict checks and cleanup preserve fresh VMU image bytes')
    check(bind(frame, 'jp_lcd_shutdown', I)() == 0 and bind(frame, 'jp_vmu_store_shutdown', I)() == 0,
          'caller retires borrowed LCD/store after genuine backend cleanup')
    check(bind(frame, 'jp_vmu_store_initialize', I, C.c_wchar_p)(str(media)) == 0 and
          bind(frame, 'jp_vmu_store_open', I, U)(0) == 0,
          'actual exclusive VMU image lock can be reacquired after original caller cleanup')
    check(bind(frame, 'jp_vmu_store_shutdown', I)() == 0, 'caller closes reacquired VMU image owner')
    check(bind(frame, 'jp_frames_shutdown', I)() == 0, 'caller retires borrowed captured-frame owner')
    bind(frame, 'jp_render_state_shutdown', None)()
    bind(frame, 'jp_textures_shutdown', None)()
    bind(frame, 'jp_queue_shutdown', None)()
    check(all(sha(metadata[name]) == digest for name, digest in dll_hashes.items()),
          'all qualified DLL bytes remain unchanged')
    faulthandler.disable()
    fault.close()
    report = dict(case=args.case, checks=len(checks), assertions=checks,
        observations=observations, build_verification_sha256=sha(build / 'build_verification.json'),
        verifier_sha256=sha(__file__), actual_constructor_or_SDK_replacements=False,
        existing_owner_snapshots_unchanged=True, prior_callers_cleanup_verified=True,
        snapshot_reports_sha256={name: sha(out / name) for name in (
            'before.json', 'after_rejection.json', 'after_shutdown.json')},
        playable=False, overall_completion_percent=10)
    (out / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
    print(args.case, len(checks), 'checks', flush=True)


if __name__ == '__main__':
    main()
