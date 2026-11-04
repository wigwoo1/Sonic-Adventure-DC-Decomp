"""Privately link unchanged original logo_start with real startup providers.

Compile the full presentation-aware task source as a separate audit object.
Link unchanged startup/allocator/task-allocation function extracts;
task stepping, destruction and replay remain the next integration dependency.
No replacement presentation/input/audio/save hooks are supplied.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser()
    for n in ['workspace', 'upstream', 'build', 'probe', 'task', 'compiler', 'output']:
        ap.add_argument('--' + n, type=Path, required=True)
    a = ap.parse_args()
    r = a.workspace.resolve(); u = a.upstream.resolve(); d = a.output.resolve()
    p = r / 'outputs/sonic-adventure'
    assert d.is_relative_to(r / 'work/sonic-adventure')
    assert not d.is_relative_to(r / 'work/sonic-adventure/build') and not d.exists()
    b = json.loads((a.build / 'build_verification.json').read_bytes())
    pb = json.loads((a.probe / 'build_verification.json').read_bytes())
    tb = json.loads((a.task / 'build_verification.json').read_bytes())
    assert sha(a.compiler) == b['compiler_sha256']
    for n, h in b['authored_source_sha256'].items():
        assert sha(r / n) == h, n
    for n in ['frames_DLL', 'GPU_DLL', 'DLL']:
        assert sha(b[n]) == b[n + '_sha256'], n
    assert pb['asset_build_sha256'] == sha(a.build / 'build_verification.json')
    assert sha(u / 'src/progress/demo.c') == pb['external_game_source_sha256']
    assert sha(u / 'src/system/task.c') == tb['original_source_sha256']
    task = a.task / 'external_task_presentation_native.c'
    assert sha(task) == tb['generated_source_sha256']
    assert tb['original_GCC_presentation_hook_guards_activated'] == 5
    d.mkdir(); (d / 'inputs/progress/demo').mkdir(parents=True)
    for n, entry in pb['private_generated_metadata'].items():
        src = a.probe / 'inputs/progress/demo' / n
        assert sha(src) == entry['sha256']
        (d / 'inputs/progress/demo' / n).write_bytes(src.read_bytes())
    # Append read-only diagnostics in the same translation unit as real state.
    task_source = d / 'external_task_startup.c'
    diagnostics = b'''
__declspec(dllexport) unsigned jp_startup_pool_info(unsigned *out)
{
    struct task *at=task_free_list,*previous=NULL;
    unsigned count=0,levels=0,errors=task_pool==NULL;
    while(at && count<TASK_COUNT) {
        if(at<task_pool || at>=task_pool+TASK_COUNT || at->prev!=previous) {
            errors++;break;
        }
        previous=at;at=at->next;count++;
    }
    if(at)errors++;
    for(unsigned i=0;i<TASK_LEVELS;i++)if(task_levels[i])levels++;
    if(out){out[0]=TASK_COUNT;out[1]=count;out[2]=levels;out[3]=errors;}
    return errors;
}
'''
    task_source.write_bytes(task.read_bytes() + diagnostics)
    task_functions = ['task_idle', 'task_get_current_root_step', 'task_init', 'task_alloc', 'heap_init',
                      'heap_alloc', 'heap_calloc', 'heap_free', 'heap_stats']
    task_raw = task.read_bytes()
    startup_task = d / 'external_task_startup_closure.c'
    startup_task.write_bytes(task_raw[:task_raw.index(b'void task_idle(')] +
                            b'\n'.join(function(task_raw, n) for n in task_functions) + diagnostics)
    raw = (u / 'src/progress/demo.c').read_bytes()
    # Retain the already verified original logo declarations and draw bytes.
    old_probe = (a.probe / 'external_original_logo_draw.c').read_bytes()
    prefix = old_probe[:old_probe.index(b'__declspec(dllexport) int jp_logo_asset_load')]
    enum = raw[raw.index(b'enum logo_mode {'):raw.index(b'\n};', raw.index(b'enum logo_mode {')) + 3]
    start = function(raw, 'logo_start')
    game = d / 'external_original_logo_start.c'
    game.write_bytes(prefix + b'\n#include "system/task.h"\n#include "system/frame.h"\n#include "progress/title.h"\n#include "draw/fade.h"\n' + enum + b'\nstatic enum logo_mode logo_mode;\nstatic int logo_frames;\n' + start + b'''
__declspec(dllexport) void jp_logo_asset_draw(void){logo_draw();}
__declspec(dllexport) NJS_TEXLIST *jp_logo_asset_list(void){return &logo_texlist;}
__declspec(dllexport) int jp_logo_asset_release(void){return njReleaseTexture(&logo_texlist);}
__declspec(dllexport) int jp_logo_mode(void){return logo_mode;}
__declspec(dllexport) int jp_logo_frames(void){return logo_frames;}
''')
    cover_raw = (u / 'src/draw/fade.c').read_bytes()
    cover = d / 'external_original_cover.c'
    cover.write_bytes(b'#include <ninja.h>\nstatic NJS_COLOR fade_cover;\n' +
                      function(cover_raw, 'fade_set_cover') + function(cover_raw, 'fade_get_cover'))
    flags = list(b['commands'][0]['command'])
    flags = flags[:flags.index('-c')] + ['-ffunction-sections', '-fdata-sections',
        '-I' + str(d / 'inputs'), '-I' + str(u / 'modules'), '-I' + str(u / 'events'),
        '-include', str(u / 'gcc/src/system/port.h'), '-DJP_NATIVE_PRESENTATION=1']
    commands = []
    for name, src in [('complete_task_audit', task_source), ('task', startup_task), ('bridge', p / 'native/jp_task_runtime_bridge.c'),
                       ('logo', game), ('cover', cover)]:
        commands.append(flags + ['-c', str(src), '-o', str(d / (name + '.obj'))])
    exports = ['logo_start', 'task_init', 'task_alloc', 'task_get_current_root_step', 'heap_init', 'heap_alloc',
               'heap_free', 'heap_calloc', 'heap_stats', 'fade_set_cover', 'fade_get_cover']
    dll = d / 'jp_native_logo_start.dll'
    commands.append([str(a.compiler), '--target=x86_64-pc-windows-msvc', '-shared', '-fuse-ld=lld',
        *[str(d / (n + '.obj')) for n in ['task', 'bridge', 'logo', 'cover']],
        str(a.build / 'frames/jp_native_frames.lib'), str(Path(b['DLL']).with_suffix('.lib')),
        *['-Wl,/export:' + n for n in exports], '-Wl,/opt:ref', '-Wl,/map:' + str(d / 'startup.map'),
        '-o', str(dll)])
    results = []
    for i, cmd in enumerate(commands):
        run = subprocess.run(cmd, cwd=r, capture_output=True)
        log = d / ('build_' + str(i) + '.txt'); log.write_bytes(run.stdout + run.stderr)
        results.append(dict(command=cmd, exit_code=run.returncode,
                            diagnostic_bytes=log.stat().st_size, log_sha256=sha(log)))
        assert run.returncode == 0 and not log.stat().st_size, log.read_text()[:6000]
    linkmap = (d / 'startup.map').read_text()
    for n in exports:
        assert n in linkmap, n
    for n in ['task_step_all', 'task_destroy', 'present_run', 'present_draw', 'present_forget']:
        assert not any(n in line.split() for line in linkmap.splitlines()), n
    imports = sorted(s['name'] for s in coff_symbols(d / 'complete_task_audit.obj') if s['section'] == 0 and s['value'] == 0)
    assert {'present_run', 'present_draw', 'present_forget'} <= set(imports)
    report = dict(status='real_original_logo_start_native_component_linked_behavior_unverified',
        commands=results, exit_code=0, diagnostic_bytes=0, compiler_sha256=sha(a.compiler),
        DLL=str(dll), DLL_sha256=sha(dll), frames_DLL=b['frames_DLL'],
        GPU_DLL=b['GPU_DLL'], present_DLL=b['DLL'], frame_build=str(a.build.resolve()),
        frame_build_sha256=sha(a.build / 'build_verification.json'),
        metadata_probe_build_sha256=sha(a.probe / 'build_verification.json'),
        full_task_object_imports=imports, full_presentation_aware_task_compiled=True,
        linked_task_functions=task_functions, complete_task_audit_object_linked=False,
        linked_task_body_sha256={n:hashlib.sha256(function(task_raw,n)).hexdigest() for n in task_functions},
        original_logo_start_body_sha256=hashlib.sha256(start).hexdigest(),
        original_logo_start_body_unchanged=True, original_cover_bodies_unchanged=True,
        task_startup_and_allocator_linked=True, task_step_destroy_replay_linked=False,
        substitute_service_hooks=False, playable=False, artifact_publishable=False,
        external_sources_sha256={n:sha(u/n) for n in ['src/progress/demo.c','src/draw/fade.c','src/system/task.c']},
        generated_sources_sha256={x.name:sha(x) for x in [task_source,startup_task,game,cover]},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in [Path(__file__).resolve(),p/'native/jp_task_runtime_bridge.c']},
        overall_completion_percent=10)
    (d / 'build_verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'])


if __name__ == '__main__':
    main()
