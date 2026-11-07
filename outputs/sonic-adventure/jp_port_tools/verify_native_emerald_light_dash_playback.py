"""Exercise scheduled Light Dash movement with leased original animation assets."""
import argparse
import ctypes as C
import json
import os
from pathlib import Path

from build_native_read_queue import sha
from native_packet_test_support import bind
from verify_native_motion_store import View
from verify_native_input_backend import Sample, Source


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--input', action='store_true')
    parser.add_argument('--charge', action='store_true')
    args = parser.parse_args()
    build = args.build.resolve()
    manifest = json.loads((build / 'build_verification.json').read_bytes())
    path = build / 'jp_native_frames.dll'
    assert sha(path) == manifest['frames_DLL_sha256']
    with os.add_dll_directory(str(build)):
        frames = C.CDLL(str(path))
        motions = C.CDLL(str(build / 'jp_native_motion_data.dll'))
    I, P, U, Q = C.c_int, C.c_void_p, C.c_uint32, C.c_uint64
    assert bind(motions, 'jp_motions_initialize', I, C.c_size_t, U)(8 << 20, 16) == 0
    acquire = bind(motions, 'jp_motions_acquire', I, U, C.c_char_p, P)
    release = bind(motions, 'jp_motions_release', I, Q)
    views = []
    try:
        assert bind(motions, 'jp_motions_select', I, I)(0) == 0
        for bank, name in ((7, b'sonic_stand_action'), (0, b'sonic_light_dash_action'),
                           (7, b'sonic_skid_action')):
            view = View()
            status = acquire(bank, name, C.byref(view))
            assert status == 0, (name, status)
            views.append(view)
        stack = C.create_string_buffer(128 * 64)
        bind(frames, 'njInitMatrix', None, P, I, I)(stack, 128, 0)
        bind(frames, 'heap_init', None)()
        bind(frames, 'task_init', None)()
        result = bind(frames, 'jp_emerald_light_dash_playback_check', I, P, P, P)(
            *(view.data for view in views))
        assert result == 0, result
        if args.input or args.charge:
            samples = (Sample * 4)()

            @Source
            def source(_context, output):
                C.memmove(output, samples, C.sizeof(samples))
                return 1

            input_phase = C.CFUNCTYPE(I, I)
            phases = []

            @input_phase
            def change(held):
                phases.append(held)
                samples[0] = Sample(1, 2 if held else 0, 0, 0, 0, 0, 0, 0)
                return 0

            init = bind(frames, 'jp_input_initialize', I, P, U, P, P)
            assert init(None, 0, C.cast(source, P), None) == 0
            try:
                bind(frames, 'pad_enable', None)()
                bind(frames, 'pad_enable_player', None, C.c_uint8)(255)
                if args.input:
                    result = bind(frames, 'jp_emerald_light_dash_input_check', I, P, P, P, input_phase)(
                        *(view.data for view in views), change)
                    assert result == 0 and phases == [1, 0], (result, phases)
                if args.charge:
                    phases.clear()
                    result = bind(frames, 'jp_emerald_light_dash_charge_input_check', I, input_phase)(change)
                    assert result == 0 and phases == [1] * 211 + [0] + [1] * 121 + [0], result
                    print('Scheduled original charge and release through native pad input passed')
            finally:
                assert bind(frames, 'jp_input_shutdown', I)() == 0
    finally:
        for view in reversed(views):
            assert release(view.token) == 0
        assert bind(motions, 'jp_motions_shutdown', I)() == 0
    print('Scheduled Light Dash and skid animation playback passed')


if __name__ == '__main__':
    main()
