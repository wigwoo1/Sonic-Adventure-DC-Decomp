"""Compile only the authored file-backed native data-track transport."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','compiler','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build');d.mkdir()
    compiler=str(a.compiler.resolve());common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror']
    commands=[common+['-c',str(p/'native/jp_native_gd_media.c'),'-o',str(d/'gd_media.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'gd_media.obj'),'-Wl,kernel32.lib','-o',str(d/'jp_native_gd_media.dll')]]
    results=[]
    for j,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,log.read_text()
    files=[p/'native/jp_native_gd_media.c',p/'native/jp_native_gd_media.h',Path(__file__).resolve()]
    b=dict(status='authored_file_backed_native_GD_data_track_transport_linked_unverified',commands=results,DLL=str(d/'jp_native_gd_media.dll'),DLL_sha256=sha(d/'jp_native_gd_media.dll'),object_sha256=sha(d/'gd_media.obj'),
        compiler_sha256=sha(compiler),authored_source_sha256={f.relative_to(r).as_posix():sha(f) for f in files},original_game_or_SDK_bodies_linked=False,
        retail_data_embedded=False,artifact_publishable=True,full_GDFS_or_system_bootstrap_integrated=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n');print(b['status'],flush=True)
if __name__=='__main__':main()
