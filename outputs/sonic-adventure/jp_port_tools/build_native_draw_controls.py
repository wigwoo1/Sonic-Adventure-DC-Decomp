"""Select genuine player draw-control helpers into the existing Frame owner."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
NAMES=tuple('draw_'+group+'_'+op for group in ('attr','control') for op in ('save','restore','set','clear'))
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=json.loads((base/'build_verification.json').read_bytes());assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 raw=(u/'src/system/util.c').read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':src/system/util.c'])
 link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')};symbols={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0};assert not set(NAMES)&symbols
 declarations=[]
 for name in ('saved_attr_and','saved_attr_or','saved_control_3d'):
  assert name not in symbols;matches=re.findall(rb'^static [^;\r\n]*\b'+name.encode()+rb';\r?\n',raw,re.M);assert len(matches)==1,(name,matches);declarations.append(matches[0])
 for name in ('_nj_constant_attr_and_','_nj_constant_attr_or_','_nj_control_3d_flag_'):assert name in symbols
 d.mkdir()
 for path in base.glob('*'):
  if path.is_file() and path.suffix in ('.dll','.lib'):shutil.copyfile(path,d/path.name)
 bodies={n:owner_function(raw,n) for n in NAMES};source=d/'external_draw_controls.c';source.write_bytes(b'#include <ninja.h>\n'+b''.join(declarations)+b'\n'.join(bodies.values()))
 recipe=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_tree_original.obj');compiler=recipe[:recipe.index('-c')];obj=d/'draw_controls_original.obj';link[link.index('-o'):link.index('-o')]=[str(obj)]+['-Wl,/export:'+n for n in NAMES];link[-1]=str(d/'jp_native_frames.dll');results=[]
 for i,cmd in enumerate((compiler+['-c',str(source),'-o',str(obj)],link)):
  executed=cmd;extra={}
  if len(subprocess.list2cmdline(cmd))>=30000:
   content='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n';assert shlex.split(content)==cmd[1:];rsp=d/f'build_{i}.rsp';rsp.write_text(content);executed=[cmd[0],'@'+str(rsp)];extra=dict(response_file=str(rsp),response_file_sha256=sha(rsp))
  run=subprocess.run(executed,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,**extra));assert not run.returncode and not log.stat().st_size,log.read_text()[-6000:]
 for n,h in kept.items():assert sha(n)==h
 updates={}
 for k,v in b.items():
  if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
 updates.update(status='original_eight_draw_control_helpers_linked_runtime_unverified',draw_controls_baseline_sha256=sha(base/'build_verification.json'),draw_controls_original_source_sha256=sha(u/'src/system/util.c'),draw_controls_original_function_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},draw_controls_generated_source_sha256=sha(source),draw_controls_kept_frame_objects_sha256=kept,commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{Path(__file__).resolve().relative_to(r).as_posix():sha(__file__)}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print('Eight original helpers linked; two clean commands; original Frame objects preserved.',flush=True)
if __name__=='__main__':main()
