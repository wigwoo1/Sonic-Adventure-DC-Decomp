"""Compile actual task source with original GCC presentation hooks enabled.
Preserves the previously qualified native heap fix. This is an object, not a
claim that retained-draw lifecycle/replay is already runtime qualified.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pin=json.loads((p/'jp_port_tools/jp_presentation_task_inputs.json').read_text())
    base=json.loads((a.baseline/'build_verification.json').read_text())
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    raw=(a.upstream/'src/system/task.c').read_bytes()
    assert sha(a.upstream/'src/system/task.c')==base['native_heap_adaptation']['original_source_sha256']
    old=b'if (room == need) {';new=b'if (room >= need && room - need < sizeof(struct heap_block)) {'
    assert raw.count(old)==1;raw=raw.replace(old,new)
    assert hashlib.sha256(raw).hexdigest()==base['native_heap_adaptation']['generated_source_sha256']
    # First two GCC guards define the console heap/pool; five later guards
    # select the real forget/run/draw call sites required by presentation.
    guards=list(re.finditer(rb'^#ifdef GCC$',raw,re.M));assert len(guards)==7
    for match in reversed(guards[2:]):
        raw=raw[:match.start()]+b'#if defined(GCC) || defined(JP_NATIVE_PRESENTATION)'+raw[match.end():]
    assert raw.count(b'#if defined(GCC) || defined(JP_NATIVE_PRESENTATION)')==5
    d.mkdir(parents=True);source=d/'external_task_presentation_native.c';source.write_bytes(raw)
    cmd=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off',
        '-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unused-parameter',
        '-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-nostdinc','-DJP_NATIVE_PRESENTATION=1',
        '-include',str(p/'native/jp_host_types.h'),'-include',str(a.upstream/'gcc/src/system/port.h'),
        *['-I'+str(a.upstream/n) for n in ['sdk/include','src','modules','events']],
        '-c',str(source),'-o',str(d/'task_present.obj')]
    run=subprocess.run(cmd,capture_output=True);log=d/'compile.txt';log.write_bytes(run.stdout+run.stderr)
    symbols=coff_symbols(d/'task_present.obj') if run.returncode==0 else []
    original=coff_symbols(a.baseline/'task.obj')
    old_imports={s['name'] for s in original if s['section']==0 and s['value']==0}
    imports=sorted(s['name'] for s in symbols if s['section']==0 and s['value']==0)
    if run.returncode==0:assert set(imports)-old_imports=={'present_forget','present_run','present_draw'}
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    report=dict(status='presentation_aware_native_task_object_compiled' if run.returncode==0 else 'task_object_compile_failed',
        command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log),
        baseline_build_report_sha256=sha(a.baseline/'build_verification.json'),
        original_source_sha256=sha(a.upstream/'src/system/task.c'),generated_source_sha256=sha(source),
        object_sha256=sha(d/'task_present.obj') if run.returncode==0 else None,
        original_GCC_presentation_hook_guards_activated=5,new_required_hooks=['present_draw','present_forget','present_run'],
        native_heap_fix_preserved=True,undefined_symbols=imports,original_source_modified=False,
        builder_sha256=sha(Path(__file__)),runtime_lifecycle_or_replay_verified=False,
        game_containing_artifact_publishable=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',log.stat().st_size)
    if run.returncode:print(log.read_text()[:5000])
    raise SystemExit(run.returncode)
if __name__=='__main__':main()
