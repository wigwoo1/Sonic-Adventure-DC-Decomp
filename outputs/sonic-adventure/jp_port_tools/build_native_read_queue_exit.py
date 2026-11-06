"""Extend the existing System/Exit registration with the actual loader queue."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,base,d=(getattr(a,n).resolve() for n in ('workspace','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include'):shutil.copytree(path,d/path.name)
    changes=[]
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old))
        changes.append(dict(reason=label,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def scoped(raw,name,old,new,label):
        body=owner_function(raw,name);assert raw.count(body)==1;return raw.replace(body,rep(body,old,new,label))
    originals={};source=p/'native/jp_native_read_queue.c';queue=source.read_bytes();originals['queue']=sha(source)
    queue=rep(queue,'#include "jp_native_read_queue.h"','#include "jp_native_read_queue_exit.h"\n#define JP_BOOT_LIFE_API __declspec(dllimport)\n#include "jp_native_boot_lifetimes.h"','actual queue lifecycle and lower card activity imports')
    queue=rep(queue,'struct jp_read_queue_info info;DWORD thread;int busy;void *probe;','struct jp_read_queue_info info;DWORD thread;int busy;void *probe;uint64_t exit_token;uint32_t exit_terminal,exit_closing,exit_finished,exit_call;','same queue owner holds monotonic terminal authority')
    queue=scoped(queue,'enter','if(!queue.info.initialized)return JP_READ_QUEUE_STATE;','if((queue.exit_closing||queue.exit_terminal)&&!queue.exit_call)return JP_READ_QUEUE_STATE;\n    if(!queue.info.initialized)return JP_READ_QUEUE_STATE;','data calls reject after close begins')
    queue=scoped(queue,'jp_read_queue_initialize','if(queue.info.initialized)return JP_READ_QUEUE_STATE;','if(queue.exit_token||queue.exit_terminal||queue.info.initialized)return JP_READ_QUEUE_STATE;','reject independent reinitialization while delegated')
    queue=scoped(queue,'jp_read_queue_shutdown','if(!queue.info.initialized)return 0;','if((queue.exit_token||queue.exit_terminal)&&!queue.exit_call)return JP_READ_QUEUE_STATE;\n    int active=jp_read_queue_exit_activity();if(active)return active;if(!queue.info.initialized)return 0;','only genuine delegated authority retires queue after actual card/GDFS activity preflight')
    queue+=(p/'native/jp_native_read_queue_exit.inc').read_bytes();(d/'read_queue_with_exit.c').write_bytes(queue)
    source=base/'system_with_history_adoption_guard.c';assert sha(source)==b['history_adoption_guard_generated_source_sha256'];system=source.read_bytes();originals['System']=sha(source)
    imports='#define JP_READ_QUEUE_API __declspec(dllimport)\n#define JP_READ_QUEUE_EXIT_API __declspec(dllimport)\n#define JP_EXIT_READ_QUEUE_API __declspec(dllimport)\n#include "jp_native_system_read_queue.h"\n#include "jp_native_exit_read_queue.h"\n'
    system=rep(system,'/* Exact input ABI,',imports+'\n/* Exact input ABI,','System imports genuine queue without new owner registration')
    system=rep(system,'uint64_t history_token;','uint64_t history_token,read_queue_token;','same System instance retains exact queue token')
    system=rep(system,'static int shutdown_owned(void)','static int system_read_queue_activity(void);\nstatic int system_read_queue_close(void);\nstatic int system_read_queue_release(void);\nstatic int shutdown_owned(void)','queue lifecycle prototypes')
    system=rep(system,'    CLOSE(JP_EXIT_HISTORY,system_history_close());','    CLOSE(JP_EXIT_READ_QUEUE,system_read_queue_close());\n    CLOSE(JP_EXIT_HISTORY,system_history_close());','card then queue then history before sound/GDFS/RTC')
    system=rep(system,'    system_owner.info.window=0;system_owner.info.phase=','    if(system_owner.terminal&&system_owner.read_queue_token){int e=jp_read_queue_exit_finish(system_owner.read_queue_token);if(e)return result(JP_SYSTEM_CLEANUP,JP_EXIT_READ_QUEUE,e);}\n    system_owner.info.window=0;system_owner.info.phase=','commit any already closed queue token during early terminal cleanup')
    system=rep(system,'    int active=system_history_activity();if(active)return active;','    int queue_active=system_read_queue_activity();if(queue_active)return queue_active;\n    int active=system_history_activity();if(active)return active;','actual queue preflight before System handoff/shutdown mutation',2)
    system=scoped(system,'jp_system_bind_exit','((system_owner.info.resources&~resources)&JP_EXIT_HISTORY)?JP_EXIT_HISTORY:JP_EXIT_SOUND','((system_owner.info.resources&~resources)&JP_EXIT_READ_QUEUE)?JP_EXIT_READ_QUEUE:((system_owner.info.resources&~resources)&JP_EXIT_HISTORY)?JP_EXIT_HISTORY:JP_EXIT_SOUND','identify omitted queue mask accurately')
    system=rep(system,'if(!system_owner.terminal){rc=system_history_release();','if(!system_owner.terminal){rc=system_read_queue_release();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_READ_QUEUE,rc);rc=system_history_release();','ordinary successful full shutdown releases nonterminal queue claim')
    system+=(p/'native/jp_native_system_read_queue.inc').read_bytes();(d/'system_with_read_queue.c').write_bytes(system)
    source=base/'exit_with_history.c';assert sha(source)==b['history_exit_generated_source_sha256']['exit_with_history.c'];exit=source.read_bytes();originals['Exit']=sha(source)
    imports='#define JP_READ_QUEUE_API __declspec(dllimport)\n#define JP_READ_QUEUE_EXIT_API __declspec(dllimport)\n#include "jp_native_exit_read_queue.h"\n'
    exit=rep(exit,'#define JP_EXIT_BOOT_SUPPORTED',imports+'#define JP_EXIT_BOOT_SUPPORTED','Exit imports actual queue without System/MainBoot dependency')
    exit=rep(exit,'(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES|JP_EXIT_HISTORY)','(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES|JP_EXIT_HISTORY|JP_EXIT_READ_QUEUE)','new explicit resource bit; all existing mask values preserved')
    exit=rep(exit,'uint64_t history_token;','uint64_t history_token,read_queue_token;','existing final state and bootstrap record carry borrowed queue authority',2)
    exit=scoped(exit,'jp_exit_bind','    uint64_t history_token=bootstrap.history_token;','    if((cfg->resources&JP_EXIT_READ_QUEUE)&&!bootstrap.read_queue_token)return JP_EXIT_ARGUMENT;\n    uint64_t read_queue_token=bootstrap.read_queue_token;\n    uint64_t history_token=bootstrap.history_token;','validate exact transferable queue token before any final claims')
    exit=rep(exit,'state.history_token=history_token;','state.history_token=history_token;state.read_queue_token=read_queue_token;','transfer same queue authority only after every final claim succeeds')
    exit=rep(exit,'    CLOSE_INT(JP_EXIT_HISTORY,jp_history_exit_finish(state.history_token));','    CLOSE_INT(JP_EXIT_READ_QUEUE,jp_read_queue_exit_finish(state.read_queue_token));\n    CLOSE_INT(JP_EXIT_HISTORY,jp_history_exit_finish(state.history_token));','final drain card then queue then history before consumers close')
    exit=scoped(exit,'boot_exit_activity','    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;','    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;\n    uint64_t read_queue_token=state.bound?state.read_queue_token:bootstrap.read_queue_token;\n    if(!rc&&read_queue_token)rc=jp_read_queue_exit_can_close(read_queue_token);\n    else if(!rc&&(mask&JP_EXIT_READ_QUEUE))rc=jp_read_queue_exit_activity();','actual queue/card/GDFS callback preflight before terminal flags and cleanup counters')
    exit+=(p/'native/jp_native_exit_read_queue.inc').read_bytes();(d/'exit_with_read_queue.c').write_bytes(exit)
    compiler=b['commands'][0]['command'][0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    specs=[('read_queue_with_exit.c','read_queue_exit_owner.obj','read_queue_DLL','read_queue_owner.obj'),
        ('exit_with_read_queue.c','exit_read_queue_owner.obj','authored_exit_DLL','exit_history_owner.obj'),
        ('system_with_read_queue.c','system_read_queue_owner.obj','System_DLL','system_history_adoption_owner.obj')]
    commands=[]
    for name,obj,key,previous_obj in specs:
        commands.append(host+['-c',str(d/name),'-o',str(d/obj)])
        previous=next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==Path(b[key]).name);link=[];replaced=0
        for v in previous:
            if v.endswith('.obj') and Path(v).name==previous_obj:link.append(str(d/obj));replaced+=1
            elif v.endswith(('.obj','.lib')) and (d/Path(v).name).exists():link.append(str(d/Path(v).name))
            else:link.append(v)
        assert replaced==1,(key,previous_obj)
        libs=[str(d/('jp_native_frames.lib' if key=='read_queue_DLL' else 'jp_native_read_queue.lib'))]
        link[link.index('-o'):link.index('-o')]=libs;link[-1]=str(d/Path(b[key]).name);commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'read_queue_exit_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_native_read_queue_exit.h','jp_native_read_queue_exit.inc','jp_native_system_read_queue.h','jp_native_system_read_queue.inc','jp_native_exit_read_queue.h','jp_native_exit_read_queue.inc')]+[Path(__file__).resolve()]
    updates.update(status='same_System_Exit_actual_read_queue_lifecycle_linked_unverified',read_queue_exit_baseline_sha256=sha(base/'build_verification.json'),
        read_queue_exit_prior_owner_sha256=originals,read_queue_exit_generated_source_sha256={n:sha(d/n) for n,_,_,_ in specs},read_queue_exit_adaptations=changes,
        commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        read_queue_global_exit_handoff_verified=False,full_game_init_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
