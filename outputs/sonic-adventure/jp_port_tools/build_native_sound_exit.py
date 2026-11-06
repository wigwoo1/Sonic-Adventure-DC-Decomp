"""Extend exact qualified owners with real audio lifetime/exit delegation.

Previous authored sources, headers and proofs stay byte-identical. Private
generated extensions retain all existing APIs/masks and actual owner state.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();base=a.baseline.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    preserved={};updates={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            source=Path(value);assert sha(source)==b[key+'_sha256'];preserved[key]=dict(path=str(source),sha256=sha(source))
            target=d/source.name
            if not target.exists():shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
            updates[key]=str(target);updates[key+'_sha256']=sha(target)
    for name in ('jp_native_g2_audio.dll','jp_native_g2_audio.lib','jp_native_aica.dll','jp_native_aica.lib','jp_native_audio.dll'):
        shutil.copyfile(base/name,d/name)
    shutil.copytree(base/'include',d/'include');shutil.copytree(base/'inputs',d/'inputs')
    for n,h in b['generated_sources_sha256'].items():
        assert sha(base/n)==h,n;target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target)
    assert sha(base/'external_game_sound.c')==b['game_sound_generated_source_sha256']
    shutil.copyfile(base/'external_game_sound.c',d/'external_game_sound.c')
    adaptations=[]
    def rep(raw,old,new,reason,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(reason,raw.count(old))
        adaptations.append(dict(reason=reason,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()))
        return raw.replace(old,new)
    def adapt_function(raw,name,old,new,reason):
        body=function(raw,name);changed=rep(body,old,new,reason);assert raw.count(body)==1;return raw.replace(body,changed)
    # Same C owner, plus guarded lifecycle claim. Never export bypass methods.
    sound=(p/'native/jp_native_game_sound.c').read_bytes()
    sound=rep(sound,'static int thread_error(void)','''#include "jp_native_sound_exit.h"
static struct {struct jp_sound_exit_info info;int call;} sound_delegation;
static int thread_error(void)''','sound lifecycle state shares original lock and actual owner')
    sound=adapt_function(sound,'jp_sound_configure','    if(sound.busy)return leave(JP_SOUND_REENTRANT);','''    if(sound.busy)return leave(JP_SOUND_REENTRANT);
    if(sound_delegation.info.token||sound_delegation.info.terminal)return leave(JP_SOUND_STATE);''','claimed sound cannot be reconfigured')
    sound=adapt_function(sound,'jp_sound_initialize','    int rc=begin(0);if(rc)return rc;','''    int rc=begin(0);if(rc)return rc;
    if(sound_delegation.info.token||sound_delegation.info.terminal)return end(JP_SOUND_STATE);''','claimed or terminal sound cannot restart')
    sound=adapt_function(sound,'jp_sound_shutdown','    int rc=begin(0);if(rc)return rc;','''    int rc=begin(0);if(rc)return rc;
    if((sound_delegation.info.token||sound_delegation.info.terminal)&&!sound_delegation.call)return end(JP_SOUND_STATE);''','only exact delegated authority closes claimed sound')
    sound+=b'\n'+(p/'native/jp_native_sound_exit.inc').read_bytes();(d/'sound_owner_exit.c').write_bytes(sound)
    # Extend real global Exit. Classic masks and observation-only bootstrap stay.
    exit=(p/'native/jp_native_exit.c').read_bytes()
    imports='''#define JP_AICA_API __declspec(dllimport)
#define JP_GAME_SOUND_API __declspec(dllimport)
#define JP_SOUND_EXIT_API __declspec(dllimport)
#include "jp_native_game_sound.h"
#include "jp_native_sound_exit.h"
#include "jp_native_exit_sound.h"
'''
    exit=rep(exit,'#include "jp_native_exit.h"','#include "jp_native_exit.h"\n'+imports,'real sound imports in global exit')
    exit=rep(exit,'uint64_t device_token,heap_token,rtc_token,gdfs_token;','uint64_t device_token,heap_token,rtc_token,gdfs_token,sound_token;uint32_t sound_timeout;','real sound token retained by final exit')
    exit=rep(exit,'uint32_t resources;int started;} bootstrap;','uint32_t resources;int started;uint64_t sound_token;uint32_t sound_timeout,held;} bootstrap;','same bootstrap holds prospective sound handoff')
    exit=rep(exit,'        if(state.busy)return JP_EXIT_REENTRANT;','        if(state.busy||bootstrap.held)return JP_EXIT_REENTRANT;','binding cannot reenter active local System cleanup')
    exit=adapt_function(exit,'jp_exit_cleanup','    int e=owner();if(e)return e;if(state.busy)return JP_EXIT_REENTRANT;','    int e=owner();if(e)return e;if(state.busy||bootstrap.held)return JP_EXIT_REENTRANT;','guard rejects cleanup before terminal bootstrap-start mutation')
    exit=rep(exit,'~JP_EXIT_SUPPORTED','~JP_EXIT_AUDIO_SUPPORTED','accept explicit sound without changing legacy masks',2)
    exit=rep(exit,'uint64_t gdfs_token,int rejection)','uint64_t gdfs_token,uint64_t sound_token,int rejection)','rollback actual newly acquired sound claims')
    exit=rep(exit,'    if(gdfs_token){int rc=jp_gdfs_exit_cancel', '''    if(sound_token){int rc=jp_sound_exit_cancel(sound_token);if(rc){retained|=JP_EXIT_SOUND;failed_resource=JP_EXIT_SOUND;error=rc;}else sound_token=0;}
    if(gdfs_token){int rc=jp_gdfs_exit_cancel''','sound cancellation precedes borrowed core rollback')
    exit=rep(exit,'state.gdfs_token=gdfs_token;state.info.remaining=retained;','state.gdfs_token=gdfs_token;state.sound_token=sound_token;state.sound_timeout=5000;state.info.remaining=retained;','unexpected rollback failure retains sound authority')
    exit=rep(exit,'uint64_t token=0,heap_token=0,rtc_token=0,gdfs_token=0;','''uint64_t token=0,heap_token=0,rtc_token=0,gdfs_token=0,sound_token=0;
    int borrowed_sound=0;uint32_t sound_timeout=bootstrap.sound_timeout?bootstrap.sound_timeout:5000;''','sound may transfer from same bootstrap instead of double claim')
    exit=rep(exit,'rollback_claims(heap_token,0,0,rc==','rollback_claims(heap_token,0,0,0,rc==','RTC reject leaves no sound claim')
    exit=rep(exit,'rollback_claims(heap_token,rtc_token,0,rc==','rollback_claims(heap_token,rtc_token,0,0,rc==','GDFS reject leaves no sound claim')
    exit=rep(exit,'rollback_claims(heap_token,rtc_token,gdfs_token,rc==','rollback_claims(heap_token,rtc_token,gdfs_token,borrowed_sound?0:sound_token,rc==','late rejection never cancels bootstrap-owned sound')
    exit=rep(exit,'    if(cfg->resources&JP_EXIT_DEVICE){','''    if(cfg->resources&JP_EXIT_SOUND){
        int rc=0;
        if(bootstrap.sound_token){
            struct jp_sound_exit_info actual;struct jp_sound_info sound;
            rc=jp_sound_exit_snapshot(&actual);
            if(!rc)rc=jp_sound_snapshot(&sound);
            if(!rc&&(actual.token!=bootstrap.sound_token||actual.terminal||actual.closing||sound.phase!=JP_SOUND_READY||sound.status))rc=JP_SOUND_STATE;
            if(!rc){sound_token=bootstrap.sound_token;borrowed_sound=1;}
        }else rc=jp_sound_exit_claim(&sound_token);
        if(rc)return rollback_claims(heap_token,rtc_token,gdfs_token,0,rc==JP_SOUND_THREAD?JP_EXIT_THREAD:JP_EXIT_SOUND_STATE);
    }
    if(cfg->resources&JP_EXIT_DEVICE){''','claim or adopt actual sound only after earlier owner validation')
    exit=rep(exit,'state.gdfs_token=gdfs_token;\n    memset(&bootstrap','state.gdfs_token=gdfs_token;state.sound_token=sound_token;state.sound_timeout=sound_timeout;\n    memset(&bootstrap','commit exact sound authority with final core claims')
    exit=rep(exit,'    CLOSE_INT(JP_EXIT_GDFS,jp_gdfs_exit_finish(state.gdfs_token));','''    CLOSE_INT(JP_EXIT_SOUND,jp_sound_exit_finish(state.sound_token,state.sound_timeout));
    CLOSE_INT(JP_EXIT_GDFS,jp_gdfs_exit_finish(state.gdfs_token));''','audio must retire before GDFS/RTC/device/wait/heap')
    exit+=b'\n'+(p/'native/jp_native_exit_sound.inc').read_bytes();(d/'exit_with_sound.c').write_bytes(exit)
    # Extend actual System, preserving one bootstrap cleanup and core APIs.
    system=(p/'native/jp_native_system.c').read_bytes()
    imports='''#define JP_AICA_API __declspec(dllimport)
#define JP_GAME_SOUND_API __declspec(dllimport)
#define JP_SOUND_EXIT_API __declspec(dllimport)
#define JP_EXIT_SOUND_API __declspec(dllimport)
#include "jp_native_game_sound.h"
#include "jp_native_sound_exit.h"
#include "jp_native_exit_sound.h"
#include "jp_native_system_sound.h"
'''
    system=rep(system,'#include "jp_native_exit.h"','#include "jp_native_exit.h"\n'+imports,'compiled System imports actual sound and extended same exit')
    system=rep(system,'int busy,registered,terminal;','int busy,registered,terminal;uint64_t sound_token;uint32_t sound_timeout;','System retains acquired sound cleanup authority')
    system=rep(system,'static int shutdown_owned(void)','static int system_sound_close(void);\nstatic int system_sound_release(void);\nstatic int shutdown_owned(void)','sound cleanup uses same bootstrap state')
    system=rep(system,'    CLOSE(JP_EXIT_GDFS,jp_gdfs_shutdown());','    CLOSE(JP_EXIT_SOUND,system_sound_close());\n    CLOSE(JP_EXIT_GDFS,jp_gdfs_shutdown());','ordinary/early System closes sound before borrowed services')
    system=rep(system,'    system_owner.info.window=0;system_owner.info.phase=', '''    if(system_owner.terminal&&system_owner.sound_token){int rc=jp_sound_exit_finish(system_owner.sound_token,system_owner.sound_timeout);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_SOUND,rc);}
    system_owner.info.window=0;system_owner.info.phase=''','terminalize a previously nonterminal closed sound claim after partial ordinary cleanup')
    system=adapt_function(system,'jp_system_bind_exit','    struct jp_exit_config cfg=', '''    if((resources&system_owner.info.resources)!=system_owner.info.resources)return result(JP_SYSTEM_EXIT,JP_EXIT_SOUND,JP_EXIT_ARGUMENT);
    struct jp_exit_config cfg=''','final mask must cover dynamically owned sound too')
    system=adapt_function(system,'jp_system_shutdown','    return 0;\n}', '''    if(!system_owner.terminal){rc=system_sound_release();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_SOUND,rc);}
    if(system_owner.registered){rc=jp_exit_hold_bootstrap(bootstrap_cleanup,&system_owner,0);if(rc)return result(JP_SYSTEM_EXIT,0,rc);}
    return 0;
}''','release only fully closed nonterminal sound after old bootstrap is cancelled')
    system=adapt_function(system,'jp_system_shutdown','    system_owner.busy=1;rc=shutdown_owned();system_owner.busy=0;if(rc)return rc;', '''    if(system_owner.registered){rc=jp_exit_hold_bootstrap(bootstrap_cleanup,&system_owner,1);if(rc)return result(JP_SYSTEM_EXIT,0,rc);}
    system_owner.busy=1;rc=shutdown_owned();system_owner.busy=0;
    if(rc){if(system_owner.registered)(void)jp_exit_hold_bootstrap(bootstrap_cleanup,&system_owner,0);return rc;}''','local System cleanup guards same bootstrap across real callbacks')
    system=adapt_function(system,'jp_system_shutdown','        rc=jp_exit_cleanup();if(rc)return result(JP_SYSTEM_CLEANUP,0,rc);','        rc=jp_exit_cleanup();if(rc==JP_EXIT_REENTRANT)return JP_SYSTEM_REENTRANT;if(rc)return result(JP_SYSTEM_CLEANUP,0,rc);','nested final-exit System call cannot poison successful outer cleanup status')
    system+=(b'\n'+(p/'native/jp_native_system_sound.inc').read_bytes());(d/'system_with_sound.c').write_bytes(system)
    compiler=b['commands'][0]['command'][0];common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    sdk=list(b['commands'][0]['command']);sdk=[x.replace(str(base),str(d)) for x in sdk]
    commands=[sdk]+[common+['-c',str(d/source),'-o',str(d/obj)] for source,obj in (('sound_owner_exit.c','sound_owner.obj'),('exit_with_sound.c','exit.obj'),('system_with_sound.c','system.obj'))]
    sound_link=list(b['commands'][2]['command']);sound_link=[x.replace(str(base),str(d)) for x in sound_link]
    commands.append(sound_link)
    libraries=lambda names:[str(d/('jp_native_'+n+'.lib')) for n in names]
    commands.append([compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'exit.obj'),*libraries(('frames','present','window_input','device','sdk_heap','rtc','gdfs','game_sound')),'-Wl,kernel32.lib','-o',str(d/'jp_native_exit.dll')])
    commands.append([compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'system.obj'),*libraries(('frames','window_input','device','sdk_heap','rtc','gdfs','exit','game_sound')),'-Wl,kernel32.lib','-o',str(d/'jp_native_system.dll')])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for key in ('game_sound_DLL','sound_SDK_DLL','System_DLL','authored_exit_DLL'):updates[key+'_sha256']=sha(updates[key])
    authored=[p/'native'/('jp_native_'+n) for n in ('sound_exit.h','sound_exit.inc','exit_sound.h','exit_sound.inc','system_sound.h','system_sound.inc')]+[Path(__file__).resolve()]
    updates.update(status='compiled_actual_sound_system_early_final_exit_ownership_linked_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        sound_exit_baseline_sha256=sha(base/'build_verification.json'),sound_exit_source_sha256={x.name:sha(x) for x in authored},
        sound_exit_generated_sha256={n:sha(d/n) for n in ('sound_owner_exit.c','exit_with_sound.c','system_with_sound.c')},
        sound_exit_native_adaptations=adaptations,sound_exit_preserved_prior_DLLs=preserved,
        sound_exit_mask=524288,sound_core_resource_mask=770050,sound_full_resource_mask=772086,
        original_classic_exit_ABI_and_masks_preserved=True,existing_System_bootstrap_cleanup_extended=True,
        global_audio_exit_or_sustained_clock_pacing_qualified=False,public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
