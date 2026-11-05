"""Adapt pinned original GDFS software to the real native file-backed drive.
Generated original code/header and combined libraries remain private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def functions(raw):
    pattern=rb'^[A-Za-z_][^;{}\n]*?\b([A-Za-z_]\w*)\([^;{}]*\)\r?\n\{'
    names=[m.group(1).decode() for m in re.finditer(pattern,raw,re.M)]
    return {name:function(raw,name) for name in names}
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','media','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(a.baseline/'build_verification.json');m=read(a.media/'build_verification.json')
    for n,h in m['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(m['DLL'])==m['DLL_sha256'];d.mkdir();inc=d/'include';(inc/'shinobi').mkdir(parents=True)
    names=['sdk/shinobi/'+n for n in ('gdfs.c','gdfshn.c','gdfsdir.c','gdfsif.c','gdDec.c','gdfs.h')]+['sdk/include/'+n for n in ('shinobi.h','sg_xpt.h','sg_chain.h','sg_ov.h','machine.h')]
    originals={n:sha(u/n) for n in names};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+n])).hexdigest()==h,n
    raws={n:(u/'sdk/shinobi'/n).read_bytes() for n in ('gdfs.c','gdfshn.c','gdfsdir.c','gdfsif.c','gdDec.c')};bodies={n:functions(raw) for n,raw in raws.items()}
    symbols=sorted({name for unit in bodies.values() for name in unit})
    macros=''.join('#define '+n+' jp_gdfs_source_'+n+'\n' for n in symbols).encode();unmacros=''.join('#undef '+n+'\n' for n in symbols).encode()
    headers=b'''#include <sg_xpt.h>
#include <shinobi.h>
#include <machine.h>
#include <sg_chain.h>
#include <sg_ov.h>
#include "shinobi/gdfs.h"
'''
    (inc/'gdfs_source_api.h').write_bytes(macros+headers+unmacros)
    header=(u/'sdk/shinobi/gdfs.h').read_bytes()
    for name in ('fid','fad'):
        old=('    Sint32 '+name+';').encode();assert header.count(old)==1;header=header.replace(old,('    intptr_t '+name+';').encode())
    (inc/'shinobi/gdfs.h').write_bytes(b'#include <stdint.h>\n'+header)
    changes=[]
    def replace(unit,old,new,label,count=1):
        raw=raws[unit];assert raw.count(old)==count,(unit,label,raw.count(old));raws[unit]=raw.replace(old,new)
        changes.append(dict(unit=unit,reason=label,count=count,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()))
    replace('gdfs.c',bodies['gdfs.c']['gdFsInit'],b'','host-owned initializer replaces console/overlay entry')
    replace('gdfs.c',bodies['gdfs.c']['gdfs_cddmae_handler'],b'','real native copies replace console G1 DMA interrupt')
    replace('gdfs.c',b'static SYCHAIN gdfs_cddmae_chain = 0;',b'','remove nonexistent hardware-chain ownership')
    raw=raws['gdfs.c'];body=function(raw,'gdfs_init_drive');start=body.index(b'        if (gdfs_cddmae_chain == 0)');end=body.index(b'        gdfs = gdFsReopenHn',start)
    replace('gdfs.c',body[start:end],b'','native drive completion has no fabricated DMA event')
    old=b'                syChainAddHandler(SYD_CHAIN_EVENT_IML6_VBLANK,\n                                  gdfs_vblank_handler, VBLANK_PRI, NULL);'
    assert old in raws['gdfs.c']
    replace('gdfs.c',old,old+b'\n            if (!gdfs_vblank_chain) { err=GDD_ERR_INIT; break; }','propagate real vblank registration failure')
    finish=function(raws['gdfs.c'],'gdFsFinishG');start=finish.index(b'    if (gdfs_cddmae_chain != 0)');end=finish.index(b'    gdfs_vblank_chain = 0;',start)
    replace('gdfs.c',finish[start:end],b'','remove console DMA-handler deletion')
    replace('gdfs.c',b'    gdfs_cddmae_chain = 0;\n',b'','remove nonexistent DMA token reset')
    descriptor=re.search(rb'OVS_LIB_OBJ gdg_fs_libhdl = \{.*?\};\r?\n',raws['gdfs.c'],re.S).group()
    replace('gdfs.c',descriptor,b'','native directly bound interface instead of console overlay-library descriptor')
    replace('gdfs.c',b'#pragma section LH\n',b'','console descriptor section is not a native COFF section')
    replace('gdfs.c',b'#pragma section\n',b'','remove console descriptor section reset')
    replace('gdfs.c',b'(Uint32)gdfs_work',b'(uintptr_t)gdfs_work','native pointer-width work alignment')
    for old,new in ((b'(Sint32)&da_play_fad',b'(intptr_t)&da_play_fad'),(b'(Sint32)&da_stop_fad',b'(intptr_t)&da_stop_fad'),(b'(Sint32)dainfo',b'(intptr_t)dainfo')):
        replace('gdfs.c',old,new,'native width for pointer-bearing fid/fad fields',raws['gdfs.c'].count(old))
    for name,statement in [('gdfs_vblank_handler',b'    jp_gdfs_note_tick();\n'),('gdFsExecServerG',None)]:
        body=function(raws['gdfs.c'],name)
        if statement is not None:
            at=body.index(b'{')+1;changed=body[:at]+b'\n'+statement+body[at:]
        else:
            at=body.rfind(b'}');changed=body[:at]+b'    jp_gdfs_dispatch_callbacks();\n'+body[at:]
        replace('gdfs.c',body,changed,'native tick observation/callback completion boundary '+name)
    for name in ('gdFsInitOvl','gdFsFinish'):replace('gdfsif.c',bodies['gdfsif.c'][name],b'','native owner supplies library lifecycle '+name)
    for text in (b'static const UUID gdfs_lid = GDFS_LID;',b'static OVLIB gdfs_ovl = NULL;'):
        replace('gdfsif.c',text,b'','remove unused console overlay state')
    raws['gdfsif.c']+=b'''\nextern const struct gdfs_lif gdg_fs_lif_tbl;
void jp_gdfs_source_bind_interface(int live){gdfs_i=live?&gdg_fs_lif_tbl:NULL;}
'''
    raws['gdfs.c']+=b'''\nuint32_t jp_gdfs_source_chain_handle(void){return gdfs_vblank_chain;}
uintptr_t jp_gdfs_source_chain_callback(void){return (uintptr_t)gdfs_vblank_handler;}
'''
    replace('gdfshn.c',b'(Uint32)buf',b'(uintptr_t)buf','native pointer-width transfer alignment',raws['gdfshn.c'].count(b'(Uint32)buf'))
    stop=function(raws['gdfshn.c'],'gdFsStopRd');new=stop
    start=new.index(b'    start = syTmrGetCount();');end=new.index(b'    mask = get_imask();',start)
    new=new[:start]+b'    /* Native cancellation needs no console firmware wait/settling spin. */\n'+new[end:]
    for decl in (b'    Uint32 start;\n',b'    Uint32 elapsed;\n'):assert new.count(decl)==1;new=new.replace(decl,b'')
    old=b'gdfs->stat != GDD_STAT_READ && gdfs->stat != GDD_STAT_BUSY';assert new.count(old)==2
    new=new.replace(old,old+b' && gdfs->stat != GDD_STAT_SEEK')
    replace('gdfshn.c',stop,new,'bounded cancellable native I/O instead of firmware wait; include real pending seek')
    close=function(raws['gdfshn.c'],'gdFsClose');changed=close.replace(b'        gdFsStopRd(gdfs);',b'        if (gdFsStopRd(gdfs) != GDD_ERR_OK) return;');assert changed!=close
    replace('gdfshn.c',close,changed,'failed close retains handle ownership until drain completes')
    wait=function(raws['gdfshn.c'],'gdFsWaitCompleteReq');new=wait
    anchor=b'    Uint32 start = gdg_fs_vcnt;';assert new.count(anchor)==1
    new=new.replace(anchor,anchor+b'\n    uint64_t native_start=jp_gdfs_host_time();\n    Sint32 native_previous=gdfs->trnsed;')
    anchor=b'        elapsed = gdg_fs_vcnt - start;';assert new.count(anchor)==1
    new=new.replace(anchor,b'''        if (gdfs->trnsed!=native_previous){native_previous=gdfs->trnsed;native_start=jp_gdfs_host_time();}
        if (jp_gdfs_expired(native_start)) return jp_gdfs_timeout(gdfs);
        jp_gdfs_host_yield();
'''+anchor)
    replace('gdfshn.c',wait,new,'real monotonic progress timeout/yield when no console IRQ advances vcnt')
    directory=function(raws['gdfsdir.c'],'gdFsGetDirEnt');new=directory
    anchor=b'    if (gf_dirrec->dir_fad == fad) {';assert new.count(anchor)==1
    new=new.replace(anchor,b'    if (!jp_gdfs_directory_begin(fad,size,gf_dirrec->max_ent)) return GDD_ERR_READ;\n'+anchor)
    anchor=b'        result = gdFsReadW(fad, 1, buf);';assert new.count(anchor)==1
    new=new.replace(anchor,b'        if (!jp_gdfs_directory_step()){gf_dirrec->dir_fad=0;return GDD_ERR_READ;}\n'+anchor)
    anchor=b'        dr = buf;';assert new.count(anchor)==1
    new=new.replace(anchor,b'        if (!jp_gdfs_directory_valid(buf)){gf_dirrec->dir_fad=0;return GDD_ERR_READ;}\n'+anchor)
    replace('gdfsdir.c',directory,new,'bound directory record geometry/names and total sector work')
    prefix=macros+b'''#pragma push_macro("NULL")
#undef NULL
'''+headers+b'''#pragma pop_macro("NULL")
#include "jp_native_gdfs_internal.h"
#undef get_imask
#undef set_imask
#define get_imask() jp_gdfs_mask_get()
#define set_imask(value) jp_gdfs_mask_set(value)
#define GCC
'''
    sources=[]
    for name,raw in raws.items():
        path=d/('external_'+name)
        path.write_bytes((macros if name=='gdDec.c' else prefix)+raw);sources.append(path)
    compiler=m['commands'][0]['command'][0]
    common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter','-include',str(p/'native/jp_host_types.h'),'-I'+str(inc),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk')]
    commands=[common+['-c',str(src),'-o',str(src.with_suffix('.obj'))] for src in sources]
    commands.append(common+['-c',str(p/'native/jp_native_gdfs_sdk.c'),'-o',str(d/'gdfs_sdk.obj')])
    commands.append([compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(p/'native/jp_native_gdfs_host.c'),'-o',str(d/'gdfs_host.obj')])
    exports=['gdFsInit','gdFsReinit','gdFsFinish','gdFsOpen','gdFsOpenFast','gdFsOpenRange','gdFsClose','gdFsCreateDirhn','gdFsSetDir','gdFsLoadDir','gdFsLoadDirFast','gdFsChangeDir','gdFsGetDirInfo',
        'gdFsExecServer','gdFsGetGlobalErr','gdFsSetGlobalErr','gdFsGetWorkHn','gdFsGetSysHn','gdFsGetStat','gdFsGetErrStat','gdFsGetNumRd','gdFsGetTransStat','gdFsTell','gdFsGetFileSize','gdFsGetFileSctSize','gdFsGetFileFad','gdFsCheckEof','gdFsSeek','gdFsRead','gdFsReqRd32','gdFsReqGdRd','gdFsTrans32','gdFsSetTmode','gdFsStopRd','gdFsMovePickup','gdFsEntryRdEndFunc','gdFsEntryTrEndFunc','gdFsEntryErrFunc','gdFsEntryErrFuncAll','gdFsGetDrvStat','gdFsIsTrayOpen','gdFsReqDrvStat','gdFsGetToc','gdFsDaPlay','gdFsDaPlaySct','gdFsDaStop','gdFsDaPause','gdFsDaRelease','gdFsDaGetInfo']
    for key in ('frames_DLL','GPU_DLL','present_DLL'):
        src=Path(b[key]);assert sha(src)==b[key+'_sha256'];shutil.copyfile(src,d/src.name);shutil.copyfile(src.with_suffix('.lib'),d/src.with_suffix('.lib').name)
    for src in (Path(m['DLL']),Path(m['DLL']).with_suffix('.lib')):shutil.copyfile(src,d/src.name)
    link=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*[str(src.with_suffix('.obj')) for src in sources],str(d/'gdfs_sdk.obj'),str(d/'gdfs_host.obj'),str(d/'jp_native_present.lib'),str(d/'jp_native_gd_media.lib'),'-Wl,kernel32.lib',*['-Wl,/export:'+name for name in exports],'-o',str(d/'jp_native_gdfs.dll')];commands.append(link)
    results=[]
    for j,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    authored=[p/'native'/n for n in ('jp_native_gdfs.h','jp_native_gdfs_internal.h','jp_native_gdfs_host.c','jp_native_gdfs_sdk.c')]+[Path(__file__).resolve()]
    original_funcs={unit:{n:hashlib.sha256(body).hexdigest() for n,body in values.items()} for unit,values in bodies.items()}
    result=dict(status='original_GDFS_software_real_native_drive_owner_and_callbacks_linked_unverified',commands=results,DLL=str(d/'jp_native_gdfs.dll'),DLL_sha256=sha(d/'jp_native_gdfs.dll'),
        baseline_build=str(a.baseline.resolve()),baseline_build_sha256=sha(a.baseline/'build_verification.json'),media_build=str(a.media.resolve()),media_build_sha256=sha(a.media/'build_verification.json'),
        frames_DLL=str(d/Path(b['frames_DLL']).name),frames_DLL_sha256=b['frames_DLL_sha256'],GPU_DLL=str(d/Path(b['GPU_DLL']).name),GPU_DLL_sha256=b['GPU_DLL_sha256'],present_DLL=str(d/Path(b['present_DLL']).name),present_DLL_sha256=b['present_DLL_sha256'],
        media_DLL=str(d/Path(m['DLL']).name),media_DLL_sha256=m['DLL_sha256'],authored_source_sha256={path.relative_to(r).as_posix():sha(path) for path in authored},original_source_sha256=originals,
        original_function_sha256=original_funcs,source_adaptations=changes,generated_sources_sha256={str(path.relative_to(d)):sha(path) for path in sources+[inc/'gdfs_source_api.h',inc/'shinobi/gdfs.h']},
        SDK_exports=exports,console_BIOS_G1_DMA_or_overlay_success_stubs_added=False,CDDA_TOC_subcode_supported=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
