"""Derive the pinned original animation-link interpreter and genuine SDK state.

Generated game/SDK source and the combined DLL stay in the private work tree.
This builds an interpreter, not a playable game or a qualification certificate.
"""
import argparse,hashlib,json,re,shlex,shutil,subprocess,sys
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for name in ('workspace','upstream','baseline','output'):ap.add_argument('--'+name,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve()
 assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
 b=read(base/'build_verification.json')
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']
 originals={}
 for name in ('src/draw/action_link.c','sdk/ninja/njGlobal.c'):
  raw=(u/name).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+name]);originals[name]=raw
 raw=originals['src/draw/action_link.c'];anchor=b'#include "draw/model.h"\n';assert raw.count(anchor)==1
 context=b'#include "jp_native_action_numeric.h"\n#include "jp_native_sprite_numeric.h"\n#define GCC 1\n#define ALWAYS_INLINE static inline __attribute__((always_inline))\n#define zam_fipr_self jp_action_fipr_self\n#define _builtin_fsrra jp_action_fsrra\n'
 raw=raw.replace(anchor,anchor+context);body=owner_function(raw,'action_link_start_motion')
 # Original matching spelling assigns link=0, then casts it three times.
 # These are zero-valued integers, not live pointers to truncate on x64.
 assert b'link = 0;' in body and body.count(b'(Uint32)link')==2 and body.count(b'(Sint32)link')==1
 raw=raw.replace(body,body.replace(b'(Uint32)link',b'0').replace(b'(Sint32)link',b'0'))
 link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
 kept={n:sha(n) for n in link if n.endswith('.obj')}
 providers={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0}
 d.mkdir()
 for path in base.iterdir():
  if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(path,d/path.name)
  elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
 recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj');sdk=recipe[:recipe.index('-c')]
 for suffix in ('sdk/include','src'):
  found=[i for i,x in enumerate(sdk) if x.startswith('-I') and Path(x[2:]).as_posix().endswith('/'+suffix)];assert len(found)==1;sdk[found[0]]='-I'+str(u/suffix)
 commands=[]
 def run(cmd,name):
  actual=cmd;extra={}
  if len(subprocess.list2cmdline(cmd))>=30000:
   text='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n';assert shlex.split(text)==cmd[1:];rsp=d/(name+'.rsp');rsp.write_text(text);actual=[cmd[0],'@'+str(rsp)];extra=dict(response_file=str(rsp),response_file_sha256=sha(rsp))
  result=subprocess.run(actual,capture_output=True);(d/(name+'.stdout')).write_bytes(result.stdout);(d/(name+'.stderr')).write_bytes(result.stderr)
  commands.append(dict(command=cmd,exit_code=result.returncode,diagnostic_bytes=len(result.stderr),**extra));assert not result.returncode and not result.stderr,(name,result.stderr.decode(errors='replace')[-6000:]);return result.stdout
 source=d/'external_action_link.c';source.write_bytes(raw);tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],'ast'));names=[]
 for node in tree['inner']:
  if node.get('kind')=='FunctionDecl' and not node.get('loc',{}).get('includedFrom') and any(x.get('kind')=='CompoundStmt' for x in node.get('inner',[])):
   loc=node['loc'];assert raw[loc['offset']:loc['offset']+loc['tokLen']].decode()==node['name'];names.append(node['name'])
 assert len(names)==39 and 'draw_action_link' in names
 adapted,stats=rewrite(raw,tree,names);rz=d/'external_action_link_rz.c';rz.write_bytes(adapted);obj=d/'action_link_original.obj';run(sdk+['-c',str(rz),'-o',str(obj)],'compile');ir=d/'action_link.ll';run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm')
 assert stats['total_operators_rewritten']==88 and not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text())
 undefined={s['name'] for s in coff_symbols(obj) if s['section']==0};missing=sorted(undefined-providers-{'_fltused'});assert len(missing)==25
 rows=[];row_hashes={}
 for name in missing:
  found=re.findall(rb'^[^\r\n]*\b'+name.encode()+rb'\b[^\r\n]*;\r?\n',originals['sdk/ninja/njGlobal.c'],re.M);assert len(found)==1,(name,found);rows.append(found[0]);row_hashes[name]=hashlib.sha256(found[0]).hexdigest()
 state=d/'external_action_link_state.c';state.write_bytes(b'#include <ninja.h>\n'+b''.join(rows));state_obj=d/'action_link_state.obj';run(sdk+['-c',str(state),'-o',str(state_obj)],'state')
 exports=('draw_action_link','draw_motion_link','draw_shape_motion_link');link[link.index('-o'):link.index('-o')]=[str(obj),str(state_obj)]+['-Wl,/export:'+name for name in exports];link[-1]=str(d/'jp_native_frames.dll');run(link,'link')
 assert all(sha(n)==h for n,h in kept.items())
 unchanged={x.name:sha(x) for x in base.glob('*.dll') if x.name!='jp_native_frames.dll'};assert all(sha(d/n)==h for n,h in unchanged.items())
 updates={}
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
 detail=dict(upstream_commit=PIN,original_source_sha256={n:hashlib.sha256(x).hexdigest() for n,x in originals.items()},source_functions=names,source_function_count=len(names),matching_zero_pointer_casts_replaced_with_literal_zero=3,scalar_arithmetic_adaptation=stats,residual_native_FP_operations=0,undefined_object_symbols=sorted(undefined),existing_Frame_object_providers_used=sorted(undefined&providers),original_globals_added=missing,original_global_definition_sha256=row_hashes,generated_source_sha256={x.name:sha(x) for x in (source,rz,ir,state)},new_object_sha256={x.name:sha(x) for x in (obj,state_obj)},new_exports=list(exports),unchanged_DLL_sha256=unchanged,private_and_exported_Frame_definitions_audited=True)
 updates.update(status='complete_original_action_link_39_functions_and_25_original_globals_linked_runtime_unverified',action_link_baseline_sha256=sha(base/'build_verification.json'),action_link_recipe=detail,action_link_kept_frame_objects_sha256=kept,commands=commands+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],flush=True)
if __name__=='__main__':main()
