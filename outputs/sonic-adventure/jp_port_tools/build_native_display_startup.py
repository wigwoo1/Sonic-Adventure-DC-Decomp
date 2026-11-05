"""Restore original startup globals and SDK cable decoding over Windows query."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((a.baseline/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    fl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    globals=['cable_mode','dat_8c78b4d8','dat_8c78b4dc'];names=globals+['syCblCheckCable','syCblCheckCableType']
    for obj in [x for x in fl if x.endswith('.obj')]:
        assert not any(s['name'] in names and s['section']>0 for s in coff_symbols(Path(obj))),obj
    inputs=['src/main.c','src/main.h','sdk/shinobi/sg_syCbl.c','sdk/include/shinobi.h'];originals={n:sha(u/n) for n in inputs};git='C:/Program Files/Git/cmd/git.exe'
    assert subprocess.check_output([git,'-C',str(u),'rev-parse','HEAD']).decode().strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','HEAD:'+n])).hexdigest()==h,n
    d.mkdir();raw=(u/'sdk/shinobi/sg_syCbl.c').read_bytes();body=function(raw,'syCblCheckCable');sdk=d/'external_cable_decoder.c';sdk.write_bytes(raw.replace(function(raw,'syCblCheckCableType'),b''))
    raw=(u/'src/main.c').read_bytes();state=d/'external_main_startup_state.c';lines=[]
    for n in globals:
        matches=re.findall(rb'^int '+n.encode()+rb';',raw,re.M);assert len(matches)==1,n;lines.append(matches[0])
    state.write_bytes(b'\n'.join(lines)+b'\n')
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    host=[common[0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror']
    own=p/'native/jp_native_display_windows.c';objects=[];commands=[]
    for source in (sdk,state,own):
        obj=d/(source.stem+'.obj');objects.append(obj);commands.append((host if source==own else common)+['-c',str(source),'-o',str(obj)])
    export=['-Wl,/export:'+n for n in names if n not in globals]+[arg for n in globals for arg in ('-Xlinker','/export:'+n+',DATA')]
    fl[fl.index('-o'):fl.index('-o')]=[str(x) for x in objects]+export;fl[-1]=str(d/'jp_native_frames.dll');commands.append(fl);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    for n,h in originals.items():assert sha(u/n)==h,n
    result=dict(status='original_main_startup_globals_and_cable_decoder_actual_Windows_display_query_linked_unverified',commands=results,frames_DLL=str(d/'jp_native_frames.dll'),frames_DLL_sha256=sha(d/'jp_native_frames.dll'),present_DLL=b['present_DLL'],present_DLL_sha256=b['present_DLL_sha256'],authored_source_sha256={str(x.relative_to(r)):sha(x) for x in (own,p/'native/jp_native_display.h',Path(__file__).resolve())},generated_sources_sha256={x.name:sha(x) for x in (sdk,state)},original_source_sha256=originals,original_cable_decoder_sha256=hashlib.sha256(body).hexdigest(),original_cable_decoder_body_unchanged=function(sdk.read_bytes(),'syCblCheckCable')==body,original_main_global_names=globals,single_new_definitions_verified=True,kept_frame_objects_sha256={n:sha(n) for n in fl if n.endswith('.obj')},baseline_build_sha256=sha(a.baseline/'build_verification.json'),real_Windows_display_query=True,physical_cable_type_claimed=False,full_sbInitSystem_or_audio_implemented=False,artifact_publishable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
