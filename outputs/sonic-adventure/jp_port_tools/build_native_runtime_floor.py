"""Derive genuine floorf/whole/_in_floors, preserving SH4 value semantics."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for name in ('workspace','upstream','baseline','output'):ap.add_argument('--'+name,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(base/'build_verification.json')
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']
 original={}
 for n in ('sdk/runtime/_i_flors.c','sdk/runtime/floorf.c','sdk/runtime/consts.c','sdk/runtime/_fmtout.c'):
  raw=(u/n).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n]);original[n]=raw
 raw=original['sdk/runtime/_i_flors.c'];assert raw.count(b'(float)(int)x')==1;raw=raw.replace(b'(float)(int)x',b'jp_runtime_trunc_scalar(x)');raw=b'#define _errno jp_sdk_errno\n#include "jp_native_runtime_floor.h"\n#include "jp_native_sprite_numeric.h"\n'+raw+b'\n'+original['sdk/runtime/floorf.c']
 link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')};providers={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0};recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj');sdk=recipe[:recipe.index('-c')]
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
 source=d/'external_runtime_floor.c';source.write_bytes(raw);tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],'ast'));names=['whole','_in_floors','floorf'];adapted,stats=rewrite(raw,tree,names);rz=d/'external_runtime_floor_rz.c';rz.write_bytes(adapted);obj=d/'runtime_floor_original.obj';run(sdk+['-c',str(rz),'-o',str(obj)],'compile');ir=d/'runtime_floor.ll';run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],'llvm');assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp|fptosi|sitofp)\b',ir.read_text())
 bridge=d/'runtime_floor_bridge.obj';run(sdk+['-c',str(p/'native/jp_native_runtime_floor.c'),'-o',str(bridge)],'bridge');available=providers|{s['name'] for s in coff_symbols(bridge) if s['section']>0}|{s['name'] for s in coff_symbols(obj) if s['section']>0};undefined={s['name'] for path in (obj,bridge) for s in coff_symbols(path) if s['section']==0};missing=sorted(undefined-available-{'_fltused'});assert not set(missing)-{'jp_sdk_errno','_NIFONES'},missing
 rows=[];definitions={}
 for name,original_name,path in (('jp_sdk_errno','_errno','sdk/runtime/_fmtout.c'),('_NIFONES','_NIFONES','sdk/runtime/consts.c')):
  if name in missing:
   found=re.findall(rb'^[^\r\n]*\b'+original_name.encode()+rb'\b[^\r\n]*;\r?\n',original[path],re.M);assert len(found)==1;rows+=found;definitions[name]=hashlib.sha256(found[0]).hexdigest()
 state=d/'external_runtime_floor_state.c';state.write_bytes(b'#define _errno jp_sdk_errno\n_Static_assert(sizeof(unsigned long)==4,"SDK word width");\n'+b''.join(rows));state_obj=d/'runtime_floor_state.obj';run(sdk+['-c',str(state),'-o',str(state_obj)],'state')
 exports=['floorf','_in_floors','jp_sdk_errno','jp_runtime_trunc_scalar','jp_runtime_floor_word'];link[link.index('-o'):link.index('-o')]=[str(obj),str(bridge),str(state_obj)]+['-Wl,/export:'+name for name in exports];link[-1]=str(d/'jp_native_frames.dll');run(link,'link');assert all(sha(n)==h for n,h in kept.items())
 unchanged={x.name:sha(x) for x in base.glob('*.dll') if x.name!='jp_native_frames.dll'};assert all(sha(d/n)==h for n,h in unchanged.items());updates={}
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
 detail=dict(upstream_commit=PIN,original_source_sha256={n:hashlib.sha256(v).hexdigest() for n,v in original.items()},source_functions=names,SH4_integer_conversion_replaced_with_explicit_value_bridge=1,scalar_arithmetic_adaptation=stats,residual_host_FP_arithmetic_compare_and_integer_conversions=0,original_globals_added=missing,original_global_definition_sha256=definitions,generated_source_sha256={x.name:sha(x) for x in (source,rz,ir,state)},new_object_sha256={x.name:sha(x) for x in (obj,bridge,state_obj)},new_exports=exports,unchanged_DLL_sha256=unchanged,FPSCR_flags_and_traps_emulated=False,fixed_numerical_policy='RM=1,DN=1;return values and shared SDK errno')
 detail.update(SDK_errno_native_symbol='jp_sdk_errno',existing_SDK_print_errno_owner_reused='jp_sdk_errno' in providers,host_CRT_errno_function_not_redefined=True)
 authored=[Path(__file__),p/'native/jp_native_runtime_floor.c',p/'native/jp_native_runtime_floor.h'];updates.update(status='genuine_runtime_floor_original_bodies_and_explicit_conversion_bridge_linked_runtime_unverified',runtime_floor_baseline_sha256=sha(base/'build_verification.json'),runtime_floor_recipe=detail,runtime_floor_kept_frame_objects_sha256=kept,commands=commands+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{x.resolve().relative_to(r).as_posix():sha(x) for x in authored}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],missing,flush=True)
if __name__=='__main__':main()
