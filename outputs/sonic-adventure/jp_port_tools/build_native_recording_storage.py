"""Build authored native recorder storage and real Windows file services only."""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,c,d=a.workspace,a.upstream,a.compiler,a.output;p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    d.mkdir();src=p/'native/jp_native_recording.c';dll=d/'jp_native_recording.dll'
    common=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'src')]
    commands=[common+['-c',str(src),'-o',str(d/'recording_storage.obj')],[str(c),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'recording_storage.obj'),'-Wl,kernel32.lib','-o',str(dll)]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    files=[src,p/'native/jp_native_recording.h',p/'native/jp_host_types.h',Path(__file__).resolve()]
    result=dict(status='authored_recording_storage_and_file_boundary_compiled_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,compiler_sha256=sha(c),DLL=str(dll),DLL_sha256=sha(dll),object_sha256=sha(d/'recording_storage.obj'),authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in files},external_header_sha256={n:sha(u/n) for n in ['src/progress/recorder.h','sdk/include/sg_xpt.h']},original_game_or_SDK_bodies_linked=False,artifact_publishable=True,original_recording_update_linked=False,original_pad_scan_linked=False,retail_assets_embedded=False,playable=False,overall_completion_percent=10)
    (d/'source-snapshot').mkdir()
    for f in files:(d/'source-snapshot'/f.name).write_bytes(f.read_bytes())
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
