"""Build the authored host queue against hash-pinned external SDK headers."""
import argparse, hashlib, json, subprocess
from pathlib import Path

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap = argparse.ArgumentParser()
    for name in ['workspace', 'upstream', 'compiler', 'output']:
        ap.add_argument('--'+name, type=Path, required=True)
    a = ap.parse_args()
    r, u, c, d = (getattr(a, n).resolve() for n in ['workspace','upstream','compiler','output'])
    p = r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    manifest = p/'jp_port_tools/jp_task_external_inputs.json'
    dep = json.loads(manifest.read_text())
    renderer_manifest = p/'jp_port_tools/jp_renderer_external_inputs.json'
    renderer_dep = json.loads(renderer_manifest.read_text())
    for name, expected in renderer_dep['additional_game_inputs_sha256'].items():
        assert sha(u/name) == expected, name
    for name, expected in dep['files_sha256'].items():
        assert sha(u/name) == expected, name
    d.mkdir(parents=True)
    common = [str(c), '--target=x86_64-pc-windows-msvc', '-std=c17', '-O2',
              '-ffp-contract=off', '-ffreestanding', '-fno-builtin', '-Wall',
              '-Wextra', '-Werror', '-Wno-unknown-pragmas',
              '-include', str(p/'native/jp_host_types.h'),
              '-I', str(u/'sdk/include'), '-I', str(u)]
    source = p/'native/jp_native_queue.c'
    commands = [common+['-c', str(source), '-o', str(d/'queue.obj')]]
    exports = ['njGetkmPointer','njSetkmPointer','njSetkmModelAttr','njEndVertex',
               'njStartVertexNonTexOpaque','njStartVertexNonTexTrans',
               'njStartVertexTexOpaque','njStartVertexTexTrans','_builtin_prefetch']
    commands.append([str(c), '--target=x86_64-pc-windows-msvc', '-shared',
                     '-fuse-ld=lld', *['-Wl,/export:'+n for n in exports],
                     str(d/'queue.obj'), '-o', str(d/'jp_native_queue.dll')])
    results = []
    for i, command in enumerate(commands):
        run = subprocess.run(command, capture_output=True)
        log = d/f'build_{i}.txt'
        log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=command, exit_code=run.returncode,
                            diagnostic_bytes=log.stat().st_size, log_sha256=sha(log)))
        if run.returncode:
            print(log.read_text()[:7000])
            break
    for name, expected in dep['files_sha256'].items():
        assert sha(u/name) == expected, name
    passed = len(results)==2 and all(x['exit_code']==0 for x in results)
    for name, expected in renderer_dep['additional_game_inputs_sha256'].items():
        assert sha(u/name) == expected, name
    authored = [source, p/'native/jp_native_queue.h', p/'native/jp_host_types.h', Path(__file__)]
    report = dict(status='native_queue_linked_behavior_unverified' if passed else 'build_failed',
                  commands=results, compiler_sha256=sha(c),
                  authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in authored},
                  external_manifest_sha256=sha(manifest), external_files_verified=len(dep['files_sha256']),
                  renderer_external_manifest_sha256=sha(renderer_manifest),
                  artifacts_sha256={s.name:sha(s) for s in d.iterdir() if s.suffix in ['.obj','.dll','.lib']},
                  game_source_linked=False, retail_assets_embedded=False,
                  complete_renderer_linked=False, playable=False,
                  overall_completion_percent=10, goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'], 'diagnostic bytes', sum(x['diagnostic_bytes'] for x in results))
    raise SystemExit(0 if passed else 1)

if __name__ == '__main__':
    main()
