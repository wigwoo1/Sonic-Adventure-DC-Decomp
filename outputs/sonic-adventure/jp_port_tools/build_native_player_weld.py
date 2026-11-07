"""Derive original weld make/update/free and tree indexing in the same Frame."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite
def sha(x):return hashlib.sha256(Path(x).read_bytes()).hexdigest()
def read(x):return json.loads(Path(x).read_bytes())
def layout_source():
 types={'player':['anim','anim.weld_mode','anim.actions','anim.welds','anim.frame']+[f'weld_point{i}' for i in range(6)]+[f'weld_axis{i}' for i in range(6)],'player_anim':['state','weld_mode','actions','welds','frame','action','blend'],'weld':['base','object_a','object_b','count','kind','node_a','node_b','vertices','indices'],'player_action':['action','nodes','mode','next','blend','speed'],'task_work':['event','pos','ang'],'event_work':['motions','previous','motion_frame'],'event_motion':['flags','blend','action','texlist','next','speed']};names=[];expressions=[]
 for name,fields in types.items():
  names.append(name+'.sizeof');expressions.append('sizeof(struct '+name+')')
  for field in fields:names.append(name+'.'+field);expressions.append('__builtin_offsetof(struct '+name+','+field+')')
 for name in ('NJS_OBJECT','NJS_MODEL','NJS_ACTION','NJS_MOTION'):names.append(name+'.sizeof');expressions.append('sizeof('+name+')')
 return names,'#include <ninja.h>\n#include "player/player.h"\n#include "event/event.h"\nconst unsigned int jp_event_pose_layout[] = {\n'+',\n'.join(expressions)+'\n};\n'
def main():
 ap=argparse.ArgumentParser()
 for name in ('workspace','upstream','baseline','output'):ap.add_argument('--'+name,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(base/'build_verification.json')
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']
 originals={}
 for name in ('src/player/player.c','src/system/util.c'):
  raw=(u/name).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+name]);originals[name]=raw
 body=owner_function(originals['src/player/player.c'],'player_update_welds');assert body.count(b'struct task_work *work,')==body.count(b'struct task_motion *motion,')==1;adapted_body=body.replace(b'struct task_work *work,',b'struct task_work *work __attribute__((unused)),').replace(b'struct task_motion *motion,',b'struct task_motion *motion __attribute__((unused)),');states=[]
 for name in ('player_weld_matrix_a','player_weld_matrix_b','player_weld_inverse_a','player_weld_inverse_b'):
  found=re.findall(rb'^static NJS_MATRIX '+name.encode()+rb';\r?\n',originals['src/player/player.c'],re.M);assert len(found)==1;states+=found
 raw=b'#include <ninja.h>\n#include <sg_xpt.h>\n#include "draw/pose.h"\n#include "player/player.h"\n#include "system/task.h"\n#include "system/util.h"\n#define GCC 1\n#include "jp_native_sprite_numeric.h"\n'+b''.join(states)+adapted_body
 tree=b'#include <ninja.h>\nstatic NJS_OBJECT *tree_find_node(NJS_OBJECT *,NJS_OBJECT *,int *);\n'+owner_function(originals['src/system/util.c'],'tree_node_index')+owner_function(originals['src/system/util.c'],'tree_find_node');link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')};providers={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0};assert not {'player_update_welds','tree_node_index'}&providers
 recipe=next(row['command'] for row in b['commands'] if '-c' in row['command'] and Path(row['command'][-1]).name=='character_tree_original.obj');sdk=recipe[:recipe.index('-c')]
 for suffix in ('sdk/include','src'):
  found=[i for i,x in enumerate(sdk) if x.startswith('-I') and Path(x[2:]).as_posix().endswith('/'+suffix)];assert len(found)==1;sdk[found[0]]='-I'+str(u/suffix)
 d.mkdir()
 for path in base.iterdir():
  if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(path,d/path.name)
  elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
 commands=[]
 def run(cmd,name):
  actual=cmd;extra={}
  if len(subprocess.list2cmdline(cmd))>=30000:
   text='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n';assert shlex.split(text)==cmd[1:];rsp=d/(name+'.rsp');rsp.write_text(text);actual=[cmd[0],'@'+str(rsp)];extra=dict(response_file=str(rsp),response_file_sha256=sha(rsp))
  x=subprocess.run(actual,capture_output=True);(d/(name+'.stdout')).write_bytes(x.stdout);(d/(name+'.stderr')).write_bytes(x.stderr);commands.append(dict(command=cmd,exit_code=x.returncode,diagnostic_bytes=len(x.stderr),**extra));assert not x.returncode and not x.stderr,(name,x.stderr.decode(errors='replace')[-6000:]);return x.stdout
 source=d/'external_player_update_welds.c';source.write_bytes(raw);ast=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],'ast'));adapted,stats=rewrite(raw,ast,['player_update_welds']);assert stats['total_operators_rewritten']==9;rz=d/'external_player_update_welds_rz.c';rz.write_bytes(adapted);obj=d/'player_update_welds.obj';run(sdk+['-c',str(rz),'-o',str(obj)],'compile');ir=d/'player_update_welds.ll';run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm');assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text());tree_source=d/'external_tree_index.c';tree_source.write_bytes(tree);tree_obj=d/'tree_index.obj';run(sdk+['-c',str(tree_source),'-o',str(tree_obj)],'tree_compile');available=providers|{s['name'] for path in (obj,tree_obj) for s in coff_symbols(path) if s['section']>0};undefined={s['name'] for path in (obj,tree_obj) for s in coff_symbols(path) if s['section']==0};assert not undefined-available-{'_fltused'}
 names,layout_c=layout_source();layout=d/'external_weld_layouts.c';layout.write_text(layout_c);layouts={};layout_files=[]
 for name,target in (('native','x86_64-pc-windows-msvc'),('original_LP32','i686-pc-windows-msvc')):
  target_sdk=[('--target='+target) if arg.startswith('--target=') else arg for arg in sdk];out=d/(name+'_weld_layouts.ll');run(target_sdk+['-S','-emit-llvm',str(layout),'-o',str(out)],name+'_layouts');found=re.search(r'@jp_event_pose_layout\s*=.*?constant \[(\d+) x i32\] \[(.*?)\]',out.read_text(),re.S);assert found;values=[int(x) for x in re.findall(r'i32 (-?\d+)',found[2])];assert len(values)==len(names)==int(found[1]);layouts[name]=dict(zip(names,values));layout_files.append(out)
 assert layouts['original_LP32']['player.sizeof']==512 and layouts['native']['player.sizeof']==600 and layouts['original_LP32']['NJS_OBJECT.sizeof']==52 and layouts['native']['NJS_OBJECT.sizeof']==72
 exports=['player_update_welds','tree_node_index'];link[link.index('-o'):link.index('-o')]=[str(obj),str(tree_obj)]+['-Wl,/export:'+n for n in exports];link[-1]=str(d/'jp_native_frames.dll');run(link,'link');assert all(sha(n)==h for n,h in kept.items());unchanged={x.name:sha(x) for x in base.glob('*.dll') if x.name!='jp_native_frames.dll'};assert all(sha(d/n)==h for n,h in unchanged.items());updates={}
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
 detail=dict(upstream_commit=PIN,original_source_sha256={n:hashlib.sha256(x).hexdigest() for n,x in originals.items()},original_function_sha256={'player_update_welds':hashlib.sha256(body).hexdigest(),**{n:hashlib.sha256(owner_function(originals['src/system/util.c'],n)).hexdigest() for n in ('tree_node_index','tree_find_node')}},original_private_matrix_definition_sha256=[hashlib.sha256(x).hexdigest() for x in states],unused_parameter_annotations=2,scalar_arithmetic_adaptation=stats,residual_host_FP_operations=0,generated_source_sha256={x.name:sha(x) for x in (source,rz,ir,tree_source,layout,*layout_files)},new_object_sha256={x.name:sha(x) for x in (obj,tree_obj)},layout_values=layouts,layout_constants_per_ABI=len(names),new_exports=exports,unchanged_DLL_sha256=unchanged,make_update_free_or_gameplay_verified=False)
 updates.update(status='genuine_player_weld_lifecycle_and_tree_index_linked_runtime_unverified',player_weld_baseline_sha256=sha(base/'build_verification.json'),player_weld_recipe=detail,player_weld_kept_frame_objects_sha256=kept,commands=commands+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],'three original functions;9 RZ operations;two clean layout builds',flush=True)
if __name__=='__main__':main()
