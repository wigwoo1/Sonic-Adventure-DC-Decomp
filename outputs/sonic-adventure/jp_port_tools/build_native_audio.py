"""Build authored real Windows PCM output; no game/SDK/retail code packaged."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','compiler','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    common=[str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror']
    cc=common+['-c',str(p/'native/jp_native_audio.c'),'-o',str(d/'audio.obj')]
    peer=d/'audio_signature_check.c';peer.write_text('#include "jp_native_audio.h"\n_Static_assert(sizeof(struct jp_audio_config)==32,"audio config ABI");\n_Static_assert(sizeof(struct jp_audio_info)==144,"audio snapshot ABI");\n')
    signature=common+['-I'+str(p/'native'),'-c',str(peer),'-o',str(d/'signature.obj')]
    link=[str(a.compiler.resolve()),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'audio.obj'),'-Wl,kernel32.lib,ole32.lib','-o',str(d/'jp_native_audio.dll')];results=[]
    for j,cmd in enumerate((cc,signature,link)):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    files=[p/'native/jp_native_audio.c',p/'native/jp_native_audio.h',Path(__file__).resolve()]
    v=dict(status='authored_real_WASAPI_worker_PCM_transport_linked_unverified',commands=results,DLL=str(d/'jp_native_audio.dll'),DLL_sha256=sha(d/'jp_native_audio.dll'),object_sha256=sha(d/'audio.obj'),compiler_sha256=sha(a.compiler),
        authored_source_sha256={str(f.relative_to(r)):sha(f) for f in files},original_game_or_SDK_bodies_linked=False,retail_assets_packaged=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(v,indent=2)+'\n');print(v['status'],flush=True)
if __name__=='__main__':main()
