"""Bind original toe-control body to actual leased canonical models/player ABI."""
import argparse,hashlib,json,re,shlex,shutil,struct,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 raw=(u/'src/player/sonic.c').read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/player/sonic.c']);body=owner_function(raw,'sonic_blend_toes');enum=re.search(rb'enum toe_model \{[^}]+\};',raw).group();action=int(re.search(rb'ACTION_FULL_SPEED = (\d+)',raw).group(1));assert action==13
 names=[];table_hashes={}
 for side in ('plus','minus'):
  table=re.search(rb'static NJS_MODEL \*sonic_toe_'+side.encode()+rb'_z_models\[TOE_MODELS\] = \{.*?\};',raw,re.S).group();table_hashes[side]=hashlib.sha256(table).hexdigest();rows=re.findall(rb'&([A-Za-z_]\w*)',table);assert len(rows)==4;names.extend(n.decode() for n in rows);names.append('sonic_toe_'+side+'_z')
 names+=['sonic_toe_plus_z_face_normals','sonic_toe_minus_z_face_normals'];d.mkdir()
 for path in base.iterdir():
  if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(path,d/path.name)
  elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
 prefix=b'#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n#undef TRUE\n#undef FALSE\n#include <string.h>\n#include <ninja.h>\n#include "player/player.h"\n#include "system/task.h"\n#include "system/util.h"\n#include "jp_native_sprite_numeric.h"\n'+enum+b'\n#define ACTION_FULL_SPEED 13\nstatic NJS_MODEL *sonic_toe_plus_z_models[TOE_MODELS],*sonic_toe_minus_z_models[TOE_MODELS];\nstatic NJS_OBJECT *toe_plus_node,*toe_minus_node;\n#define sonic_toe_plus_z (*toe_plus_node)\n#define sonic_toe_minus_z (*toe_minus_node)\n'
 prefix+=('static const char *const toe_view_names[]={'+','.join('"'+n+'"' for n in names)+'};\n').encode();source=d/'external_sonic_toes.c';source.write_bytes(prefix+body.replace(b'*work,',b'*work __attribute__((unused)),').replace(b'*motion,',b'*motion __attribute__((unused)),')+b'\n#include "jp_native_sonic_toes.inc"\n')
 recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj');sdk=recipe[:recipe.index('-c')];commands=[]
 def run(cmd,name):
  x=subprocess.run(cmd,capture_output=True);(d/(name+'.stdout')).write_bytes(x.stdout);(d/(name+'.stderr')).write_bytes(x.stderr);commands.append(dict(command=cmd,exit_code=x.returncode,diagnostic_bytes=len(x.stderr)));assert not x.returncode and not x.stderr,(name,x.stderr.decode(errors='replace')[-6000:]);return x.stdout
 ast=json.loads(run(sdk+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],'ast'));adapted,stats=rewrite(source.read_bytes(),ast,['sonic_blend_toes']);rz=d/'external_sonic_toes_rz.c';rz.write_bytes(adapted);obj=d/'sonic_toes_original.obj';run(sdk+['-c',str(rz),'-o',str(obj)],'compile')
 layout=d/'player_layout32.c';layout.write_text('#include <ninja.h>\n#include "player/player.h"\nconst unsigned jp_sonic_toes_lp32_layout[]={sizeof(struct player),__builtin_offsetof(struct player,anim.action),__builtin_offsetof(struct player,anim.frame),__builtin_offsetof(struct player,own.sonic.toe_blend_plus_z),__builtin_offsetof(struct player,own.sonic.toe_blend_minus_z),sizeof(NJS_MODEL),sizeof(NJS_OBJECT)};\n');sdk32=[x.replace('--target=x86_64-pc-windows-msvc','--target=i686-pc-windows-msvc') for x in sdk];layout_obj=d/'player_layout32.obj';run(sdk32+['-c',str(layout),'-o',str(layout_obj)],'layout32');data=layout_obj.read_bytes();machine,sections=struct.unpack_from('<HH',data);assert machine==0x14c;found=[]
 for i in range(sections):
  off=20+i*40;name=data[off:off+8].rstrip(b'\0');size,start=struct.unpack_from('<II',data,off+16)
  if name==b'.rdata':found.append(data[start:start+size])
 assert len(found)==1 and len(found[0])==28;lp32=list(struct.unpack('<7I',found[0]));assert lp32[-2:]==[40,52]
 link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')};link[link.index('-o'):link.index('-o')]=[str(obj)];link[-1]=str(d/'jp_native_frames.dll');content='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in link[1:])+'\n';assert shlex.split(content)==link[1:];rsp=d/'link.rsp';rsp.write_text(content);run([link[0],'@'+str(rsp)],'link');commands[-1].update(command=link,response_file=str(rsp),response_file_sha256=sha(rsp))
 for n,h in kept.items():assert sha(n)==h
 updates={}
 for k,v in b.items():
  if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
 authored=[Path(__file__).resolve(),p/'native/jp_native_sonic_toes.h',p/'native/jp_native_sonic_toes.inc'];updates.update(status='original_Sonic_toe_control_and_owned_canonical_views_linked_unverified',sonic_toes_baseline_sha256=sha(base/'build_verification.json'),sonic_toes_original_source_sha256=sha(u/'src/player/sonic.c'),sonic_toes_original_body_sha256=hashlib.sha256(body).hexdigest(),sonic_toes_original_table_sha256=table_hashes,sonic_toes_view_names=names,sonic_toes_original_LP32_player_layout=lp32,sonic_toes_arithmetic_adaptation=stats,sonic_toes_kept_frame_objects_sha256=kept,sonic_toes_generated_source_sha256={x.name:sha(x) for x in (source,rz,layout)},commands=commands+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print('Original toe control and twelve-view owner linked; LP32 player layout',lp32,flush=True)
if __name__=='__main__':main()
