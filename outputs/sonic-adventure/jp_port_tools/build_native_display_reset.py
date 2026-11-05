"""Link the genuine complete display_reset_full with actual main-owned data.
All game-source objects and resulting runtime DLLs remain private. Existing
SDK/numeric/queue/texture/frame/GPU owners are reused, never mocked or doubled.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(row['exit_code']==row['diagnostic_bytes']==0 for row in b['commands'])
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    oldfront=read(r/'work/sonic-adventure/JP_NATIVE_PRINT_FRONTIER_AFTER_RANDOM_DIRECT.json');main_source=u/'src/main.c';assert sha(main_source)==oldfront['main_source_sha256'];raw=main_source.read_bytes();body=function(raw,'display_reset_full');assert hashlib.sha256(body).hexdigest()==oldfront['display_reset_function_sha256']
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']));kept={n:sha(n) for n in link if n.endswith('.obj')};defs={}
    for n in kept:
        for s in coff_symbols(Path(n)):
            if s['section']>0:defs.setdefault(s['name'],[]).append(n)
    assert 'display_reset_full' not in defs
    owners=['matrix_stack','screen','view','texmemlist','fog_table'];assert all(n not in defs for n in owners),{n:defs[n] for n in owners if n in defs}
    d.mkdir();(d/'frames').mkdir();common=list(next(row['command'] for row in b['commands'] if row['command'][-1]==str(base/'print_sdk.obj')));common=common[:common.index('-c')]
    prelude=b'#include <ninja.h>\n#include <sg_xpt.h>\n#include <stdlib.h>\n#include "main.h"\n#include "draw/model.h"\n#define MATRIX_DEPTH 128\nextern NJS_MATRIX matrix_stack[];\n'
    probe=d/'external_display_reset_probe.c';probe.write_bytes(prelude+body);probe_cmd=common+['-c',str(probe),'-o',str(d/'display_reset_probe.obj')]
    run=subprocess.run(probe_cmd,capture_output=True);(d/'probe_compile.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()[:6000]
    symbols=coff_symbols(d/'display_reset_probe.obj');undefined=sorted(s['name'] for s in symbols if s['section']==0 and s['value']==0);abi=sorted(set(undefined)&{'_fltused'});missing=sorted(set(undefined)-set(defs)-set(abi));assert missing==sorted(owners),(missing,undefined)
    declarations=[]
    for name in owners:
        found=re.findall(rb'^NJS_[A-Z_]+\s+'+name.encode()+rb'[^;\n]*;\n',raw,re.M);assert len(found)==1,(name,found);declarations.append(found[0])
    # SHC's toward-zero decimal literal is the lower adjacent binary32 value.
    # Preserve that exact value with an exactly representable C hex literal;
    # native Clang otherwise rounds decimal 0.05F to the upper adjacent value.
    before=b'njPolygonCullingSize(0.05F);';after=b'njPolygonCullingSize(0x1.999998p-5F);';assert body.count(before)==1;native_body=body.replace(before,after)
    sdk=d/'external_main_display_reset.c';sdk.write_bytes(prelude+b''.join(declarations)+native_body+b'\n_Static_assert(sizeof(NJS_MATRIX)==64 && sizeof(Float)==4 && sizeof(void*)==8,"native main display data ABI");\n')
    commands=[common+['-c',str(sdk),'-o',str(d/'main_display_reset.obj')]]
    exports=['display_reset_full',*['njInitMatrix','njGetMatrix','njInitView','njSetView','njSetScreen','njControl3D','njSetConstantAttr','njSetConstantMaterial','njSetDepthQueue','njClipZ','njTextureFilterMode','njSuperSampleMode','njTextureClampMode','njTextureFlipMode','njPolygonCullingMode','njPolygonCullingSize','njSetFogColor','njSetFogDensity','njGenerateFogTable','njSetFogTable','njFogEnable']]
    already={x.split('/export:',1)[1].split(',')[0] for x in link if '/export:' in x}
    data_exports=owners+['_nj_base_matrix_ptr_','_nj_current_matrix_ptr_','_stack_size_','_max_stack_size_','_nj_matrix_flag_','_nj_view_','_nj_view_matrix_','_nj_screen_','_nj_scr_aspect_x_','_nj_scr_aspect_y_','_nj_2d_clip_','_nj_control_3d_flag_','_nj_constant_attr_and_','_nj_constant_attr_or_','_nj_constant_material_','_nj_depth_near_','_nj_depth_far_','_nj_depth_range_','_nj_near_clip_','_nj_far_clip_','_rnext']
    assert all(n in defs or n in owners for n in data_exports)
    at=link.index('-o');link[at:at]=[str(d/'main_display_reset.obj')]+['-Wl,/export:'+n for n in exports if n not in already]+[x for n in data_exports if n not in already for x in ['-Xlinker','/export:'+n+',DATA']];link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    newlibs={'frames_DLL':d/'frames/jp_native_frames.lib'}
    for label,name,imports in [('GPU_DLL','jp_native_window_input.dll',['frames_DLL']),('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b[label]))
        for n in imports:
            previous=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(previous)==1;cmd[cmd.index(previous)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(n)==h,n
    generated=dict(b['generated_sources_sha256']);generated[sdk.name]=sha(sdk)
    for n,h in b['generated_sources_sha256'].items():target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    shutil.copytree(base/'inputs',d/'inputs');sources=dict(b['authored_source_sha256']);sources[Path(__file__).resolve().relative_to(r).as_posix()]=sha(__file__)
    shared=dict(status='complete_genuine_display_reset_and_main_owned_data_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=kept,all_prior_production_objects_preserved=True,main_display_source_sha256=sha(main_source),main_display_original_function_sha256=hashlib.sha256(body).hexdigest(),main_display_native_function_sha256=hashlib.sha256(native_body).hexdigest(),main_display_literal_adaptation=dict(original='0.05F',native='0x1.999998p-5F',original_RZ_binary32_word='0x3d4ccccc',native_default_RNE_word='0x3d4ccccd',only_native_body_change=True),main_display_data_declarations_sha256={n:hashlib.sha256(x).hexdigest() for n,x in zip(owners,declarations)},main_display_object_sha256=sha(d/'main_display_reset.obj'),main_display_probe=dict(command=probe_cmd,exit_code=0,diagnostic_bytes=0,source_sha256=sha(probe),object_sha256=sha(d/'display_reset_probe.obj'),undefined_symbols=undefined,compiler_ABI_imports_resolved_by_existing_CRT=abi,original_functions_already_provided={n:defs[n] for n in undefined if n in defs},only_missing_game_symbols_were_main_data=missing),previous_format_build_sha256=sha(base/'build_verification.json'),frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),original_display_reset_body_unchanged=False,original_display_reset_control_flow_and_call_order_unchanged=True,main_owned_matrix_screen_view_texture_fog_state_linked=True,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
