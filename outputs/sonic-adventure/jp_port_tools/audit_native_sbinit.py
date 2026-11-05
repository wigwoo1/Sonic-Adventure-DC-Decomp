"""Inventory original system bootstrap after actual Ninja device bring-up.
The console-address/mask probe is compile-only and is never linked/executed.
"""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','device','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--sdk-heap',dest='sdk_heap',type=Path,help='Optional independently built original SDK heap provider')
    ap.add_argument('--rtc',type=Path,help='Optional independently built original-calendar/native-clock RTC provider')
    ap.add_argument('--gdfs',type=Path,help='Optional qualified original-GDFS/native-media provider; export availability alone is not parent integration')
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.baseline/'build_verification.json').read_bytes());owner=json.loads((a.device/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    source=u/'src/system/sbinit.c';raw=source.read_bytes();body=function(raw,'sbInitSystem');git='C:/Program Files/Git/cmd/git.exe'
    assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:src/system/sbinit.c'])).hexdigest()==sha(source)
    d.mkdir();generated=d/'external_sbinit_probe.c';generated.write_bytes(raw[:raw.index(body)]+body)
    common=list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and any(n.startswith('-I') and Path(n[2:]).resolve()==u/'sdk/include' for n in x['command'])))
    common=common[:common.index('-c')]
    # A native SDK-only object can be first in a composed graph. The original
    # sbinit probe also needs the game's private system/cache.h include root.
    game_include='-I'+str(u/'src')
    if game_include not in common:common.append(game_include)
    obj=d/'sbinit_probe.obj';cmd=common+['-Wno-pointer-to-int-cast','-Wno-int-to-pointer-cast','-c',str(generated),'-o',str(obj)]
    run=subprocess.run(cmd,capture_output=True);(d/'compile.txt').write_bytes(run.stdout+run.stderr)
    assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()
    modules={n:dict(path=b[n],sha256=sha(b[n])) for n in ('frames_DLL','GPU_DLL','present_DLL','vertex_lifecycle_DLL','DLL','authored_exit_DLL')}
    for kind in ('frames','GPU','present','vertex_lifecycle'):assert owner[kind+'_DLL']==b[kind+'_DLL'] and owner[kind+'_DLL_sha256']==sha(b[kind+'_DLL'])
    assert sha(owner['device_DLL'])==owner['device_DLL_sha256'];modules['device_DLL']=dict(path=owner['device_DLL'],sha256=owner['device_DLL_sha256'])
    if a.sdk_heap:
        sdk=json.loads((a.sdk_heap/'build_verification.json').read_bytes())
        assert sha(sdk['DLL'])==sdk['DLL_sha256']
        modules['SDK_heap_DLL']=dict(path=sdk['DLL'],sha256=sdk['DLL_sha256'])
    if a.rtc:
        rtc=json.loads((a.rtc/'build_verification.json').read_bytes())
        assert sha(rtc['RTC_DLL'])==rtc['RTC_DLL_sha256']
        modules['RTC_DLL']=dict(path=rtc['RTC_DLL'],sha256=rtc['RTC_DLL_sha256'])
    if a.gdfs:
        gdfs=json.loads((a.gdfs/'build_verification.json').read_bytes())
        assert sha(gdfs['DLL'])==gdfs['DLL_sha256']
        modules['GDFS_DLL']=dict(path=gdfs['DLL'],sha256=gdfs['DLL_sha256'])
    libs={n:C.CDLL(v['path']) for n,v in modules.items()};providers={}
    for s in coff_symbols(obj):
        if s['section']!=0 or not s['name']:continue
        name=s['name'];providers[name]=dict(kind='data' if name=='_BSG_END' else 'function',owners=[n for n,lib in libs.items() if hasattr(lib,name)])
    missing={n:v for n,v in providers.items() if not v['owners']}
    result=dict(status='complete_original_sbInitSystem_compile_and_actual_dependencies_inventoried_not_linked_or_executed',command=cmd,compile_exit_code=run.returncode,diagnostic_bytes=len(run.stdout+run.stderr),
        original_source_sha256=sha(source),original_function_sha256=hashlib.sha256(body).hexdigest(),generated_source_sha256=sha(generated),object_sha256=sha(obj),
        original_function_text_unchanged=function(generated.read_bytes(),'sbInitSystem')==body,
        unsafe_original_console_heap_address_and_pointer_truncating_alignment_retained_in_unexecuted_probe=True,
        warning_suppression_limited_to_known_original_pointer_casts_in_compile_only_probe=True,
        RTC_provider_available=bool(a.rtc),RTC_export_availability_does_not_prove_complete_parent_execution=True,
        GDFS_provider_available=bool(a.gdfs),GDFS_export_availability_does_not_prove_complete_parent_execution=True,
        undefined_symbols=providers,missing_symbols=missing,actual_DLLs=modules,
        native_Ninja_initializer_available='device_DLL' in providers['njInitSystem']['owners'],
        original_heap_pointer_width_and_interrupt_cache_GDFS_peripheral_RTC_need_actual_host_adaptation=True,
        native_device_exit_authority_integration_pending=not b.get('borrowed_device_handles_never_independently_owned_by_exit',False),SDK_heap_export_availability_only_not_parent_integration=bool(a.sdk_heap),baseline_build_sha256=sha(a.baseline/'build_verification.json'),device_build_sha256=sha(a.device/'build_verification.json'),
        auditor_sha256=sha(__file__),never_linked_or_executed=True,overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(providers),'imports',len(missing),'unavailable',flush=True)
if __name__=='__main__':main()
