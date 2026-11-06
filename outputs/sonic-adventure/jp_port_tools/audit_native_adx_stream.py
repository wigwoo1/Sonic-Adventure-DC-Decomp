"""Compile original stream task without executing its fixed-address work setup."""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    originals={n:sha(u/n) for n in ('src/sound/stream.c','src/sound/stream.h')};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    source=d/'external_original_stream.c';source.write_bytes(b'#include <ninja.h>\n#include <shinobi.h>\n#include <sofdec.h>\n#define GCC\n#include "gcc/src/system/port.h"\n'+(u/'src/sound/stream.c').read_bytes())
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    # Original stream_create assigns its task local only for SHC matching;
    # retain the real task_create side effect and suppress this one warning.
    cmd=common+['-Wno-unused-but-set-variable','-c',str(source),'-o',str(d/'stream.obj')];run=subprocess.run(cmd,capture_output=True);(d/'compile.log').write_bytes(run.stdout+run.stderr)
    assert not run.returncode and not (run.stdout+run.stderr),(run.returncode,(run.stdout+run.stderr).decode())
    keys=('frames_DLL','present_DLL','System_DLL','GDFS_DLL','game_sound_DLL');modules={};libraries=[]
    for key in keys:
        assert sha(b[key])==b[key+'_sha256'];modules[key]=C.CDLL(b[key]);libraries.append(str(Path(b[key]).with_suffix('.lib')))
    refs={}
    for row in coff_symbols(d/'stream.obj'):
        if row['section'] or not row['name']:continue
        n=row['name'];refs[n]=[key for key,lib in modules.items() if hasattr(lib,n.removeprefix('__imp_'))]
    target=d/'unexecuted_original_stream.dll';link=[common[0],'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'stream.obj'),*libraries,'-Wl,/export:stream_init_step','-Wl,/export:stream_request_voice','-Wl,/errorlimit:0','-o',str(target)]
    run=subprocess.run(link,capture_output=True);(d/'link.log').write_bytes(run.stdout+run.stderr)
    missing=sorted(set(n.decode() for n in re.findall(rb'undefined symbol: ([^\s]+)',run.stdout+run.stderr)))
    assert run.returncode and missing and not target.exists()
    result=dict(status='original_stream_task_native_compiles_real_ADXT_frontend_dependencies_inventoried_not_executed',
        compile_command=cmd,compile_exit_code=0,compile_diagnostic_bytes=0,expected_incomplete_link_command=link,expected_incomplete_link_exit_code=run.returncode,
        references=refs,actual_unresolved_link_symbols=missing,
        genuine_frontend_providers_not_implemented=[n for n in missing if n!='adxpd_internal_error'],
        implemented_core_state_needing_same_module_or_explicit_data_import=['adxpd_internal_error'],
        original_source_sha256=originals,generated_source_sha256=sha(source),object_sha256=sha(d/'stream.obj'),
        original_fixed_work_address='0x0cca8000',fixed_work_replaced=False,original_stream_module_linked_or_executed=False,
        dummy_ADXT_or_stream_services=False,build_verification_sha256=sha(a.build/'build_verification.json'),script_sha256=sha(__file__),
        required_next_step='Genuine ADXT/SJ/ADXF/RNA frontend and existing AICA/PCM owner integration with owned work buffers and ordered cleanup; no second independent unmixed output client.',
        playable=False,overall_completion_percent=10)
    (d/'dependency_inventory.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(missing),'unresolved dependencies',flush=True);print(', '.join(missing))
if __name__=='__main__':main()
