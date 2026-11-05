"""Link genuine original recording_update and complete pad_scan privately.

The recorder's only stage_end argument is QUIT. Compile the original stage_end
body as a local always-inlined specialization: the unrelated clear-event branch
is optimized away, not stubbed. The resulting original stage-end field is a
single initial owner; the rest of the stage engine must adopt it when ported.
No complete stage engine, real frame polling or logo loop is claimed here.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','storage','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,c,d=a.workspace,a.upstream,a.compiler,a.output;p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=read(a.baseline/'build_verification.json');s=read(a.storage/'build_verification.json')
    assert b['original_pad_scan_linked'] is False and b['full_pad_link_audit']['remaining_imports']==['recording_update']
    assert b['compiler_sha256']==s['compiler_sha256']==sha(c)
    for report in [b,s]:
        for n,h in report['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.baseline/n)==h,n
    assert sha(a.storage/'recording_storage.obj')==s['object_sha256']
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    d.mkdir();(d/'frames').mkdir()
    raw=(u/'src/progress/recorder.c').read_bytes();stage=(u/'src/stage/stage.c').read_bytes();demo=(u/'src/progress/demo.c').read_bytes()
    changes=[]
    def replace(old,new,why):
        nonlocal raw
        assert raw.count(old)==1,(why,raw.count(old));raw=raw.replace(old,new);changes.append(why)
    # Stage and character function bodies come directly from the pinned source.
    # Keeping the clear-event declaration unresolved ensures that any new
    # non-QUIT use fails the production link until real dependencies exist.
    end=function(stage,'stage_end').replace(b'void stage_end(',b'static __attribute__((always_inline)) inline void jp_recorder_stage_end(')
    field=re.search(rb'^static Sint16 stage_end_kind = STAGE_END_NONE;\r?\n',stage,re.M).group()
    character=re.search(rb'^Uint16 character_current;\r?\n',stage,re.M).group()
    event=re.search(rb'^int demo_event = 0;\r?\n',demo,re.M).group()
    prefix=b'''#include "jp_native_recording.h"
extern struct pad_sample jp_recording_samples[JP_RECORDING_STORAGE_SAMPLES];
#undef DEMO_RECORDING
#define DEMO_RECORDING jp_recording_samples
extern Bool stage_clear_event(void);
'''+field+character+event+function(stage,'get_character')+function(stage,'set_character_number')+end+b'''
__declspec(dllexport) int jp_recorder_stage_end_kind(void){return stage_end_kind;}
__declspec(dllexport) void jp_recorder_stage_reset_end(void){stage_end_kind=STAGE_END_NONE;}
'''
    replace(b'/* the last sample a recording',prefix+b'\n/* the last sample a recording','native storage and original stage/character state')
    replace(b'if (recording_next > RECORD_LAST || (pad_records[0].on & PDD_DGT_TX))',b'if ((uintptr_t)recording_next > (uintptr_t)DEMO_RECORDING + JP_RECORDING_RECORD_LAST_OFFSET || (pad_records[0].on & PDD_DGT_TX))','native record comparison without unrelated pointer ordering')
    replace(b'if (recording_next > PLAY_LAST || sample->on == PAD_SAMPLE_END)',b'if ((uintptr_t)recording_next > (uintptr_t)DEMO_RECORDING + JP_RECORDING_PLAY_LAST_OFFSET || sample->on == PAD_SAMPLE_END)','native inclusive original playback comparison')
    replace(b'    sample = recording_next;',b'''    if (jp_recording_check_cursor(recording_next, recording_mode) != JP_RECORDING_OK) {
        recording_on = FALSE;
        if (recording_mode == RECORDER_PLAY) jp_recorder_stage_end(STAGE_END_QUIT);
        return;
    }
    sample = recording_next;''','validate owned cursor and actual loaded playback extent before access')
    assert raw.count(b'host_save_file(')==6;raw=raw.replace(b'host_save_file(',b'jp_recording_save(');changes.append('six genuine filename branches use actual native atomic saves')
    replace(b'\n            stage_end(STAGE_END_QUIT);',b'\n            jp_recorder_stage_end(STAGE_END_QUIT);','original constant-QUIT stage_end body specialization')
    replace(b'njPrintH(NJM_LOCATION(28, 12), (Uint32)sample, 8);',b'jp_recording_debug_cursor(sample);','real native debugger output of mapped guest cursor')
    recorder=d/'external_recorder.c';recorder.write_bytes(raw)
    pad=d/'external_pad_complete.c';padraw=(a.baseline/'external_pad_full.c').read_bytes()
    helper=(a.baseline/'external_pad_helpers.c').read_bytes();diagnostics=helper[helper.index(b'__declspec(dllexport) void jp_pad_bind_test_ports'):]
    pad.write_bytes(padraw+b'\n'+diagnostics)
    common=list(b['commands'][0]['command']);assert common[-4]=='-c';common=common[:-4]
    commands=[common+['-c',str(src),'-o',str(d/(name+'.obj'))] for src,name in [(recorder,'recorder'),(pad,'pad_complete')]]
    commands.append(common+['-S','-emit-llvm',str(recorder),'-o',str(d/'recorder.ll')])
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    previous=str(a.baseline/'pad_helpers.obj');assert link.count(previous)==1
    link[link.index(previous)]=str(d/'pad_complete.obj')
    exports=['pad_scan','recording_update','get_character','set_character_number']
    globals=['recording_on','recording_mode','recording_kind','demo_next','recording_next','recording_buttons','dat_8c751454','demo_event','character_current']
    at=link.index('-o');link[at:at]=[str(d/'recorder.obj'),str(a.storage/'recording_storage.obj')]+['-Wl,/export:'+n for n in exports]+[v for n in globals for v in ['-Xlinker','/export:'+n+',DATA']]
    link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    presenter=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    oldlib=str(a.baseline/'frames/jp_native_frames.lib');assert presenter.count(oldlib)==1
    presenter[presenter.index(oldlib)]=str(d/'frames/jp_native_frames.lib');presenter[-1]=str(d/'jp_native_present.dll');commands.append(presenter)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:7000]
    ir=(d/'recorder.ll').read_text();assert not re.search(r'\bcall\b[^\n]*@stage_clear_event',ir)
    assert not re.search(r'\bcall\b[^\n]*@jp_recorder_stage_end',ir)
    result=dict(b,status='genuine_original_recorder_and_complete_pad_scan_linked_native_storage_and_QUIT_effect',commands=results,exit_code=0,diagnostic_bytes=0,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),baseline_build_sha256=sha(a.baseline/'build_verification.json'),recording_storage_build_sha256=sha(a.storage/'build_verification.json'),recording_storage_object_sha256=sha(a.storage/'recording_storage.obj'),original_game_source_sha256={n:sha(u/n) for n in ['src/progress/recorder.c','src/progress/recorder.h','src/stage/stage.c','src/progress/demo.c']},recorder_source_adaptations=changes,original_recording_update_linked=True,original_pad_scan_linked=True,full_pad_link_audit=dict(remaining_imports=[],forced_or_dummy_services=False),stage_QUIT_original_body_inlined=True,stage_clear_event_provider_stubbed=False,complete_stage_engine_linked=False,frame_wait_input_trace_replaced=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    result['authored_source_sha256']=dict(b['authored_source_sha256'],**s['authored_source_sha256'])
    result['authored_source_sha256'][Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    result['generated_sources_sha256']={x.name:sha(x) for x in [pad,recorder]}
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
