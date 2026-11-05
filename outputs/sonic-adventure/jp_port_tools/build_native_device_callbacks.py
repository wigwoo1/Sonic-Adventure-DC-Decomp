"""Connect Ninja EOV/error state to real native presentation events.

Original SDK bodies and combined DLLs are generated privately. Reuse existing
single owners and the presenter's established shared-clock import recipe.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','preserved','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=read(a.baseline/'build_verification.json');history=read(a.preserved/'preserved_source.json')
    for n,h in b['authored_source_sha256'].items():
        if n in history:
            assert history[n]['sha256']==h and sha(history[n]['preserved_path'])==h,n
        else:assert sha(r/n)==h,n
    for kind in ('frames','present','GPU'):assert sha(b[kind+'_DLL'])==b[kind+'_DLL_sha256']
    fl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    pl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['present_DLL']))
    original_objects={n:sha(n) for cmd in (fl,pl) for n in cmd if n.endswith('.obj')}
    names=['njSetEOVflag','njSetERRflag','_nj_eov_flag_','_nj_fatal_err_callback_']
    for obj in original_objects:
        assert not any(s['section']>0 and s['name'] in names for s in coff_symbols(Path(obj))),obj
    original_files=['sdk/ninja/njSystem.c','sdk/ninja/njGlobal.c','sdk/ninja/kmcalbac_kamui_lib.c','sdk/include/kamui.h']
    originals={n:sha(u/n) for n in original_files};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():
        assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+n])).hexdigest()==h,n
    d.mkdir();raw=(u/'sdk/ninja/njSystem.c').read_bytes();state=(u/'sdk/ninja/njGlobal.c').read_bytes()
    bodies={n:function(raw,n) for n in ('njSetEOVflag','njSetERRflag')}
    src=b'#include <ninja.h>\n#include <stdint.h>\n'
    for n in names[2:]:
        found=re.findall(rb'^volatile Int '+n.encode()+rb';',state,re.M);assert len(found)==1;src+=found[0]+b'\n'
    src+=bodies['njSetEOVflag']
    old=b'(Int)arg';new=b'(Int)(Uint32)(uintptr_t)arg';assert bodies['njSetERRflag'].count(old)==1
    src+=bodies['njSetERRflag'].replace(old,new)
    generated=d/'external_ninja_completion_callbacks.c';generated.write_bytes(src)
    frame_common=list(next(x['command'] for x in b['commands'] if '-c' in x['command']))
    frame_common=frame_common[:frame_common.index('-c')];commands=[]
    commands.append(frame_common+['-c',str(generated),'-o',str(d/'ninja_callbacks.obj')])
    fl[fl.index('-o'):fl.index('-o')]=[str(d/'ninja_callbacks.obj'),'-Wl,/export:njSetEOVflag','-Wl,/export:njSetERRflag',
        '-Xlinker','/export:_nj_eov_flag_,DATA','-Xlinker','/export:_nj_fatal_err_callback_,DATA']
    fl[-1]=str(d/'jp_native_frames.dll');commands.append(fl)
    recipes=read(r/'work/sonic-adventure/jp-native-task-presentation-runtime-v6/build_verification.json')
    pc=list(next(x['command'] for x in recipes['commands'] if '-c' in x['command'] and x['command'][-1].endswith('kamui_present.obj')))
    own=p/'native/jp_native_present.c';pc[pc.index('-c')+1]=str(own);pc[-1]=str(d/'kamui_present.obj');commands.append(pc)
    oldobj=next(n for n in pl if n.endswith('kamui_present.obj'));pl[pl.index(oldobj)]=pc[-1]
    for i,n in enumerate(pl):
        if n.endswith('jp_native_frames.lib'):pl[i]=str(d/'jp_native_frames.lib')
    pl[pl.index('-o'):pl.index('-o')]=['-Wl,/export:kmSetEndOfVertexCallback','-Wl,/export:kmfSetFatalErrorCallback']
    pl[-1]=str(d/'jp_native_present.dll');commands.append(pl);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in originals.items():assert sha(u/n)==h,n
    for n,h in original_objects.items():assert sha(n)==h,n
    authored=dict(b['authored_source_sha256'])
    for n in history:authored[n]=sha(r/n)
    authored[Path(__file__).resolve().relative_to(r).as_posix()]=sha(__file__)
    result=dict(status='real_native_EOV_fatal_callbacks_and_original_Ninja_flags_linked_unverified',commands=results,
        frames_DLL=str(d/'jp_native_frames.dll'),frames_DLL_sha256=sha(d/'jp_native_frames.dll'),
        present_DLL=str(d/'jp_native_present.dll'),present_DLL_sha256=sha(d/'jp_native_present.dll'),
        GPU_DLL=b['GPU_DLL'],GPU_DLL_sha256=b['GPU_DLL_sha256'],authored_source_sha256=authored,
        superseded_authored_source=history,generated_sources_sha256={generated.name:sha(generated)},
        original_source_sha256=originals,original_function_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},
        source_adaptations={'njSetEOVflag':'original body unchanged','njSetERRflag':'encoded 32-bit error argument passes through uintptr_t, then original Uint32/Int widths'},
        kept_frame_objects_sha256={n:sha(n) for n in fl if n.endswith('.obj')},
        kept_present_objects_sha256={n:sha(n) for n in pl if n.endswith('.obj')},
        original_objects_preserved_sha256=original_objects,baseline_build_sha256=sha(a.baseline/'build_verification.json'),
        one_real_clock_and_one_real_device_owner=True,shared_DLL_circular_dependency_added=False,
        njInitSystem_or_device_creation_implemented=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)

if __name__=='__main__':main()
