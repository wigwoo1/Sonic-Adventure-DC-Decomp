"""Link the authored GPU consumer to qualified packet/texture/frame providers.
The existing real producer runtime remains a private diagnostic dependency.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','packets','texels','frames','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    authored={};baselines={}
    for folder in [a.packets,a.texels,a.frames]:
        b=json.loads((folder/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h;authored[n]=h
        assert sha(Path(b['DLL']))==b['DLL_sha256'];baselines[str(folder/'build_verification.json')]=sha(folder/'build_verification.json')
    d.mkdir(parents=True);obj=d/'jp_native_gpu.obj';dll=d/'jp_native_gpu.dll';src=p/'native/jp_native_gpu.c'
    dependencies=[a.packets/'jp_native_packets.obj',a.packets/'jp_native_packets_frames.obj',a.texels/'jp_native_texels.obj']
    commands=[[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-c',str(src),'-o',str(obj)],
        [str(a.compiler),'-shared','-fuse-ld=lld',str(obj),*map(str,dependencies),str(a.frames/'jp_native_frames.lib'),'-Wl,d3d11.lib','-Wl,d3dcompiler.lib','-Wl,dxguid.lib','-o',str(dll)]];results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode or log.stat().st_size:
            print(log.read_text());raise RuntimeError('Build failed; preserve logs and inspect')
    for path in [src,p/'native/jp_native_gpu.h',Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    report=dict(status='native_GPU_consumer_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),authored_source_sha256=authored,
        existing_objects_sha256={str(path):sha(path) for path in dependencies},baseline_reports_sha256=baselines,DLL=str(dll),DLL_sha256=sha(dll),GPU_object_sha256=sha(obj),
        frames_DLL=str(a.frames.resolve()/'jp_native_frames.dll'),frames_DLL_sha256=sha(a.frames/'jp_native_frames.dll'),game_body_in_GPU_object=False,
        frame_provider_contains_game_source=True,game_frame_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
