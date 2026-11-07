"""Provide shared morph math; preserve sound ABI with an actual PE forwarder."""
import argparse,hashlib,json,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function,PIN
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 original={}
 for name in ('sdk/ninja/njSin.c','sdk/ninja/njUnitVector.c'):
  raw=(u/name).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+name]);original[name]=sha(u/name)
 frame=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'));sound=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_game_sound.dll'));kept={n:sha(n) for n in frame+sound if n.endswith('.obj')}
 frame_symbols={s['name'] for n in frame if n.endswith('.obj') for s in coff_symbols(Path(n)) if s['section']>0};assert not {'njSin','njUnitVector'}&frame_symbols
 old=p/'native/jp_native_sound_spatial.c';body=owner_function(old.read_bytes(),'njUnitVector');assert body==owner_function((p/'native/jp_native_morph_math.c').read_bytes(),'njUnitVector')
 d.mkdir()
 for path in base.iterdir():
  if path.is_file() and path.suffix in ('.dll','.lib','.c','.h','.obj'):shutil.copyfile(path,d/path.name)
  elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
 raw=old.read_bytes();assert raw.count(body)==1;raw=raw.replace(body,b'');oldinclude=b'"../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"';assert raw.count(oldinclude)==1;raw=raw.replace(oldinclude,('"'+(p/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h').as_posix()+'"').encode());source=d/'sound_spatial_shared_math.c';source.write_bytes(raw)
 header=(p/'native/jp_native_sound_spatial.h').read_bytes();line=b'JP_SPATIAL_API float njUnitVector(void *vector);';assert header.count(line)==1;(d/'jp_native_sound_spatial.h').write_bytes(header.replace(line,b'__declspec(dllimport) float njUnitVector(void *vector);'))
 compiler=b['commands'][0]['command'][0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
 obj=d/'morph_math_native.obj';frame[frame.index('-o'):frame.index('-o')]=[str(obj),'-Wl,/export:njSin','-Wl,/export:njUnitVector'];frame[-1]=str(d/'jp_native_frames.dll')
 oldobj=next(n for n in sound if n.endswith('sound_spatial.obj'));replacement=d/'sound_spatial_forwarded.obj';sound[sound.index(oldobj)]=str(replacement)
 oldlib=next(n for n in sound if n.endswith('jp_native_frames.lib'));sound[sound.index(oldlib)]=str(d/'jp_native_frames.lib');sound[sound.index('-o'):sound.index('-o')]=['-Wl,/export:njUnitVector=jp_native_frames.njUnitVector'];sound[-1]=str(d/'jp_native_game_sound.dll')
 commands=[host+['-c',str(p/'native/jp_native_morph_math.c'),'-o',str(obj)],frame,host+['-c',str(source),'-o',str(replacement)],sound];results=[]
 for i,cmd in enumerate(commands):
  executed=cmd;extra={}
  if len(subprocess.list2cmdline(cmd))>=30000:
   text='\n'.join('"'+v.replace('\\','\\\\').replace('"','\\"')+'"' for v in cmd[1:])+'\n';assert shlex.split(text)==cmd[1:];rsp=d/f'build_{i}.rsp';rsp.write_text(text);executed=[cmd[0],'@'+str(rsp)];extra=dict(response_file=str(rsp),response_file_sha256=sha(rsp))
  run=subprocess.run(executed,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,**extra));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()[-6500:]
 for n,h in kept.items():assert sha(n)==h
 updates={}
 for k,v in b.items():
  if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
 authored=[Path(__file__).resolve(),p/'native/jp_native_morph_math.c',p/'native/jp_native_morph_math.h']
 updates.update(status='shared_native_sine_normalizer_and_sound_export_forwarder_linked_unverified',morph_math_baseline_sha256=sha(base/'build_verification.json'),morph_math_original_source_sha256=original,morph_math_kept_objects_sha256=kept,morph_math_generated_source_sha256={source.name:sha(source),'jp_native_sound_spatial.h':sha(d/'jp_native_sound_spatial.h')},morph_math_original_sound_normalizer_body_sha256=hashlib.sha256(body).hexdigest(),commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{n.relative_to(r).as_posix():sha(n) for n in authored}),playable=False,overall_completion_percent=10)
 (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print('Shared sine/normalizer and sound forwarder linked; four clean commands.',flush=True)
if __name__=='__main__':main()
