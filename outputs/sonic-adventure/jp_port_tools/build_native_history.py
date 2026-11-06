"""Real UF/game history with native owned work, identity and atomic flash saves."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_flash_manager import FM,PIN
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'))
    p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir();inc=d/'history_include';inc.mkdir();changes=[];originals={}
    def source(n):
        raw=(u/n).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n]);originals[n]=sha(u/n);return raw
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old));changes.append(dict(reason=label,count=count,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
    for key,v in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
            assert sha(v)==b[key+'_sha256'];target=d/Path(v).name
            if not target.exists():shutil.copyfile(v,target);shutil.copyfile(Path(v).with_suffix('.lib'),target.with_suffix('.lib'))
    support={}
    folders={Path(v).parent for key,v in b.items() if (key=='DLL' or key.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll')}
    for folder in folders:
        for path in folder.iterdir():
            if path.is_file() and path.suffix in ('.dll','.lib'):
                target=d/path.name
                if not target.exists():shutil.copyfile(path,target)
                support[str(path)]=sha(path)
    flash=(p/'native/jp_native_flash.c').read_bytes()
    flash=b'#include "jp_native_flash_history.h"\nstatic int history_blocked(void);\n'+flash
    flash=rep(flash,'DWORD thread;uint64_t sequence;int fm_call;','DWORD thread;uint64_t sequence;int fm_call;uint64_t history_token,history_programs,history_erases;int history_active,created_profile;unsigned char *history_backup;','same media owner gains transaction authority')
    flash=rep(flash,'if(flash.fm_call)return JP_FLASH_BUSY;flash.fm_call=1;','if(history_blocked()||flash.fm_call)return JP_FLASH_BUSY;flash.fm_call=1;','FM calls require exact history permit')
    flash=rep(flash,'if(!jp_flash_owner_access()&&!flash.fm_call)','if(!jp_flash_owner_access()&&!flash.fm_call&&!history_blocked())','only current owner clears errors')
    flash=rep(flash,'if(flash.fm_call)return JP_FLASH_BUSY;\n    rc=jp_flash_fm_close();','if(flash.history_token||flash.history_backup||flash.fm_call)return JP_FLASH_BUSY;\n    rc=jp_flash_fm_close();','legacy close cannot steal live history profile')
    flash=rep(flash,'memset(image,255,IMAGE_BYTES);rc=persist(image,0);','memset(image,255,IMAGE_BYTES);rc=persist(image,0);if(!rc)flash.created_profile=1;','actual new-file creation observation')
    flash=rep(flash,'    if(region(address,bytes,buffer))return -1;','    if(history_blocked())return -1;\n    if(region(address,bytes,buffer))return -1;','raw programming cannot bypass history owner')
    flash=rep(flash,'    int rc=load(next);\n    if(!rc&&memcmp(next,flash.image,IMAGE_BYTES))rc=fail(JP_FLASH_CHANGED,0);','    int rc=0;if(flash.history_backup)memcpy(next,flash.image,IMAGE_BYTES);\n    else{rc=load(next);if(!rc&&memcmp(next,flash.image,IMAGE_BYTES))rc=fail(JP_FLASH_CHANGED,0);}','stage exact media inside owned transaction')
    flash=rep(flash,'        rc=persist(next,1);','        if(!flash.history_backup)rc=persist(next,1);','defer transaction disk commit')
    flash=rep(flash,'flash.info.commits++;\n        if(erase)flash.info.erases++;else flash.info.programs++;','if(flash.history_backup){if(erase)flash.history_erases++;else flash.history_programs++;}\n        else{flash.info.commits++;if(erase)flash.info.erases++;else flash.info.programs++;}','committed counters exclude staged writes')
    needle='__declspec(dllexport) int32_t syBtFlDeleteSct(int32_t address)\n{\n'
    flash=rep(flash,needle,needle+'    if(history_blocked())return -1;\n','raw erase cannot bypass history owner')
    flash+=(p/'native/jp_native_flash_history.inc').read_bytes();(d/'flash_with_history.c').write_bytes(flash)
    header=source('sdk/include/sg_uf.h');header=b'#include <stdint.h>\n'+rep(header,'void ufGetUserState(Sint32 buf);','void ufGetUserState(intptr_t buf);','UF address ABI declaration widened')
    (inc/'sg_uf.h').write_bytes(header)
    fm_macros=''.join('#define '+n+' jp_history_'+n+'\n' for n in FM)
    pre=(fm_macros+'#include <ninja.h>\n#include <shinobi.h>\n#include <sg_uf.h>\n#include <string.h>\n#include "jp_native_history.h"\n#define GCC\n#define _builtin_strcpy strcpy\n').encode()
    pre+=b'int jp_history_rtc_get(SYS_RTC_DATE *);\nint jp_history_rtc_count(const SYS_RTC_DATE *,Uint32 *);\nconst PDS_PERIPHERAL *jp_history_get_peripheral(Uint32);\nconst PDS_PERIPHERALINFO *jp_history_get_peripheral_info(Uint32);\n'
    units=[]
    for name in ('sg_flcommon.c','sg_playtime.c','sg_rdalahd.c','sg_userflag.c','sg_userhis.c','sg_fhref.c'):
        raw=source('sdk/shinobi/'+name)
        if name=='sg_userflag.c':
            raw=rep(raw,'void ufGetUserState(Sint32 buf)','void ufGetUserState(intptr_t buf)','UF address ABI definition widened')
            raw=rep(raw,'    ufLoadPacket(FLAG_PACKET, NULL);','    if(!ufLoadPacket(FLAG_PACKET,NULL))return false;','do not consume failed flag read')
        elif name=='sg_userhis.c':
            raw=rep(raw,'while ((busy = (fmMngExecServer() == FMD_STAT_RUNNING)))','while ((busy = fmMngExecServer()) == FMD_STAT_RUNNING)','preserve actual completion error rather than boolean',2)
        elif name=='sg_flcommon.c':
            raw=rep(raw,'    ufSetBank(HISTORY_BANK);\n    ufLoadUserFlag();','    if(!ufSetBank(HISTORY_BANK)||!ufLoadUserFlag()){ufgSafeFlag=false;return false;}','stop failed real initialization')
            raw=rep(raw,'    syBtGetBootSystemID(&console_id);','    if(!syBtGetBootSystemID(&console_id)){ufgSafeFlag=false;ufgErrno=UFD_ERR_INIT_FAIL;return false;}','propagate original identity failure')
        elif name=='sg_rdalahd.c':raw=rep(raw,'        ufSetBank(HISTORY_BANK);','        if(!ufSetBank(HISTORY_BANK))return false;','stop failed history bank open')
        elif name=='sg_playtime.c':
            raw,n=re.subn(rb'(?m)^(\s*)syRtcGetDate\(([^;]+)\);',rb'\1if(jp_history_rtc_get(\2))return false;',raw);assert n==5
            raw,n=re.subn(rb'(?m)^(\s*)syRtcDateToCount\(([^;]+)\);',rb'\1if(jp_history_rtc_count(\2))return false;',raw);assert n==5
            raw=raw.replace(b'pdGetPeripheral(',b'jp_history_get_peripheral(').replace(b'pdGetPeripheralInfo(',b'jp_history_get_peripheral_info(')
            raw,n=re.subn(rb'(?m)^( +)(ufLoadPacket|ufSavePacket)\(([^;{}\n]+)\);',rb'\1if(!\2(\3))return false;',raw);assert n==11,n
            raw=rep(raw,'            fhSafeRefresh(ufgWorkBuff, ufgReadBuff, ufgReflash3, ufgReflash4);','            if(fhSafeRefresh(ufgWorkBuff,ufgReadBuff,ufgReflash3,ufgReflash4))return false;','propagate refresh failure')
            raw=rep(raw,'            ufSetBank(-3);','            if(!ufSetBank(-3))return false;','propagate post-refresh bank failure')
        elif name=='sg_fhref.c':
            raw,n=re.subn(rb'(?m)^( +)fhSetStatus\(([^;]+)\);',rb'\1if(fhSetStatus(\2)<0)return -1;',raw);assert n==5,n
        target=d/('history_'+name);target.write_bytes(pre+raw);units.append(target)
    raw=source('src/system/history.c')
    for old,new in [('HISTORY_WORK3 WORK_AREA','HISTORY_WORK3 ((Uint8 *)jp_history_work(2))'),('HISTORY_WORK4 (WORK_AREA + UFD_WORK3_SIZ)','HISTORY_WORK4 ((Uint8 *)jp_history_work(3))')]:raw=rep(raw,old,new,'owned native refresh work')
    raw=rep(raw,'    ufInitSystem(history_work1, history_work2, HISTORY_WORK3, HISTORY_WORK4);','    if(!ufInitSystem(history_work1,history_work2,HISTORY_WORK3,HISTORY_WORK4)){jp_history_source_problem(JP_HISTORY_SOURCE,(unsigned)ufgErrno);return;}','propagate actual history library initialization failure',3)
    raw=rep(raw,'    ufReadAllHead(NULL, &history);','    if(!ufReadAllHead(NULL,&history)){jp_history_source_problem(JP_HISTORY_SOURCE,(unsigned)ufgErrno);return;}','failed original history read preserves caller output')
    raw=rep(raw,'    ufSynch();','    if(!ufSynch())jp_history_source_problem(JP_HISTORY_SOURCE,(unsigned)ufgErrno);','propagate real original history save failure')
    mappings={'history_init':'jp_history_source_init','history_write':'jp_history_source_write','history_read':'jp_history_source_read','history_restart':'jp_history_source_restart','history_count_play':'jp_history_source_count_play','history_count_clear':'jp_history_source_count_clear'}
    hp=pre+b''.join(('#define '+n+' '+v+'\n').encode() for n,v in mappings.items())
    raw+=b'\nvoid *jp_history_source_work(unsigned kind){return kind==0?(void *)history_work1:kind==1?(void *)history_work2:NULL;}\n'
    target=d/'history_game.c';target.write_bytes(hp+raw);units.append(target)
    identity=source('sdk/shinobi/sg_syb2.c');first=function(identity,'syBtGetBootSystemID')
    prefix=identity[:identity.index(first)]+b'\nconst char *jp_history_boot_header(void);\n#undef BOOT_HEADER\n#define BOOT_HEADER ((char *)jp_history_boot_header())\n'
    target=d/'history_identity.c';target.write_bytes(prefix+b'\n'.join(function(identity,n) for n in ('syBtGetBootSystemID','read_boot_identity','read_disc_numbers')));units.append(target)
    proxy=pre+b''.join(('#undef '+n+'\n').encode() for n in FM)
    proxy+=b'extern __declspec(dllimport) Sint32 fmMngGetErr(void);\n'
    for name,params,args,empty in [('fmMngInit','Sint32 p,void *w','p,w','FMD_NOT_OPEN'),('fmMngFormat','Sint32 p','p','FMD_NOT_OPEN'),('fmMngRead','Sint32 p,void *w','p,w','FMD_NOT_OPEN'),('fmMngWrite','Sint32 p,void *w','p,w','FMD_NOT_OPEN'),('fmMngExecServer','void','','FMD_STAT_ERR'),('fmMngGetErr','void','','FMD_NOT_OPEN'),('fmMngRefresh','void','','FMD_NOT_OPEN'),('fmMngGetFree','void','','FMD_NOT_OPEN')]:
        proxy+=('extern __declspec(dllimport) Sint32 '+name+'('+params+');\nSint32 jp_history_'+name+'('+params+'){if(jp_history_source_begin_flash())return '+empty+';Sint32 code='+name+'('+args+');').encode()
        if name=='fmMngExecServer':proxy+=b'if(code==FMD_STAT_ERR)jp_history_source_problem(JP_HISTORY_FLASH,(unsigned)fmMngGetErr());'
        proxy+=b'jp_history_source_end_flash();return code;}\n'
    proxy+=b'extern __declspec(dllimport) void fmMngFinish(void);\nvoid jp_history_fmMngFinish(void){if(jp_history_source_begin_flash())return;fmMngFinish();jp_history_source_end_flash();}\n#include "shinobi/uf_work.h"\n'
    proxy+=(p/'native/jp_native_history_source.inc').read_bytes();target=d/'history_source_bridge.c';target.write_bytes(proxy);units.append(target)
    for relative in ('sdk/ninja/njMem.c','sdk/runtime/_ctype.c'):
        target=d/('history_'+Path(relative).name);target.write_bytes(source(relative));units.append(target)
    compiler=b['commands'][0]['command'][0];common=b['commands'][0]['command'];common=common[:common.index('-c')];common.insert(1,'-I'+str(inc));common+=['-Wno-unused-variable']
    host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    commands=[host+['-c',str(d/'flash_with_history.c'),'-o',str(d/'flash_history_owner.obj')]]
    commands.append([compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'flash_original.obj'),str(d/'flash_crc.obj'),str(d/'flash_history_owner.obj'),'-Wl,/export:fmcalccrc','-o',str(d/'jp_native_flash.dll')])
    objects=[]
    for i,unit in enumerate(units):
        obj=d/('history_'+str(i)+'.obj');objects.append(obj);commands.append(common+['-c',str(unit),'-o',str(obj)])
    commands.append(host+['-c',str(p/'native/jp_native_history.c'),'-o',str(d/'history_owner.obj')]);objects.append(d/'history_owner.obj')
    commands.append([compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*map(str,objects),str(d/'jp_native_flash.lib'),str(d/Path(b['frames_DLL']).with_suffix('.lib').name),str(d/Path(b['RTC_DLL']).with_suffix('.lib').name),'-Wl,/export:history_counts','-Wl,/export:syBtGetBootSystemID','-o',str(d/'jp_native_history.dll')])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'history_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_native_flash_history.h','jp_native_flash_history.inc','jp_native_history.h','jp_native_history.c','jp_native_history_source.inc')]+[Path(__file__).resolve()]
    updates.update(status='native_owned_original_UF_game_history_and_transactional_flash_linked_unverified',history_DLL=str(d/'jp_native_history.dll'),history_DLL_sha256=sha(d/'jp_native_history.dll'),history_baseline_sha256=sha(base/'build_verification.json'),history_copied_support_artifacts=support,history_original_source_sha256=originals,history_adaptations=changes,history_generated_source_sha256={x.name:sha(x) for x in units+[d/'flash_with_history.c',inc/'sg_uf.h']},authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),commands=results+b['commands'],full_game_init_executed=False,combined_game_SDK_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
