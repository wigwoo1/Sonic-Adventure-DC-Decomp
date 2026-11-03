"""Adapt the complete pinned presentation C to native timer/hash boundaries."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','compiler','output','sh4zam']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,c,d=(getattr(a,n).resolve() for n in ['workspace','upstream','compiler','output'])
    p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    manifest=p/'jp_port_tools/jp_presentation_external_inputs.json';pin=json.loads(manifest.read_text())
    base=json.loads((p/'jp_port_tools/jp_task_external_inputs.json').read_text())
    renderer=json.loads((p/'jp_port_tools/jp_renderer_external_inputs.json').read_text())
    def verify():
        for n,h in dict(base['files_sha256'],**pin['files_sha256']).items():assert sha(u/n)==h,n
        for n,h in renderer['sh4zam_files_sha256'].items():assert sha(a.sh4zam/n)==h,n
    verify();d.mkdir(parents=True)
    original=u/'gcc/src/system/present.c';raw=original.read_bytes();changes=[]
    def replace(old,new,expected,reason):
        nonlocal raw
        old,new=old.encode(),new.encode();assert raw.count(old)==expected,(reason,raw.count(old))
        raw=raw.replace(old,new);changes.append(dict(reason=reason,occurrences=expected,
            old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()))
    replace('#define TSTR (*(volatile Uint8 *)0xffd80004)',
            '#include "jp_native_clock.h"',1,'explicit native clock boundary')
    replace('Uint8 run = TSTR;','Uint8 run = jp_clock_read_tstr();',1,'read timer start state')
    replace('TSTR = run & ~STR1;','jp_clock_write_tstr(run & ~STR1);',1,'pause timer 1')
    replace('TSTR = run;','jp_clock_write_tstr(run);',4,'restore timer start state')
    replace('TSTR = untimed_run;','jp_clock_write_tstr(untimed_run);',1,'restore nested untimed state')
    replace('((Uint32)task >> 4)',
        '(((Uint32)(uintptr_t)task ^ (Uint32)((uint64_t)(uintptr_t)task >> 32)) >> 4)',
        1,'fold full native pointer into hash; compare full task identity unchanged')
    source=d/'external_present_native.c';source.write_bytes(raw)
    ground=u/'src/collision/ground.h';header=ground.read_bytes();assert header.count(b'#ifdef GCC')==1
    header=header.replace(b'#ifdef GCC',b'#if defined(GCC) || defined(JP_NATIVE_PRESENTATION)')
    overlay=d/'include/collision/ground.h';overlay.parent.mkdir(parents=True);overlay.write_bytes(header)
    # Explicitly import the port interface; no global GCC/SH-4 target fiction.
    cmd=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off',
        '-ffreestanding','-fno-builtin','-ffunction-sections','-fdata-sections',
        '-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unknown-pragmas',
        '-Wno-ignored-qualifiers','-Wno-unknown-attributes',
        '-Wno-missing-field-initializers','-DJP_NATIVE_PRESENTATION=1',
        '-include',str(p/'native/jp_host_types.h'),'-include',str(p/'native/jp_presentation_host_compat.h'),
        '-I',str(p/'native'),'-I',str(d/'include'),
        *['-I'+str(u/n) for n in ['sdk/include','src','modules','events','gcc/src','.']],
        '-isystem',str(a.sh4zam/'include'),
        '-c',str(source),'-o',str(d/'present.obj')]
    run=subprocess.run(cmd,capture_output=True);log=d/'compile.txt';log.write_bytes(run.stdout+run.stderr)
    verify();symbols=coff_symbols(d/'present.obj') if run.returncode==0 else []
    authored=[Path(__file__),p/'native/jp_native_clock.h',p/'native/jp_host_types.h',
        p/'native/jp_presentation_host_compat.h',p/'native/jp_renderer_host_compat_v2.h',p/'native/jp_sh4zam_host_config.h']
    report=dict(status='complete_native_presentation_object_compiled' if run.returncode==0 else 'native_presentation_compile_failed',
        command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log),
        compiler_sha256=sha(c),external_manifest_sha256=sha(manifest),
        original_source_sha256=sha(original),generated_source_sha256=sha(source),adaptations=changes,
        authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in authored},
        object_sha256=sha(d/'present.obj') if run.returncode==0 else None,
        native_ground_header_sha256=sha(overlay),original_ground_header_sha256=sha(ground),
        ground_header_native_condition_changes=1,
        undefined_symbols=sorted(s['name'] for s in symbols if s['section']==0 and s['value']==0),
        defined_function_symbols=sorted(s['name'] for s in symbols if s['section']>0 and s['function']),
        original_sources_modified=False,game_source_or_object_publishable=False,
        linked_or_runtime_qualified=False,complete_renderer_linked=False,
        overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',log.stat().st_size,'imports',len(report['undefined_symbols']))
    if run.returncode:print(log.read_text()[:6500])
    raise SystemExit(run.returncode)
if __name__=='__main__':main()
