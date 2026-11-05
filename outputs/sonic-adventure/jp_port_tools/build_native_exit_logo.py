"""Real native exit services and unchanged original complete logo loop.
External game source/generated metadata and game-containing DLL stay private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','startup','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,bdir,sdir,c,d=a.workspace,a.upstream,a.baseline,a.startup,a.compiler,a.output;p=r/'outputs/sonic-adventure'
    b=read(bdir/'build_verification.json');s=read(sdir/'build_verification.json')
    assert b['frame_wait_input_trace_replaced'] and s['frame_build_sha256']==sha(bdir/'build_verification.json') and sha(c)==b['compiler_sha256']
    for old in [b,s]:
        for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in s['generated_sources_sha256'].items():assert sha(sdir/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    d.mkdir();(d/'include/progress').mkdir(parents=True);(d/'inputs/progress/demo').mkdir(parents=True)
    for f in (sdir/'inputs/progress/demo').glob('*'):shutil.copyfile(f,d/'inputs/progress/demo'/f.name)
    record=u/'src/progress/recorder.h';header=record.read_bytes();assert header.count(b'extern int demo_next;')==1
    overlay=d/'include/progress/recorder.h';overlay.write_bytes(header.replace(b'extern int demo_next;',b'__declspec(dllimport) extern int demo_next;'))
    game=u/'src/progress/demo.c';raw=game.read_bytes();step=function(raw,'logo_step')
    constants=b'\n'.join(re.search(rb'^#define '+n+rb'[^\r\n]*',raw,re.M).group() for n in [b'LOGO_FRAMES',b'LOGO_FADE_SPEED'])
    source=d/'external_original_complete_logo.c';source.write_bytes((sdir/'external_original_logo_start.c').read_bytes()+b'\n#include <shinobi.h>\n#include "system/pad.h"\n#include "system/sbinit.h"\n#include "progress/demo.h"\n#include "progress/recorder.h"\n'+constants+b'\n'+step)
    common=list(s['commands'][0]['command']);common=common[:common.index('-c')];common.insert(1,'-I'+str(d/'include'));common.insert(1,'-I'+str(d/'inputs'))
    # Header-only ABI checks for every imported lifecycle provider. No game or
    # SDK bodies are compiled into the authored exit support component.
    audit=d/'exit_signature_audit.c';includes=['jp_native_present.h','jp_native_input.h','jp_native_recording.h','jp_native_assets.h','jp_native_frames.h','jp_native_textures.h','jp_native_queue.h','jp_native_render_state.h','system/task.h']
    ints=['jp_present_shutdown','jp_input_shutdown','jp_recording_shutdown','jp_wait_shutdown','jp_assets_release_data','jp_assets_recover_pending','jp_assets_shutdown','jp_queue_reset_frame','jp_frames_shutdown','jp_memory_unbind_source','jp_memory_shutdown','jp_pvm_unbind_destination']
    voids=['task_destroy_all','jp_assets_clear_error','jp_memory_clear_error','jp_textures_shutdown','jp_render_state_shutdown','jp_queue_shutdown','jp_clock_shutdown']
    text='\n'.join('#include "'+n+'"' for n in includes)+'\n'
    for ret,names in [('int',ints),('void',voids)]:
        for n in names:text+='_Static_assert(__builtin_types_compatible_p(__typeof__(&'+n+'),'+ret+' (*)(void)),"'+n+' ABI");\n'
    for n,typ in [('jp_window_close','void (*)(struct jp_window *)'),('jp_gpu_close','void (*)(struct jp_gpu *)'),('jp_gpu_jobs_live','unsigned (*)(struct jp_gpu *)'),('jp_textures_live_surfaces','unsigned (*)(void)'),('jp_textures_allocated_bytes','size_t (*)(void)')]:text+='_Static_assert(__builtin_types_compatible_p(__typeof__(&'+n+'),'+typ+'),"'+n+' ABI");\n'
    audit.write_text(text)
    bridge=p/'native/jp_task_runtime_bridge.c';assert b'JP_EXPORT void jp_heap_reset(void) { heap_init(); }' in bridge.read_bytes()
    native=p/'native/jp_native_exit.c';ec=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    exitdll=d/'jp_native_exit.dll';dll=d/'jp_native_logo_start.dll'
    auditflags=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'src')]
    commands=[auditflags+['-fsyntax-only',str(audit)],ec+['-c',str(native),'-o',str(d/'exit.obj')],
        [str(c),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'exit.obj'),str(Path(b['frames_DLL']).with_suffix('.lib')),str(Path(b['DLL']).with_suffix('.lib')),str(Path(b['GPU_DLL']).with_suffix('.lib')),'-Wl,kernel32.lib','-o',str(exitdll)],
        common+['-c',str(source),'-o',str(d/'logo_complete.obj')]]
    link=list(s['commands'][-1]['command']);link[link.index(str(sdir/'logo.obj'))]=str(d/'logo_complete.obj');link[-1]=str(dll);link[link.index('-o'):link.index('-o')]=[str(exitdll.with_suffix('.lib')),'-Wl,/export:logo_step'];commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:7000]
    result=dict(s,status='complete_original_logo_step_and_real_native_exit_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,DLL=str(dll),DLL_sha256=sha(dll),authored_exit_DLL=str(exitdll),authored_exit_DLL_sha256=sha(exitdll),authored_exit_object_sha256=sha(d/'exit.obj'),authored_exit_component_contains_game_or_SDK_bodies=False,authored_exit_artifact_publishable=True,baseline_startup_build_sha256=sha(sdir/'build_verification.json'),original_logo_step_body_sha256=hashlib.sha256(step).hexdigest(),original_logo_step_body_unchanged=True,original_demo_source_sha256=sha(game),original_recorder_header_sha256=sha(record),private_recorder_header_overlay_sha256=sha(overlay),private_metadata_sha256={f.name:sha(f) for f in (d/'inputs/progress/demo').glob('*')},native_exit_import_signature_checks=len(ints)+len(voids)+5,remaining_logo_imports=[],dummy_exit_services=False,frame_wait_input_trace_replaced=True,original_logo_step_executed=False,playable=False,overall_completion_percent=10)
    result['authored_source_sha256']=dict(b['authored_source_sha256'],**s['authored_source_sha256'])
    for f in [native,p/'native/jp_native_exit.h',bridge,Path(__file__).resolve()]:result['authored_source_sha256'][f.relative_to(r).as_posix()]=sha(f)
    result['generated_sources_sha256']={f.name:sha(f) for f in [source,audit]}
    (d/'source-snapshot').mkdir()
    for f in [native,p/'native/jp_native_exit.h',Path(__file__).resolve()]:(d/'source-snapshot'/f.name).write_bytes(f.read_bytes())
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
