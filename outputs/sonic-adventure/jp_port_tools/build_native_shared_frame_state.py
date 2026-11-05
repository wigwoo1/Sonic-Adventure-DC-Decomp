"""Unify real pacing counters across renderer and frame-wait components.

Preserve existing sources/objects. Generate private copies removing diagnostic
counter definitions; original game declarations provide the single storage.
This does not replace the still-diagnostic presentation recording/task hooks.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','present','compiler','output']:
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in ['workspace','upstream','baseline','present','compiler','output']:setattr(a,n,getattr(a,n).resolve())
    r=a.workspace;u=a.upstream;p=r/'outputs/sonic-adventure';d=a.output
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((a.baseline/'build_verification.json').read_bytes())
    pb=json.loads((a.present/'build_verification.json').read_bytes())
    assert b['exit_code']==b['diagnostic_bytes']==pb['exit_code']==pb['diagnostic_bytes']==0
    assert sha(a.compiler)==b['compiler_sha256']==pb['compiler_sha256']
    for old in [b,pb]:
        for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    raw=(u/'src/system/frame.c').read_bytes();names=['frame_interval','frame_countdown','frame_step_count','frame_step']
    definitions=[]
    for n in names:
        matches=re.findall(rb'^int '+n.encode()+rb';$',raw,re.M);assert len(matches)==1,n;definitions.append(matches[0])
    d.mkdir();(d/'frames').mkdir();overlay=d/'diagnostics';overlay.mkdir()
    frame_header=(u/'src/system/frame.h').read_bytes()
    for n in names:
        old=('extern int '+n+';').encode();assert frame_header.count(old)==1
        frame_header=frame_header.replace(old,b'JP_FRAME_STATE_API '+old)
    header=d/'include/system/frame.h';header.parent.mkdir(parents=True)
    header.write_bytes(b'#include "jp_native_frame_state.h"\n'+frame_header)
    game=d/'external_original_frame_state.c'
    game.write_bytes(b'#define JP_FRAME_STATE_OWNER\n#include "jp_native_frame_state.h"\n'+b'\n'.join(definitions)+b'\n')
    originals={};generated={};commands=[]
    for n in ['frame_diagnostic_probe.c','primitive_diagnostic_probe.c','sprite_diagnostic_probe.c']:
        path=p/'jp_port_tools'/n;originals[str(path.relative_to(r))]=sha(path);copy=path.read_bytes()
        if n=='sprite_diagnostic_probe.c':
            old=b'int frame_step,frame_later_draws;';assert copy.count(old)==1
            copy=copy.replace(old,b'#include "jp_native_frame_state.h"\nint frame_later_draws;')
        target=overlay/n;target.write_bytes(copy);generated[str(target.relative_to(d))]=sha(target)
    framebase=json.loads((r/'work/sonic-adventure/jp-native-frame-runtime-v3/build_verification.json').read_bytes())
    compile_probe=list(next(x['command'] for x in framebase['commands'] if x['command'][-1].endswith('frame_probe.obj')))
    compile_probe.insert(compile_probe.index('-c'),'-DJP_FRAME_STATE_OWNER=1')
    compile_probe[compile_probe.index('-c')+1]=str(overlay/'frame_diagnostic_probe.c');compile_probe[-1]=str(d/'frame_probe.obj');commands.append(compile_probe)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    commands.insert(0,common+['-c',str(game),'-o',str(d/'frame_state.obj')])
    frameslink=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    oldprobe=[x for x in frameslink if x.endswith('frame_probe.obj')];assert len(oldprobe)==1
    frameslink[frameslink.index(oldprobe[0])]=str(d/'frame_probe.obj')
    at=frameslink.index('-o');frameslink[at:at]=[str(d/'frame_state.obj')]
    # Existing exported counters would create a misleading second data owner.
    for n in names:assert not any('/export:'+n+',' in x for x in frameslink)
    frameslink[-1]=str(d/'frames/jp_native_frames.dll');commands.append(frameslink)
    for name,obj in [('frame','frame_pacing.obj'),('diagnostic','diagnostic.obj')]:
        source=a.present/('external_'+name+'_native.c');body=source.read_bytes();originals[str(source.relative_to(r))]=sha(source)
        for definition in definitions:
            assert body.count(definition)==1,(name,definition);body=body.replace(definition,b'')
        target=d/('external_'+name+'_native.c');target.write_bytes(b'#include "jp_native_frame_state.h"\n'+body);generated[target.name]=sha(target)
        command=list(next(x['command'] for x in pb['commands'] if '-c' in x['command'] and x['command'][-1]==str(a.present/obj)))
        command.insert(1,'-I'+str(d/'include'))
        command[command.index('-c')+1]=str(target);command[-1]=str(d/obj);commands.append(command)
    presentlink=list(next(x['command'] for x in pb['commands'] if '-shared' in x['command'] and x['command'][-1]==pb['DLL']))
    for old,new in [(str(a.present/'diagnostic.obj'),str(d/'diagnostic.obj')),(str(a.present/'frames/jp_native_frames.lib'),str(d/'frames/jp_native_frames.lib'))]:
        assert presentlink.count(old)==1,(old,presentlink);presentlink[presentlink.index(old)]=new
    presentlink[-1]=str(d/'jp_native_present.dll');commands.append(presentlink)
    kept={x:sha(x) for cmd in [frameslink,presentlink] for x in cmd if x.endswith('.obj') and not Path(x).is_relative_to(d)}
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    definitions_by_object={}
    for n in ['frame_state.obj','frame_probe.obj','diagnostic.obj','frame_pacing.obj']:
        symbols=coff_symbols(d/n);definitions_by_object[n]=sorted(s['name'] for s in symbols if s['name'] in names and s['section']>0)
    assert definitions_by_object=={'frame_state.obj':sorted(names),'frame_probe.obj':[],'diagnostic.obj':[],'frame_pacing.obj':[]}
    for n,h in kept.items():assert sha(n)==h,n
    result=dict(b,status='single_original_frame_counter_owner_and_native_imports_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,
        frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),
        DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),
        counter_definitions_by_object=definitions_by_object,original_frame_source_sha256=sha(u/'src/system/frame.c'),
        original_counter_definitions_sha256=hashlib.sha256(b'\n'.join(definitions)).hexdigest(),
        original_frame_header_sha256=sha(u/'src/system/frame.h'),native_frame_header_sha256=sha(header),
        baseline_build_sha256=sha(a.baseline/'build_verification.json'),present_baseline_sha256=sha(a.present/'build_verification.json'),
        kept_objects_sha256=kept,private_input_sources_sha256=originals,private_generated_sources_sha256=dict(generated,**{game.name:sha(game)}),
        shared_frame_counters=names,one_frame_counter_owner=True,task_hooks_or_real_presentation_recording_integrated=False,
        production_frame_pacing_object_compiled=True,complete_game_verified=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    result['authored_source_sha256']=dict(b['authored_source_sha256'])
    for src in [Path(__file__).resolve(),p/'native/jp_native_frame_state.h']:result['authored_source_sha256'][src.relative_to(r).as_posix()]=sha(src)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])


if __name__=='__main__':main()
