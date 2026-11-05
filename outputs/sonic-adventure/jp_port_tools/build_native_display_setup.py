"""Integrate the real original display setters and native assembly value ports.
Keep each old source/proof/object intact. Reuse the retained frame link recipe
because the latest GPU-only build deliberately does not rebuild that owner.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
NAMES=['njTextureFilterMode','njSuperSampleMode','njTextureClampMode','njTextureFlipMode','njPolygonCullingSize','njPolygonCullingMode']
EXPORTS=NAMES+['njTan','njSetPerspective']
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','frame-recipe','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,recipe,c,d=a.workspace,a.upstream,a.baseline,a.frame_recipe,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');old=read(recipe/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert b['visible_GPU_table_fog_implemented'] and sha(c)==b['compiler_sha256']==old['compiler_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256']==old['frames_DLL_sha256']
    assert sha(recipe/'build_verification.json')==b['previous_boot_fog_build_sha256']
    for report in [b,old]:
        for n,h in report['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir();(d/'frames').mkdir()
    raw=(u/'sdk/ninja/njSystem.c').read_bytes();bodies={n:function(raw,n) for n in NAMES};source=d/'external_display_setters.c';source.write_bytes(b'#include <kamui.h>\n#include <ninja.h>\n#include <sg_xpt.h>\n'+b'\n'.join(bodies.values()))
    common=list(old['commands'][0]['command']);common=common[:common.index('-c')];projection=p/'native/jp_native_projection.c'
    commands=[common+['-c',str(source),'-o',str(d/'display_setters.obj')],common+['-c',str(projection),'-o',str(d/'projection.obj')],common+['-S','-emit-llvm',str(projection),'-o',str(d/'projection.ll')]]
    fl=list(next(row['command'] for row in old['commands'] if '-shared' in row['command'] and row['command'][-1]==old['frames_DLL']));kept={n:sha(n) for n in fl if n.endswith('.obj')}
    assert not any(s['section']>0 and s['name'] in EXPORTS for n in kept for s in coff_symbols(Path(n)))
    fl[fl.index('-o'):fl.index('-o')]=[str(d/'display_setters.obj'),str(d/'projection.obj'),*['-Wl,/export:'+n for n in EXPORTS]];fl[-1]=str(d/'frames/jp_native_frames.dll');commands.append(fl)
    gl=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['GPU_DLL']));oldlib=str(Path(b['frames_DLL']).with_suffix('.lib'));assert gl.count(oldlib)==1;gl[gl.index(oldlib)]=str(d/'frames/jp_native_frames.lib');gl[-1]=str(d/'jp_native_window_input.dll');commands.append(gl)
    newlibs={'frames_DLL':d/'frames/jp_native_frames.lib','GPU_DLL':d/'jp_native_window_input.lib'}
    for label,name,imports in [('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b[label]))
        for n in imports:
            oldlib=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(oldlib)==1;cmd[cmd.index(oldlib)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(d/'projection.ll').read_text())
    for n,h in kept.items():assert sha(n)==h,n
    for n,h in b['generated_sources_sha256'].items():shutil.copyfile(base/n,d/n);assert sha(d/n)==h
    shutil.copytree(base/'inputs',d/'inputs');authored=dict(b['authored_source_sha256'])
    for f in [projection,Path(__file__).resolve()]:authored[f.relative_to(r).as_posix()]=sha(f)
    generated=dict(b['generated_sources_sha256']);generated[source.name]=sha(source)
    shared=dict(status='original_display_setters_and_projection_value_ports_linked_behavior_unverified',commands=results,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),authored_source_sha256=authored,generated_sources_sha256=generated,kept_frame_objects_sha256=kept,display_functions=EXPORTS,original_display_source_sha256=sha(u/'sdk/ninja/njSystem.c'),original_display_function_sha256={n:hashlib.sha256(body).hexdigest() for n,body in bodies.items()},original_assembly_sources_sha256={n:sha(u/n) for n in ['sdk/ninja/njTan.c','sdk/ninja/njSetPerspective.c']},native_projection_source_sha256=sha(projection),projection_uses_existing_RZ_DN1_FSCA_providers=True,residual_native_projection_FP_arithmetic=0,existing_GPU_object_sha256=b['authored_GPU_object_sha256'],frame_owner_bytes_unchanged=False,original_sources_modified=False,visible_GPU_table_fog_implemented=True,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:shutil.copyfile(d/n,fd/n)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'),previous_GPU_fog_build_sha256=sha(base/'build_verification.json'),frame_recipe_sha256=sha(recipe/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
