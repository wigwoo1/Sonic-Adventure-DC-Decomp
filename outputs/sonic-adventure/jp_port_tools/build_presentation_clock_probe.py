"""Link byte-preserved actual timer/mark functions in an isolated fixture.
The full source was compiled separately. This extracts a named test boundary,
not a production substitute, and supplies no dummy rendering services.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','presentation','clock','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    parent=a.presentation;pb=json.loads((parent/'build_verification.json').read_text())
    assert pb['exit_code']==0 and pb['diagnostic_bytes']==0
    for n,h in pb['authored_source_sha256'].items():assert sha(r/n)==h
    source=parent/'external_present_native.c';assert sha(source)==pb['generated_source_sha256']
    cb=json.loads((a.clock/'build_verification.json').read_text())
    for n,h in cb['authored_source_sha256'].items():assert sha(r/n)==h
    objects=[a.clock/n for n in cb['object_sha256']]
    for o in objects:assert sha(o)==cb['object_sha256'][o.name]
    d.mkdir(parents=True)
    # frame_step is fixture-owned input, not a replacement production scheduler.
    # Calls below exercise only the actual source's timer and mark algorithms.
    raw=source.read_bytes();names=['timer_stop','marks_slot','marks_find','marks_add','present_untimed']
    selected={name:function(raw,name) for name in names}
    declarations=[]
    for pattern in [rb'^#define STR1[^\n]*\n',rb'^#define MARKS_BITS[^\n]*\n',
        rb'^#define MARKS_MOST[^\n]*\n',rb'^int frame_split;\n',rb'^static int lost;\n',
        rb'^static Uint8 untimed_run;\n',rb'^static int untimed_depth;\n',
        rb'^static struct \{\n    struct task \*task;\n    Uint32 marks;\n\} marks\[MARKS_MOST\];\n',
        rb'^static int marks_used;\n']:
        found=re.findall(pattern,raw,re.M);assert len(found)==1,pattern
        declarations.append(found[0])
    extracted=d/'private_selected_presentation.c'
    extracted.write_bytes(b'#include <ninja.h>\n#include <string.h>\n#include "jp_native_clock.h"\nstruct task;\nextern int frame_step;\n'+
        b''.join(declarations)+b''.join(selected.values()))
    bridge=d/'private_presentation_probe.c'
    bridge.write_text('#include "private_selected_presentation.c"\n'+'''
int frame_step;
__declspec(dllexport) void jp_probe_reset(int split,int step)
{
    frame_split=split;frame_step=step;untimed_depth=0;untimed_run=0;
    memset(marks,0,sizeof marks);marks_used=0;lost=FALSE;
}
__declspec(dllexport) int jp_probe_untimed_depth(void) {return untimed_depth;}
__declspec(dllexport) int jp_probe_marks_used(void) {return marks_used;}
__declspec(dllexport) int jp_probe_lost(void) {return lost;}
__declspec(dllexport) int jp_probe_add(void *key,Uint32 flags)
{
    if(!key)return -1;
    marks_add((struct task *)key,flags);return lost;
}
__declspec(dllexport) int jp_probe_find(void *key,Uint32 *flags)
{
    if(!key || !flags)return 0;
    Uint32 *found=marks_find((struct task *)key);
    if(!found)return 0;
    *flags=*found;return 1;
}
''')
    cmd=list(pb['command']);i=cmd.index('-c');cmd[i+1]=str(bridge);cmd[-1]=str(d/'probe.obj')
    link=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
        '-Wl,/opt:ref','-Wl,/export:present_untimed','-Wl,/export:syTmrGetCount',
        str(d/'probe.obj'),*map(str,objects),'-Wl,kernel32.lib','-o',str(d/'presentation_clock_probe.dll')]
    results=[]
    for index,command in enumerate([cmd,link]):
        run=subprocess.run(command,capture_output=True);log=d/f'build_{index}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:6000]);break
    passed=len(results)==2 and all(v['exit_code']==0 for v in results)
    report=dict(status='isolated_actual_presentation_clock_probe_linked_behavior_unverified' if passed else 'probe_build_failed',
        commands=results,parent_presentation_report_sha256=sha(parent/'build_verification.json'),
        clock_build_report_sha256=sha(a.clock/'build_verification.json'),private_probe_source_sha256=sha(bridge),
        builder_sha256=sha(Path(__file__)),DLL_sha256=sha(d/'presentation_clock_probe.dll') if passed else None,
        selected_private_source_sha256=sha(extracted),
        preserved_actual_function_sha256={n:hashlib.sha256(v).hexdigest() for n,v in selected.items()},
        actual_source_functions=['present_untimed','timer_stop','marks_slot','marks_add','marks_find'],
        fixture_owned_frame_step=True,unrelated_production_functions_linked=False,
        full_presentation_linked=False,game_source_linked=True,game_containing_artifact_publishable=False,
        overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',sum(v['diagnostic_bytes'] for v in results));raise SystemExit(0 if passed else 1)
if __name__=='__main__':main()
