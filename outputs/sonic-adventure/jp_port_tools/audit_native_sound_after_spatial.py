"""Current full-module link frontier; no substitute services or execution."""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','upstream','build','output'):ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(a.build/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    originals={n:sha(u/n) for n in ('src/sound/sound.c','src/sound/sound.h')};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    inc=d/'include';(inc/'sound').mkdir(parents=True)
    header=(u/'src/sound/sound.h').read_bytes();assert len(re.findall(rb'\bint id\b',header))==10
    (inc/'sound/sound.h').write_bytes(b'#include <stdint.h>\n'+re.sub(rb'\bint id\b',b'intptr_t id',header))
    raw=(u/'src/sound/sound.c').read_bytes();raw=re.sub(rb'\bint id\b',b'intptr_t id',raw)
    for before,after in ((b'    Sint32 id;',b'    intptr_t id;'),(b'(int)task',b'(intptr_t)task'),
                         (b'static int qsound_owners[QSOUND_SES][QSOUND_COPIES];',b'static uintptr_t qsound_owners[QSOUND_SES][QSOUND_COPIES];')):
        assert raw.count(before)==1;raw=raw.replace(before,after)
    assert raw.count(b'sound_slots[slot].id + 1')==2
    raw=raw.replace(b'sound_slots[slot].id + 1',b'((uintptr_t)sound_slots[slot].id + (uintptr_t)1)')
    before=b'    side = (way.x + 1.0F) * 15.5F;\n    place = (int)side * 4;';assert raw.count(before)==1
    raw=raw.replace(before,b'    side = jp_qsound_position_side(way.x);\n    place = jp_qsound_position_channel(side);')
    prefix=b'#include <stdint.h>\n#include <ninja.h>\n#include <shinobi.h>\n#include <machine.h>\n#define GCC\nfloat jp_qsound_position_side(float);\nint32_t jp_qsound_position_channel(float);\n'
    suffix=b'\n_Static_assert(sizeof(((struct sound_slot *)0)->id)==sizeof(void *),"native ID");\n_Static_assert(sizeof(qsound_owners[0][0])==sizeof(void *),"native copy owner");\n'
    generated=d/'external_full_sound.c';generated.write_bytes(prefix+raw+suffix)
    compiler=b['commands'][0]['command'][0]
    cmd=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffunction-sections','-fdata-sections','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter','-Wno-unused-variable','-include',str(p/'native/jp_host_types.h'),'-I'+str(inc),'-I'+str(a.build.resolve()/'include'),'-I'+str(u/'sdk/include'),'-I'+str(u/'src'),'-I'+str(u/'modules'),'-c',str(generated),'-o',str(d/'sound.obj')]
    run=subprocess.run(cmd,capture_output=True);(d/'compile.log').write_bytes(run.stdout+run.stderr)
    assert not run.returncode and not (run.stdout+run.stderr),(run.returncode,(run.stdout+run.stderr).decode())
    keys=('frames_DLL','GPU_DLL','present_DLL','vertex_lifecycle_DLL','device_DLL','SDK_heap_DLL','RTC_DLL','GDFS_DLL','System_DLL','authored_exit_DLL','DLL','game_sound_DLL')
    modules={};libraries=[]
    for key in keys:
        assert sha(b[key])==b[key+'_sha256'];modules[key]=C.CDLL(b[key]);lib=str(Path(b[key]).with_suffix('.lib'))
        if lib not in libraries:libraries.append(lib)
    providers={}
    for row in coff_symbols(d/'sound.obj'):
        if row['section']!=0 or not row['name']:continue
        name=row['name'];providers[name]=dict(data_import=name.startswith('__imp_'),owners=[key for key,lib in modules.items() if hasattr(lib,name.removeprefix('__imp_'))])
    crt={'__chkstk','memset','memcpy','_fltused'};missing=sorted(n for n,row in providers.items() if not row['owners'] and n not in crt)
    target=d/'unexecuted_full_sound_audit.dll'
    link=[compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sound.obj'),*libraries,'-Wl,/export:sound_system_init','-Wl,/export:sound_send_queue','-Wl,/opt:ref','-Wl,/errorlimit:0','-o',str(target)]
    run=subprocess.run(link,capture_output=True);(d/'link.log').write_bytes(run.stdout+run.stderr)
    unresolved=sorted(set(n.decode() for n in re.findall(rb'undefined symbol: ([^\s]+)',run.stdout+run.stderr)))
    assert run.returncode and unresolved and not target.exists()
    repairs=[n for n in unresolved if n in providers and providers[n]['owners']]
    result=dict(status='entire_original_sound_module_current_spatial_graph_compiles_actual_link_frontier_inventoried',
        compile_command=cmd,compile_exit_code=0,compile_diagnostic_bytes=0,link_command=link,expected_incomplete_link_exit_code=run.returncode,
        build_verification_sha256=sha(a.build/'build_verification.json'),upstream_pin=PIN,original_source_sha256=originals,
        generated_source_sha256=sha(generated),object_sha256=sha(d/'sound.obj'),private_header_sha256=sha(inc/'sound/sound.h'),
        references=providers,missing_genuine_providers=missing,actual_unresolved_link_symbols=unresolved,
        existing_exports_requiring_native_link_declaration_repairs=repairs,
        native_ID_and_copy_owner_bits=64,other_full_module_float_numerics_or_runtime_verified=False,
        entire_original_sound_object_linked_or_executed=False,dummy_services_introduced=False,
        source_compatibility_requirement='All native callers must use the same intptr_t ID header and consistent native data-import declarations.',
        authored_source_sha256={Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)},
        artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(missing),'missing genuine providers;',len(unresolved),'actual unresolved symbols;',len(repairs),'existing-provider binding repairs',flush=True)
    print('Missing:',', '.join(missing));print('Binding repairs:',', '.join(repairs))

if __name__=='__main__':main()
