"""Compile the real Ninja global initializer and map actual missing providers.
This is a dependency probe, not a linked/runtime qualification or a fake init.
"""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.baseline/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    source=u/'sdk/ninja/njSystem.c';raw=source.read_bytes();body=function(raw,'njInitGlobal');pin='C:/Program Files/Git/cmd/git.exe'
    assert hashlib.sha256(subprocess.check_output([pin,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:sdk/ninja/njSystem.c'])).hexdigest()==sha(source)
    d.mkdir();generated=d/'external_ninja_global_init.c'
    prefix=b'''#include <kamui.h>
#include <ninja.h>
#include <sg_chain.h>
#include <sg_xpt.h>
#include <shinobi.h>
#include "ninja/njkamui.h"
void njInitContext(KMVERTEXCONTEXT *context);
void njSetCurrentContext(Int n);
extern Float _nj_cnk_easy_multilight_buffer_dst_[32];
static SYCHAIN vsync_handle;
#define GCC
'''
    generated.write_bytes(prefix+body)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    obj=d/'ninja_global_init.obj';cmd=common+['-c',str(generated),'-o',str(obj)];run=subprocess.run(cmd,capture_output=True);log=d/'compile.txt';log.write_bytes(run.stdout+run.stderr)
    assert not run.returncode and not log.stat().st_size,log.read_text()
    imports=sorted({s['name'] for s in coff_symbols(obj) if s['section']==0 and s['name']})
    toolchain={}
    if '_fltused' in imports:
        # This is the compiler's floating-point marker, not game state. Prove
        # the normal runtime resolves it without providing a made-up definition.
        fixture=d/'runtime_marker.c';fixture.write_text('extern int _fltused;\n__declspec(dllexport) int runtime_marker(void){return _fltused;}\n')
        runtime=d/'runtime_marker.dll';runtime_cmd=[common[0],'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(fixture),'-o',str(runtime)]
        rr=subprocess.run(runtime_cmd,capture_output=True);(d/'runtime_marker.txt').write_bytes(rr.stdout+rr.stderr)
        assert not rr.returncode and not rr.stdout and not rr.stderr,(rr.stdout+rr.stderr).decode()
        toolchain['_fltused']=dict(provider='actual_default_Windows_C_runtime_link',command=runtime_cmd,fixture_sha256=sha(fixture),DLL_sha256=sha(runtime))
    calls=set(re.findall(rb'\b([A-Za-z_]\w*)\s*\(',re.sub(rb'/\*.*?\*/',b'',body,flags=re.S)));calls={n.decode() for n in calls}-{'if','for','while','sizeof','njInitGlobal'}
    modules={n:dict(path=b[n],sha256=sha(b[n])) for n in ['frames_DLL','GPU_DLL','present_DLL','vertex_lifecycle_DLL','DLL','authored_exit_DLL']}
    libs={n:C.CDLL(v['path']) for n,v in modules.items()};providers={}
    for name in imports:
        found=[]
        for owner,lib in libs.items():
            try:getattr(lib,name);found.append(owner)
            except AttributeError:pass
        providers[name]=dict(kind='toolchain' if name in toolchain else 'function' if name in calls else 'data',owners=found)
    missing={n:v for n,v in providers.items() if not v['owners'] and n not in toolchain}
    result=dict(status='original_njInitGlobal_native_compile_and_actual_provider_frontier_audited_not_linked',command=cmd,compile_exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,original_source_sha256=sha(source),original_function_sha256=hashlib.sha256(body).hexdigest(),generated_source_sha256=sha(generated),object_sha256=sha(obj),upstream_GCC_16_plus_32_float_boundary_repair_enabled=True,original_function_text_unchanged=function(generated.read_bytes(),'njInitGlobal')==body,undefined_symbol_count=len(imports),undefined_symbols=providers,toolchain_providers=toolchain,missing_symbols=missing,missing_function_count=sum(x['kind']=='function' for x in missing.values()),missing_data_count=sum(x['kind']=='data' for x in missing.values()),actual_DLLs=modules,baseline_build_sha256=sha(a.baseline/'build_verification.json'),auditor_sha256=sha(__file__),linked_or_runtime_qualified=False,sbInitSystem_implemented=False,artifact_publishable=False,overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(imports),'imports',len(missing),'unavailable',flush=True)
if __name__=='__main__':main()
