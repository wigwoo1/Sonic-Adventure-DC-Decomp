"""Compile complete original game_init with checked native owned adapters."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function,PIN
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include'):shutil.copytree(path,d/path.name)
    originals={};changes=[]
    def source(n):
        raw=(u/n).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n]);originals[n]=sha(u/n);return raw
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old))
        changes.append(dict(reason=label,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    system_path=base/'system_with_read_queue.c';assert sha(system_path)==b['read_queue_exit_generated_source_sha256']['system_with_read_queue.c'];system=system_path.read_bytes()
    system=rep(system,'/* Exact input ABI,','#include "jp_native_system_game_init.h"\n\n/* Exact input ABI,','same System exposes constructor capability')
    system=rep(system,'uint64_t history_token,read_queue_token;','uint64_t history_token,read_queue_token,game_init_token;uint32_t game_init_call;','same System owner holds higher construction token')
    guarded=('jp_system_configure','jp_system_initialize','jp_system_bind_exit','jp_system_shutdown','jp_system_start_sound',
        'jp_system_start_sound_clock','jp_system_main_initialization_guard','jp_system_adopt_boot_lifetimes','jp_system_adopt_history',
        'jp_system_start_history','jp_system_adopt_read_queue','jp_system_start_read_queue')
    for n in guarded:
        body=owner_function(system,n);old='if(system_owner.busy)return JP_SYSTEM_REENTRANT;'
        new=old+'if(system_owner.game_init_token&&!system_owner.game_init_call)return JP_SYSTEM_REENTRANT;'
        system=system.replace(body,rep(body,old,new,'higher constructor blocks nested System mutation '+n))
    system+=(p/'native/jp_native_system_game_init.inc').read_bytes();(d/'system_with_game_init.c').write_bytes(system)
    raw=source('src/main.c');body=function(raw,'game_init');assert hashlib.sha256(body).hexdigest()==b['game_init_original_body_sha256']
    adapted=body
    for call,count in (('system_boot_init();',1),('frame_wait();',2),('ADXT_Init();',1),('read_queue_init();',1),('history_init();',1),('gdFsEntryErrFuncAll(loader_note_error, NULL);',1)):
        adapted=rep(adapted,'    '+call,'    '+call+'\n    if(jp_game_init_failed())return;','checked native failure return after genuine '+call,count)
    prefix=raw[:raw.index(b'enum menu_entrance menu_entrance;')]
    declarations=b'enum game_mode game_mode;\nBool loader_error_flag;\n'
    assert declarations.splitlines()[0] in raw and declarations.splitlines()[1] in raw
    prototype=b'''extern int jp_game_init_failed(void);
void jp_game_parent_start(void),jp_game_frame_wait(void),jp_game_adxt_start(void),jp_game_read_queue_start(void),jp_game_history_start(void);
void jp_game_loader_error_bind(GDFS_ERRFUNC,void *);
#define game_init jp_original_game_init
#define system_boot_init jp_game_parent_start
#define frame_wait jp_game_frame_wait
#define ADXT_Init jp_game_adxt_start
#define read_queue_init jp_game_read_queue_start
#define history_init jp_game_history_start
#define gdFsEntryErrFuncAll jp_game_loader_error_bind
'''
    game_source=d/'external_game_init.c';game_source.write_bytes(prefix+declarations+prototype+adapted+b'\n_Static_assert(sizeof(game_mode)==4&&sizeof(loader_error_flag)==4,"original shared main data width");\n')
    loader=source('src/system/loader.c');loader_body=function(loader,'loader_note_error')
    (d/'external_loader_note_error.c').write_bytes(b'#include <shinobi.h>\nextern Bool loader_error_flag;\n#define loader_note_error jp_original_loader_note_error\n'+loader_body)
    sofdec=source('sdk/sofdec/mwsfdini.c');(d/'external_sofdec_preinit.c').write_bytes(sofdec)
    hw=source('sdk/shinobi/syhw.c');mode=b'Uint32 sygIntMode = SYD_INT_MODE_LOCALSTACK;';stack=b'void *sygIntStack = NULL;'
    assert hw.count(b'\n'+mode)==1 and hw.count(b'\n'+stack)==1
    (d/'native_sofdec_config.c').write_bytes(b'#include <sg_xpt.h>\n#include <shinobi.h>\n'+mode+b'\n'+stack+b'\n_Static_assert(sizeof(Uint32)==4&&sizeof(void *)==8,"native preinit configuration width");\n')
    # Preserve the verified native-width SDK compile policy for private source.
    old=next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name=='external_game_init_probe.c')
    common=old[:old.index('-c')];assert '-DGCC' not in common
    imports=d/'game_init_imports';imports.mkdir()
    clock=(p/'native/jp_native_clock.h').read_bytes();assert clock.count(b'__declspec(dllexport)')==1
    (imports/'jp_game_clock_imports.h').write_bytes(clock.replace(b'__declspec(dllexport)',b'__declspec(dllimport)'))
    wait=(p/'native/jp_native_frame_wait.h').read_bytes();assert wait.count(b'"jp_native_clock.h"')==1
    (imports/'jp_game_frame_wait_imports.h').write_bytes(wait.replace(b'"jp_native_clock.h"',b'"jp_game_clock_imports.h"'))
    compiler=common[0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native'),'-I'+str(imports)]
    commands=[host+['-c',str(d/'system_with_game_init.c'),'-o',str(d/'system_game_init_owner.obj')]]
    previous=next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==Path(b['System_DLL']).name);link=[];changed=0
    for v in previous:
        if v.endswith('.obj') and Path(v).name=='system_read_queue_owner.obj':link.append(str(d/'system_game_init_owner.obj'));changed+=1
        elif v.endswith(('.obj','.lib')) and (d/Path(v).name).exists():link.append(str(d/Path(v).name))
        else:link.append(v)
    assert changed==1;link[-1]=str(d/'jp_native_system.dll');commands.append(link)
    parent_row=next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name=='main_boot_with_lifetimes.c')
    parent_path=Path(parent_row[parent_row.index('-c')+1]);assert sha(parent_path)==b['boot_exit_generated_source_sha256']['main_boot_with_lifetimes.c']
    parent=parent_path.read_bytes();parent=rep(parent,'#include "jp_native_system.h"','#include "jp_native_system.h"\n#define JP_SYSTEM_GAME_INIT_API __declspec(dllimport)\n#include "jp_native_system_game_init.h"','MainBoot imports real higher-constructor activity')
    shutdown=owner_function(parent,'jp_main_boot_shutdown');parent=parent.replace(shutdown,rep(shutdown,'if(boot.busy)return JP_MAIN_BOOT_REENTRANT;',
        'if(boot.busy)return JP_MAIN_BOOT_REENTRANT;\n    int active=jp_system_game_init_activity();if(active)return active==JP_SYSTEM_THREAD?JP_MAIN_BOOT_THREAD:JP_MAIN_BOOT_REENTRANT;',
        'reject higher-constructor cleanup before actual card retirement'))
    (d/'main_boot_with_game_init.c').write_bytes(parent)
    parent_common=parent_row[:parent_row.index('-c')];commands.append(parent_common+['-c',str(d/'main_boot_with_game_init.c'),'-o',str(d/'main_boot_game_init_owner.obj')])
    parent_link=next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==Path(b['main_boot_DLL']).name)
    assert sha(parent_link[-1])==b['main_boot_DLL_sha256'];link=[];changed=0
    for v in parent_link:
        if v.endswith('.obj') and Path(v).name=='main_boot_owner.obj':link.append(str(d/'main_boot_game_init_owner.obj'));changed+=1
        elif v.endswith(('.obj','.lib')):
            target=d/Path(v).name
            if not target.exists():shutil.copyfile(v,target)
            link.append(str(target))
        else:link.append(v)
    assert changed==1;link[-1]=str(d/'jp_native_main_boot.dll');commands.append(link)
    units=[('external_game_init.c','game_init_original.obj'),('external_loader_note_error.c','loader_error_original.obj'),
        ('external_sofdec_preinit.c','sofdec_preinit_original.obj'),('native_sofdec_config.c','sofdec_native_config.obj')]
    commands += [common+['-c',str(d/n),'-o',str(d/o)] for n,o in units]
    commands += [host+['-c',str(p/'native/jp_native_game_init.c'),'-o',str(d/'game_init_owner.obj')]]
    labels=('main_boot_DLL','System_DLL','frames_DLL','present_DLL','game_sound_DLL','GDFS_DLL','history_DLL','read_queue_DLL')
    libs=list(dict.fromkeys(str(d/Path(b[k]).with_suffix('.lib').name) for k in labels))
    exports=[]
    for name in ('game_mode','loader_error_flag','sygIntMode','sygIntStack','mwsfdg_intStack'):exports+=['-Xlinker','/export:'+name+',DATA']
    dll=d/'jp_native_game_init.dll';commands.append([compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*[str(d/o) for _,o in units],str(d/'game_init_owner.obj'),*libs,*exports,'-Wl,kernel32.lib','-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'game_init_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_native_game_init.h','jp_native_game_init.c','jp_native_system_game_init.h','jp_native_system_game_init.inc')]+[Path(__file__).resolve()]
    generated=[n for n,_ in units]+['system_with_game_init.c','main_boot_with_game_init.c']
    updates.update(status='complete_original_game_init_with_checked_native_adapters_linked_unverified',game_init_DLL=str(dll),game_init_DLL_sha256=sha(dll),
        game_init_baseline_sha256=sha(base/'build_verification.json'),game_init_original_source_sha256=originals,
        game_init_native_body_sha256=hashlib.sha256(adapted).hexdigest(),game_init_loader_error_body_sha256=hashlib.sha256(loader_body).hexdigest(),
        game_init_generated_source_sha256={n:sha(d/n) for n in generated},game_init_prior_System_source_sha256=sha(system_path),game_init_adaptations=changes,
        game_init_import_header_sha256={x.name:sha(x) for x in imports.iterdir()},
        game_init_prior_MainBoot_source_sha256=sha(parent_path),game_init_new_build_commands=len(results),
        commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        full_game_init_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
