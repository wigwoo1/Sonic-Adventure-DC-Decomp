"""Add real creator-thread firmware/PCM scheduling to the qualified graph."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';base=a.baseline.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['sound_exit_generated_sha256'].items():assert sha(base/n)==h,n;shutil.copyfile(base/n,d/n)
    assert sha(base/'external_game_sound.c')==b['game_sound_generated_source_sha256'];shutil.copyfile(base/'external_game_sound.c',d/'external_game_sound.c')
    preserved={};updates={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            source=Path(value);assert sha(source)==b[key+'_sha256'];target=d/source.name
            if not target.exists():shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
            preserved[key]=dict(path=str(source),sha256=sha(source));updates[key]=str(target);updates[key+'_sha256']=sha(target)
    for name in ('jp_native_g2_audio.dll','jp_native_g2_audio.lib','jp_native_aica.dll','jp_native_aica.lib','jp_native_audio.dll'):shutil.copyfile(base/name,d/name)
    shutil.copytree(base/'include',d/'include');shutil.copytree(base/'inputs',d/'inputs')
    for n,h in b['generated_sources_sha256'].items():
        assert sha(base/n)==h,n;target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target)
    changes=[]
    def rep(raw,old,new,reason):
        old,new=old.encode(),new.encode();assert raw.count(old)==1,(reason,raw.count(old))
        changes.append(dict(reason=reason,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()))
        return raw.replace(old,new)
    def patch_function(raw,name,old,new,reason):
        before=function(raw,name);after=rep(before,old,new,reason);return raw.replace(before,after)
    idle=(p/'native/jp_native_frame_wait_windows.c').read_bytes()
    idle=rep(idle,'#include "jp_native_window.h"','#include "jp_native_window.h"\n#include "jp_native_wait_service.h"','same real native wait calls optional owned periodic service')
    idle=rep(idle,'    /* WM_QUIT','    if(jp_wait_service_run())return -1;\n    milliseconds=jp_wait_service_limit(milliseconds);\n    /* WM_QUIT','service before real message wait and cap active sleep period')
    idle=rep(idle,'    return jp_window_pump(window)?1:0;','    if(jp_wait_service_run())return -1;\n    return jp_window_pump(window)?1:0;','service elapsed real time after wait without worker game callbacks')
    (d/'windows_idle_clock.c').write_bytes(idle)
    source=(base/'sound_owner_exit.c').read_bytes()
    declarations='''#include "jp_native_sound_clock.h"
#define JP_WAIT_SERVICE_API __declspec(dllimport)
#include "jp_native_wait_service.h"
extern __declspec(dllimport) uint64_t jp_clock_wall_counts(void);
extern __declspec(dllimport) int jp_clock_status(void);
static struct {struct jp_sound_clock_options options;struct jp_sound_clock_info info;} sound_clock;
static int jp_sound_clock_close_owned(void);
static int thread_error(void)'''
    source=rep(source,'static int thread_error(void)',declarations,'one sound clock shares actual owner, core and thread')
    source=rep(source,'static int cleanup(uint32_t timeout)\n{','static int cleanup(uint32_t timeout)\n{\n    if(jp_sound_clock_close_owned())return JP_SOUND_CLEANUP;','retire timed callback before SDK/frame/transport/core storage')
    for name in ('jp_sound_render','jp_sound_pump'):
        source=patch_function(source,name,'    int rc=begin(1);if(rc)return rc;','    int rc=begin(1);if(rc)return rc;\n    if(sound_clock.info.wait_token)return end(JP_SOUND_STATE);','exclusive firmware progression while timed '+name)
    source+=b'\n'+(p/'native/jp_native_sound_clock.inc').read_bytes();(d/'sound_owner_clock.c').write_bytes(source)
    system=(base/'system_with_sound.c').read_bytes()
    system=rep(system,'#include "jp_native_system_sound.h"','''#include "jp_native_system_sound.h"
#define JP_SOUND_CLOCK_API __declspec(dllimport)
#include "jp_native_sound_clock.h"
#define JP_EXIT_SOUND_CLOCK_API __declspec(dllimport)
#include "jp_native_exit_sound_clock.h"
#define JP_WAIT_SERVICE_API __declspec(dllimport)
#include "jp_native_wait_service.h"
#include "jp_native_system_sound_clock.h"''','compiled System timed startup uses same real cleanup authority')
    system+=b'\n'+(p/'native/jp_native_system_sound_clock.inc').read_bytes();(d/'system_with_sound_clock.c').write_bytes(system)
    exit=(base/'exit_with_sound.c').read_bytes()
    exit=rep(exit,'#include "jp_native_exit_sound.h"','#include "jp_native_exit_sound.h"\n#include "jp_native_exit_sound_clock.h"','same real Exit guards live firmware callbacks before mutation')
    exit=rep(exit,'uint32_t sound_timeout;','uint32_t sound_timeout,sound_activity;','explicit current activity guard in existing final owner')
    for name in ('jp_exit_cleanup','jp_exit_bind'):
        exit=patch_function(exit,name,'if(state.busy||bootstrap.held)return JP_EXIT_REENTRANT;',
            'if(state.busy||bootstrap.held||state.sound_activity)return JP_EXIT_REENTRANT;','reject nested global exit during actual timed sound '+name)
    exit+=b'\n'+(p/'native/jp_native_exit_sound_clock.inc').read_bytes();(d/'exit_with_sound_clock.c').write_bytes(exit)
    compiler=b['commands'][0]['command'][0];common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    present_link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==Path(b['present_DLL']).name))
    old_object=next(x for x in present_link if Path(x).name=='windows_idle.obj');assert Path(old_object).exists()
    old_wait=next(x for x in present_link if Path(x).name=='wait_cancel.obj');assert Path(old_wait).exists()
    link_input_sha256={x:sha(x) for x in present_link if Path(x).suffix in ('.obj','.lib') and Path(x).exists()}
    present_link[present_link.index(old_object)]=str(d/'windows_idle.obj');present_link.insert(3,str(d/'wait_service.obj'));present_link[-1]=str(d/'jp_native_present.dll')
    present_link[present_link.index(old_wait)]=str(d/'wait_retire.obj')
    commands=[common+['-c',str(p/'native/jp_native_wait_service.c'),'-o',str(d/'wait_service.obj')],
        common+['-c',str(d/'windows_idle_clock.c'),'-o',str(d/'windows_idle.obj')],
        common+['-c',str(p/'native/jp_native_vblank_retire.c'),'-o',str(d/'wait_retire.obj')],present_link]
    sdk=[x.replace(str(base),str(d)) for x in b['commands'][0]['command']];commands.append(sdk)
    commands.append(common+['-c',str(d/'sound_owner_clock.c'),'-o',str(d/'sound_owner.obj')])
    sound_link=[x.replace(str(base),str(d)) for x in b['commands'][4]['command']]
    sound_link=[str(d/'jp_native_present.lib') if Path(x).name=='jp_native_present.lib' else x for x in sound_link];commands.append(sound_link)
    commands.append(common+['-c',str(d/'system_with_sound_clock.c'),'-o',str(d/'system.obj')])
    commands.append(common+['-c',str(d/'exit_with_sound_clock.c'),'-o',str(d/'exit.obj')])
    commands.append([x.replace(str(base),str(d)) for x in b['commands'][5]['command']])
    system_link=[x.replace(str(base),str(d)) for x in b['commands'][6]['command']]
    system_link.insert(4,str(d/'jp_native_present.lib'));commands.append(system_link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for key in ('present_DLL','game_sound_DLL','sound_SDK_DLL','System_DLL','authored_exit_DLL'):updates[key+'_sha256']=sha(updates[key])
    authored=[p/'native'/('jp_native_'+n) for n in ('wait_service.c','wait_service.h','sound_clock.h','sound_clock.inc','system_sound_clock.h','system_sound_clock.inc','exit_sound_clock.h','exit_sound_clock.inc','vblank_retire.c')]+[Path(__file__).resolve()]
    updates.update(status='native_elapsed_time_firmware_PCM_clock_wait_service_and_System_start_linked_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        sound_clock_baseline_sha256=sha(base/'build_verification.json'),sound_clock_source_sha256={x.name:sha(x) for x in authored},
        sound_clock_generated_sha256={n:sha(d/n) for n in ('windows_idle_clock.c','sound_owner_clock.c','system_with_sound_clock.c','exit_with_sound_clock.c')},
        sound_clock_native_adaptations=changes,sound_clock_preserved_prior_DLLs=preserved,present_link_inputs_sha256=link_input_sha256,
        original_frame_wait_and_clock_bodies_preserved=True,worker_thread_executes_game_firmware=False,
        sustained_real_time_audio_scheduler_qualified=False,public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
