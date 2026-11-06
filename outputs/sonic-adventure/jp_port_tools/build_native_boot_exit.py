"""Compose real card/font claims with existing System and Exit authorities.
All previous qualified source/proof/object bytes remain untouched.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,base,d=(getattr(a,n).resolve() for n in ('workspace','baseline','output'));p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(w)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','inputs','sound_imports','rna_inputs','adxt_inputs','placement_inputs','matching_marker','main_boot_imports'):shutil.copytree(path,d/path.name)
    changes=[]
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old));changes.append(dict(reason=label,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def scoped(raw,name,old,new,label):
        body=function(raw,name);return raw.replace(body,rep(body,old,new,label))
    system=(base/'system_with_sound_clock.c').read_bytes()
    system=rep(system,'/* Exact input ABI,','#define JP_BOOT_LIFE_API __declspec(dllimport)\n#define JP_EXIT_BOOT_API __declspec(dllimport)\n#include "jp_native_boot_exit.h"\n\n/* Exact input ABI,','downward lifetime imports in System')
    system=rep(system,'uint32_t sound_timeout;','uint32_t sound_timeout;uint64_t card_token,print_token;uint32_t boot_timeout,main_guard;','same System owns new tokens')
    system=rep(system,'static int shutdown_owned(void)','static int system_boot_close(uint32_t);\nstatic int system_boot_release(void);\nstatic int shutdown_owned(void)','same-owner close prototypes')
    system=rep(system,'    CLOSE(JP_EXIT_SOUND,system_sound_close());','    CLOSE(JP_EXIT_CARD,system_boot_close(JP_BOOT_CARD));\n    CLOSE(JP_EXIT_SOUND,system_sound_close());','card before sound and core')
    system=rep(system,'    CLOSE(JP_EXIT_SDK_HEAP,jp_sdk_heap_shutdown());','    CLOSE(JP_EXIT_PRINT,system_boot_close(JP_BOOT_PRINT));\n    CLOSE(JP_EXIT_SDK_HEAP,jp_sdk_heap_shutdown());','font after device quiescence before SDK heap')
    system=rep(system,'    system_owner.info.window=0;system_owner.info.phase=', '    if(system_owner.terminal){if(system_owner.card_token){int e=jp_boot_lifetime_finish(0,system_owner.card_token,system_owner.boot_timeout);if(e)return result(JP_SYSTEM_CLEANUP,JP_EXIT_CARD,e);}if(system_owner.print_token){int e=jp_boot_lifetime_finish(1,system_owner.print_token,system_owner.boot_timeout);if(e)return result(JP_SYSTEM_CLEANUP,JP_EXIT_PRINT,e);}}\n    system_owner.info.window=0;system_owner.info.phase=','commit already closed claims on terminal early exit')
    system=rep(system,'    if(!system_owner.terminal){rc=system_sound_release();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_SOUND,rc);}','    if(!system_owner.terminal){rc=system_boot_release();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_MAIN_LIFETIMES,rc);rc=system_sound_release();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_SOUND,rc);}','ordinary close cancels finished claims')
    system+=(p/'native/jp_native_system_boot_exit.inc').read_bytes();(d/'system_with_boot_lifetimes.c').write_bytes(system)
    exit=(base/'exit_with_sound_clock.c').read_bytes()
    exit=rep(exit,'#define JP_DEVICE_API','#define JP_BOOT_LIFE_API __declspec(dllimport)\n#define JP_SYSTEM_BOOT_API __declspec(dllimport)\n#include "jp_native_boot_exit.h"\n#define JP_EXIT_BOOT_SUPPORTED (JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES)\n\n#define JP_DEVICE_API','Exit lifetime imports')
    exit=rep(exit,'uint32_t sound_timeout,sound_activity;','uint32_t sound_timeout,sound_activity;uint64_t card_token,print_token;uint32_t boot_timeout;','final Exit keeps actual tokens')
    exit=rep(exit,'uint32_t sound_timeout,held;} bootstrap;','uint32_t sound_timeout,held;uint64_t card_token,print_token;uint32_t boot_timeout;} bootstrap;','existing bootstrap new token fields')
    exit=rep(exit,'static int rollback_claims(','static int boot_exit_activity(void);\nstatic int rollback_claims(','activity prototype')
    exit=rep(exit,'~JP_EXIT_AUDIO_SUPPORTED','~JP_EXIT_BOOT_SUPPORTED','retain legacy masks and add explicit new bits',2)
    exit=scoped(exit,'jp_exit_bind','    uint64_t token=0,heap_token=0,rtc_token=0,gdfs_token=0,sound_token=0;','    int active=boot_exit_activity();if(active)return active;\n    if(((cfg->resources&JP_EXIT_CARD)&&!bootstrap.card_token)||((cfg->resources&JP_EXIT_PRINT)&&!bootstrap.print_token))return JP_EXIT_ARGUMENT;\n    uint64_t card_token=bootstrap.card_token,print_token=bootstrap.print_token;uint32_t boot_timeout=bootstrap.boot_timeout;\n    uint64_t token=0,heap_token=0,rtc_token=0,gdfs_token=0,sound_token=0;','validate borrowed claims before final bind')
    exit=scoped(exit,'jp_exit_bind','state.sound_token=sound_token;state.sound_timeout=sound_timeout;','state.sound_token=sound_token;state.sound_timeout=sound_timeout;state.card_token=card_token;state.print_token=print_token;state.boot_timeout=boot_timeout;','transfer existing claims only after complete bind success')
    exit=scoped(exit,'jp_exit_cleanup','    state.busy=1;state.info.cleanup_calls++;','    int active=boot_exit_activity();if(active)return active;\n    state.busy=1;state.info.cleanup_calls++;','reject provider reentry before terminal mutation')
    exit=rep(exit,'    CLOSE_INT(JP_EXIT_SOUND,','    CLOSE_INT(JP_EXIT_CARD,jp_boot_lifetime_finish(JP_BOOT_CARD,state.card_token,state.boot_timeout));\n    CLOSE_INT(JP_EXIT_SOUND,','final card before native sound')
    exit=rep(exit,'    if(state.info.remaining&JP_EXIT_TEXTURES){','    CLOSE_INT(JP_EXIT_PRINT,jp_boot_lifetime_finish(JP_BOOT_PRINT,state.print_token,state.boot_timeout));\n    if(state.info.remaining&JP_EXIT_TEXTURES){','print after captured frame retirement before textures')
    exit+=(p/'native/jp_native_exit_boot.inc').read_bytes();(d/'exit_with_boot_lifetimes.c').write_bytes(exit)
    main=(p/'native/jp_native_main_boot.c').read_bytes()
    main=rep(main,'#include "jp_native_main_boot.h"','#include "jp_native_main_boot.h"\n#define JP_BOOT_LIFE_API __declspec(dllimport)\n#define JP_SYSTEM_BOOT_API __declspec(dllimport)\n#define JP_EXIT_BOOT_API __declspec(dllimport)\n#include "jp_native_boot_exit.h"','parent real System delegation imports')
    main=rep(main,'    if(rc)result(JP_MAIN_BOOT_SOUND,rc);','    if(!rc)rc=jp_system_main_initialization_guard(1);\n    if(rc)result(JP_MAIN_BOOT_SOUND,rc);','protect subsequent original font/card constructors')
    main=rep(main,'    struct jp_card_state card;\n    if(jp_card_state_read','    struct jp_card_state card;\n    int observed=jp_card_state_read((uint32_t)save_drive,&card);\n    uint32_t claims=(boot.printed?JP_EXIT_PRINT:0)|(!observed&&card.backend_initialized?JP_EXIT_CARD:0);\n    int adopted=jp_system_adopt_boot_lifetimes(claims,5000);\n    struct jp_system_info acquired;if(!jp_system_snapshot(&acquired)){if(acquired.resources&JP_EXIT_CARD)boot.info.card_owned=0;if(acquired.resources&JP_EXIT_PRINT)boot.printed=0;}\n    if(adopted){boot.busy=0;boot.info.phase=4;return result(JP_MAIN_BOOT_CLEANUP,adopted);}\n    if(jp_card_state_read','adopt actual completed resources even before late parent rejection')
    main=rep(main,'        rc=jp_system_shutdown();','        (void)jp_system_main_initialization_guard(0);\n        rc=jp_system_shutdown();','only release held constructor guard before local System teardown')
    (d/'main_boot_with_lifetimes.c').write_bytes(main)
    # Reuse exact qualified link recipes, changing only these authored owners.
    compiler=b['commands'][0]['command'][0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-I'+str(d/'main_boot_imports')]
    specs=[('exit_with_boot_lifetimes.c','exit.obj','authored_exit_DLL'),('system_with_boot_lifetimes.c','system.obj','System_DLL'),('main_boot_with_lifetimes.c','main_boot_owner.obj','main_boot_DLL')];commands=[];updates={}
    for source,obj,label in specs:
        commands.append(host+['-c',str(d/source),'-o',str(d/obj)])
        old=next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==Path(b[label]).name)
        cmd=list(old)
        for i,value in enumerate(cmd):
            if value.endswith('.obj') and Path(value).name==obj:cmd[i]=str(d/obj)
            elif value.endswith('.obj') and Path(value).name=='main_boot_original.obj':cmd[i]=str(d/'main_boot_original.obj')
            elif value.endswith('.lib') and (d/Path(value).name).exists():cmd[i]=str(d/Path(value).name)
        cmd[-1]=str(d/Path(b[label]).name);commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);(d/f'boot_exit_build_{i}.txt').write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=len(run.stdout+run.stderr)));(d/'boot_exit_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    authored=[p/'native'/name for name in ('jp_native_boot_exit.h','jp_native_system_boot_exit.inc','jp_native_exit_boot.inc')]+[Path(__file__).resolve()]
    updates.update(status='actual_card_font_System_early_and_final_exit_linked_runtime_unverified',boot_exit_baseline_sha256=sha(base/'build_verification.json'),boot_exit_adaptations=changes,boot_exit_generated_source_sha256={name:sha(d/name) for name,_,_ in specs},commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{path.relative_to(r).as_posix():sha(path) for path in authored}),terminal_global_exit_integrated=True,terminal_global_exit_runtime_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
