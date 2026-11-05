"""Compile pinned original SDK constructor/transfers/driver loader privately."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_sd_memory import PIN,UNITS as POOL
from build_native_gdfs import functions

UNITS=POOL+('sd_buffer','sd_lib_init','sd_sys_wrk_init','sd_memblk_que_init','sd_prim_host_cmd_init_lst','sd_2nd_host_cmd_init_lst','sd_module_port','sd_module_check_port',
    'sd_memblk_set_transfer_mode','sd_memblk_cpu_transfer','sd_memblk_dma_transfer','sd_memblk_transfer','sd_move_wrk_mem_to_snd_mem','sd_memblk_end_transfer_handller',
    'sd_memblk_que_push','sd_memblk_que_pop','sd_memblk_que_get_push_num','sd_aica_reset_keep','sd_aica_reset_rel','sd_snd_mem_clear','sd_drv_init','sd_drv_get_info',
    'sd_snd_mem_read_sint8','sd_snd_mem_read_sint32','sd_snd_mem_read_uint32')
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','g2','compiler','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();inc=d/'include';inc.mkdir()
    b=json.loads((a.g2/'build_verification.json').read_bytes());assert sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ('jp_native_g2_audio.dll','jp_native_g2_audio.lib','jp_native_aica.dll','jp_native_aica.lib','jp_native_audio.dll'):shutil.copyfile(a.g2/n,d/n)
    names=['sdk/include/'+n for n in ('shinobi.h','machine.h','sg_syg2.h','sg_xpt.h','sg_chain.h','stdarg.h','stdio.h','string.h')]+['sdk/shinobi/sd_work.h']+['sdk/shinobi/'+n+'.c' for n in UNITS]
    originals={n:sha(u/n) for n in names};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    header=(u/'sdk/include/shinobi.h').read_bytes();old=b'struct SDS_MEM_BLK {\n    Uint32 m_Member[8];\n};'
    newline=b'\r\n' if b'\r\n' in header else b'\n';old=old.replace(b'\n',newline)
    new=b'struct SDS_MEM_BLK {\n    _Alignas(void *) Uint8 m_Member[sizeof(void *) == 8 ? 48 : 32];\n};'.replace(b'\n',newline)
    assert header.count(old)==1;(inc/'shinobi.h').write_bytes(header.replace(old,new))
    raws={n:(u/'sdk/shinobi'/f'{n}.c').read_bytes() for n in UNITS};before={name:sha_body for raw in raws.values() for name,body in functions(raw).items() for sha_body in (hashlib.sha256(body).hexdigest(),)};changes=[]
    def replace(unit,old,new,reason,count=1):
        raw=raws[unit];assert raw.count(old)==count,(unit,reason,raw.count(old));raws[unit]=raw.replace(old,new)
        changes.append(dict(unit=unit,reason=reason,count=count,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()))
    for name in ('sdgDefaultWrkImg','sdgDefPrimaryHostCmdBuf'):
        replace('sd_sys_wrk_init',('(Uint32)'+name).encode(),('(uintptr_t)'+name).encode(),'full native pointer alignment')
    replace('sd_sys_wrk_init',b'~31UL',b'~(uintptr_t)31','native width alignment masks',2)
    replace('sd_sys_wrk_init',b'    syG2DmaOpen(&sdgWrkPtr->dma, SYE_G2_DMA_TYPE_AICA);',b'    if (syG2DmaOpen(&sdgWrkPtr->dma, SYE_G2_DMA_TYPE_AICA) != SYE_G2_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','propagate actual native channel acquisition failure')
    replace('sd_move_wrk_mem_to_snd_mem',b'(Uint32)src_ptr',b'(uintptr_t)src_ptr','native source alignment')
    # These values are guest bus tokens, not dereferenceable native pointers.
    for unit,raw in list(raws.items()):
        old=b'(void *)(SD_SND_MEM +';count=raw.count(old)
        if count:replace(unit,old,b'(void *)(uintptr_t)(SD_SND_MEM +','explicit 32-bit bus token conversion',count)
    replace('sd_drv_get_info',b'#define INFO(info, field) ((Sint32)(&((struct drv_info *)(info))->field))',
        b'#define INFO(info, field) ((Sint32)((Uint32)(info) + offsetof(struct drv_info, field)))','numeric sound offsets without fake native pointers')
    replace('sd_memblk_dma_transfer',b'void sdMemBlkWaitSync(void)\n{\n}',b'void sdMemBlkWaitSync(void)\n{\n    jp_sd_wait_transfer();\n}','service actual posted transfer completion instead of empty host spin')
    replace('sd_memblk_transfer',b'                sdMemBlkWaitSync();',b'                sdMemBlkWaitSync();\n                if (jp_sd_transfer_error) { err=SDE_ERR_UNKNOW_NUM; goto error; }','bound synchronous wait on real transport failure')
    replace('sd_memblk_transfer',b'    block->done = false;',b'    block->done = false;\n    jp_sd_transfer_error=0;','reset synchronous wait error for each requested transfer')
    replace('sd_move_wrk_mem_to_snd_mem',b'        if (callback_func != SDD_MEMBLK_NO_FUNC &&',b'        if (sdgG2LibResult != SYE_G2_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;\n        if (callback_func != SDD_MEMBLK_NO_FUNC &&','failed native copy must not invoke successful completion')
    replace('sd_memblk_dma_transfer',b'        sdMemBlkG2DmaErrTrap();',b'        sdMemBlkG2DmaErrTrap();\n        err=SDE_ERR_UNKNOW_NUM; goto error;','return actual setup/start failure rather than continuing',2)
    # Reset accesses and the full memory fill must also propagate real failures.
    for unit in ('sd_aica_reset_keep','sd_aica_reset_rel'):
        raw=raws[unit]
        for call in re.findall(rb'    syG2(?:Read|Write)\([^;]+\);',raw):
            new=b'    if ('+call.strip()[:-1]+b' != SYE_G2_ERR_NOTHING) { set_imask(mask); return SDE_ERR_UNKNOW_NUM; }'
            replace(unit,call,new,'propagate actual reset-register access failure')
    replace('sd_snd_mem_clear',b'    sdARM7ResetKeep();',b'    if (sdARM7ResetKeep()!=SDE_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','propagate real reset failure before fill')
    raw=raws['sd_snd_mem_clear'];call=re.search(rb'    syG2Write\([^;]+\);',raw).group()
    replace('sd_snd_mem_clear',call,b'    if ('+call.strip()[:-1]+b' != SYE_G2_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','propagate real RAM fill failure')
    replace('sd_drv_init',b'    Sint32 delay;',b'    driver_block=NULL;','do not destroy a retained stale descriptor on early failure')
    for call in re.findall(rb'        (?:sdARM7ResetKeep|sdSndMemClear|sdMemBlkCreate|sdMemBlkSetPrm|sdMemBlkTransfer|sdARM7ResetRel|sdDrvGetInfo)\([^;]+\);',raws['sd_drv_init']):
        replace('sd_drv_init',call,b'        err='+call.strip()[:-1]+b';\n        if (err!=SDE_ERR_NOTHING) goto error;','propagate actual initialization/transfer errors')
    raw=raws['sd_drv_init'];start=raw.index(b'        for (delay = DRIVER_START_DELAY;');end=raw.index(b'        sdgExistSndDrv = true;',start)
    replace('sd_drv_init',raw[start:end],b'        if (jp_sd_wait_driver()) { err=SDE_ERR_SND_DRV_BUSY; goto error; }\n','real bounded ARM/AICA progress and actual ready byte replace desktop spin')
    after={name:hashlib.sha256(body).hexdigest() for raw in raws.values() for name,body in functions(raw).items()}
    adapter=p/'jp_port_tools/sd_g2_adapter.inc';fixture=p/'jp_port_tools/sd_driver_observation.inc'
    prefix=b'''#include <stdint.h>
#include "shinobi/sd_work.h"
#include <machine.h>
#define GCC
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#define JP_G2_API __declspec(dllimport)
#define JP_AICA_API __declspec(dllimport)
#include "jp_native_g2_audio.h"
#include "jp_native_aica.h"
#undef get_imask
#undef set_imask
#define get_imask() jp_g2_gate_get()
#define set_imask(level) ((void)jp_g2_gate_set(level))
static int jp_sd_wait_driver(void);
'''
    generated=d/'external_sd_driver.c';generated.write_bytes(prefix+adapter.read_bytes()+b'\n'+b'\n'.join(raws.values())+b'\n'+fixture.read_bytes())
    flags=[str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter','-include',str(p/'native/jp_host_types.h'),'-I'+str(inc),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk')]
    commands=[flags+['-c',str(generated),'-o',str(d/'sd_driver.obj')],
        [str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sd_driver.obj'),str(d/'jp_native_g2_audio.lib'),str(d/'jp_native_aica.lib'),*['-Wl,/export:'+name for name in after],'-Wl,legacy_stdio_definitions.lib','-o',str(d/'jp_native_sd_driver.dll')]]
    results=[]
    for index,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{index}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for n,h in originals.items():assert sha(u/n)==h,n
    result=dict(status='original_SDK_constructor_transfers_and_driver_native_linked_unverified',commands=results,DLL=str(d/'jp_native_sd_driver.dll'),DLL_sha256=sha(d/'jp_native_sd_driver.dll'),
        original_source_sha256=originals,original_function_sha256=before,adapted_function_sha256=after,unchanged_function_bodies=sum(before[n]==after[n] for n in before),native_adaptations=changes,
        private_header_sha256=sha(inc/'shinobi.h'),generated_source_sha256=sha(generated),g2_build_sha256=sha(a.g2/'build_verification.json'),
        dependencies={n:sha(d/n) for n in ('jp_native_g2_audio.dll','jp_native_g2_audio.lib','jp_native_aica.dll','jp_native_aica.lib','jp_native_audio.dll')},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in (adapter,fixture,Path(__file__).resolve(),p/'native/jp_host_types.h')},
        no_seeded_work_or_success_only_hardware_exports=True,artifact_publishable=False,physical_console_timing_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(UNITS),'original modules',flush=True)

if __name__=='__main__':main()
