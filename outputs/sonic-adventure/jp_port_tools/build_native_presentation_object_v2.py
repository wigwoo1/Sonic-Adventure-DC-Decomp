"""Add native replay lifetime invalidation to the preserved complete object.

Prevents restoring records into a task deleted/reused by its replay callback.
Original external source and all previous generated sources remain unchanged.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    for n in ['workspace', 'baseline', 'compiler', 'output']:
        ap.add_argument('--' + n, type=Path, required=True)
    a = ap.parse_args()
    r, b, d = a.workspace.resolve(), a.baseline.resolve(), a.output.resolve()
    p = r / 'outputs/sonic-adventure'
    assert d.is_relative_to(r / 'work/sonic-adventure')
    assert not d.is_relative_to(r / 'work/sonic-adventure/build') and not d.exists()
    pb = json.loads((b / 'build_verification.json').read_text())
    assert pb['exit_code'] == pb['diagnostic_bytes'] == 0
    assert sha(a.compiler) == pb['compiler_sha256']
    for n, h in pb['authored_source_sha256'].items():
        assert sha(r / n) == h
    raw = (b / 'external_present_native.c').read_bytes()
    assert hashlib.sha256(raw).hexdigest() == pb['generated_source_sha256']
    changes = []
    def replace(name, old, new):
        nonlocal raw
        before = function(raw, name)
        assert before.count(old) == 1, name
        after = before.replace(old, new)
        raw = raw.replace(before, after)
        changes.append(dict(function=name, before_sha256=hashlib.sha256(before).hexdigest(),
                            after_sha256=hashlib.sha256(after).hexdigest()))
    replace('present_forget', b'    if (keeping == KEEP_NONE)',
            b'    jp_replay_destroying(task);\n    if (keeping == KEEP_NONE)')
    replace('present_draw', b'    struct records keep;\n',
            b'    struct records keep;\n    struct jp_replay_scope native_scope;\n')
    replace('present_draw', b'        present_task = task;\n        draw(task);',
            b'        jp_replay_enter(&native_scope, task);\n        present_task = task;\n        draw(task);')
    replace('present_draw', b'        records_put(&keep, task);',
            b'        if (jp_replay_leave(&native_scope)) {\n            records_put(&keep, task);\n        }')
    raw = b'#include "jp_replay_lifetime.h"\n' + raw
    d.mkdir(parents=True)
    source = d / 'external_present_native.c'
    source.write_bytes(raw)
    overlay = d / 'include/collision/ground.h'
    overlay.parent.mkdir(parents=True)
    assert sha(b / 'include/collision/ground.h') == pb['native_ground_header_sha256']
    overlay.write_bytes((b / 'include/collision/ground.h').read_bytes())
    cmd = [str(d / 'include') if v == str(b / 'include') else v for v in pb['command']]
    cmd[cmd.index('-c') + 1], cmd[-1] = str(source), str(d / 'present.obj')
    lifetime = p / 'native/jp_replay_lifetime.c'
    lc = [str(a.compiler), '--target=x86_64-pc-windows-msvc', '-std=c17', '-O2',
          '-ffreestanding', '-fno-builtin', '-Wall', '-Wextra', '-Werror',
          '-c', str(lifetime), '-o', str(d / 'replay_lifetime.obj')]
    results = []
    for i, command in enumerate([cmd, lc]):
        run = subprocess.run(command, capture_output=True)
        log = d / f'compile_{i}.txt'
        log.write_bytes(run.stdout + run.stderr)
        results.append(dict(command=command, exit_code=run.returncode,
                            diagnostic_bytes=log.stat().st_size, log_sha256=sha(log)))
        if run.returncode:
            print(log.read_text()[:6000])
            break
    passed = len(results) == 2 and all(v['exit_code'] == 0 for v in results)
    report = dict(pb)
    report.update(status='complete_native_presentation_lifetime_object_compiled' if passed else 'compile_failed',
                  command=cmd, commands=results, exit_code=0 if passed else 1,
                  diagnostic_bytes=sum(v['diagnostic_bytes'] for v in results),
                  parent_report_sha256=sha(b / 'build_verification.json'),
                  generated_source_sha256=sha(source), lifetime_adaptations=changes,
                  native_ground_header_sha256=sha(overlay),
                  object_sha256=sha(d / 'present.obj') if passed else None,
                  lifetime_object_sha256=sha(d / 'replay_lifetime.obj') if passed else None,
                  lifetime_provider_source_sha256=sha(lifetime),
                  undefined_symbols=sorted(s['name'] for s in coff_symbols(d / 'present.obj')
                                           if s['section'] == 0 and s['value'] == 0) if passed else [],
                  linked_or_runtime_qualified=False)
    for v in [Path(__file__).resolve(), lifetime, p / 'native/jp_replay_lifetime.h']:
        report['authored_source_sha256'][v.relative_to(r).as_posix()] = sha(v)
    (d / 'build_verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], 'diagnostic bytes', report['diagnostic_bytes'])
    raise SystemExit(0 if passed else 1)


if __name__ == '__main__':
    main()
