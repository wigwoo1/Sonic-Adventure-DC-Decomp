"""Compile the original loader queue against the actual leased native GDFS."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def owner_function(raw,name):
    matches=list(re.finditer(rb'^[A-Za-z_][^;{}\n]*\b'+name.encode()+rb'\([^;{}]*\)\r?\n\{',raw,re.M));assert len(matches)==1,name
    m=matches[0];depth=1;at=m.end()
    while depth:
        if raw[at]==123:depth+=1
        elif raw[at]==125:depth-=1
        at+=1
    if raw[at:at+2]==b'\r\n':at+=2
    elif raw[at:at+1]==b'\n':at+=1
    return raw[m.start():at]

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    gbase=Path(b['GDFS_build']);g=read(gbase/'build_verification.json')
    assert sha(gbase/'build_verification.json')==b['GDFS_build_sha256'] and sha(g['DLL'])==b['GDFS_DLL_sha256']
    d.mkdir()
    for folder in (base,gbase):
        for path in folder.iterdir():
            if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):
                target=d/path.name
                if not target.exists():shutil.copyfile(path,target)
            elif path.is_dir() and path.name in ('include','history_include') and not (d/path.name).exists():shutil.copytree(path,d/path.name)
    changes=[]
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old))
        changes.append(dict(reason=label,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def scoped(raw,name,old,new,label):
        body=owner_function(raw,name);return raw.replace(body,rep(body,old,new,label))
    owner=(p/'native/jp_native_gdfs_sdk.c').read_bytes()
    owner=rep(owner,'#include "jp_native_gdfs_internal.h"','#include "jp_native_gdfs_read_queue.h"\n#include "jp_native_gdfs_internal.h"','GDFS exports explicit queue lease')
    owner=rep(owner,'void *read_arg,*transfer_arg,*error_arg;','void *read_arg,*transfer_arg,*error_arg;uint64_t read_queue_token;','actual handle callback record carries authority')
    owner=rep(owner,'uint64_t exit_token;int exit_started;','uint64_t exit_token,read_queue_token;int exit_started,read_queue_active;','same GDFS owner holds nonterminal client')
    owner=rep(owner,'static int output_valid(const void *p,size_t bytes)','static int read_queue_handle_access(GDFS);\nstatic int output_valid(const void *p,size_t bytes)','protected handle preflight declaration')
    for name in ('shutdown_owned','jp_gdfs_exit_finish'):
        if name=='shutdown_owned':old='    if(fs.info.phase<JP_GDFS_STARTING)';new='    if(fs.read_queue_token)return JP_GDFS_ACTIVE;\n'+old
        else:old='    fs.exit_started=1;';new='    if(fs.read_queue_token)return JP_GDFS_ACTIVE;\n'+old
        owner=scoped(owner,name,old,new,'do not retire GDFS while queue lease exists '+name)
    owner=scoped(owner,'gdFsReinit','if(fs.exit_token||fs.callback_depth','if(fs.exit_token||fs.read_queue_token||fs.callback_depth','queue lease rejects independent reinitialization')
    for name in ('gdFsOpen','gdFsOpenRange'):
        owner=scoped(owner,name,'fs.records[n].handle=h;','fs.records[n].handle=h;if(fs.read_queue_active&&!fs.callback_depth)fs.records[n].read_queue_token=fs.read_queue_token;','stamp only real queue-owned opens '+name)
    for name in ('gdFsSeek','gdFsRead','gdFsReqRd32','gdFsReqGdRd','gdFsTrans32','gdFsSetTmode','gdFsStopRd','gdFsMovePickup'):
        body=owner_function(owner,name)
        marker='if(begin())return GDD_ERR_INIT;';assert body.count(marker.encode())==1
        owner=scoped(owner,name,marker,marker+'if(!read_queue_handle_access(h)){end();return GDD_ERR_ILLHNDL;}','protect leased mutation '+name)
    for name in ('gdFsClose','gdFsEntryRdEndFunc','gdFsEntryTrEndFunc','gdFsEntryErrFunc'):
        owner=scoped(owner,name,'if(begin())return;','if(begin())return;if(!read_queue_handle_access(h)){end();return;}','protect leased close/callback '+name)
    owner+=(p/'native/jp_native_gdfs_read_queue.inc').read_bytes();(d/'gdfs_with_read_queue.c').write_bytes(owner)
    raw=(u/'src/system/loader.c').read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/system/loader.c'])
    names=('read_queue_init','read_queue_selected_slot','read_queue_add','read_queue_cancel','read_queue_serve')
    bodies={n:function(raw,n) for n in names};originals={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()}
    cancel=bodies['read_queue_cancel']
    cancel=rep(cancel,'    for (i = 0; i < READ_QUEUE_SIZE; i++) {','    if(read_queue_file){if(jp_read_queue_source_close(read_queue_file))return;read_queue_file=NULL;}\n    for (i = 0; i < READ_QUEUE_SIZE; i++) {','cancel closes/drains before releasing slots')
    cancel=rep(cancel,'    if (read_queue_current != 0) {\n        gdFsClose(read_queue_file);\n    }\n','','remove premature unchecked cancellation close')
    serve=bodies['read_queue_serve']
    serve=rep(serve,'    char unused[32];','    char unused[32];(void)unused;','retain original scratch declaration under native warnings')
    serve=rep(serve,'if (status == GDD_STAT_READ)','if (status == GDD_STAT_READ || status == GDD_STAT_SEEK || status == GDD_STAT_BUSY)','all actually pending native states retain ownership')
    at=serve.index(b'        if (status == GDD_STAT_ERR) {');end=serve.index(b'{',at)+1;depth=1
    while depth:
        if serve[end]==123:depth+=1
        elif serve[end]==125:depth-=1
        end+=1
    end=serve.rfind(b'\n',0,end-1)+1;assert serve[end:].startswith(b'        } else {')
    serve=serve[:at]+b'        if (status == GDD_STAT_ERR || status == GDD_STAT_FATAL) {\n            jp_read_queue_source_problem(JP_READ_QUEUE_SOURCE,gdFsGetErrStat(read_queue_file));return;\n'+serve[end:]
    serve=rep(serve,'            gdFsClose(read_queue_file);','            if(status!=GDD_STAT_COMPLETE){jp_read_queue_source_problem(JP_READ_QUEUE_SOURCE,-19);return;}\n            if(!jp_read_queue_source_complete(read_queue_file,(unsigned)read_queue_current-1))return;\n            if(jp_read_queue_source_close(read_queue_file))return;\n            read_queue_file=NULL;jp_read_queue_source_retired((unsigned)read_queue_current-1);','validate actual completed byte count and checked close before retiring slot')
    serve=rep(serve,'    for (i = 0; i < READ_QUEUE_SIZE; i++) {','    if(gdFsGetWorkHn()!=NULL)return;\n    for (i = 0; i < READ_QUEUE_SIZE; i++) {','defer queue start while another real GDFS client reads')
    serve=rep(serve,'            if (read_queue_file == NULL) {\n                return;','            if (read_queue_file == NULL) {\n                jp_read_queue_source_problem(JP_READ_QUEUE_SOURCE,gdFsGetGlobalErr());return;','actual missing open fails explicitly')
    serve=rep(serve,'            gdFsGetFileSize(read_queue_file, &size);','            if(!gdFsGetFileSize(read_queue_file,&size)){jp_read_queue_source_problem(JP_READ_QUEUE_SOURCE,-15);return;}\n            if(!jp_read_queue_source_validate(read_queue_file,size,(unsigned)i))return;','validate original metadata identity and retained native buffer capacity')
    serve=rep(serve,'            gdFsReqRd32(read_queue_file, size,\n                        read_queue[read_queue_current - 1].dest);','            if(gdFsReqRd32(read_queue_file,size,read_queue[read_queue_current-1].dest)!=size){jp_read_queue_source_problem(JP_READ_QUEUE_SOURCE,-19);return;}','propagate actual short or failed request')
    bodies['read_queue_cancel']=cancel;bodies['read_queue_serve']=serve
    declaration=raw[raw.index(b'#define READ_QUEUE_SIZE'):raw.index(b'static int loader_read(')]
    declaration+=b'static GDFS read_queue_file;\nstatic struct read_request read_queue[READ_QUEUE_SIZE];\nstatic int read_queue_current;\n'
    assert b'#define SECTOR_SIZE 2048' in (u/'src/system/loader.h').read_bytes()
    prefix=b'#include <stdint.h>\n#include <string.h>\n#include <shinobi.h>\n#include "jp_native_read_queue.h"\n#define SECTOR_SIZE 2048\nSint32 gdFsGetGlobalErr(void);\n'
    prefix+=b'void jp_read_queue_source_problem(int,int);\nint jp_read_queue_source_close(void *);\nint jp_read_queue_source_validate(void *,int32_t,uint32_t);\nint jp_read_queue_source_complete(void *,uint32_t);\nvoid jp_read_queue_source_retired(uint32_t);\n'
    prefix+=''.join('#define '+n+' jp_original_'+n+'\n' for n in names).encode()
    bridge=b'''
void jp_read_queue_source_slot(uint32_t i,struct jp_read_queue_slot *out)
{memset(out,0,sizeof *out);memcpy(out->name,read_queue[i].name,32);out->destination=(uintptr_t)read_queue[i].dest;out->state=read_queue[i].state;out->index=i;}
void *jp_read_queue_source_handle(void){return read_queue_file;}
int jp_read_queue_source_output(const void *p,size_t bytes)
{uintptr_t at=(uintptr_t)p;const void *blocks[]={read_queue,&read_queue_file,&read_queue_current};size_t sizes[]={sizeof read_queue,sizeof read_queue_file,sizeof read_queue_current};
for(unsigned i=0;i<3;i++){uintptr_t base=(uintptr_t)blocks[i];if(at<base+sizes[i]&&base<at+bytes)return 0;}return 1;}
'''
    source=d/'external_read_queue.c';source.write_bytes(prefix+declaration+b'\n\n'.join(bodies.values())+bridge)
    compiler=g['commands'][0]['command'][0];sdk=g['commands'][5]['command'][:g['commands'][5]['command'].index('-c')]
    sdk=[('-I'+str(d/'include')) if v=='-I'+str(gbase/'include') else v for v in sdk]
    commands=[sdk+['-c',str(d/'gdfs_with_read_queue.c'),'-o',str(d/'gdfs_read_queue_owner.obj')]]
    link=[]
    for v in g['commands'][-1]['command']:
        if v.endswith('.obj') and Path(v).name=='gdfs_sdk.obj':link.append(str(d/'gdfs_read_queue_owner.obj'))
        elif v.endswith(('.obj','.lib')) and (d/Path(v).name).exists():link.append(str(d/Path(v).name))
        else:link.append(v)
    link[-1]=str(d/'jp_native_gdfs.dll');commands.append(link)
    commands.append(sdk+['-c',str(source),'-o',str(d/'read_queue_original.obj')])
    host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    commands.append(host+['-c',str(p/'native/jp_native_read_queue.c'),'-o',str(d/'read_queue_owner.obj')])
    commands.append([compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'read_queue_original.obj'),str(d/'read_queue_owner.obj'),str(d/'jp_native_gdfs.lib'),'-Wl,kernel32.lib','-o',str(d/'jp_native_read_queue.dll')])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'read_queue_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_native_read_queue.h','jp_native_read_queue.c','jp_native_gdfs_read_queue.h','jp_native_gdfs_read_queue.inc')]+[Path(__file__).resolve()]
    updates.update(status='original_read_queue_and_actual_GDFS_client_authority_linked_unverified',read_queue_DLL=str(d/'jp_native_read_queue.dll'),read_queue_DLL_sha256=sha(d/'jp_native_read_queue.dll'),
        read_queue_baseline_sha256=sha(base/'build_verification.json'),read_queue_original_source_sha256=sha(u/'src/system/loader.c'),read_queue_original_function_sha256=originals,
        read_queue_generated_source_sha256=sha(source),read_queue_GDFS_owner_sha256=sha(d/'gdfs_with_read_queue.c'),read_queue_adaptations=changes,
        commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        full_game_init_executed=False,read_queue_global_exit_handoff_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
