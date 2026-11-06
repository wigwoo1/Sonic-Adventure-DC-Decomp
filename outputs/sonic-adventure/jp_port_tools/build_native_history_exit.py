"""Compose History lifecycle authority into the SAME System/Exit registration."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
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
        elif path.is_dir() and path.name=='history_include':shutil.copytree(path,d/path.name)
    changes=[]
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old));changes.append(dict(reason=label,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def scoped(raw,name,old,new,label):
        body=function(raw,name);assert raw.count(body)==1;return raw.replace(body,rep(body,old,new,label))
    def previous(name):
        row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name==name)
        path=Path(row[row.index('-c')+1]);assert sha(path)==b['boot_exit_generated_source_sha256'][name];return path
    originals={}
    history=(p/'native/jp_native_history.c').read_bytes();originals['history_owner']=sha(p/'native/jp_native_history.c')
    history=rep(history,'#include "jp_native_history.h"','#include "jp_native_history_exit.h"\n#define JP_BOOT_LIFE_API __declspec(dllimport)\n#include "jp_native_boot_lifetimes.h"\nstatic int history_exit_card_activity(void);','history owner lifecycle declarations and actual card activity query')
    history=rep(history,'void *work3,*work4;int busy,source_live,created;','void *work3,*work4;int busy,source_live,created;uint64_t exit_token;uint32_t exit_terminal,exit_closing,exit_finished,exit_call;','same actual History owner holds claim flags')
    for n in ('jp_history_configure','jp_history_initialize'):
        history=scoped(history,n,'    int rc=access();if(rc)return rc;','    int rc=access();if(rc)return rc;\n    if(history.exit_token||history.exit_terminal)return JP_HISTORY_STATE;','reject independent '+n)
    for n in ('jp_history_save','jp_history_read','count','jp_history_peer_begin','jp_history_refresh'):
        history=scoped(history,n,'int rc=access();if(rc)return rc;','int rc=access();if(rc)return rc;if(history.exit_closing||history.exit_terminal)return JP_HISTORY_STATE;','retire History producer '+n+' when closing')
    history=scoped(history,'jp_history_shutdown','int rc=access();if(rc)return rc;','int rc=access();if(rc)return rc;if((history.exit_token||history.exit_terminal)&&!history.exit_call)return JP_HISTORY_STATE;','only delegated authority may close History')
    history=scoped(history,'jp_history_shutdown','if(!history.info.resources)return 0;','int active=history_exit_card_activity();if(active)return active;if(!history.info.resources)return 0;','do not retire live card callback consumers')
    history=scoped(history,'jp_history_shutdown','if(history.source_live&&history.info.dirty){','if(history.source_live){history.info.dirty=1;','final elapsed time must flush even after an earlier explicit save')
    history+=(p/'native/jp_native_history_exit.inc').read_bytes();(d/'history_with_exit.c').write_bytes(history)
    system_path=previous('system_with_boot_lifetimes.c');system=system_path.read_bytes();originals['System']=sha(system_path)
    imports='#define JP_HISTORY_API __declspec(dllimport)\n#define JP_HISTORY_EXIT_API __declspec(dllimport)\n#define JP_EXIT_HISTORY_API __declspec(dllimport)\n#include "jp_native_system_history.h"\n#include "jp_native_exit_history.h"\n'
    system=rep(system,'/* Exact input ABI,',imports+'\n/* Exact input ABI,','System imports actual lower History owner')
    system=rep(system,'uint32_t boot_timeout,main_guard;','uint32_t boot_timeout,main_guard;uint64_t history_token;','same System instance holds History claim')
    system=rep(system,'static int shutdown_owned(void)','static int system_history_activity(void);\nstatic int system_history_close(void);\nstatic int system_history_release(void);\nstatic int shutdown_owned(void)','history close prototypes')
    system=rep(system,'    CLOSE(JP_EXIT_CARD,system_boot_close(JP_BOOT_CARD));','    CLOSE(JP_EXIT_CARD,system_boot_close(JP_BOOT_CARD));\n    CLOSE(JP_EXIT_HISTORY,system_history_close());','card completion before history before sound/core')
    system=rep(system,'    system_owner.info.window=0;system_owner.info.phase=','    if(system_owner.terminal&&system_owner.history_token){int e=jp_history_exit_finish(system_owner.history_token);if(e)return result(JP_SYSTEM_CLEANUP,JP_EXIT_HISTORY,e);}\n    system_owner.info.window=0;system_owner.info.phase=','commit any already closed history claim on early terminal exit')
    system=scoped(system,'jp_system_bind_exit','    if(system_owner.info.phase!=JP_SYSTEM_READY','    int active=system_history_activity();if(active)return active;\n    if(system_owner.info.phase!=JP_SYSTEM_READY','reject active History before final System handoff')
    system=scoped(system,'jp_system_bind_exit','return result(JP_SYSTEM_EXIT,JP_EXIT_SOUND,JP_EXIT_ARGUMENT);','return result(JP_SYSTEM_EXIT,((system_owner.info.resources&~resources)&JP_EXIT_HISTORY)?JP_EXIT_HISTORY:JP_EXIT_SOUND,JP_EXIT_ARGUMENT);','identify an omitted History resource accurately')
    system=scoped(system,'jp_system_shutdown','    if(system_owner.info.phase==JP_SYSTEM_COLD)return 0;','    int active=system_history_activity();if(active)return active;\n    if(system_owner.main_guard){rc=jp_system_main_initialization_guard(0);if(rc)return rc;}\n    if(system_owner.info.phase==JP_SYSTEM_COLD)return 0;','reject active History before System cleanup mutation')
    system=rep(system,'if(!system_owner.terminal){rc=system_boot_release();','if(!system_owner.terminal){rc=system_history_release();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_HISTORY,rc);rc=system_boot_release();','ordinary close cancels finished nonterminal History claim')
    system+=(p/'native/jp_native_system_history.inc').read_bytes();(d/'system_with_history.c').write_bytes(system)
    exit_path=previous('exit_with_boot_lifetimes.c');exit=exit_path.read_bytes();originals['Exit']=sha(exit_path)
    imports='#define JP_HISTORY_API __declspec(dllimport)\n#define JP_HISTORY_EXIT_API __declspec(dllimport)\n#include "jp_native_exit_history.h"\n'
    exit=rep(exit,'#define JP_EXIT_BOOT_SUPPORTED',imports+'#define JP_EXIT_BOOT_SUPPORTED','Exit imports History without any System dependency')
    exit=rep(exit,'(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES)','(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES|JP_EXIT_HISTORY)','new explicit mask; preserve all legacy bits')
    exit=rep(exit,'uint32_t boot_timeout;','uint32_t boot_timeout;uint64_t history_token;','final Exit and existing bootstrap store exact History token',2)
    exit=scoped(exit,'jp_exit_bind','    uint64_t card_token=bootstrap.card_token,print_token=bootstrap.print_token;','    if((cfg->resources&JP_EXIT_HISTORY)&&!bootstrap.history_token)return JP_EXIT_ARGUMENT;\n    uint64_t history_token=bootstrap.history_token;\n    uint64_t card_token=bootstrap.card_token,print_token=bootstrap.print_token;','validate transferable actual History authority before any claims')
    exit=rep(exit,'state.boot_timeout=boot_timeout;','state.boot_timeout=boot_timeout;state.history_token=history_token;','transfer same existing History token only after all final claims succeed')
    exit=rep(exit,'    CLOSE_INT(JP_EXIT_CARD,jp_boot_lifetime_finish(JP_BOOT_CARD,state.card_token,state.boot_timeout));','    CLOSE_INT(JP_EXIT_CARD,jp_boot_lifetime_finish(JP_BOOT_CARD,state.card_token,state.boot_timeout));\n    CLOSE_INT(JP_EXIT_HISTORY,jp_history_exit_finish(state.history_token));','final drain card then History before all remaining consumers')
    exit=scoped(exit,'boot_exit_activity','    return !rc?0:','    uint64_t history_token=state.bound?state.history_token:bootstrap.history_token;\n    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;\n    if(!rc&&history_token)rc=jp_history_exit_can_close(history_token);\n    else if(!rc&&(mask&JP_EXIT_HISTORY))rc=jp_history_exit_activity();\n    return !rc?0:','actual History activity checked before existing terminal mutation')
    exit+=(p/'native/jp_native_exit_history.inc').read_bytes();(d/'exit_with_history.c').write_bytes(exit)
    compiler=b['commands'][0]['command'][0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    specs=[('history_with_exit.c','history_exit_owner.obj','history_DLL','history_owner.obj'),('exit_with_history.c','exit_history_owner.obj','authored_exit_DLL','exit.obj'),('system_with_history.c','system_history_owner.obj','System_DLL','system.obj')]
    commands=[]
    for source,obj,key,previous_obj in specs:
        commands.append(host+['-c',str(d/source),'-o',str(d/obj)])
        old=next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==Path(b[key]).name)
        link=[]
        for value in old:
            if value.endswith('.obj') and Path(value).name==previous_obj:link.append(str(d/obj))
            elif value.endswith(('.obj','.lib')) and (d/Path(value).name).exists():link.append(str(d/Path(value).name))
            else:link.append(value)
        if key!='history_DLL':link[link.index('-o'):link.index('-o')]=[str(d/'jp_native_history.lib')]
        link[-1]=str(d/Path(b[key]).name);commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'history_exit_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_native_history_exit.h','jp_native_history_exit.inc','jp_native_system_history.h','jp_native_system_history.inc','jp_native_exit_history.h','jp_native_exit_history.inc')]+[Path(__file__).resolve()]
    updates.update(status='real_History_claims_same_System_registration_and_global_Exit_linked_unverified',history_exit_baseline_sha256=sha(base/'build_verification.json'),history_exit_original_owner_sha256=originals,history_exit_generated_source_sha256={n:sha(d/n) for n,_,_,_ in specs},history_exit_adaptations=changes,commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),global_history_exit_runtime_verified=False,full_game_init_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
