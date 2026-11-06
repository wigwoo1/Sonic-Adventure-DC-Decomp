"""Compile the entire pinned game sound module; inventory real remaining links."""
import argparse,ctypes as C,hashlib,json,re,subprocess,sys
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','system','sdk','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();inc=d/'include';(inc/'sound').mkdir(parents=True)
    names=('src/sound/sound.c','src/sound/sound.h');originals={n:sha(u/n) for n in names};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    header=(u/names[1]).read_bytes();parameter_count=len(re.findall(rb'\bint id\b',header));assert parameter_count==10
    overlay=b'#include <stdint.h>\n'+re.sub(rb'\bint id\b',b'intptr_t id',header);(inc/'sound/sound.h').write_bytes(overlay)
    raw=(u/names[0]).read_bytes();source_count=len(re.findall(rb'\bint id\b',raw));assert source_count>parameter_count
    raw=re.sub(rb'\bint id\b',b'intptr_t id',raw);assert raw.count(b'    Sint32 id;')==1 and raw.count(b'(int)task')==1
    raw=raw.replace(b'    Sint32 id;',b'    intptr_t id;').replace(b'(int)task',b'(intptr_t)task')
    # Parse console SDK declarations first. Game headers and function bodies then
    # select the actual upstream GCC guards, including repaired return values.
    prefix=b'#include <stdint.h>\n#include <ninja.h>\n#include <shinobi.h>\n#include <machine.h>\n#define GCC\n'
    suffix=b'\n_Static_assert(sizeof(((struct sound_slot *)0)->id)==sizeof(void *),"native sound identity retains pointer width");\n_Static_assert(sizeof(int)==4,"game scalars remain32bit");\n'
    generated=d/'external_sound.c';generated.write_bytes(prefix+raw+suffix)
    baseline=read(a.system/'build_verification.json');sdk=read(a.sdk/'build_verification.json');compiler=baseline['commands'][0]['command'][0]
    command=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffunction-sections','-fdata-sections','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter','-Wno-unused-variable','-include',str(p/'native/jp_host_types.h'),'-I'+str(inc),'-I'+str(a.sdk/'include'),'-I'+str(u/'sdk/include'),'-I'+str(u/'src'),'-I'+str(u/'modules'),'-c',str(generated),'-o',str(d/'sound.obj')]
    run=subprocess.run(command,capture_output=True);log=d/'compile.txt';log.write_bytes(run.stdout+run.stderr)
    (d/'compile_attempt.json').write_text(json.dumps(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size),indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    labels=('frames_DLL','GPU_DLL','present_DLL','vertex_lifecycle_DLL','device_DLL','SDK_heap_DLL','RTC_DLL','GDFS_DLL','System_DLL','authored_exit_DLL','DLL');modules={};providers={}
    for name in labels:
        assert sha(baseline[name])==baseline[name+'_sha256'];modules[name]=C.CDLL(baseline[name])
    assert sha(sdk['DLL'])==sdk['DLL_sha256'];modules['sound_SDK']=C.CDLL(sdk['DLL'])
    for symbol in coff_symbols(d/'sound.obj'):
        if symbol['section']!=0 or not symbol['name']:continue
        name=symbol['name'];providers[name]=dict(data_import=name.startswith('__imp_'),owners=[label for label,module in modules.items() if hasattr(module,name.removeprefix('__imp_'))])
    crt={'__chkstk','memset','_fltused'};missing=[n for n,row in providers.items() if not row['owners'] and n not in crt]
    libraries=[]
    for label in labels:
        library=Path(baseline[label]).with_suffix('.lib')
        if library.exists() and str(library) not in libraries:libraries.append(str(library))
    library=Path(sdk['DLL']).with_suffix('.lib')
    if str(library) not in libraries:libraries.append(str(library))
    link=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sound.obj'),*libraries,'-Wl,/export:sound_system_init','-Wl,/opt:ref','-Wl,/errorlimit:0','-o',str(d/'unexecuted_sound_constructor_probe.dll')]
    linked=subprocess.run(link,capture_output=True);(d/'link.txt').write_bytes(linked.stdout+linked.stderr)
    unresolved=sorted(set(n.decode() for n in re.findall(rb'undefined symbol: ([^\s]+)',linked.stdout+linked.stderr)))
    assert linked.returncode and unresolved and not (d/'unexecuted_sound_constructor_probe.dll').exists()
    binding_repairs=[n for n in unresolved if n in providers and providers[n]['owners']]
    for n,h in originals.items():assert sha(u/n)==h,n
    result=dict(status='entire_game_sound_module_native_compiles_remaining_genuine_providers_inventoried',command=command,exit_code=0,diagnostic_bytes=0,
        upstream_pin=PIN,original_source_sha256=originals,generated_source_sha256=sha(generated),object_sha256=sha(d/'sound.obj'),private_header_sha256=sha(inc/'sound/sound.h'),
        native_header_ID_prototypes=parameter_count,native_source_ID_parameters=source_count,native_sound_ID_bits=64,game_scalar_bits=32,
        upstream_GCC_game_branches_enabled_without_SH4_SDK_inlines=True,whole_game_sound_object_linked_or_executed=False,
        references=providers,standard_CRT_references=sorted(crt),missing_genuine_providers=missing,remaining_provider_count=len(missing),
        actual_link_command=link,actual_link_exit_code=linked.returncode,actual_unresolved_link_symbols=unresolved,
        existing_exports_requiring_native_link_declaration_repairs=binding_repairs,
        runtime_behavior_or_float_numerics_verified=False,init_path_missing_settings_providers=['syCfgInit','syCfgGetSoundMode','syCfgExit'],
        source_compatibility_requirement='Compile all native sound API callers against the same intptr_t ID header; do not mix old32bit task-ID callers with native64bit sound IDs.',
        system_build_sha256=sha(a.system/'build_verification.json'),SDK_build_sha256=sha(a.sdk/'build_verification.json'),
        authored_source_sha256={Path(__file__).resolve().relative_to(r).as_posix():sha(__file__),(p/'native/jp_host_types.h').relative_to(r).as_posix():sha(p/'native/jp_host_types.h')},
        artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(providers),'references;',len(missing),'provider names absent;',len(unresolved),'actual unresolved link symbols',flush=True)
if __name__=='__main__':main()
