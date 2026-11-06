"""Integrate the whole original ADXSTM file-stream module in the existing sound owner."""
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
    n='sdk/sofdec/adx_stms.c';raw=(u/n).read_bytes();original=sha(u/n)
    assert hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()==original
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for name in ('include','inputs','sound_imports','rna_inputs'):shutil.copytree(base/name,d/name)
    fsbase=Path(b['GDFS_build']);fsproof=fsbase/'build_verification.json'
    assert sha(fsproof)==b['GDFS_build_sha256'];fsmeta=read(fsproof)
    assert fsmeta['DLL_sha256']==b['GDFS_DLL_sha256']
    fsheader=fsbase/'include/shinobi/gdfs.h';assert sha(fsheader)==fsmeta['generated_sources_sha256']['include\\shinobi\\gdfs.h']
    nativeheader=d/'include/shinobi/gdfs.h';nativeheader.parent.mkdir(parents=True,exist_ok=True)
    shutil.copyfile(fsheader,nativeheader)
    for name,h in b['generated_sources_sha256'].items():
        src,dst=base/name,d/name;assert sha(src)==h and dst.resolve().is_relative_to(d)
        dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists():assert sha(dst)==h
        else:shutil.copyfile(src,dst)
    for name in ('ADXSTM_OpenFname','ADXSTM_OpenRange','ADXSTM_OpenFileRange'):
        body=function(raw,name);matches=list(re.finditer(rb'    stm = ADXSTMF_Create\([^;]+;',body));assert len(matches)==1
        at=matches[0].end();changed=body[:at]+b'\n    if(stm==NULL){gdFsClose(gdfs);return NULL;}'+body[at:]
        raw=raw.replace(body,changed)
    body=function(raw,'ADXSTM_Stop');old=b'        gdFsStopRd(obj->gdfs);';assert body.count(old)==1
    changed=body.replace(old,b'        Sint32 rc=gdFsStopRd(obj->gdfs);\n        if(rc!=GDD_ERR_OK){jp_adxstm_native_error=rc;return;}')
    changed=changed.replace(b'    ADXSTM_OBJ *obj = stm;',b'    ADXSTM_OBJ *obj = stm;\n    jp_adxstm_native_error=0;')
    raw=raw.replace(body,changed)
    body=function(raw,'ADXSTM_Close');changed=body.replace(b'        ADXSTM_Stop(stm);',b'        ADXSTM_Stop(stm);\n        if(jp_adxstm_native_error)return;')
    changed=changed.replace(b'            gdFsClose(obj->gdfs);',b'            gdFsClose(obj->gdfs);\n            if(obj->gdfs->used){jp_adxstm_native_error=GDD_ERR_BUSY;ADXCRS_Unlock();return;}')
    raw=raw.replace(body,changed)
    assert raw.count(b'    GDFS_DIRINFO info;')==3;raw=raw.replace(b'    GDFS_DIRINFO info;',b'    GDFS_DIRINFO info={0};')
    prefix=b'''#include <stdint.h>
#include <machine.h>
#include <shinobi.h>
#include <stddef.h>
#include <shinobi/gdfs.h>
#define GCC
static int jp_adxstm_native_error;
_Static_assert(sizeof(GDFS_HANDLE)==120,"shared native GDFS handle layout");
'''
    source=d/'external_adx_stms.c';source.write_bytes(prefix+raw+b'\n'+(p/'jp_port_tools/adxstm_observation.inc').read_bytes())
    sdk=(base/'external_game_sound.c').read_bytes();marker=b'extern int jp_rna_service_enabled(void);';assert sdk.count(marker)==1
    sdk=sdk.replace(marker,b'extern int jp_adxstm_service_enabled(void);\nextern void jp_adxstm_service_observe(void);\nextern int jp_adxstm_exec_native(void);\n'+marker)
    marker=b'    if(!error&&jp_rna_service_enabled()){';assert sdk.count(marker)==1
    sdk=sdk.replace(marker,b'''    if(!error&&jp_adxstm_service_enabled()){
        jp_adxstm_service_observe();
        if(jp_adxstm_exec_native())return (int32_t)SDE_ERR_UNKNOW_NUM;
    }
'''+marker);(d/'external_game_sound.c').write_bytes(sdk)
    owner=(base/'sound_owner_rna.c').read_bytes();marker=b'static int jp_rna_close_owned(void);';assert owner.count(marker)==1
    owner=owner.replace(marker,marker+b'\n#include "jp_native_adxstm.h"\nstatic int jp_adxstm_close_owned(void);')
    marker=b'    if(jp_rna_close_owned())return JP_SOUND_CLEANUP;';assert owner.count(marker)==1
    owner=owner.replace(marker,b'    if(jp_adxstm_close_owned())return JP_SOUND_CLEANUP;\n'+marker)
    owned=d/'sound_owner_adxstm.c';owned.write_bytes(owner+b'\n'+(p/'native/jp_native_adxstm.inc').read_bytes())
    def command(name):return list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name==name))
    sdkcmd=[x.replace(str(base),str(d)) for x in command('external_game_sound.c')];common=sdkcmd[:sdkcmd.index('-c')]
    ownercmd=[x.replace(str(base),str(d)) for x in command('sound_owner_rna.c')];ownercmd[ownercmd.index('-c')+1]=str(owned)
    commands=[sdkcmd,ownercmd,common+['-c',str(source),'-o',str(d/'external_adx_stms.obj')]]
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link]
    exports=sorted(set(m.decode() for m in re.findall(rb'^(?:void|Sint32|ADXSTM|ADXSTM_OBJ\s*\*|SJ)\s*(ADXSTM[A-Za-z0-9_]+)\(',raw,re.M)))
    link[4:4]=[str(d/'external_adx_stms.obj')]+['-Wl,/export:'+name for name in exports]
    commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'adxstm_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'adxstm_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'],key;preserved[key]=dict(path=value,sha256=sha(value))
            updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    for key in ('game_sound_generated_source_sha256','qsound_generated_source_sha256','spatial_generated_source_sha256','sound_math_generated_source_sha256','pstm_generated_source_sha256'):
        if key in b:updates[key]=sha(d/'external_game_sound.c')
    authored=[Path(__file__).resolve(),p/'native/jp_native_adxstm.h',p/'native/jp_native_adxstm.inc',p/'jp_port_tools/adxstm_observation.inc']
    updates.update(status='original_ADXSTM_file_stream_module_linked_in_shared_sound_owner_runtime_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{src.relative_to(r).as_posix():sha(src) for src in authored}),
        adxstm_baseline_sha256=sha(base/'build_verification.json'),adxstm_original_source_sha256={n:original},
        adxstm_GDFS_native_header_sha256=sha(nativeheader),
        adxstm_generated_source_sha256=sha(source),adxstm_owner_generated_source_sha256=sha(owned),adxstm_exports=exports,
        adxstm_native_adaptations=['Guard all three exhausted-pool constructors and release the real file handle.',
            'Stop preserves pending read and borrowed joint until actual cancellation succeeds.',
            'Close retains actual stream if native file close does not release its handle.',
            'Initialize metadata query outputs instead of leaking uninitialized stack bytes.',
            'Service and global retirement share the actual sound/System/GDFS owner; no second clock/output.'],
        adxstm_preserved_prior_DLLs=preserved,full_ADXT_AFS_voice_streaming_or_gameplay_verified=False,
        combined_SDK_game_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n')
    print(updates['status'],len(exports),'original stream exports',flush=True)

if __name__=='__main__':main()
