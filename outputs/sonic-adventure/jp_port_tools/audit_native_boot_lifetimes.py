"""Audit actual linked card/font authority and preserve prior qualifications."""
import argparse, ctypes as C, hashlib, json, re, subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from package_native_sound_placement import prior


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def read(path):
    return json.loads(Path(path).read_bytes())


def main():
    ap = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'baseline', 'providers', 'build', 'output'):
        ap.add_argument('--' + name, type=Path, required=True)
    a = ap.parse_args()
    r, base, providers, build, out = (getattr(a, n).resolve() for n in
                                    ('workspace', 'baseline', 'providers', 'build', 'output'))
    assert not out.exists() and out.is_relative_to(r / 'work/sonic-adventure')
    b = read(build / 'build_verification.json')
    assert sha(base / 'build_verification.json') == b['boot_lifetimes_baseline_sha256']
    assert sha(providers / 'build_verification.json') == b['boot_exit_baseline_sha256']
    for name, digest in b['authored_source_sha256'].items():
        assert sha(r / name) == digest, name
    for field in ('boot_lifetimes_generated_source_sha256', 'boot_exit_generated_source_sha256'):
        for name, digest in b[field].items():
            assert sha(build / name) == digest, name
    old = r / 'outputs/sonic-adventure/analysis/jp_native_main_boot_package.json'
    seen = set()
    prior(r, old, seen)
    assert read(old)['combined_qualified_scoped_checks'] == 940335
    upstream = a.upstream / 'src/main.c'
    pinned = subprocess.check_output(['C:/Program Files/Git/cmd/git.exe', '-C',
        str(a.upstream), 'show', '25a3d3154dbe657325e97dbe8c5291086f6af5c0:src/main.c'])
    assert upstream.read_bytes() == pinned
    original = function(pinned, 'system_boot_init')
    adapted = original.replace(b'    sound_system_init();',
        b'    sound_system_init();\n    if(jp_main_boot_sound_failed())return;')
    assert function((build / 'external_main_boot.c').read_bytes(), 'system_boot_init') == adapted
    assert sha(base / 'main_boot_original.obj') == sha(build / 'main_boot_original.obj')
    symbols = coff_symbols(build / 'main_boot_original.obj')
    for name in ('texture_buffer', 'shape_buffer', 'per_info', 'cable_mode'):
        assert any(s['name'] == '__imp_' + name and s['section'] == 0 for s in symbols)
        assert not any(s['name'] == name and s['section'] > 0 for s in symbols)
    frame_link = next(row['command'] for row in b['commands'] if '-shared' in row['command']
                      and Path(row['command'][-1]).name == 'jp_native_frames.dll')
    old_objects = b['boot_lifetimes_original_objects']
    replaced = b['boot_lifetimes_replaced_objects']
    assert len(old_objects) == 149 and len(replaced) == 5
    preserved = {}
    for name, row in old_objects.items():
        assert sha(row['path']) == row['sha256'], name
        if name not in replaced:
            assert frame_link.count(row['path']) == 1, name
            preserved[name] = row['sha256']
        else:
            assert row['path'] not in frame_link
            assert sha(build / ('life_' + name)) == sha(providers / ('life_' + name))
    assert sum(x.endswith('.obj') for x in frame_link) == 150
    unchanged, changed = {}, {}
    for path in base.iterdir():
        if path.suffix not in ('.dll', '.lib', '.obj'):
            continue
        assert (build / path.name).exists(), path.name
        target = unchanged if sha(path) == sha(build / path.name) else changed
        target[path.name] = sha(build / path.name)
    assert set(changed) == {'jp_native_exit.dll', 'jp_native_exit.lib',
        'jp_native_frames.dll', 'jp_native_frames.lib', 'jp_native_main_boot.dll',
        'jp_native_system.dll', 'jp_native_system.lib', 'main_boot_owner.obj'}
    system = (build / 'system_with_boot_lifetimes.c').read_bytes()
    exit_source = (build / 'exit_with_boot_lifetimes.c').read_bytes()
    parent = (build / 'main_boot_with_lifetimes.c').read_bytes()
    for raw in (system, exit_source, parent):
        assert b'setjmp' not in raw and b'longjmp' not in raw
    close = function(system, 'shutdown_owned')
    order = [b'CLOSE(JP_EXIT_' + name for name in
             (b'CARD', b'SOUND', b'GDFS', b'RTC', b'INPUT', b'DEVICE', b'PRINT', b'SDK_HEAP')]
    assert [close.index(x) for x in order] == sorted(close.index(x) for x in order)
    assert function(system, 'bootstrap_cleanup') == function(
        (base / 'system_with_sound_clock.c').read_bytes(), 'bootstrap_cleanup')
    clean = function(exit_source, 'jp_exit_cleanup')
    assert clean.index(b'boot_exit_activity()') < clean.index(b'state.busy=1') < clean.index(b'bootstrap.started=1')
    order = [b'CLOSE_INT(JP_EXIT_CARD', b'CLOSE_INT(JP_EXIT_SOUND',
        b'CLOSE_INT(JP_EXIT_GDFS', b'CLOSE_INT(JP_EXIT_RTC',
        b'jp_device_exit_quiesce', b'CLOSE_INT(JP_EXIT_FRAMES',
        b'CLOSE_INT(JP_EXIT_PRINT', b'if(state.info.remaining&JP_EXIT_TEXTURES)',
        b'CLOSE_INT(JP_EXIT_SDK_HEAP']
    assert [clean.index(x) for x in order] == sorted(clean.index(x) for x in order)
    font_source = (build / 'print_with_lifetime.c').read_bytes()
    signature = b'int jp_boot_font_identity(uint64_t *identity){'
    assert font_source.count(signature) == 1
    font = function(font_source.replace(signature, signature[:-1] + b'\n{'), 'jp_boot_font_identity')
    assert b'tex_param&0x001fffffu' in font and b'jp_texture_live_surface_view' in font
    assert b'clear_error' not in font and b'jp_texture_surface_word(' not in font
    assert parent.count(b'jp_system_adopt_boot_lifetimes(') == 1
    reader = Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
    imports, dll_hashes = {}, {}
    out.mkdir()
    for key in ('frames_DLL', 'System_DLL', 'authored_exit_DLL', 'main_boot_DLL'):
        path = Path(b[key]); assert sha(path) == b[key + '_sha256']
        run = subprocess.run([str(reader), '--coff-imports', str(path)], capture_output=True)
        assert not run.returncode and not run.stderr
        (out / (key + '_imports.txt')).write_bytes(run.stdout)
        imports[key] = re.findall(r'^  Name: (.+)$', run.stdout.decode(), re.M)
        imports[key] = [n.strip().lower() for n in imports[key]]
        dll_hashes[key] = sha(path)
        C.CDLL(str(path))
    assert not any(n.startswith('jp_native_') for n in imports['frames_DLL'])
    assert 'jp_native_system.dll' not in imports['authored_exit_DLL']
    assert 'jp_native_main_boot.dll' not in imports['authored_exit_DLL']
    assert 'jp_native_main_boot.dll' not in imports['System_DLL']
    assert 'jp_native_exit.dll' in imports['System_DLL']
    assert 'jp_native_system.dll' in imports['main_boot_DLL']
    assert all(row['exit_code'] == row['diagnostic_bytes'] == 0 for row in b['commands'][:13])
    report = dict(status='actual_boot_lifetime_link_graph_authority_order_and_prior_proofs_audited',
        build_verification_sha256=sha(build / 'build_verification.json'),
        auditor_sha256=sha(__file__), original_parent_source_unchanged=True,
        original_parent_object_preserved=True, reused_Frame_objects=preserved,
        replaced_Frame_objects=replaced, new_Frame_owner_objects=1,
        unchanged_prior_artifacts=unchanged, deliberately_changed_artifacts=changed,
        actual_PE_imports=imports, DLL_sha256=dll_hashes,
        same_existing_System_bootstrap_callback=True,
        provider_reentry_checked_before_terminal_mutation=True,
        sticky_font_identity_observation_does_not_clear_errors=True,
        prior_qualification_packages_checked=len(seen), prior_package_sha256=sha(old),
        combined_game_SDK_binaries_publishable=False, playable=False,
        overall_completion_percent=10)
    (out / 'verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], len(preserved), 'reused Frame objects;',
          len(unchanged), 'unchanged prior artifacts;', len(seen), 'prior packages', flush=True)


if __name__ == '__main__':
    main()
