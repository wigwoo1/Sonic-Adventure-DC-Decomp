"""Link real original boot buffer setters and one extended scratch owner.
Original game/SDK objects and resulting game-containing DLLs remain private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'])
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']));objects={n:sha(n) for n in link if n.endswith('.obj')}
    old=next(n for n in objects if Path(n).name=='jp_native_render_state.obj');kept={n:h for n,h in objects.items() if n!=old};definitions=set()
    for n in kept:definitions.update(s['name'] for s in coff_symbols(Path(n)) if s['section']>0)
    functions={'njSetTextureMemorySize':'sdk/ninja/njSystem.c','njSetBorderColor':'sdk/ninja/njSystem.c','njInitTextureBuffer':'sdk/ninja/njTexture.c','njInitShape':'sdk/ninja/njMotSrc.c'}
    globals=['_nj_texture_memory_size','_nj_texture_buffer_st','_nj_texture_buffer_max_size','_nj_shape_buf_','texture_buffer','shape_buffer']
    assert not (set(functions)|set(globals))&definitions
    source_hashes={n:sha(u/n) for n in set(functions.values())|{'sdk/ninja/njGlobal.c','src/main.c'}}
    assert source_hashes['src/main.c']==b['main_display_source_sha256']
    raw=(u/'sdk/ninja/njGlobal.c').read_bytes();declarations=[]
    for name in globals[:4]:
        rows=re.findall(rb'^.*\b'+name.encode()+rb'\b[^;\n]*;\n',raw,re.M);assert len(rows)==1,(name,rows);declarations+=rows
    raw=(u/'src/main.c').read_bytes()
    for name in globals[4:]:
        rows=re.findall(rb'^(?:Sint8|Float) '+name.encode()+rb'\[[^\n]*;\n',raw,re.M);assert len(rows)==1,(name,rows);declarations+=rows
    bodies={n:function((u/f).read_bytes(),n) for n,f in functions.items()}
    d.mkdir();(d/'frames').mkdir();sdk=d/'external_boot_buffers.c'
    sdk.write_bytes(b'#include <ninja.h>\n#include <kamui.h>\n#include <sg_xpt.h>\n#define SECTOR_SHIFT 11\n#define TEXTURE_BUFFER_SIZE 0x40000\n'+b''.join(declarations)+b'\n'.join(bodies.values()))
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')];owner=p/'native/jp_native_boot_scratch.c'
    commands=[common+['-c',str(owner),'-o',str(d/'boot_scratch_owner.obj')],common+['-c',str(sdk),'-o',str(d/'boot_buffer_sdk.obj')]]
    link[link.index(old)]=str(d/'boot_scratch_owner.obj');at=link.index('-o');existing={x.split('/export:',1)[1].split(',')[0] for x in link if '/export:' in x}
    link[at:at]=[str(d/'boot_buffer_sdk.obj')]+['-Wl,/export:'+n for n in [*functions,'njInit3D'] if n not in existing]+[x for n in [*globals,'_nj_vertex_buf_','_nj_vertex_num_'] if n not in existing for x in ['-Xlinker','/export:'+n+',DATA']];link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    newlibs={'frames_DLL':d/'frames/jp_native_frames.lib'}
    for label,name,imports in [('GPU_DLL','jp_native_window_input.dll',['frames_DLL']),('present_DLL','jp_native_present.dll',['frames_DLL','GPU_DLL']),('authored_exit_DLL','jp_native_exit.dll',['frames_DLL','GPU_DLL','present_DLL']),('DLL','jp_native_logo_start.dll',['frames_DLL','present_DLL','authored_exit_DLL'])]:
        cmd=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b[label]))
        for n in imports:
            previous=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(previous)==1;cmd[cmd.index(previous)]=str(newlibs[n])
        cmd[-1]=str(d/name);newlibs[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(n)==h,n
    sources=dict(b['authored_source_sha256'])
    for path in [owner,p/'native/jp_native_boot_scratch.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    generated=dict(b['generated_sources_sha256']);generated[sdk.name]=sha(sdk)
    for n,h in b['generated_sources_sha256'].items():target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='original_boot_buffer_setters_and_shared_borrowed_scratch_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,generated_sources_sha256=generated,kept_frame_objects_sha256=kept,replaced_scratch_owner=dict(path=old,sha256=objects[old]),boot_scratch_owner_sha256=sha(d/'boot_scratch_owner.obj'),boot_buffer_sdk_sha256=sha(d/'boot_buffer_sdk.obj'),boot_buffer_external_sources_sha256=source_hashes,boot_buffer_original_functions_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},all_other_production_objects_preserved=True,all_prior_production_objects_preserved=False,original_buffer_setter_bodies_unchanged=True,main_texture_buffer_bytes=0x40000,main_original_vertex_capacity=4096,main_shape_floats=6144,one_shared_scratch_owner=True,previous_main_reset_build_sha256=sha(base/'build_verification.json'),frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),GPU_DLL=str(d/'jp_native_window_input.dll'),GPU_DLL_sha256=sha(d/'jp_native_window_input.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),artifact_publishable=False,game_containing_DLL_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);fd=d/'frame_view';fd.mkdir()
    for n in generated:target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
