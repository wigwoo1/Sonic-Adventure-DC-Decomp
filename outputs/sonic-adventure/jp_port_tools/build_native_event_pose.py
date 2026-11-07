"""Link genuine pose/weld/event functions into the existing Frame owner.

Compilation/linking are not a runtime qualification of the player or event path.
"""
import argparse,hashlib,json,re,shlex,shutil,struct,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def comdat_any(obj,section):
 raw=Path(obj).read_bytes();machine,count,stamp,start,symbols,opt,flags=struct.unpack_from('<HHIIIHH',raw);assert machine==0x8664 and opt==0 and 0<section<=count
 assert struct.unpack_from('<I',raw,20+40*(section-1)+36)[0]&0x1000;i=0
 while i<symbols:
  off=start+18*i;name,value,sec,kind,storage,aux=struct.unpack_from('<8sIhHBB',raw,off)
  if storage==3 and sec==section and aux:return raw[off+18+14]==2
  i+=1+aux
 return False
def derive(original):
 pose=original['src/draw/pose.c'];prefix=pose[:pose.index(b'/* a node\'s three axes')];state=pose[pose.index(b'static NJS_MATRIX pose_matrices'):pose.index(b'/* pose object at')];names=['pose_node','pose_action','pose_get_count','pose_get_matrix'];body=b'\n'.join(owner_function(pose,n) for n in names);assert body.count(b'(NJS_MKEY_A *)y')==2;body=body.replace(b'(NJS_MKEY_A *)y',b'(NJS_MKEY_A *)(uintptr_t)(Uint32)y');units={'pose':(prefix+state+body,names)}
 units['weld']=(original['src/player/weld.c'],['player_weld'])
 units['player_get']=(b'#include <ninja.h>\n#include "player/player.h"\nextern struct player *player_records[PLAYER_COUNT];\n'+owner_function(original['src/player/player.c'],'player_get'),['player_get'])
 event=original['src/event/event.c'];units['event_draw']=(event[:event.index(b'/* an angle part')]+owner_function(event,'event_motions_draw'),['event_motions_draw'])
 found=re.findall(rb'^struct task_work \*player_work\[PLAYER_COUNT\];\r?\n',original['src/player/player.c'],re.M);assert len(found)==1
 units['bindings']=(b'#include <ninja.h>\n#include "player/player.h"\n#include "system/util.h"\n'+found[0]+owner_function(original['src/system/util.c'],'angle_difference'),['angle_difference'])
 out={}
 for tag,(raw,names) in units.items():
  includes=list(re.finditer(rb'^#include[^\n]*\n',raw,re.M));assert includes;at=includes[-1].end();raw=b'#include <stdint.h>\n'+raw[:at]+b'#define GCC 1\n#include "jp_native_sprite_numeric.h"\n'+raw[at:];out[tag]=(raw,names)
 return out
def main():
 ap=argparse.ArgumentParser()
 for name in ('workspace','upstream','baseline','output'):ap.add_argument('--'+name,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');b=read(base/'build_verification.json');assert b['runtime_floor_recipe']['existing_SDK_print_errno_owner_reused']
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']
 original={}
 for name in ('src/draw/pose.c','src/player/weld.c','src/player/player.c','src/event/event.c','src/system/util.c'):
  raw=(u/name).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+name]);original[name]=raw
 link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')};providers={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0};assert 'player_records' in providers and 'player_work' not in providers
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
 records={};all_defined=set(providers);all_undefined=set();objects=[]
 for tag,(raw,names) in derive(original).items():
  source=d/('external_'+tag+'.c');source.write_bytes(raw);tree=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],tag+'_ast'));adapted,stats=rewrite(raw,tree,names);rz=d/('external_'+tag+'_rz.c');rz.write_bytes(adapted);obj=d/(tag+'.obj');run(sdk+['-c',str(rz),'-o',str(obj)],tag+'_compile');ir=d/(tag+'.ll');run(sdk+['-S','-emit-llvm',str(rz),'-o',str(ir)],tag+'_llvm');assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',ir.read_text());symbols=coff_symbols(obj);undefined={s['name'] for s in symbols if s['section']==0};defined={s['name'] for s in symbols if s['section']>0};duplicate=defined&all_defined
  for sym in symbols:
   if sym['name'] in duplicate:assert re.fullmatch('__real@[0-9a-f]+',sym['name']) and comdat_any(obj,sym['section']),(tag,sym)
  all_defined|=defined;all_undefined|=undefined;objects.append(obj);records[tag]=dict(functions=names,scalar_arithmetic_adaptation=stats,generated_source_sha256={x.name:sha(x) for x in (source,rz,ir)},object_sha256=sha(obj),undefined=sorted(undefined),defined=sorted(defined),only_shared_constant_COMDAT_Any_collisions=sorted(duplicate))
 missing=sorted(all_undefined-all_defined-{'_fltused'});assert not missing,missing
 existing_exports={m.group(1) for arg in link if (m:=re.search(r'/export:([^,]+)',arg))}
 exports=[n for n in ('pose_action','pose_get_count','pose_get_matrix','player_weld','player_get','event_motions_draw','angle_difference','player_work','player_records') if n not in existing_exports];link[link.index('-o'):link.index('-o')]=list(map(str,objects))+['-Wl,/export:'+n for n in exports];link[-1]=str(d/'jp_native_frames.dll');run(link,'link');assert all(sha(n)==h for n,h in kept.items())
 unchanged={x.name:sha(x) for x in base.glob('*.dll') if x.name!='jp_native_frames.dll'};assert all(sha(d/n)==h for n,h in unchanged.items());updates={}
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
 detail=dict(upstream_commit=PIN,original_source_sha256={n:hashlib.sha256(x).hexdigest() for n,x in original.items()},units=records,original_source_function_count=8,pose_matching_unused_integer_address_casts_explicitly_zero_extended=2,original_player_work_BSS_added=True,existing_player_records_and_SDK_floor_owner_reused=True,remaining_joint_missing_providers=[],new_exports=exports,unchanged_DLL_sha256=unchanged,original_player_lifetime_or_gameplay_verified=False)
 updates.update(status='genuine_pose_weld_player_get_event_draw_and_angle_helper_linked_runtime_unverified',event_pose_baseline_sha256=sha(base/'build_verification.json'),event_pose_recipe=detail,event_pose_kept_frame_objects_sha256=kept,commands=commands+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],'zero missing providers',flush=True)
if __name__=='__main__':main()
