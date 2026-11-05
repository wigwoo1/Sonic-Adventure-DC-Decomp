"""Compile authored Windows sampling and original-SDK peripheral record ABI."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r=a.workspace;u=a.upstream;p=r/'outputs/sonic-adventure';d=a.output
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    d.mkdir();source=p/'native/jp_native_input.c'
    common=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include')]
    exports=['njGetPeripheral','pdGetPeripheral','pdGetPeripheralInfo','pdExecPeripheralServer','pdExitPeripheral'];dll=d/'jp_native_input.dll'
    commands=[common+['-c',str(source),'-o',str(d/'input.obj')],[str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'input.obj'),*['-Wl,/export:'+n for n in exports],'-Wl,user32.lib','-Wl,kernel32.lib','-o',str(dll)]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    report=dict(status='authored_native_Windows_input_and_original_peripheral_ABI_compiled_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,DLL=str(dll),DLL_sha256=sha(dll),object_sha256=sha(d/'input.obj'),compiler_sha256=sha(a.compiler),authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in [source,p/'native/jp_native_input.h',p/'native/jp_host_types.h',Path(__file__).resolve()]},external_header_sha256={n:sha(u/n) for n in ['sdk/include/ninja.h','sdk/include/shinobi.h','sdk/include/sg_xpt.h','sdk/include/njtypes.h']},original_game_or_SDK_bodies_linked=False,retail_assets_embedded=False,artifact_publishable=True,physical_controller_verified=False,original_pad_scan_linked=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
