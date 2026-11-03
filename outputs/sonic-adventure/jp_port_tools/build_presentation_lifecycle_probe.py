"""Link real presentation lifecycle functions with the complete native task object.

This is an isolated test boundary. Generated game source and its DLL stay
private. No renderer/frame service is stubbed to make the full game link.
"""
import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path
from build_native_texture_runtime import function


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def coff_payload(p):
    raw = p.read_bytes()
    assert raw[:2] == b'\x64\x86', 'Expected ordinary AMD64 COFF'
    # Clang records the compile time in bytes 4..7 of the COFF header.
    # Keep original files unchanged; compare every other byte exactly.
    return raw[:4] + bytes(4) + raw[8:]


def main():
    ap = argparse.ArgumentParser()
    for name in ['workspace', 'presentation', 'task', 'baseline', 'clock', 'compiler', 'output']:
        ap.add_argument('--' + name, type=Path, required=True)
    a = ap.parse_args()
    r, d = a.workspace.resolve(), a.output.resolve()
    p = r / 'outputs/sonic-adventure'
    assert d.is_relative_to(r / 'work/sonic-adventure')
    assert not d.is_relative_to(r / 'work/sonic-adventure/build') and not d.exists()
    reports = {name: json.loads((getattr(a, name) / 'build_verification.json').read_text())
               for name in ['presentation', 'task', 'baseline', 'clock']}
    pb, tb, bb, cb = [reports[n] for n in ['presentation', 'task', 'baseline', 'clock']]
    for b in [pb, bb, cb]:
        for name, h in b['authored_source_sha256'].items():
            assert sha(r / name) == h, name
    assert pb['exit_code'] == bb['exit_code'] == tb['exit_code'] == 0
    assert pb['diagnostic_bytes'] == tb['diagnostic_bytes'] == 0
    assert sha(a.task / 'task_present.obj') == tb['object_sha256']
    source = a.presentation / 'external_present_native.c'
    assert sha(source) == pb['generated_source_sha256']
    raw = source.read_bytes()
    first = function(raw, 'state_get')
    prefix = raw[:raw.index(first)]
    names = ['timer_stop', 'marks_slot', 'marks_find', 'marks_add', 'released',
             'draws_forget', 'flats_forget', 'shadows_forget', 'present_forget',
             'present_wide', 'present_run', 'records_keep', 'records_put',
             'present_draw', 'replays']
    selected = {name: function(raw, name) for name in names}
    declarations = []
    for pattern in [rb'^static struct task deleted_task;\r?\n',
                    rb'^static struct task released_task;\r?\n',
                    rb'^struct records \{.*?^\};\r?\n']:
        found = re.findall(pattern, raw, re.M | re.S)
        assert len(found) == 1, pattern
        declarations.append(found[0])
    d.mkdir(parents=True)
    extracted = d / 'private_selected_lifecycle.c'
    extracted.write_bytes(prefix + b''.join(declarations) + b''.join(selected.values()))
    fixture = p / 'native/jp_presentation_lifecycle_fixture.c'
    bridge = d / 'private_lifecycle_probe.c'
    bridge.write_bytes(b'#include "private_selected_lifecycle.c"\n' + fixture.read_bytes())
    command = list(pb['command'])
    i = command.index('-c')
    command[i + 1] = str(bridge)
    command[-1] = str(d / 'probe.obj')
    # The full original prefix retains state unrelated to this test boundary.
    command[i:i] = ['-Wno-unused-variable']
    commands = [command]
    objects = [d / 'probe.obj', a.task / 'task_present.obj']
    if 'lifetime_object_sha256' in pb:
        obj = a.presentation / 'replay_lifetime.obj'
        assert sha(obj) == pb['lifetime_object_sha256']
        objects.append(obj)
    # Recompile real providers from their already pinned sources; prove their
    # object bytes equal the previous verified composition before reusing them.
    rebuilt = {}
    for name in ['lifecycle', 'bridge', 'math', 'matrix_state']:
        cmd = list(next(c['command'] for c in bb['commands'] if c['phase'] == name))
        obj = d / (name + '.obj')
        cmd[-1] = str(obj)
        commands.append(cmd)
        objects.append(obj)
        rebuilt[name] = sha(a.baseline / (name + '.obj'))
    for name, h in cb['object_sha256'].items():
        obj = a.clock / name
        assert sha(obj) == h
        objects.append(obj)
    exports = ['task_init', 'task_create', 'task_create_child', 'task_destroy', 'task_kill',
               'task_step_all', 'task_draw_all', 'task_step_children', 'task_draw_children',
               'task_destroy_all', 'present_run', 'present_draw', 'present_forget',
               'syTmrGetCount']
    dll = d / 'presentation_lifecycle_probe.dll'
    commands.append([str(a.compiler), '--target=x86_64-pc-windows-msvc', '-shared',
                     '-fuse-ld=lld', *['-Wl,/export:' + n for n in exports],
                     *map(str, objects), '-Wl,kernel32.lib', '-o', str(dll)])
    results = []
    for index, cmd in enumerate(commands):
        run = subprocess.run(cmd, capture_output=True)
        log = d / f'build_{index}.txt'
        log.write_bytes(run.stdout + run.stderr)
        results.append(dict(command=cmd, exit_code=run.returncode,
                            diagnostic_bytes=log.stat().st_size, log_sha256=sha(log)))
        if run.returncode:
            print(log.read_text()[:6000])
            break
    passed = len(results) == len(commands) and all(v['exit_code'] == 0 for v in results)
    if passed:
        for name, h in rebuilt.items():
            assert sha(a.baseline / (name + '.obj')) == h
            assert coff_payload(d / (name + '.obj')) == coff_payload(a.baseline / (name + '.obj')), name
    report = dict(status='actual_presentation_lifecycle_probe_linked' if passed else 'build_failed',
                  commands=results, DLL=str(dll), DLL_sha256=sha(dll) if passed else None,
                  parent_reports_sha256={n: sha(getattr(a, n) / 'build_verification.json') for n in reports},
                  selected_functions_sha256={n: hashlib.sha256(v).hexdigest() for n, v in selected.items()},
                  prefix_sha256=hashlib.sha256(prefix).hexdigest(), selected_source_sha256=sha(extracted),
                  task_object_sha256=sha(a.task / 'task_present.obj'), rebuilt_objects_sha256=rebuilt,
                  replay_lifetime_guard_linked='lifetime_object_sha256' in pb,
                  rebuilt_provider_comparison='all_bytes_equal_except_COFF_header_compile_timestamp',
                  rebuilt_actual_objects_sha256={n: sha(d / (n + '.obj')) for n in rebuilt} if passed else {},
                  authored_source_sha256={str(v.relative_to(r)).replace('\\', '/'): sha(v)
                                         for v in [Path(__file__).resolve(), fixture]},
                  fixture_owned_inputs=['frame_step', 'camera_work', 'player_tasks', 'player_records',
                                        'port_wide_anchor', 'port_wide_cover'],
                  game_containing_artifact_publishable=False, full_presentation_linked=False,
                  renderer_stubs=False, behavior_verified=False, overall_completion_percent=10,
                  goal_paused=False, playable=False)
    (d / 'build_verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], 'diagnostic bytes', sum(v['diagnostic_bytes'] for v in results))
    raise SystemExit(0 if passed else 1)


if __name__ == '__main__':
    main()
