"""Link original model-tree draw and same-matrix vector scaling privately."""
import argparse,hashlib,json,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'));kept={n:sha(n) for n in link if n.endswith('.obj')}
    symbols={s['name'] for n in kept for s in coff_symbols(Path(n)) if s['section']>0};assert not {'njScaleV','draw_object'}&symbols and {'njScale','draw_model'}<=symbols
    original={}
    for n in ('src/draw/helpers.c','src/draw/helpers.h','src/system/util.h','sdk/ninja/njScaleV.c'):
        raw=(u/n).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n]);original[n]=sha(u/n)
    body=owner_function((u/'src/draw/helpers.c').read_bytes(),'draw_object');d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    source=d/'external_character_tree.c';source.write_bytes(b'#include "draw/draw.h"\n#include "draw/helpers.h"\n#include "draw/model.h"\n#include "system/util.h"\n'+body)
    compiler=b['commands'][0]['command'][0];row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_switch_owner.obj');sdk=row[:row.index('-c')]
    host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    commands=[host+['-c',str(p/'native/jp_ninja_scale_vector.c'),'-o',str(d/'scale_vector_native.obj')],sdk+['-c',str(source),'-o',str(d/'character_tree_original.obj')]]
    link[link.index('-o'):link.index('-o')]=[str(d/'scale_vector_native.obj'),str(d/'character_tree_original.obj'),'-Wl,/export:draw_object'];link[-1]=str(d/'jp_native_frames.dll');commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        executed=cmd;response={}
        if len(subprocess.list2cmdline(cmd))>=30000:
            content='\n'.join('"'+arg.replace('\\','\\\\').replace('"','\\"')+'"' for arg in cmd[1:])+'\n';assert shlex.split(content)==cmd[1:];path=d/f'character_tree_build_{i}.rsp';path.write_text(content);executed=[cmd[0],'@'+str(path)];response=dict(response_file=str(path),response_file_sha256=sha(path))
        run=subprocess.run(executed,capture_output=True);log=d/f'character_tree_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,executed_command=executed,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,**response));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()[:6500]
    for n,h in kept.items():assert sha(n)==h
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native/jp_ninja_scale_vector.h',p/'native/jp_ninja_scale_vector.c',Path(__file__).resolve()]
    updates.update(status='original_model_tree_draw_and_same_native_matrix_vector_scale_linked_unverified',character_tree_baseline_sha256=sha(base/'build_verification.json'),character_tree_original_source_sha256=original,character_tree_original_body_sha256=hashlib.sha256(body).hexdigest(),character_tree_generated_source_sha256={source.name:sha(source)},character_tree_kept_frame_objects_sha256=kept,character_tree_new_build_commands=len(results),commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{n.relative_to(r).as_posix():sha(n) for n in authored}),real_3D_character_tree_or_scale_vector_verified=False,automatic_render_view_acquisition=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(results),'clean commands',flush=True)
if __name__=='__main__':main()
