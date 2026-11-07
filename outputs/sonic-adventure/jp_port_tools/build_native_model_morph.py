"""Derive and link six genuine model-morph helpers with existing RZ services."""
import argparse,hashlib,json,re,shlex,shutil,subprocess,sys
from pathlib import Path
def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','output'):ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(w)
    sys.path.insert(0,str(p/'jp_port_tools'));from build_native_read_queue import PIN,owner_function
    from rewrite_sprite_float_operations import rewrite
    from build_native_renderer_object_v3 import coff_symbols
    def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
    raw=(u/'src/system/util.c').read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/system/util.c']);b=json.loads((base/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    names=('model_blend','points_blend','points_blend_sine','points_blend_square','model_calc_face_normals','model_calc_point_normals');bodies={n:owner_function(raw,n) for n in names};macro=raw[raw.index(b'#define STRIP_LENGTH'):raw.index(b'void model_calc_face_normals')];assert b'#define CROSS' in macro
    adapted_bodies=dict(bodies)
    for name in ('points_blend','points_blend_sine','points_blend_square'):
        toward=b'*toward++' if name=='points_blend_sine' else b'*toward'
        before=b'(keep * *src) + ('+toward+b' * blend)';after=b'jp_model_morph_fmac(keep, *src, '+toward+b' * blend)'
        assert adapted_bodies[name].count(before)==1;adapted_bodies[name]=adapted_bodies[name].replace(before,after)
    unit=b'#include <ninja.h>\n#include "system/util.h"\n#include "jp_native_sprite_numeric.h"\n#include "jp_native_model_morph_numeric.h"\n'+macro+b'\n'.join(adapted_bodies.values());d.mkdir()
    for path in base.iterdir():
     if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(path,d/path.name)
     elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    source=d/'external_model_morph.c';source.write_bytes(unit)
    recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj');sdk=recipe[:recipe.index('-c')];commands=[]
    def run(cmd,name):
     x=subprocess.run(cmd,capture_output=True);(d/(name+'.stdout')).write_bytes(x.stdout);(d/(name+'.stderr')).write_bytes(x.stderr);commands.append(dict(command=cmd,exit_code=x.returncode,diagnostic_bytes=len(x.stderr)));assert not x.returncode and not x.stderr,(name,x.stderr.decode(errors='replace')[-6000:]);return x.stdout
    # Expand original vector macros using the real compiler before source-location
    # rewriting, so CROSS/VECTOR_DIFF retain their actual arithmetic expression order.
    expanded=d/'external_model_morph_expanded.c';expanded.write_bytes(run(sdk+['-E','-P',str(source)],'preprocess'))
    ast=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(expanded)],'ast'))
    adapted,stats=rewrite(expanded.read_bytes(),ast,names);rz=d/'external_model_morph_rz.c';rz.write_bytes(adapted);obj=d/'model_morph_original_rz.obj';run(sdk+['-c',str(rz),'-o',str(obj)],'compile');ir=d/'model_morph.ll';run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm');residual=re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text());assert not residual,residual
    symbols=coff_symbols(obj);undefined=sorted({x['name'] for x in symbols if x['section']==0});reader=Path(sdk[0]).with_name('llvm-readobj.exe');exports=run([str(reader),'--coff-exports',b['frames_DLL']],'frame_exports').decode();available=set(re.findall(r'^  Name: (.+)$',exports,re.M));missing=sorted(set(undefined)-available-{'_fltused','jp_model_morph_fmac'})
    out=dict(status='six_original_model_morph_helpers_native_RZ_object_compiled_runtime_unverified',source_functions=list(names),upstream_commit=PIN,original_source_sha256=sha(u/'src/system/util.c'),original_function_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},original_vector_macros_compiler_expanded=True,scalar_arithmetic_adaptation=stats,residual_native_FP_operations=0,undefined_object_symbols=undefined,missing_current_Frame_providers=missing,commands=commands,generated_source_sha256={x.name:sha(x) for x in (source,expanded,rz,ir)},object_sha256=sha(obj),baseline_build_sha256=sha(base/'build_verification.json'),recovery_script_sha256=sha(__file__),original_source_or_prior_proofs_modified=False,linked_runtime_or_morph_behavior_verified=False,whole_project_completion_percent=10,goal_active=True)
    assert not missing,missing
    numeric_obj=d/'model_morph_numeric.obj';run(sdk+['-c',str(p/'native/jp_native_model_morph_numeric.c'),'-o',str(numeric_obj)],'numeric_compile')
    out['original_instruction_FMAC_sites']={'points_blend':'0x8c058ea2','points_blend_sine':'0x8c058ef8','points_blend_square':'0x8c058f32'}
    out['FMAC_operand_policy']='keep * src + RZ(toward * blend); first product uses original fused multiply-accumulate'
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')};link[link.index('-o'):link.index('-o')]=[str(obj),str(numeric_obj)]+['-Wl,/export:'+n for n in names];link[-1]=str(d/'jp_native_frames.dll');text='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in link[1:])+'\n';assert shlex.split(text)==link[1:];rsp=d/'link.rsp';rsp.write_text(text);run([link[0],'@'+str(rsp)],'link')
    commands[-1].update(command=link,response_file=str(rsp),response_file_sha256=sha(rsp))
    for n,h in kept.items():assert sha(n)==h
    updates={}
    for k,v in b.items():
     if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    updates.update(status='six_original_model_morph_helpers_native_RZ_linked_runtime_unverified',model_morph_baseline_sha256=sha(base/'build_verification.json'),model_morph_recipe=out,model_morph_kept_frame_objects_sha256=kept,commands=commands+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{path.resolve().relative_to(r).as_posix():sha(path) for path in (Path(__file__),p/'native/jp_native_model_morph_numeric.c',p/'native/jp_native_model_morph_numeric.h')}),playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],'six original functions linked',flush=True)
if __name__=='__main__':main()
