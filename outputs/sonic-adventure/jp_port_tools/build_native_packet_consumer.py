"""Build authored packet decoding plus an adapter to a qualified real frame DLL.
The diagnostic game-containing dependency stays private; this creates no game EXE.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','frames','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();f=a.frames.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    baseline=json.loads((f/'build_verification.json').read_text());assert baseline['exit_code']==baseline['diagnostic_bytes']==0
    assert sha(a.compiler)==baseline['compiler_sha256'] and sha(f/'jp_native_frames.dll')==baseline['DLL_sha256']
    for n,h in baseline['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir(parents=True);flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    sources=['jp_native_packets.c','jp_native_packets_frames.c'];commands=[]
    for name in sources:
        commands.append([str(a.compiler),*flags,'-c',str(p/'native'/name),'-o',str(d/Path(name).with_suffix('.obj'))])
    commands.append([str(a.compiler),'-shared','-fuse-ld=lld',*[str(d/Path(n).with_suffix('.obj')) for n in sources],str(f/'jp_native_frames.lib'),'-o',str(d/'jp_native_packets.dll')])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode or log.stat().st_size:
            print(log.read_text());raise RuntimeError('Build failed; inspect preserved log')
    report=dict(status='native_packet_consumer_compiled_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,
        compiler_sha256=sha(a.compiler),frames_DLL=str(f/'jp_native_frames.dll'),frames_DLL_sha256=sha(f/'jp_native_frames.dll'),
        baseline_report_sha256=sha(f/'build_verification.json'),DLL=str(d/'jp_native_packets.dll'),DLL_sha256=sha(d/'jp_native_packets.dll'),
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in [*[p/'native'/n for n in sources],p/'native/jp_native_packets.h',Path(__file__).resolve()]},
        production_objects_sha256={Path(n).with_suffix('.obj').name:sha(d/Path(n).with_suffix('.obj')) for n in sources},
        rasterizer_implemented=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])

if __name__=='__main__':main()
