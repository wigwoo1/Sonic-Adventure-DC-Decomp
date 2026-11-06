"""Integrate the complete pinned ADXF archive/file layer with actual owned native storage."""
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
    n='sdk/sofdec/adx_fs.c';raw=(u/n).read_bytes();original=sha(u/n)
    assert original==hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest()
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for name in ('include','inputs','sound_imports','rna_inputs'):shutil.copytree(base/name,d/name)
    for name,h in b['generated_sources_sha256'].items():
        src,dst=base/name,d/name;assert sha(src)==h and dst.resolve().is_relative_to(d);dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists():assert sha(dst)==h
        else:shutil.copyfile(src,dst)
    def edit(name,old,new,count=1):
        nonlocal raw
        body=function(raw,name);assert body.count(old)==count,(name,old,body.count(old));raw=raw.replace(body,body.replace(old,new))
    old=function(raw,'adxf_wait_1ms');raw=raw.replace(old,b'void adxf_wait_1ms(void){jp_adxf_host_yield();}\n')
    for name,buf in [('ADXF_LoadPartition','adxf_load_buf'),('ADXF_AddPartition','adxf_add_buf')]:
        edit(name,('(Uint32)'+buf).encode(),('(uintptr_t)'+buf).encode())
        anchor=b'    if (ret < 0) {\n        return ret;\n    }'
        edit(name,anchor,anchor+b'\n    if(nmax<=0||nmax>65535)return ADXF_ERR_PRM;')
        if name=='ADXF_AddPartition':
            edit(name,b'    pt->next = adxf_ptinfo[ptid];',b'    if(!adxf_ptinfo[ptid]){ADXF_Close(adxf);return ADXF_ERR_PRM;}\n    pt->next = adxf_ptinfo[ptid];')
        anchor=b'                if (buf[AFS_NFILE] > ADXF_AFS_FILE_MAX) {'
        edit(name,anchor,b'                if((uint32_t)buf[0]!=UINT32_C(0x00534641)||buf[AFS_NFILE]<=0){ret=ADXF_ERR_AFS_FILE;goto done;}\n'+anchor)
    edit('ADXF_LoadPartition',b'                *first = buf[AFS_FILES] / ADXF_SCT_SIZE;',b'                if(buf[AFS_FILES]<0||buf[AFS_FILES]/ADXF_SCT_SIZE>65535){ret=ADXF_ERR_AFS_FILE;goto done;}\n                *first = buf[AFS_FILES] / ADXF_SCT_SIZE;')
    edit('ADXF_LoadPartition',b'                fsize[file++] = (buf[i] + ADXF_SCT_SIZE - 1) / ADXF_SCT_SIZE;',b'                if(buf[i]<0||((uint64_t)buf[i]+ADXF_SCT_SIZE-1)/ADXF_SCT_SIZE>65535){ret=ADXF_ERR_AFS_FILE;goto done;}\n                fsize[file++] = (buf[i] + ADXF_SCT_SIZE - 1) / ADXF_SCT_SIZE;')
    edit('ADXF_AddPartition',b'                    entry = &files[nentry++];',b'                    if(buf[i]<0||buf[i+1]<0||((uint64_t)buf[i+1]+ADXF_SCT_SIZE-1)/ADXF_SCT_SIZE>65535){ret=ADXF_ERR_AFS_FILE;goto done;}\n                    entry = &files[nentry++];')
    edit('adxf_LoadData',b'        if (n > 0) {',b'        if(n<0||(n==0&&ADXF_Tell(adxf)>=ADXF_GetFsizeSct(adxf)))return 0;\n        if (n > 0) {')
    edit('adxf_LoadData',b'        if (ADXF_GetStat(adxf) == ADXF_STAT_READEND) {',b'        if(ADXF_GetStat(adxf)==ADXF_STAT_ERROR)return 0;\n        if (ADXF_GetStat(adxf) == ADXF_STAT_READEND) {')
    edit('adxf_ChkPrmGfr',b'    if (ptid >= ADXF_PART_MAX) {',b'    if (ptid < 0 || ptid >= ADXF_PART_MAX || flid < 0 || adxf_ptinfo[ptid]==NULL) {')
    edit('ADXF_GetPtinfoSize',b'    pt = adxf_ptinfo[ptid];',b'    if(ptid<0||ptid>=ADXF_PART_MAX||adxf_ptinfo[ptid]==NULL)return ADXF_ERR_PRM;\n    pt = adxf_ptinfo[ptid];')
    edit('ADXF_GetFileRange',b'    ret = adxf_ChkPrmGfr(ptid, flid);',b'    if(!fad||!fnsct)return ADXF_ERR_PRM;\n    ret = adxf_ChkPrmGfr(ptid, flid);')
    edit('ADXF_GetFileRange',b'    tbl = (Uint16 *)((char *)pt + sizeof *pt);',b'    if(flid>=pt->nfile){*fad=*fnsct=-1;ret=ADXF_ERR_PRM;goto end;}\n    tbl = (Uint16 *)((char *)pt + sizeof *pt);')
    edit('adxf_CloseSjStm',b'        ADXSTM_Close(adxf->stm);',b'        ADXSTM_Close(adxf->stm);\n        if(jp_adxstm_handle_live(adxf->stm)){jp_adxf_native_error=jp_adxstm_last_native_error();return;}')
    edit('ADXF_Close',b'        if (adxf->stat == ADXF_STAT_READING) {',b'        if (adxf->stm != NULL) {')
    edit('ADXF_Close',b'        memset(adxf, 0, sizeof(ADXF_OBJ));',b'        if(adxf->stm!=NULL){ADXCRS_Unlock();return;}\n        if(adxf->sj!=NULL)adxf_CloseSjStm(adxf);\n        memset(adxf, 0, sizeof(ADXF_OBJ));')
    edit('ADXF_Stop',b'        ADXSTM_Stop(adxf->stm);',b'        ADXSTM_Stop(adxf->stm);\n        if(jp_adxstm_handle_pending(adxf->stm)){jp_adxf_native_error=jp_adxstm_last_native_error();ADXCRS_Unlock();return ADXF_ERR_FATAL;}')
    edit('ADXF_Stop',b'        adxf_CloseSjStm(adxf);',b'        adxf_CloseSjStm(adxf);\n        if(adxf->stm!=NULL){ADXCRS_Unlock();return ADXF_ERR_FATAL;}')
    edit('ADXF_ReadNw32',b'    sj = SJRBF_Create(buf,',b'    if(nsct>adxf->fsize_sct-adxf->skpos)nsct=adxf->fsize_sct-adxf->skpos;\n    if(nsct==0)return 0;\n    if(nsct<0||nsct>INT32_MAX/ADXF_SCT_SIZE)return ADXF_ERR_PRM;\n    sj = SJRBF_Create(buf,')
    edit('ADXF_ReadSj32',b'    ADXCRS_Lock();',b'    if(nsct==0||adxf->skpos>=adxf->fsize_sct)return 0;\n    ADXCRS_Lock();')
    prefix=b'''#include <stdint.h>
#include <machine.h>
#include <shinobi.h>
#include <stddef.h>
#define GCC
extern void jp_adxf_host_yield(void);
extern int jp_adxstm_handle_live(void *);
extern int jp_adxstm_handle_pending(void *);
extern int jp_adxstm_last_native_error(void);
static int jp_adxf_native_error;
/* These original public entries are absent from the abbreviated SDK header. */
extern long ADXF_Tell(void *);
extern long ADXF_GetFsizeSct(void *);
'''
    # ADXF is a private typed pointer, so declare the two extra prototypes after
    # its actual typedef rather than publishing a conflicting void* signature.
    prefix=prefix[:prefix.index(b'/* These original public entries')]
    marker=b'typedef ADXF_OBJ *ADXF;';assert raw.count(marker)==1
    raw=raw.replace(marker,marker+b'\nSint32 ADXF_Tell(ADXF);\nSint32 ADXF_GetFsizeSct(ADXF);')
    source=d/'external_adx_fs.c';source.write_bytes(prefix+raw+b'\n'+(p/'jp_port_tools/adxf_observation.inc').read_bytes())
    stms=d/'external_adx_stms.c';stms.write_bytes(stms.read_bytes()+b'\n'+(p/'jp_port_tools/adxstm_adxf_bridge.inc').read_bytes())
    sdk=(base/'external_game_sound.c').read_bytes();marker=b'extern int jp_adxstm_service_enabled(void);';assert sdk.count(marker)==1
    sdk=sdk.replace(marker,b'extern int jp_adxf_service_enabled(void);\nextern void jp_adxf_service_observe(void);\nextern int jp_adxf_exec_native(void);\n'+marker)
    marker=b'    if(!error&&jp_rna_service_enabled()){';assert sdk.count(marker)==1
    sdk=sdk.replace(marker,b'''    if(!error&&jp_adxf_service_enabled()){
        jp_adxf_service_observe();
        if(jp_adxf_exec_native())return (int32_t)SDE_ERR_UNKNOW_NUM;
    }
'''+marker);(d/'external_game_sound.c').write_bytes(sdk)
    owner=(base/'sound_owner_adxstm.c').read_bytes();marker=b'static int jp_adxstm_close_owned(void);';assert owner.count(marker)==1
    owner=owner.replace(marker,marker+b'\n#include "jp_native_adxf.h"\nstatic int jp_adxf_close_owned(void);\nint jp_adxf_blocks_lower_shutdown(void);')
    marker=b'    if(jp_adxstm_close_owned())return JP_SOUND_CLEANUP;';assert owner.count(marker)==1
    owner=owner.replace(marker,b'    if(jp_adxf_close_owned())return JP_SOUND_CLEANUP;\n'+marker)
    body=function(owner,'jp_adxstm_shutdown');marker=b'    return end(jp_adxstm_close_owned());';assert body.count(marker)==1
    owner=owner.replace(body,body.replace(marker,b'    if(jp_adxf_blocks_lower_shutdown())return end(JP_SOUND_STATE);\n'+marker))
    owned=d/'sound_owner_adxf.c';owned.write_bytes(owner+b'\n'+(p/'native/jp_native_adxf.inc').read_bytes())
    def command(name):return list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name==name))
    sdkcmd=[x.replace(str(base),str(d)) for x in command('external_game_sound.c')];common=sdkcmd[:sdkcmd.index('-c')]
    ownercmd=[x.replace(str(base),str(d)) for x in command('sound_owner_adxstm.c')];ownercmd[ownercmd.index('-c')+1]=str(owned)
    commands=[sdkcmd,ownercmd,common+['-c',str(stms),'-o',str(d/'external_adx_stms.obj')],common+['-c',str(source),'-o',str(d/'external_adx_fs.obj')]]
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link]
    exports=sorted(set(n.decode() for n in re.findall(rb'^(?:void|Sint32|ADXF)\s+(ADXF_[A-Za-z0-9_]+)\(',raw,re.M)))
    link[4:4]=[str(d/'external_adx_fs.obj')]+['-Wl,/export:'+name for name in exports];commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'adxf_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'adxf_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value));updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    for key in ('game_sound_generated_source_sha256','qsound_generated_source_sha256','spatial_generated_source_sha256','sound_math_generated_source_sha256','pstm_generated_source_sha256'):
        if key in b:updates[key]=sha(d/'external_game_sound.c')
    authored=[Path(__file__).resolve(),p/'native/jp_native_adxf.h',p/'native/jp_native_adxf.inc',p/'jp_port_tools/adxf_observation.inc',p/'jp_port_tools/adxstm_adxf_bridge.inc']
    updates.update(status='complete_original_ADXF_archive_file_layer_linked_in_actual_sound_owner_runtime_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{src.relative_to(r).as_posix():sha(src) for src in authored}),
        adxf_baseline_sha256=sha(base/'build_verification.json'),adxf_original_source_sha256={n:original},adxf_generated_source_sha256=sha(source),
        adxf_owner_generated_source_sha256=sha(owned),adxstm_generated_source_sha256=sha(stms),adxf_exports=exports,adxf_preserved_prior_DLLs=preserved,
        adxf_native_adaptations=['Native-width aligned scratch buffers and partition links/header sizing.',
            'Guard missing/negative archive IDs and file indices, empty/bad headers and table-width overflow.',
            'Preserve actual lower stream and owned joint when cancellation/close is rejected.',
            'Clamp buffer reads before size multiplication and return zero at EOF.',
            'Yield native I/O waits; stop retries on fatal read errors/EOF.',
            'Owned guarded partition storage with rollback and shared sound/System cleanup; lower shutdown rejects live parent.'],
        full_ADXT_AFS_voice_playback_or_gameplay_verified=False,combined_SDK_game_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(exports),'original ADXF exports',flush=True)

if __name__=='__main__':main()
