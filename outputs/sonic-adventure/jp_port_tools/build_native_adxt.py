"""Adapt the four complete pinned ADXT modules to the existing native sound owner.

Original source, matching includes and combined game/SDK binaries stay private.
No replacement ADXT state machine, second clock or synthetic firmware completion.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','inputs','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,inputs,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','inputs','output'))
    p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');matching=read(inputs/'verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,e in matching['generated'].items():assert sha(inputs/n)==e['sha256'],n
    names=['sdk/sofdec/'+n+'.c' for n in ('adx_inis','adx_tlk','adx_tlk2','adx_tsvr')]+['sdk/shinobi/sh4tmr.c','sdk/include/sofdec.h']
    originals={}
    for n in names:
        originals[n]=sha(u/n)
        assert originals[n]==hashlib.sha256(subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])).hexdigest(),n
    d.mkdir()
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports','rna_inputs'):shutil.copytree(base/n,d/n)
    for n,h in b['generated_sources_sha256'].items():
        src,dst=base/n,d/n;assert sha(src)==h and dst.resolve().is_relative_to(d);dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists():assert sha(dst)==h
        else:shutil.copyfile(src,dst)
    shutil.copytree(inputs/'includes',d/'adxt_inputs')
    header=(u/'sdk/include/sofdec.h').read_bytes();marker=b'} ADX_TALK;';assert header.count(marker)==1
    (d/'include/sofdec.h').write_bytes(header.replace(marker,b'    Uint32 native_tail_padded;\n'+marker))
    prefix=b'''#include <stdint.h>
#include <machine.h>
#include <shinobi.h>
#include <stddef.h>
#include <string.h>
#include <sofdec.h>
#define GCC
static int jp_adxt_native_error;
extern int jp_adxt_sdk_drain(void);
extern int jp_adxstm_handle_pending(void *),jp_adxstm_handle_live(void *);
extern int jp_rna_clock_error(void);
extern int jp_g2_gate_get(void);
extern void jp_rna_service_observe(void),jp_adxf_service_observe(void),jp_adxstm_service_observe(void);
uint32_t jp_adxt_records_live(void);
static void jp_adxt_pad_tail(void);
static void jp_adxt_error_claim(void (*)(void *,char *),void *);
static void jp_adxt_error_release(void);
'''
    modules=[];exports=set();changes=[]
    for n in names[:4]:
        raw=(u/n).read_bytes()
        exports.update(x.decode() for x in re.findall(rb'^(?:void|long|Sint32|ADXT|SJ)\s+([A-Za-z_]\w*)\(',raw,re.M))
        def edit(name,old,new,count=1):
            nonlocal raw
            old,new=old.encode(),new.encode();body=function(raw,name);assert body.count(old)==count,(n,name,old,body.count(old))
            raw=raw.replace(body,body.replace(old,new));changes.append(n+':'+name)
        def replace(name,new):
            nonlocal raw
            raw=raw.replace(function(raw,name),new.encode());changes.append(n+':'+name)
        if n.endswith('adx_inis.c'):
            for call in ('ADXCRS_Init();','ADXRNA_Init();','ADXF_Init();','SJRBF_Init();','SJMEM_Init();','ADXSTM_Init();','ADXSJD_Init();','ADXERR_Init();'):
                edit('ADXT_Init','    '+call+'\n','')
            body=function(raw,'ADXT_Init');start=body.index(b'    if (adxt_vsync_svr_flag');end=body.index(b'    ADXCRS_Unlock();',start)
            raw=raw.replace(body,body[:start]+body[end:])
            edit('ADXT_Init','    ADXCRS_Lock();','    if(adxt_init_cnt||jp_adxt_records_live()){jp_adxt_native_error=-1;return;}\n    ADXCRS_Lock();')
            replace('ADXT_Finish','''void ADXT_Finish(void)
{
    int failed=0;
    for(unsigned i=0;i<ADXT_MAX_OBJ;i++)if(adxt_obj[i].used){
        ADXT_Destroy(&adxt_obj[i]);if(adxt_obj[i].used)failed=1;}
    if(failed){jp_adxt_native_error=-1;return;}
    jp_adxt_error_release();
    if(adxt_init_cnt)adxt_init_cnt--;
}
''')
        elif n.endswith('adx_tlk.c'):
            edit('ADXT_Create','    buf = (char *)(((unsigned long)work + 31) & ~31);','''    if(maxnch<1||maxnch>ADXT_MAX_NCH||!work||worksize<=0||
        (uintptr_t)work>UINTPTR_MAX-31)return NULL;
    buf = (char *)(((uintptr_t)work + 31) & ~(uintptr_t)31);''')
            edit('ADXT_Create','    size = worksize - (buf - (char *)work);','''    size = worksize - (buf - (char *)work);
    if(size<(long)ADXT_CALC_OBUFSIZE(maxnch)+ADXT_IBUF_XLEN+ADXT_SCT_SIZE||
        size>16*1024*1024)return NULL;''')
            edit('ADXT_Create','    memset(adxt, 0, sizeof(ADX_TALK));','    memset(adxt, 0, sizeof(ADX_TALK));\n    adxt->used=1;')
            edit('ADXT_Destroy','    ADXT_Stop(adxt);','    ADXT_Stop(adxt);\n    if(jp_adxt_native_error)return;')
            for field,call in (('rna','ADXRNA_Destroy'),('sjd','ADXSJD_Destroy')):
                edit('ADXT_Destroy',f'        {call}(adxt->{field});',f'        {call}(adxt->{field});adxt->{field}=NULL;')
            edit('ADXT_Destroy','    ADXCRS_Lock();','    if(jp_adxt_sdk_drain()){jp_adxt_native_error=-1;return;}\n    ADXCRS_Lock();')
            edit('ADXT_Stop','    ADXCRS_Lock();','''    jp_adxt_native_error=0;
    if(!adxt||jp_adxt_sdk_drain()){jp_adxt_native_error=-1;return;}
    ADXCRS_Lock();''')
            for field,call in (('rna','ADXRNA_SetTransSw(adxt->rna, 0);'),('rna','ADXRNA_SetPlaySw(adxt->rna, 0);'),('sjd','ADXSJD_Stop(adxt->sjd);')):
                edit('ADXT_Stop','    '+call,'    if(adxt->'+field+')'+call)
            edit('ADXT_Stop','        ADXSTM_Stop(adxt->stm);','''        ADXSTM_Stop(adxt->stm);
        if(jp_adxstm_handle_pending(adxt->stm)){jp_adxt_native_error=-1;ADXCRS_Unlock();return;}''')
            edit('ADXT_Stop','        ADXSTM_Close(adxt->stm);','''        ADXSTM_Close(adxt->stm);
        if(jp_adxstm_handle_live(adxt->stm)){jp_adxt_native_error=-1;ADXCRS_Unlock();return;}''')
            edit('ADXT_Stop','        SJ_Reset(adxt->sji);','        if(adxt->sji)SJ_Reset(adxt->sji);')
            edit('ADXT_Stop','        SJ_Reset(adxt->sjo[i]);','        if(adxt->sjo[i])SJ_Reset(adxt->sjo[i]);')
            edit('ADXT_Stop','    adxt->stat = ADXT_STAT_STOP;','    adxt->stat = ADXT_STAT_STOP;\n    adxt->native_tail_padded=0;')
            for name in ('adxt_start_sj','ADXT_StartSj'):
                edit(name,'    adxt->lpcnt = 0;','    adxt->lpcnt = 0;\n    adxt->native_tail_padded=0;')
            edit('ADXT_StartSj','    ADXT_Stop(adxt);','    ADXT_Stop(adxt);\n    if(jp_adxt_native_error)return;')
            edit('ADXT_SetOutPan','    adxt->outpan[ch] = pan;','    if(ch<0||ch>=adxt->maxnch)return;\n    adxt->outpan[ch] = pan;')
            edit('ADXT_SetSvrFreq','    adxt->svrfreq = freq;','    if(freq>0&&freq<=1000)adxt->svrfreq = freq;')
            edit('ADXT_ExecServer','    ADXSJD_ExecServer();','    ADXSJD_ExecServer();\n    jp_adxt_pad_tail();')
            edit('ADXT_ExecServer','    ADXRNA_ExecServer();','''    jp_rna_service_observe();ADXRNA_ExecServer();
    if(jp_adxt_sdk_drain()){jp_adxt_native_error=-1;return;}''')
            edit('ADXT_ExecServer','    ADXF_ExecServer();','    jp_adxf_service_observe();ADXF_ExecServer();')
            edit('ADXT_ExecServer','    ADXSTM_ExecServer();','    jp_adxstm_service_observe();ADXSTM_ExecServer();')
            edit('ADXT_EntryErrFunc','    ADXERR_EntryErrFunc(func, obj);','    jp_adxt_error_claim(func,obj);\n    ADXERR_EntryErrFunc(func, obj);')
        elif n.endswith('adx_tlk2.c'):
            for name in ('ADXT_StartAfs','ADXT_StartFname','ADXT_StartMem','ADXT_StartMemIdx'):
                edit(name,'    ADXT_Stop(adxt);','    ADXT_Stop(adxt);\n    if(jp_adxt_native_error)return;')
            edit('ADXT_StartAfs','    ADXF_GetFileRange(patid, fid, &ofst, &nsct);','    if(ADXF_GetFileRange(patid,fid,&ofst,&nsct)<0){jp_adxt_native_error=-1;ADXCRS_Unlock();return;}')
            for name,code in (('ADXT_StartAfs','E8101202'),('ADXT_StartFname','E8101205'),('ADXT_StartMem','E8101207'),('ADXT_StartMemIdx','E8101207')):
                edit(name,'        ADXCRS_Unlock();\n        ADXERR_CallErrFunc("'+code,'        jp_adxt_native_error=-1;\n        ADXERR_CallErrFunc("'+code)
        else:
            edit('adxt_stat_prep','    nroom = ADXRNA_GetNumRoom(rna);','    nroom = mwRnaGetNumRoom(rna);')
            edit('adxt_stat_prep','    if (ndata >= adxt->maxdecsmpl || nroom <= ADXSJD_GetBlkSmpl(sjd)) {','    if (ndata >= adxt->maxdecsmpl || nroom <= ADXSJD_GetBlkSmpl(sjd) || (ndata>0&&ADXSJD_GetStat(sjd)==ADXSJD_STAT_DECEND)) {')
            edit('adxt_RcvrReplay','    ADXCRS_Lock();','    if(jp_adxt_sdk_drain()){jp_adxt_native_error=-1;return;}\n    ADXCRS_Lock();')
            edit('adxt_RcvrReplay','        ADXSTM_Stop(adxt->stm);','        ADXSTM_Stop(adxt->stm);\n        if(jp_adxstm_handle_pending(adxt->stm)){jp_adxt_native_error=-1;ADXCRS_Unlock();return;}')
        modules.append(raw)
    timer=function((u/'sdk/shinobi/sh4tmr.c').read_bytes(),'syTmrDiffCount')
    source=d/'external_adxt.c';source.write_bytes(prefix+timer+b'\n'+b'\n'.join(modules)+b'\n'+(p/'jp_port_tools/adxt_observation.inc').read_bytes())
    # A four-byte wrap mirror is a real synchronous CPU write, not a short
    # streaming DMA that changes sdMemBlk's global transfer mode. The existing
    # SDK implementation performs bounds/alignment checks and actual G2 I/O.
    rna=(base/'external_mw_rna.c').read_bytes()
    for name in ('mwlRnaAddWrPos','mwlRnaAddFlshPos'):
        body=function(rna,name);old=b'            mwlRnaStartCopy(rna, i, '+(b'src' if name=='mwlRnaAddWrPos' else b'zero')+b', dst, 4);'
        assert body.count(old)==1
        new=b'            if(jp_rna_mirror_write('+(b'src' if name=='mwlRnaAddWrPos' else b'zero')+b',dst))mwRnaErrCallFunc(TEXT_8);'
        rna=rna.replace(body,body.replace(old,new))
    (d/'external_mw_rna.c').write_bytes(b'extern int jp_rna_mirror_write(void *,int);\n'+rna)
    sdk=(base/'external_game_sound.c').read_bytes();marker=b'int32_t jp_game_sdk_service(void)';assert sdk.count(marker)==1
    sdk=sdk.replace(marker,b'extern int jp_adxt_service_enabled(void),jp_adxt_exec_due(void);\n'+marker)
    marker=b'    if(!error&&jp_adxstm_service_enabled())';assert sdk.count(marker)==1
    sdk=sdk.replace(marker,b'''    if(!error&&jp_adxt_service_enabled()){
        gdFsExecServer();
        if(jp_adxt_exec_due())return (int32_t)SDE_ERR_UNKNOW_NUM;
        return (int32_t)sdSysServer();
    }
'''+marker)
    sdk+=b'''
int jp_adxt_sdk_drain(void)
{
    if(jp_g2_gate_get()!=0)return -1;
    for(unsigned i=0;i<128;i++){
        int n=jp_rna_sdk_pending();if(n<0)return -1;if(!n)return 0;
        if(sdSysServer()!=SDE_ERR_NOTHING)return -1;
    }
    return jp_rna_sdk_pending()!=0;
}
int jp_rna_mirror_write(void *source,int destination)
{
    if(destination<0||destination>(2<<20)-4)return -1;
    return (int)sdMoveWrkMemToSndMem(source,destination,4,SDD_MEMBLK_NO_FUNC,NULL);
}
'''
    # The G2 gate import already exists in the generated SDK; no second owner.
    (d/'external_game_sound.c').write_bytes(sdk)
    owner=(base/'sound_owner_adxf.c').read_bytes();marker=b'static int jp_adxf_close_owned(void);';assert owner.count(marker)==1
    owner=owner.replace(marker,marker+b'\n#include "jp_native_adxt.h"\nstatic int jp_adxt_close_owned(void);\nint jp_adxt_blocks_lower_shutdown(void);')
    marker=b'    if(jp_adxf_close_owned())return JP_SOUND_CLEANUP;';assert owner.count(marker)==1
    owner=owner.replace(marker,b'    if(jp_adxt_close_owned())return JP_SOUND_CLEANUP;\n'+marker)
    for name in ('jp_rna_shutdown','jp_adxstm_shutdown','jp_adxf_shutdown'):
        body=function(owner,name);at=body.index(b'    if(!sound.owner)');owner=owner.replace(body,body[:at]+b'    if(jp_adxt_blocks_lower_shutdown())return end(JP_SOUND_STATE);\n'+body[at:])
    owned=d/'sound_owner_adxt.c';owned.write_bytes(owner+b'\n'+(p/'native/jp_native_adxt.inc').read_bytes())
    def command(name):return list(next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][row['command'].index('-c')+1]).name==name))
    sdkcmd=[x.replace(str(base),str(d)) for x in command('external_game_sound.c')];common=sdkcmd[:sdkcmd.index('-c')]
    ownercmd=[x.replace(str(base),str(d)) for x in command('sound_owner_adxf.c')];ownercmd[ownercmd.index('-c')+1]=str(owned)
    rnacmd=command('external_mw_rna.c');rna_base=str(Path(rnacmd[rnacmd.index('-c')+1]).parent)
    rnacmd=[x.replace(rna_base,str(d)) for x in rnacmd]
    commands=[sdkcmd,ownercmd,common+['-I'+str(d/'adxt_inputs'),'-c',str(source),'-o',str(d/'external_adxt.obj')],rnacmd]
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link];exports.add('syTmrDiffCount')
    link[4:4]=[str(d/'external_adxt.obj')]+['-Wl,/export:'+n for n in sorted(exports)];commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'adxt_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'adxt_compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(str(log),log.read_text())
    updates={};preserved={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value));updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    for key in ('game_sound_generated_source_sha256','qsound_generated_source_sha256','spatial_generated_source_sha256','sound_math_generated_source_sha256','pstm_generated_source_sha256'):
        if key in b:updates[key]=sha(d/'external_game_sound.c')
    authored=[Path(__file__).resolve(),p/'native/jp_native_adxt.h',p/'native/jp_native_adxt.inc',p/'jp_port_tools/adxt_observation.inc']
    updates.update(status='complete_original_ADXT_frontend_linked_with_existing_native_sound_owner_runtime_unverified',commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{src.relative_to(r).as_posix():sha(src) for src in authored}),
        adxt_baseline_sha256=sha(base/'build_verification.json'),adxt_matching_inputs_proof_sha256=sha(inputs/'verification.json'),
        adxt_original_source_sha256=originals,adxt_generated_source_sha256=sha(source),adxt_owner_generated_source_sha256=sha(owned),
        adxt_generated_header_sha256=sha(d/'include/sofdec.h'),adxt_exports=sorted(exports),adxt_preserved_prior_DLLs=preserved,
        rna_generated_sources_sha256=dict(b['rna_generated_sources_sha256'],**{'external_mw_rna.c':sha(d/'external_mw_rna.c')}),
        adxt_RNA_mirror_uses_actual_SDK_synchronous_four_byte_CPU_write=True,
        adxt_adapted_functions=changes,ADXT_runtime_playback_verified=False,combined_SDK_game_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(exports),'original exports',flush=True)

if __name__=='__main__':main()
