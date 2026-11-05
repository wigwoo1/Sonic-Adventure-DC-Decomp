"""Build authored native G2 RAM/transfer transport against qualified AICA."""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path

def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','aica','compiler','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    core=json.loads((a.aica/'build_verification.json').read_bytes())
    assert sha(core['DLL'])==core['DLL_sha256'] and sha(core['audio_DLL'])==core['audio_DLL_sha256']
    for name,h in core['authored_source_sha256'].items():assert sha(r/name)==h,name
    for name in ('jp_native_aica.dll','jp_native_aica.lib','jp_native_audio.dll'):
        shutil.copyfile(a.aica/name,d/name)
    source=p/'native/jp_native_g2_audio.c';header=source.with_suffix('.h')
    commands=[[str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(source),'-o',str(d/'g2_audio.obj')],
        [str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'g2_audio.obj'),str(d/'jp_native_aica.lib'),'-o',str(d/'jp_native_g2_audio.dll')]]
    results=[]
    for index,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{index}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    result=dict(status='native_G2_sound_transport_linked_unverified',commands=results,
        DLL=str(d/'jp_native_g2_audio.dll'),DLL_sha256=sha(d/'jp_native_g2_audio.dll'),
        aica_build_sha256=sha(a.aica/'build_verification.json'),
        dependencies={name:sha(d/name) for name in ('jp_native_aica.dll','jp_native_aica.lib','jp_native_audio.dll')},
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in (source,header,Path(__file__).resolve())},
        original_G2_DMA_hardware_or_cycle_accuracy=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],flush=True)

if __name__=='__main__':main()
