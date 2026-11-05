"""Recover complete original Ninja global/light init with real shared state."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((a.baseline/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    fl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    names=['_nj_cnk_easy_multilight_buffer_','_nj_cnk_easy_multilight_buffer_dst_','_nj_motion_callback_','_nj_light_control_','_nj_light_queue_','_nj_light_material_']
    for obj in [x for x in fl if x.endswith('.obj')]:
        assert not any(s['section']>0 and s['name'] in names+['njInitGlobal','njLightSystemInit'] for s in coff_symbols(Path(obj))),obj
    originals={n:sha(u/n) for n in ['sdk/ninja/njSystem.c','sdk/ninja/njGlobal.c','sdk/ninja/njLightSystemInit.c','sdk/include/ninja.h','sdk/include/njtypes.h']}
    git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+n])).hexdigest()==h,n
    d.mkdir();raw=(u/'sdk/ninja/njSystem.c').read_bytes();body=function(raw,'njInitGlobal')
    prefix=b'''#include <kamui.h>
#include <ninja.h>
#include <sg_chain.h>
#include <shinobi.h>
#include "ninja/njkamui.h"
void njInitContext(KMVERTEXCONTEXT *context);
void njSetCurrentContext(Int n);
extern Float _nj_cnk_easy_multilight_buffer_dst_[32];
static SYCHAIN vsync_handle;
#define GCC
'''
    generated=d/'external_ninja_global_init.c';generated.write_bytes(prefix+body.replace(b'void njInitGlobal(void)',b'void jp_original_ninja_global(void)',1))
    # These original objects are reserved in njGlobal.c's SH-4 assembly. Keep
    # capacities/alignment, and use actual header types for native pointers.
    raw=(u/'sdk/ninja/njGlobal.c').read_bytes();motion=re.findall(rb'^void \(\*_nj_motion_callback_\)\(NJS_OBJECT \*obj\);',raw,re.M);assert len(motion)==1
    state=d/'external_ninja_light_state.c';state.write_bytes(b'''#include <ninja.h>
_Alignas(32) Float _nj_cnk_easy_multilight_buffer_[16];
_Alignas(32) Float _nj_cnk_easy_multilight_buffer_dst_[32];
_Alignas(32) NJS_LIGHT_MATERIAL _nj_light_material_;
_Alignas(32) struct nj_light_control _nj_light_control_;
_Alignas(32) struct nj_light_queue _nj_light_queue_;
'''+motion[0]+b'\n'+(u/'sdk/ninja/njLightSystemInit.c').read_bytes())
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    own=p/'native/jp_native_ninja_global.c';sources=[generated,state,own];commands=[];objects=[]
    for source in sources:
        obj=d/(source.stem+'.obj');objects.append(obj);commands.append(common+['-c',str(source),'-o',str(obj)])
    exports=['-Wl,/export:njInitGlobal','-Wl,/export:njLightSystemInit']+[arg for n in names for arg in ('-Xlinker','/export:'+n+',DATA')]
    fl[fl.index('-o'):fl.index('-o')]=[str(x) for x in objects]+exports;fl[-1]=str(d/'jp_native_frames.dll');commands.append(fl);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in originals.items():assert sha(u/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    result=dict(status='complete_original_Ninja_global_light_init_and_typed_shared_state_linked_unverified',commands=results,frames_DLL=str(d/'jp_native_frames.dll'),frames_DLL_sha256=sha(d/'jp_native_frames.dll'),present_DLL=b['present_DLL'],present_DLL_sha256=b['present_DLL_sha256'],authored_source_sha256={str(x.relative_to(r)):sha(x) for x in (own,own.with_suffix('.h'),Path(__file__).resolve())},generated_sources_sha256={x.name:sha(x) for x in (generated,state)},original_source_sha256=originals,original_global_function_sha256=hashlib.sha256(body).hexdigest(),original_light_function_sha256=hashlib.sha256(function((u/'sdk/ninja/njLightSystemInit.c').read_bytes(),'njLightSystemInit')).hexdigest(),original_global_body_preserved_except_private_entry_name=True,upstream_GCC_16_plus_32_float_repair_enabled=True,original_light_body_unchanged=True,new_original_data_owners=names,all_existing_global_providers_reused_in_single_frame_owner=True,shared_DLL_circular_dependency_added=False,kept_frame_objects_sha256={n:sha(n) for n in fl if n.endswith('.obj')},baseline_build_sha256=sha(a.baseline/'build_verification.json'),full_njInitSystem_or_sbInitSystem_implemented=False,artifact_publishable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
