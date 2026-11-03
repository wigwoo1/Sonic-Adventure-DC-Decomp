"""Build an authored matrix extension with one existing real active state.

The DLL contains no game source or retail assets. Production integration links
only multi_matrix.obj into the existing runtime, never a second state DLL.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    for name in ['workspace', 'upstream', 'baseline', 'baseline-audit', 'sh4zam', 'compiler', 'output']:
        ap.add_argument('--' + name, dest=name.replace('-', '_'), type=Path, required=True)
    a = ap.parse_args()
    r, u, b, d = [getattr(a, n).resolve() for n in ['workspace', 'upstream', 'baseline', 'output']]
    p = r / 'outputs/sonic-adventure'
    assert d.is_relative_to(r / 'work/sonic-adventure')
    assert not d.is_relative_to(r / 'work/sonic-adventure/build') and not d.exists()
    pin_path = p / 'jp_port_tools/jp_multi_matrix_external_inputs.json'
    pin = json.loads(pin_path.read_text())
    for name, h in pin['files_sha256'].items():
        assert sha(u / name) == h, name
    bb = json.loads((b / 'build_verification.json').read_text())
    for name, h in bb['authored_source_sha256'].items():
        assert sha(r / name) == h, name
    dep_path = p / 'jp_port_tools/jp_renderer_external_inputs.json'
    dep = json.loads(dep_path.read_text())
    for name, h in dep['sh4zam_files_sha256'].items():
        assert sha(a.sh4zam / name) == h, name
    audit = json.loads(a.baseline_audit.read_text())
    providers = [b / name for name in ['math.obj', 'matrix_state.obj']]
    observed = {Path(n).resolve(): h for n, h in audit['linked_objects_sha256'].items()}
    for obj in providers:
        assert sha(obj) == observed[obj]
    d.mkdir(parents=True)
    flags = ['--target=x86_64-pc-windows-msvc', '-std=c17', '-O2', '-ffp-contract=off',
             '-ffreestanding', '-fno-builtin', '-Wall', '-Wextra', '-Werror']
    source = p / 'native/jp_ninja_multi_matrix.c'
    command = [str(a.compiler), *flags, '-c', str(source), '-o', str(d / 'multi_matrix.obj')]
    commands = [command]
    objects = [d / 'multi_matrix.obj', *providers]
    probes = [p / 'native/jp_sh4zam_probe.c', p / 'native/jp_sh4zam_probe_other.c']
    for i, probe in enumerate(probes):
        obj = d / f'cross_state_probe_{i}.obj'
        commands.append([str(a.compiler), *flags, '-Wno-unknown-attributes',
                         '-include', str(p / 'native/jp_sh4zam_host_config.h'),
                         '-isystem', str(a.sh4zam / 'include'), '-c', str(probe), '-o', str(obj)])
        objects.append(obj)
    dll = d / 'jp_shared_matrix_multi.dll'
    commands.append([str(a.compiler), '--target=x86_64-pc-windows-msvc', '-shared', '-fuse-ld=lld',
                     '-Wl,/export:shz_xmtrx_state_', *map(str, objects), '-o', str(dll)])
    results = []
    for i, cmd in enumerate(commands):
        run = subprocess.run(cmd, capture_output=True)
        log = d / f'build_{i}.txt'
        log.write_bytes(run.stdout + run.stderr)
        results.append(dict(command=cmd, exit_code=run.returncode,
                            diagnostic_bytes=log.stat().st_size, log_sha256=sha(log)))
        if run.returncode:
            print(log.read_text()[:6000])
            break
    passed = len(results) == len(commands) and all(v['exit_code'] == 0 for v in results)
    authored = [source, p / 'native/jp_ninja_multi_matrix.h', pin_path, Path(__file__).resolve()]
    authored += [r / n for n in bb['authored_source_sha256'] if n.startswith('outputs/')]
    for name, h in pin['files_sha256'].items():
        assert sha(u / name) == h, name
    report = dict(status='native_multi_matrix_shared_runtime_linked' if passed else 'build_failed',
                  commands=results, exit_code=0 if passed else 1,
                  diagnostic_bytes=sum(v['diagnostic_bytes'] for v in results),
                  compiler_sha256=sha(a.compiler), upstream_commit=pin['upstream_commit'],
                  external_input_manifest_sha256=sha(pin_path),
                  baseline_report_sha256=sha(b / 'build_verification.json'),
                  baseline_audit_sha256=sha(a.baseline_audit),
                  existing_provider_objects_sha256={str(o): sha(o) for o in providers},
                  production_objects_sha256={'multi_matrix.obj': sha(d / 'multi_matrix.obj')} if passed else {},
                  expected_resolved_services=['njMultiMatrix'],
                  authored_source_sha256={s.relative_to(r).as_posix(): sha(s) for s in authored},
                  DLL=str(dll), DLL_sha256=sha(dll) if passed else None,
                  source_reference_sha256=pin['files_sha256'], game_source_linked=False,
                  retail_assets_embedded=False, authored_asset_free_artifact_publishable=True,
                  game_containing_artifact_publishable=False,
                  separate_SH4ZAM_state_source_linked=False, separate_active_matrix_instance=False,
                  original_sources_modified=False, behavior_verified=False,
                  overall_completion_percent=10, goal_paused=False, playable=False)
    (d / 'build_verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], 'diagnostic bytes', report['diagnostic_bytes'])
    raise SystemExit(0 if passed else 1)


if __name__ == '__main__':
    main()
