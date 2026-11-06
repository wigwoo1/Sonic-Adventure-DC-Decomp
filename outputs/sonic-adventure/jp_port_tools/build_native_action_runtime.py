"""Link real action math,then the original GCC action interpreter with RZ ops."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite,span
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('dependencies','action'),required=True)
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'];link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')};symbols={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0}
    original={}
    for name in ('sdk/ninja/njRotateXYZ.c','sdk/ninja/njRotateZXY.c','sdk/ninja/njGlobal.c','src/draw/action.c'):
        raw=(u/name).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+name]);original[name]=sha(u/name)
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    compiler=b['commands'][0]['command'][0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    commands=[];generated={};added=[];arithmetic={};names=[];fields=[];extras={}
    if a.stage=='dependencies':
        exports=('njRotateXYZ','njRotateZXY','jp_action_fipr_self','jp_action_fsrra');assert not set(exports)&symbols
        for name,obj in [('jp_ninja_rotation_order.c','rotation_order_native.obj'),('jp_native_action_numeric.c','action_numeric_native.obj')]:
            commands.append(host+['-c',str(p/'native'/name),'-o',str(d/obj)]);added.append(str(d/obj))
    else:
        assert {'njRotateXYZ','njRotateZXY','jp_action_fipr_self','jp_action_fsrra'}<=symbols
        raw=(u/'src/draw/action.c').read_bytes();anchor=b'#include "draw/model.h"\n';assert raw.count(anchor)==1
        context=b'''/* Select only the existing upstream GCC body branches. The existing native
 * SDK types/matrix calls remain unchanged; real RZ numerical providers follow. */
#include "jp_native_action_numeric.h"
#include "jp_native_sprite_numeric.h"
#define GCC 1
#define ALWAYS_INLINE static inline __attribute__((always_inline))
#define zam_fipr_self jp_action_fipr_self
#define _builtin_fsrra jp_action_fsrra
'''
        raw=raw.replace(anchor,anchor+context);initial=d/'external_action_unrewritten.c';initial.write_bytes(raw);generated[initial.name]=sha(initial)
        recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj');sdk=recipe[:recipe.index('-c')]
        ast_cmd=sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(initial)];run=subprocess.run(ast_cmd,capture_output=True);(d/'action_ast_diagnostics.txt').write_bytes(run.stderr);assert not run.returncode and not run.stderr,run.stderr.decode(errors='replace')[:6000]
        tree=json.loads(run.stdout)
        for node in tree['inner']:
            if node.get('kind')!='FunctionDecl' or node.get('loc',{}).get('includedFrom'):continue
            bodies=[v for v in node.get('inner',[]) if v.get('kind')=='CompoundStmt']
            if bodies:
                loc=node['loc'];assert raw[loc['offset']:loc['offset']+loc['tokLen']].decode()==node['name'];assert span(bodies[0]);names.append(node['name'])
        assert 'draw_action' in names and 'action_start_motion' in names and len(names)>30
        adapted,arithmetic=rewrite(raw,tree,names);unit=d/'external_action_native_rz.c';unit.write_bytes(adapted);generated[unit.name]=sha(unit);ir=d/'action_native_rz.ll'
        commands += [sdk+['-c',str(unit),'-o',str(d/'action_interpreter_original.obj')],sdk+['-S','-emit-llvm',str(unit),'-o',str(ir)]];added.append(str(d/'action_interpreter_original.obj'))
        fields=['_nj_motion_data_func_','_nj_shape_data_func_','_nj_motion_translate_','_nj_motion_rotate_','_nj_motion_scale_','_nj_motion_data_info_','_nj_shape_data_info_'];assert not set(fields)&symbols
        definitions=[];source=(u/'sdk/ninja/njGlobal.c').read_bytes()
        for name in fields:
            rows=re.findall(rb'^[^\r\n]*\b'+name.encode()+rb'\b[^\r\n]*;\r?\n',source,re.M);assert len(rows)==1,(name,rows);definitions.append(rows[0])
        globals_=d/'external_action_original_state.c';globals_.write_bytes(b'#include <ninja.h>\n'+b''.join(definitions));generated[globals_.name]=sha(globals_);commands.append(sdk+['-c',str(globals_),'-o',str(d/'action_original_state.obj')]);added.append(str(d/'action_original_state.obj'))
        exports=[n for n in names if not re.search(rb'\bstatic\s+[^\n]*\b'+n.encode()+rb'\(',raw) and n!='squared_length'];assert 'draw_action' in exports
        extras=dict(action_AST_command=ast_cmd,action_arithmetic_adaptation=arithmetic,action_source_function_names=names,action_original_state_fields=fields)
    link[link.index('-o'):link.index('-o')]=added+['-Wl,/export:'+n for n in exports];link[-1]=str(d/'jp_native_frames.dll');commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        executed=cmd;response={}
        if len(subprocess.list2cmdline(cmd))>=30000:
            content='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n';assert shlex.split(content)==cmd[1:];path=d/f'action_{a.stage}_build_{i}.rsp';path.write_text(content);executed=[cmd[0],'@'+str(path)];response=dict(response_file=str(path),response_file_sha256=sha(path))
        run=subprocess.run(executed,capture_output=True);log=d/f'action_{a.stage}_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,executed_command=executed,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,**response));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()[:7000]
    if a.stage=='action':
        residual=re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text());assert not residual,residual[:15];extras.update(action_LLVM_IR_sha256=sha(ir),residual_native_action_FP_arithmetic=0)
    for n,h in kept.items():assert sha(n)==h
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_ninja_rotation_order.h','jp_ninja_rotation_order.c','jp_native_action_numeric.h','jp_native_action_numeric.c')]+[Path(__file__).resolve()]
    prefix='action_'+a.stage;updates.update(status=prefix+'_same_actual_Frame_original_GCC_source_and_RZ_value_services_linked_unverified',**{prefix+'_baseline_sha256':sha(base/'build_verification.json'),prefix+'_new_build_commands':len(results),prefix+'_original_source_sha256':original,prefix+'_generated_source_sha256':generated,prefix+'_kept_frame_objects_sha256':kept},**extras,commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{n.relative_to(r).as_posix():sha(n) for n in authored}),actual_action_animation_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(results),'clean commands',len(names),'action functions',flush=True)
if __name__=='__main__':main()
