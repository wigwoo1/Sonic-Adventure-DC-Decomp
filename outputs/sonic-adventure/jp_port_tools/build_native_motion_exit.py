"""Delegate the existing motion owner through the same System/bootstrap/Exit."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','motion','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,base,motion,d=(getattr(a,n).resolve() for n in ('workspace','baseline','motion','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');m=read(motion/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for old in (b,m):
        for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(m['DLL'])==m['DLL_sha256'];d.mkdir()
    for folder in (base,motion):
        for path in folder.iterdir():
            target=d/path.name
            if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):
                if target.exists():assert sha(target)==sha(path),path
                else:shutil.copyfile(path,target)
            elif path.is_dir() and path.name in ('include','history_include','game_init_imports') and not target.exists():shutil.copytree(path,target)
    changes=[]
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old))
        changes.append(dict(reason=label,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def scoped(raw,name,old,new,label):
        body=owner_function(raw,name);return raw.replace(body,rep(body,old,new,label))
    originals={}
    raw=(p/'native/jp_native_motion_store.c').read_bytes();originals['motion']=sha(p/'native/jp_native_motion_store.c')
    raw=rep(raw,'#include "jp_native_motion_store.h"','#include "jp_native_motion_exit.h"','same owner exposes lifecycle capability')
    raw=rep(raw,'uint64_t bank_generation[8];int32_t requested_character;','uint64_t bank_generation[8];int32_t requested_character;uint64_t exit_token;uint32_t exit_terminal,exit_closing,exit_finished,exit_call;','lifecycle fields in the existing actual motion owner')
    raw=rep(raw,'static int access_owner(void)','static DWORD motion_owner_thread(void)\n{return store.thread?store.thread:(store.exit_token||store.exit_terminal)?store.info.creator_thread:0;}\nstatic int access_owner(void)','retain claimed creator affinity after actual close')
    raw=rep(raw,'return store.thread&&store.thread!=GetCurrentThreadId()?JP_MOTION_THREAD:store.busy?JP_MOTION_REENTRANT:!store.active?JP_MOTION_STATE:0;',
        'DWORD thread=motion_owner_thread();return thread&&thread!=GetCurrentThreadId()?JP_MOTION_THREAD:store.busy?JP_MOTION_REENTRANT:((store.exit_closing||store.exit_terminal)&&!store.exit_call)?JP_MOTION_STATE:!store.active?JP_MOTION_STATE:0;','block new consumers after delegated close begins')
    raw=rep(raw,'if(store.thread&&store.thread!=GetCurrentThreadId())return JP_MOTION_THREAD;','DWORD thread=motion_owner_thread();if(thread&&thread!=GetCurrentThreadId())return JP_MOTION_THREAD;','same creator check for snapshot/initialize/shutdown',3)
    raw=scoped(raw,'jp_motions_initialize','if(store.busy)return JP_MOTION_REENTRANT;if(store.active)',
        'if(store.busy)return JP_MOTION_REENTRANT;if(store.exit_token||store.exit_terminal)return result(JP_MOTION_STATE);if(store.active)','no independent reinitialization of delegated or terminal owner')
    raw=scoped(raw,'jp_motions_shutdown','if(store.busy)return JP_MOTION_REENTRANT;if(!store.active)',
        'if(store.busy)return JP_MOTION_REENTRANT;if((store.exit_token||store.exit_terminal)&&!store.exit_call)return result(JP_MOTION_STATE);if(!store.active)','only exact delegated authority may close claimed motion data')
    raw+=(p/'native/jp_native_motion_exit.inc').read_bytes();(d/'motion_store_with_exit.c').write_bytes(raw)
    source=base/'system_with_game_init.c';originals['System']=sha(source);raw=source.read_bytes()
    imports='#define JP_MOTION_API __declspec(dllimport)\n#define JP_MOTION_EXIT_API __declspec(dllimport)\n#define JP_EXIT_MOTION_API __declspec(dllimport)\n#include "jp_native_system_motion.h"\n#include "jp_native_exit_motion.h"\n'
    raw=rep(raw,'/* Exact input ABI,',imports+'\n/* Exact input ABI,','System imports existing motion/Exit authority')
    raw=rep(raw,'uint64_t history_token,read_queue_token,game_init_token;','uint64_t history_token,read_queue_token,game_init_token,motion_token;','same System instance retains motion token')
    raw=rep(raw,'static int shutdown_owned(void)','static int system_motion_activity(int);\nstatic int system_motion_close(void);\nstatic int system_motion_release(void);\nstatic int shutdown_owned(void)','lifecycle prototypes')
    raw=rep(raw,'    CLOSE(JP_EXIT_CARD,','    CLOSE(JP_EXIT_MOTIONS,system_motion_close());\n    CLOSE(JP_EXIT_CARD,','retire actual motion data before callback-producing/card/core teardown')
    raw=rep(raw,'    system_owner.info.window=0;system_owner.info.phase=',
        '    if(system_owner.terminal&&system_owner.motion_token){int e=jp_motion_exit_finish(system_owner.motion_token);if(e)return result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,e);}\n    system_owner.info.window=0;system_owner.info.phase=','terminalize any already ordinary-closed motion claim on early global cleanup')
    raw=scoped(raw,'jp_system_shutdown','    int queue_active=system_read_queue_activity();','    int motion_active=system_motion_activity(1);if(motion_active)return motion_active;\n    int queue_active=system_read_queue_activity();','live motion views reject ordinary shutdown before main guard or parent mutation')
    raw=scoped(raw,'jp_system_bind_exit','    int queue_active=system_read_queue_activity();','    int motion_active=system_motion_activity(0);if(motion_active)return motion_active;\n    int queue_active=system_read_queue_activity();','handoff identity check permits live motion views')
    raw=scoped(raw,'jp_system_bind_exit','((system_owner.info.resources&~resources)&JP_EXIT_READ_QUEUE)?','((system_owner.info.resources&~resources)&JP_EXIT_MOTIONS)?JP_EXIT_MOTIONS:((system_owner.info.resources&~resources)&JP_EXIT_READ_QUEUE)?','accurate omitted motion resource reporting')
    raw=rep(raw,'if(!system_owner.terminal){rc=system_read_queue_release();','if(!system_owner.terminal){rc=system_motion_release();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,rc);rc=system_read_queue_release();','release ordinary motion authority only after complete parent cleanup')
    raw+=(p/'native/jp_native_system_motion.inc').read_bytes();(d/'system_with_motions.c').write_bytes(raw)
    source=base/'exit_with_read_queue.c';originals['Exit']=sha(source);raw=source.read_bytes()
    imports='#define JP_MOTION_API __declspec(dllimport)\n#define JP_MOTION_EXIT_API __declspec(dllimport)\n#include "jp_native_exit_motion.h"\n'
    raw=rep(raw,'#define JP_EXIT_BOOT_SUPPORTED',imports+'#define JP_EXIT_BOOT_SUPPORTED','Exit imports motion without a System dependency')
    raw=rep(raw,'(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES|JP_EXIT_HISTORY|JP_EXIT_READ_QUEUE)',
        '(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES|JP_EXIT_HISTORY|JP_EXIT_READ_QUEUE|JP_EXIT_MOTIONS)','new explicit resource bit preserves legacy masks')
    raw=rep(raw,'uint64_t history_token,read_queue_token;','uint64_t history_token,read_queue_token,motion_token;','existing bootstrap and final state retain the same token',2)
    raw=rep(raw,'static int boot_exit_activity(void);','static int motion_retire_preflight(void);\nstatic int boot_exit_activity(void);','strict retirement preflight prototype')
    raw=scoped(raw,'jp_exit_bind','    uint64_t read_queue_token=bootstrap.read_queue_token;',
        '    if((cfg->resources&JP_EXIT_MOTIONS)&&!bootstrap.motion_token)return JP_EXIT_ARGUMENT;\n    uint64_t motion_token=bootstrap.motion_token;\n    uint64_t read_queue_token=bootstrap.read_queue_token;','require transferable actual motion authority before final claims')
    raw=rep(raw,'state.read_queue_token=read_queue_token;','state.read_queue_token=read_queue_token;state.motion_token=motion_token;','transfer exact motion token only after every final claim succeeds')
    raw=scoped(raw,'boot_exit_activity','    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;',
        '    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;\n    uint64_t motion_token=state.bound?state.motion_token:bootstrap.motion_token;\n    if(!rc&&motion_token)rc=jp_motion_exit_can_close(motion_token);\n    else if(!rc&&(mask&JP_EXIT_MOTIONS))rc=jp_motion_exit_activity();','non-retiring capability checks allow live-view adoption/final bind')
    raw=scoped(raw,'jp_exit_cleanup','    state.busy=1;state.info.cleanup_calls++;',
        '    int motion_active=motion_retire_preflight();if(motion_active)return motion_active;\n    state.busy=1;state.info.cleanup_calls++;','live views stop global cleanup before counters terminal flags or parent retirement')
    raw=rep(raw,'    CLOSE_INT(JP_EXIT_CARD,','    CLOSE_INT(JP_EXIT_MOTIONS,jp_motion_exit_finish(state.motion_token));\n    CLOSE_INT(JP_EXIT_CARD,','final motion retirement precedes card callbacks and core teardown')
    raw+=(p/'native/jp_native_exit_motion.inc').read_bytes();(d/'exit_with_motions.c').write_bytes(raw)
    source=base/'main_boot_with_game_init.c';originals['MainBoot']=sha(source);raw=source.read_bytes()
    raw=rep(raw,'#include "jp_native_system.h"','#include "jp_native_system.h"\n#define JP_MOTION_API __declspec(dllimport)\n#define JP_MOTION_EXIT_API __declspec(dllimport)\n#include "jp_native_system_motion.h"','MainBoot imports real same-System cleanup preflight')
    raw=scoped(raw,'jp_main_boot_shutdown','    if(!timeout_ms||timeout_ms>60000)return JP_MAIN_BOOT_ARGUMENT;',
        '    if(!timeout_ms||timeout_ms>60000)return JP_MAIN_BOOT_ARGUMENT;\n    int motion_active=jp_system_motion_cleanup_activity();if(motion_active)return motion_active==JP_SYSTEM_THREAD?JP_MAIN_BOOT_THREAD:motion_active==JP_SYSTEM_REENTRANT?JP_MAIN_BOOT_REENTRANT:result(JP_MAIN_BOOT_CLEANUP,motion_active);','reject before MainBoot can retire original card/backend')
    raw=scoped(raw,'jp_main_boot_shutdown','    boot.busy=1;',
        '    boot.busy=1;\n    if(boot.system_owned){uint32_t retired=0;rc=jp_system_prepare_boot_shutdown(&retired);if(rc){boot.busy=0;return result(JP_MAIN_BOOT_CLEANUP,rc);}\n        if(retired&JP_EXIT_MOTIONS)boot.system_owned=0;if(retired&JP_EXIT_CARD)boot.info.card_owned=0;if(retired&JP_EXIT_PRINT)boot.printed=0;}','motion-owned path uses actual System motion-first shutdown before any card callback; legacy path unchanged')
    (d/'main_boot_with_motions.c').write_bytes(raw)
    compiler=b['commands'][0]['command'][0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    specs=[('motion_store_with_exit.c','motion_exit_owner.obj','motion_DLL','motion_store_owner.obj',m),('exit_with_motions.c','exit_motion_owner.obj','authored_exit_DLL','exit_read_queue_owner.obj',b),
        ('system_with_motions.c','system_motion_owner.obj','System_DLL','system_game_init_owner.obj',b),('main_boot_with_motions.c','main_boot_motion_owner.obj','main_boot_DLL','main_boot_game_init_owner.obj',b)]
    commands=[]
    for name,obj,key,replaced,old in specs:
        common=host
        if key=='main_boot_DLL':
            row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name=='main_boot_with_game_init.c')
            common=row[:row.index('-c')]
        commands.append(common+['-c',str(d/name),'-o',str(d/obj)])
        dll=Path(m['DLL']).name if key=='motion_DLL' else Path(b[key]).name
        old_link=next(row['command'] for row in old['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==dll);link=[];changed=0
        for arg in old_link:
            if arg.endswith('.obj') and Path(arg).name==replaced:link.append(str(d/obj));changed+=1
            elif arg.endswith(('.obj','.lib')) and (d/Path(arg).name).exists():link.append(str(d/Path(arg).name))
            else:link.append(arg)
        assert changed==1,(key,replaced)
        if key in ('authored_exit_DLL','System_DLL'):link[link.index('-o'):link.index('-o')]=[str(d/'jp_native_motion_data.lib')]
        link[-1]=str(d/dll);commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'motion_exit_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()[:7500]
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_native_motion_exit.h','jp_native_motion_exit.inc','jp_native_system_motion.h','jp_native_system_motion.inc','jp_native_exit_motion.h','jp_native_exit_motion.inc')]+[Path(__file__).resolve()]
    sources=dict(b['authored_source_sha256'],**m['authored_source_sha256']);sources.update({x.relative_to(r).as_posix():sha(x) for x in authored})
    generated={name:sha(d/name) for name,obj,key,replaced,old in specs};data_view=d/'motion_view';data_view.mkdir()
    for n in dict(m['generated_source_sha256'],**{'motion_store_with_exit.c':generated['motion_store_with_exit.c']}):shutil.copyfile(d/n,data_view/n)
    md=dict(m,DLL=str(d/'jp_native_motion_data.dll'),DLL_sha256=sha(d/'jp_native_motion_data.dll'),authored_source_sha256=sources,
        generated_source_sha256=dict(m['generated_source_sha256'],**{'motion_store_with_exit.c':generated['motion_store_with_exit.c']}),commands=results+m['commands'],motion_exit_baseline_sha256=sha(motion/'build_verification.json'))
    (data_view/'build_verification.json').write_text(json.dumps(md,indent=2)+'\n')
    updates.update(status='same_System_and_Exit_genuine_motion_authority_live_view_preflight_and_restart_linked_unverified',motion_DLL=str(d/'jp_native_motion_data.dll'),motion_DLL_sha256=sha(d/'jp_native_motion_data.dll'),
        motion_exit_core_baseline_sha256=sha(base/'build_verification.json'),motion_exit_data_baseline_sha256=sha(motion/'build_verification.json'),motion_exit_prior_source_sha256=originals,
        motion_exit_generated_source_sha256=generated,motion_data_view=str(data_view),motion_data_view_sha256=sha(data_view/'build_verification.json'),motion_exit_adaptations=changes,
        commands=results+b['commands'],authored_source_sha256=sources,motion_exit_new_build_commands=8,automatic_motion_exit_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
