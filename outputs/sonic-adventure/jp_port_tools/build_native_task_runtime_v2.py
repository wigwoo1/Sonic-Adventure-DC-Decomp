"""Build the pinned Japanese task source with real native dependencies.

Generated game-source extracts and game-containing artifacts remain private.
All four selected lifecycle bodies retain exact original bytes and provenance.
This Windows x64 component is not the complete game or a disc-structure ABI.
"""
import argparse
import hashlib
import json
import re
import subprocess
from pathlib import Path

PIN = '25a3d3154dbe657325e97dbe8c5291086f6af5c0'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--workspace', type=Path, required=True)
    ap.add_argument('--upstream', type=Path, required=True)
    ap.add_argument('--compiler', type=Path, required=True)
    ap.add_argument('--output', type=Path, required=True)
    a = ap.parse_args()
    r, u, c, d = (path.resolve() for path in [a.workspace, a.upstream, a.compiler, a.output])
    p, w = r / 'outputs/sonic-adventure', r / 'work/sonic-adventure'
    assert d.is_relative_to(w) and not d.is_relative_to(w / 'build')
    assert not d.is_relative_to(p) and not d.exists(), 'Use a fresh private output directory.'
    manifest = p / 'jp_port_tools/jp_task_external_inputs.json'
    pinned = json.loads(manifest.read_text())
    assert pinned['upstream_commit'] == PIN
    inputs = pinned['files_sha256']
    actual_headers = {path.relative_to(u).as_posix()
                      for folder in ['sdk/include', 'src', 'modules', 'events']
                      for path in (u / folder).rglob('*.h')}
    assert actual_headers == {name for name in inputs if name.endswith('.h')}
    for name, digest in inputs.items():
        assert sha(u / name) == digest, name
    d.mkdir(parents=True)
    pieces, provenance = [], []
    for relative, names in [
        ('src/collision/collision.c', ['collision_free']),
        ('src/event/event.c', ['event_free', 'event_motions_clear', 'event_paths_clear']),
    ]:
        raw = (u / relative).read_bytes()
        for name in names:
            pattern = rb'^void ' + name.encode() + rb'\([^;]*?\)\r?\n\{.*?^\}\r?\n'
            matches = list(re.finditer(pattern, raw, re.M | re.S))
            assert len(matches) == 1, name
            match = matches[0]
            pieces.append(match.group())
            provenance.append(dict(function=name, source=relative,
                                   source_sha256=sha(u / relative),
                                   byte_start=match.start(), byte_end=match.end(),
                                   body_sha256=hashlib.sha256(match.group()).hexdigest()))
    cleanup = d / 'external_lifecycle.c'
    cleanup.write_bytes(b'#include "system/task.h"\n#include "collision/collision.h"\n'
                        b'#include "event/event.h"\n' + b'\n'.join(pieces))
    # Keep the complete external source unchanged on disk. Its original
    # 16-byte minimum split is shorter than a native four-pointer header.
    # Consume that short tail instead of overlapping the end block.
    original_task = (u / 'src/system/task.c').read_bytes()
    old_condition = b'if (room == need) {'
    new_condition = b'if (room >= need && room - need < sizeof(struct heap_block)) {'
    assert original_task.count(old_condition) == 1
    adapted_task = d / 'external_task_native.c'
    adapted_task.write_bytes(original_task.replace(old_condition, new_condition))
    flags = ['--target=x86_64-pc-windows-msvc', '-std=c17', '-O2',
             '-ffp-contract=off', '-ffreestanding', '-fno-builtin',
             '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter',
             '-Wno-unknown-pragmas', '-Wno-ignored-qualifiers']
    includes = ['-I' + str(u / f) for f in ['sdk/include', 'src', 'modules', 'events']]
    units = [('task', adapted_task, True),
             ('lifecycle', cleanup, True),
             ('bridge', p / 'native/jp_task_runtime_bridge.c', True),
             ('math', p / 'native/jp_ninja_math.c', False),
             ('runtime', p / 'native/jp_math_freestanding_runtime.c', False)]
    results = []
    for name, source, game_headers in units:
        command = [str(c), *flags]
        if game_headers:
            command += ['-nostdinc', '-include', str(p / 'native/jp_host_types.h'), *includes]
        command += ['-c', str(source), '-o', str(d / (name + '.obj'))]
        result = subprocess.run(command, capture_output=True)
        log = d / (name + '.txt')
        log.write_bytes(result.stdout + result.stderr)
        results.append(dict(phase=name, command=command, exit_code=result.returncode,
                            log_sha256=sha(log), diagnostic_bytes=log.stat().st_size))
        print(name, result.returncode, log.stat().st_size, 'diagnostic bytes', flush=True)
        if result.returncode:
            break
    exports = ['task_init', 'task_create', 'task_alloc', 'task_destroy', 'task_kill',
               'task_step_all', 'task_draw_all', 'task_destroy_all', 'task_step_level',
               'task_draw_level', 'task_step_children', 'task_draw_children',
               'task_create_child', 'task_create_children', 'task_destroy_children',
               'task_get_current_root_step', 'collision_free', 'event_free']
    dll = d / 'jp_task_x64.dll'
    if len(results) == len(units) and all(row['exit_code'] == 0 for row in results):
        command = [str(c), '--target=x86_64-pc-windows-msvc', '-shared', '-nostdlib',
                   '-fuse-ld=lld', '-Wl,/noentry']
        command += ['-Wl,/export:' + name for name in exports]
        command += [str(d / (name + '.obj')) for name, _, _ in units]
        command += ['-o', str(dll)]
        result = subprocess.run(command, capture_output=True)
        log = d / 'link.txt'
        log.write_bytes(result.stdout + result.stderr)
        results.append(dict(phase='link', command=command, exit_code=result.returncode,
                            log_sha256=sha(log), diagnostic_bytes=log.stat().st_size))
        print('link', result.returncode, log.stat().st_size, 'diagnostic bytes', flush=True)
    success = len(results) == 6 and all(row['exit_code'] == 0 for row in results)
    for name, digest in inputs.items():
        assert sha(u / name) == digest, 'Input changed during build: ' + name
    authored = [source for _, source, _ in units if source.is_relative_to(p)]
    authored += [p / 'native/jp_host_types.h', p / 'native/jp_ninja_math.h',
                 p / 'native/jp_task_runtime_layout.h',
                 p / 'third_party/skmp-sh4-fpu/sh4_fpu_approx.h', Path(__file__)]
    report = dict(status='real_native_task_runtime_linked_behavior_unverified' if success else 'native_task_runtime_build_failed',
                  upstream_commit=PIN, external_input_manifest_sha256=sha(manifest),
                  external_input_files_verified=len(inputs), upstream_source_modified=False,
                  commands=results, compiler_sha256=sha(c),
                  lifecycle_function_provenance=provenance,
                  external_generated_source_sha256=sha(cleanup),
                  authored_source_sha256={s.relative_to(r).as_posix(): sha(s) for s in authored},
                  private_DLL=str(dll), DLL_sha256=sha(dll) if success else None,
                  complete_task_source_compiled_with_documented_host_adaptation=True,
                  native_heap_adaptation=dict(original_source_sha256=sha(u / 'src/system/task.c'),
                      generated_source_sha256=sha(adapted_task),
                      reason='Consume free remainders shorter than the native heap header',
                      replacement_count=1, changes_original_32bit_valid_behavior=False),
                  placeholder_lifecycle_functions=False,
                  game_containing_artifact_publishable=False,
                  runtime_behavior_verified=False, full_game=False,
                  overall_completion_percent=10, goal_paused=False)
    (d / 'build_verification.json').write_text(json.dumps(report, indent=2) + '\n')
    raise SystemExit(0 if success else 1)

if __name__ == '__main__':
    main()
