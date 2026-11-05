"""Compile pinned original sound memory-block pool routines; private outputs."""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_gdfs import functions

PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
UNITS=('sd_global_variable','sd_sys_err_trap','sd_debug_printf','sd_memblk_init','sd_memblk_create','sd_memblk_destroy','sd_memblk_get_stat','sd_memblk_set_stat','sd_memblk_set_adr','sd_memblk_set_sz','sd_memblk_set_callback','sd_memblk_set_prm','sd_memblk_execute_callback')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','compiler','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    names=['sdk/include/shinobi.h','sdk/include/sg_xpt.h','sdk/include/stdarg.h','sdk/include/stdio.h','sdk/shinobi/sd_work.h']+['sdk/shinobi/'+n+'.c' for n in UNITS]
    git='C:/Program Files/Git/cmd/git.exe';originals={n:sha(u/n) for n in names}
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    inc=d/'include';inc.mkdir();header=(u/'sdk/include/shinobi.h').read_bytes()
    before=b'struct SDS_MEM_BLK {\n    Uint32 m_Member[8];\n};';newline=b'\r\n' if b'\r\n' in header else b'\n';before=before.replace(b'\n',newline)
    after=b'struct SDS_MEM_BLK {\n    _Alignas(void *) Uint8 m_Member[sizeof(void *) == 8 ? 48 : 32];\n};'.replace(b'\n',newline)
    assert header.count(before)==1;overlay=header.replace(before,after);(inc/'shinobi.h').write_bytes(overlay)
    sources=[(u/'sdk/shinobi'/f'{n}.c').read_bytes() for n in UNITS];bodies={name:hashlib.sha256(body).hexdigest() for raw in sources for name,body in functions(raw).items()}
    fixture=p/'jp_port_tools/sd_memory_fixture.inc';generated=d/'external_sd_memory.c';generated.write_bytes(b'#include "shinobi/sd_work.h"\n#define GCC\n#include <stdarg.h>\n#include <stddef.h>\n#include <string.h>\n'+b'\n'.join(sources)+b'\n'+fixture.read_bytes())
    flags=[str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(inc),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk')]
    commands=[flags+['-c',str(generated),'-o',str(d/'sd_memory.obj')],[str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sd_memory.obj'),*['-Wl,/export:'+name for name in bodies],'-Wl,legacy_stdio_definitions.lib','-o',str(d/'jp_native_sd_memory.dll')]];results=[]
    for j,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for n,h in originals.items():assert sha(u/n)==h,n
    result=dict(status='original_sound_memory_pool_native_opaque_64bit_ABI_linked_unverified',commands=results,DLL=str(d/'jp_native_sd_memory.dll'),DLL_sha256=sha(d/'jp_native_sd_memory.dll'),original_source_sha256=originals,original_function_sha256=bodies,original_function_bodies_unchanged=True,private_header_overlay_sha256=sha(inc/'shinobi.h'),
        generated_source_sha256=sha(generated),authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in (fixture,Path(__file__).resolve(),p/'native/jp_host_types.h')},test_fixture_seeds_controlled_work=True,sdLibInit_or_driver_G2_success_stubs_added=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],'original functions',len(bodies),flush=True)

if __name__=='__main__':main()
