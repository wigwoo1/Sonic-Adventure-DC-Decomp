"""Integrate the complete original music/voice task with native audio ownership.

Preserve original source and all previous qualified adapters/proofs. Generated
game-source copies and combined binaries remain private. No task/clock stubs.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    originals={}
    for n in ('src/sound/stream.c','src/sound/stream.h'):
        originals[n]=sha(u/n);assert originals[n]==hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest(),n
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports','rna_inputs','adxt_inputs'):shutil.copytree(base/n,d/n)
    for n,h in b['generated_sources_sha256'].items():
        src,dst=base/n,d/n;assert sha(src)==h and dst.resolve().is_relative_to(d);dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists():assert sha(dst)==h
        else:shutil.copyfile(src,dst)
    raw=(u/'src/sound/stream.c').read_bytes();exports=set(x.decode() for x in re.findall(rb'^(?:void|int|Sint32|Bool)\s+(stream_\w+)\(',raw,re.M));changes=[]
    def edit(name,old,new,count=1):
        nonlocal raw
        old,new=old.encode(),new.encode();body=function(raw,name);assert body.count(old)==count,(name,old,body.count(old))
        raw=raw.replace(body,body.replace(old,new));changes.append(name)
    def guard(name,preamble,epilogue=''):
        nonlocal raw
        body=function(raw,name);at=body.index(b'{')+1;new=body[:at]+b'\n'+preamble.encode()+body[at:]
        if epilogue:new=new[:-2]+epilogue.encode()+b'}\n'
        raw=raw.replace(body,new);changes.append(name)
    old=b'#define SERVER_FREQ 20';assert raw.count(old)==1;raw=raw.replace(old,b'#define SERVER_FREQ 60')
    for old,new in [(b'static void *music_work = (void *)STREAM_WORK;',b'static void *music_work = NULL;'),(b'static void *voice_work = (void *)(STREAM_WORK + MUSIC_WORK_SIZE);',b'static void *voice_work = NULL;')]:
        assert raw.count(old)==1;raw=raw.replace(old,new)
    guard('stream_ignore_error','    jp_game_stream_error_observe();')
    guard('stream_init_step','    if(jp_game_stream_task_enter())return;','    jp_game_stream_task_leave();\n')
    edit('stream_init_step','        stream_task = task;','''        stream_task = task;
        if(stream_pending_task==task)stream_pending_task=NULL;
        task->destructor=stream_close;''')
    edit('stream_init_step','        ADXT_EntryErrFunc(stream_ignore_error, NULL);','        jp_stream_error_claim();\n        ADXT_EntryErrFunc(stream_ignore_error, NULL);')
    edit('stream_init_step','        ADXF_LoadPartition(VOICE_PARTITION, VOICE_ARCHIVE, voice_ptinfo,\n                           VOICE_FILES);','''        if(jp_game_stream_archive_load()){
            task_destroy(task);jp_game_stream_task_leave();return;
        }''')
    edit('stream_init_step','        music_stream = ADXT_Create(MUSIC_CHANNELS, music_work, MUSIC_WORK_SIZE);','        music_stream = jp_game_stream_player_create(MUSIC_CHANNELS,MUSIC_WORK_SIZE);\n        music_work=music_stream?music_stream->obuf:NULL;')
    edit('stream_init_step','        voice_stream = ADXT_Create(VOICE_CHANNELS, voice_work, VOICE_WORK_SIZE);','        voice_stream = jp_game_stream_player_create(VOICE_CHANNELS,VOICE_WORK_SIZE);\n        voice_work=voice_stream?voice_stream->obuf:NULL;')
    edit('stream_init_step','        task->destructor = stream_close;','''        if(!music_stream||!voice_stream){
            task_destroy(task);jp_game_stream_task_leave();return;
        }
        jp_game_stream_ready_observe();
        task->destructor = stream_close;''')
    guard('stream_step','    if(jp_game_stream_task_enter())return;\n    jp_game_stream_step_observe();','    jp_game_stream_task_leave();\n')
    guard('stream_close','''    int scope=jp_game_stream_close_enter();
    if(stream_task==task)stream_task=NULL;
    if(stream_pending_task==task)stream_pending_task=NULL;
    if(scope<0)return;''','''    int complete=music_stream==NULL&&voice_stream==NULL;
    if(complete){
        if(jp_game_stream_archive_release())complete=0;
        else jp_stream_error_release();
    }
    jp_game_stream_close_leave(scope,complete);
''')
    for name in ('music','voice'):
        edit('stream_close',f'        ADXT_Destroy({name}_stream);\n        {name}_stream = NULL;',f'        if(!jp_game_stream_player_destroy({name}_stream)){{{name}_stream=NULL;{name}_work=NULL;}}')
    edit('stream_close','    stream_reset_music();\n    stream_reset_voice();','    if(!music_stream&&!voice_stream){stream_reset_music();stream_reset_voice();}')
    guard('stream_request_interlude','    int previous_request=music_request;')
    edit('stream_request_interlude','            VARS(task->work)->interlude = music;','            if(!task){music_request=previous_request;jp_game_stream_fail(9);return;}\n            VARS(task->work)->interlude = music;')
    edit('stream_reset','    task = task_create(0, 1, stream_init_step);','''    if(stream_task||stream_pending_task)return;
    task = task_create(0, 1, stream_init_step);
    stream_pending_task=task;
    if(task)task->destructor=jp_stream_pending_close;
    else jp_game_stream_fail(9);''')
    prefix=b'''#include <stdint.h>
#include <ninja.h>
#include <shinobi.h>
#include <sofdec.h>
#include <string.h>
#define GCC
#include "gcc/src/system/port.h"
#include "jp_native_game_stream.h"
static struct task *stream_pending_task;
static void jp_stream_pending_close(struct task *);
static void jp_stream_error_claim(void),jp_stream_error_release(void);
extern int jp_game_stream_task_enter(void),jp_game_stream_close_enter(void);
extern void jp_game_stream_task_leave(void),jp_game_stream_close_leave(int,int);
extern void jp_game_stream_ready_observe(void),jp_game_stream_step_observe(void),jp_game_stream_error_observe(void);
extern void jp_game_stream_fail(int);
extern int jp_game_stream_archive_load(void),jp_game_stream_archive_release(void);
extern void *jp_game_stream_player_create(uint32_t,uint32_t);
extern int jp_game_stream_player_destroy(void *);
extern uint32_t jp_stream_adxf_file_count(uint32_t);
extern int jp_adxt_record_error(void);
'''
    source=d/'external_game_stream.c';source.write_bytes(prefix+raw+b'\n'+(p/'jp_port_tools/game_stream_observation.inc').read_bytes())
    # Move only native API bodies into same-owner internal operations. Public
    # entries retain their original busy/thread guards and exact return codes.
    owner=(base/'sound_owner_adxt.c').read_bytes()
    def internal(name,newname,args):
        nonlocal owner
        body=function(owner,name);prefix=body[:body.index(b'{')]
        inner=body.replace(name.encode()+b'(',newname.encode()+b'(',1)
        marker=b'    int error=begin(1);if(error)return error;';assert inner.count(marker)==1,name
        inner=inner.replace(marker,b'    int error;').replace(b'return end(',b'return (')
        if not inner.startswith(b'static '):inner=b'static '+inner
        wrapper=prefix+b'{\n    int error=begin(1);if(error)return error;\n    return end('+newname.encode()+b'('+args.encode()+b'));\n}\n'
        owner=owner.replace(body,inner+b'\n'+wrapper)
    internal('jp_adxt_create','jp_stream_adxt_create_owned','channels,bytes,out')
    internal('jp_adxt_control','jp_stream_adxt_control_owned','h,destroy')
    internal('jp_adxf_partition_change','jp_stream_adxf_partition_owned','id,filename,count,added')
    marker=b'static int jp_adxt_close_owned(void);';assert owner.count(marker)==1
    owner=owner.replace(marker,marker+b'\n#include "jp_native_game_stream.h"\nstatic int jp_game_stream_close_owned(void);\nint jp_game_stream_blocks_parent(void),jp_game_stream_blocks_partition(uint32_t);')
    marker=b'    if(jp_adxt_close_owned())return JP_SOUND_CLEANUP;';assert owner.count(marker)==1
    owner=owner.replace(marker,b'    if(jp_game_stream_close_owned())return JP_SOUND_CLEANUP;\n'+marker)
    body=function(owner,'jp_adxt_shutdown');marker=b'    if(!sound.owner)';assert body.count(marker)==1
    owner=owner.replace(body,body.replace(marker,b'    if(jp_game_stream_blocks_parent())return end(JP_SOUND_STATE);\n'+marker))
    for name in ('jp_adxf_partition_change','jp_adxf_partition_unload'):
        body=function(owner,name);marker=b'    int error=begin(1);if(error)return error;';assert body.count(marker)==1
        owner=owner.replace(body,body.replace(marker,marker+b'\n    if(jp_game_stream_blocks_partition(id))return end(JP_SOUND_STATE);'))
    owned=d/'sound_owner_stream.c';owned.write_bytes(owner+b'\n'+(p/'native/jp_native_game_stream.inc').read_bytes())
    afs=d/'external_adx_fs.c';afs.write_bytes(afs.read_bytes()+b'\nuint32_t jp_stream_adxf_file_count(uint32_t id){return id<ADXF_PART_MAX&&adxf_ptinfo[id]?(uint32_t)adxf_ptinfo[id]->nfile:0;}\n')
    def command(name):return list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name==name))
    owncmd=[x.replace(str(base),str(d)) for x in command('sound_owner_adxt.c')];owncmd[owncmd.index('-c')+1]=str(owned)
    sdkcmd=[x.replace(str(base),str(d)) for x in command('external_game_sound.c')];common=sdkcmd[:sdkcmd.index('-c')]
    commands=[owncmd,common+['-Wno-unused-but-set-variable','-c',str(source),'-o',str(d/'external_game_stream.obj')],common+['-c',str(afs),'-o',str(d/'external_adx_fs.obj')]]
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link];link[4:4]=[str(d/'external_game_stream.obj')]+['-Wl,/export:'+n for n in sorted(exports)];commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'game_stream_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'game_stream_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value));updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    authored=[Path(__file__).resolve(),p/'native/jp_native_game_stream.h',p/'native/jp_native_game_stream.inc',p/'jp_port_tools/game_stream_observation.inc']
    updates.update(status='complete_original_game_music_voice_task_linked_with_actual_native_owners_runtime_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{src.relative_to(r).as_posix():sha(src) for src in authored}),game_stream_baseline_sha256=sha(base/'build_verification.json'),
        game_stream_original_source_sha256=originals,game_stream_generated_source_sha256=sha(source),game_stream_owner_generated_source_sha256=sha(owned),
        game_stream_adxf_generated_source_sha256=sha(afs),game_stream_exports=sorted(exports),game_stream_adapted_functions=changes,game_stream_preserved_prior_DLLs=preserved,
        adxf_generated_source_sha256=sha(afs),
        game_stream_work_fixed_addresses_replaced=True,game_stream_shared_ADXPD_error_flag_bound_within_same_DLL=True,original_game_task_heap_or_pool_reinitialized=False,
        game_stream_runtime_playback_verified=False,combined_SDK_game_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(exports),'original game stream exports',flush=True)

if __name__=='__main__':main()
