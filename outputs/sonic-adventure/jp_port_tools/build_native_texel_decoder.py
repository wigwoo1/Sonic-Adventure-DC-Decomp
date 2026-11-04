"""Compile authored encoded-surface decoder; no game/SDK code is linked."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists();d.mkdir(parents=True)
    obj=d/'jp_native_texels.obj';dll=d/'jp_native_texels.dll';src=p/'native/jp_native_texels.c'
    commands=[[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(src),'-o',str(obj)],
        [str(a.compiler),'-shared','-fuse-ld=lld',str(obj),'-o',str(dll)]];results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==log.stat().st_size==0,log.read_text()
    report=dict(status='authored_texel_decoder_compiled_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,
        compiler_sha256=sha(a.compiler),DLL=str(dll),DLL_sha256=sha(dll),object_sha256=sha(obj),
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in [src,p/'native/jp_native_texels.h',p/'native/jp_native_packets.h',Path(__file__).resolve()]},
        game_source_linked=False,game_assets_included=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
