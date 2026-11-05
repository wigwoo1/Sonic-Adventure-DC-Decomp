"""Compile original njInitSystem and identify actual next device dependencies.
The unadapted MMIO body is never linked or executed on the PC.
"""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--device',type=Path,help='Optional verified native device build directory; owners must match the selected baseline')
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.baseline/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    source=u/'sdk/ninja/njSystem.c';raw=source.read_bytes();body=function(raw,'njInitSystem');git='C:/Program Files/Git/cmd/git.exe'
    assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:sdk/ninja/njSystem.c'])).hexdigest()==sha(source)
    prefix=raw[:raw.index(b'const Uint32 _nj_version')]+b'''void waitVsync(void);
void njSetVideoMode(Uint32 cable);
void njSetEOVflag(void *arg);
void njSetERRflag(void *arg);
#define GCC
'''
    d.mkdir();generated=d/'external_ninja_device_init.c';generated.write_bytes(prefix+body)
    common=list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and any(n.startswith('-I') and Path(n[2:]).resolve()==u/'sdk/include' for n in x['command'])))
    common=common[:common.index('-c')];obj=d/'ninja_device_init.obj';cmd=common+['-c',str(generated),'-o',str(obj)];run=subprocess.run(cmd,capture_output=True);(d/'compile.txt').write_bytes(run.stdout+run.stderr)
    assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()
    calls={n.decode() for n in re.findall(rb'\b([A-Za-z_]\w*)\s*\(',re.sub(rb'/\*.*?\*/',b'',body,flags=re.S))}-{'if','for','while','switch','sizeof','njInitSystem'}
    modules={n:dict(path=b[n],sha256=sha(b[n])) for n in ('frames_DLL','GPU_DLL','present_DLL','vertex_lifecycle_DLL','DLL','authored_exit_DLL')}
    if a.device:
        owner=json.loads((a.device/'build_verification.json').read_bytes())
        for kind in ('frames','GPU','present','vertex_lifecycle'):
            assert owner[kind+'_DLL']==b[kind+'_DLL'] and owner[kind+'_DLL_sha256']==sha(b[kind+'_DLL']),kind
        assert sha(owner['device_DLL'])==owner['device_DLL_sha256']
        modules['device_DLL']=dict(path=owner['device_DLL'],sha256=owner['device_DLL_sha256'])
    libs={n:C.CDLL(v['path']) for n,v in modules.items()};providers={}
    for name in sorted({x['name'] for x in coff_symbols(obj) if x['section']==0 and x['name']}):
        owners=[]
        for owner,lib in libs.items():
            try:getattr(lib,name);owners.append(owner)
            except AttributeError:pass
        providers[name]=dict(kind='toolchain' if name=='_fltused' else 'function' if name in calls or name.startswith('njSet') else 'data',owners=owners)
    missing={n:v for n,v in providers.items() if not v['owners'] and v['kind']!='toolchain'}
    # An unexported object can already exist in the actual frame link. Record
    # it before recommending new state, to avoid shadow copies of SDK owners.
    for value in missing.values():value['current_frame_object_definitions']=[]
    for path in b['kept_frame_objects_sha256']:
        for symbol in coff_symbols(Path(path)):
            if symbol['section']>0 and symbol['name'] in missing:
                missing[symbol['name']]['current_frame_object_definitions'].append(path)
    result=dict(status='complete_original_njInitSystem_compile_and_actual_provider_inventory_not_linked_or_executed',command=cmd,compile_exit_code=run.returncode,diagnostic_bytes=len(run.stdout+run.stderr),original_source_sha256=sha(source),original_function_sha256=hashlib.sha256(body).hexdigest(),generated_source_sha256=sha(generated),object_sha256=sha(obj),original_function_text_unchanged=function(generated.read_bytes(),'njInitSystem')==body,upstream_GCC_invalid_framebuffer_format_repair_enabled=True,undefined_symbols=providers,missing_symbols=missing,missing_function_count=sum(v['kind']=='function' for v in missing.values()),missing_data_count=sum(v['kind']=='data' for v in missing.values()),missing_but_already_local_count=sum(bool(v['current_frame_object_definitions']) for v in missing.values()),actual_DLLs=modules,unadapted_video_MMIO_register_access_remaining=True,never_linked_or_executed=True,baseline_build_sha256=sha(a.baseline/'build_verification.json'),auditor_sha256=sha(__file__),overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(providers),'imports',len(missing),'unavailable',flush=True)
if __name__=='__main__':main()
