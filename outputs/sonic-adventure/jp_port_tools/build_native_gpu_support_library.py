"""Package only the qualified authored GPU object, preserving real dependencies."""
import argparse,collections,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','runtime','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();s=a.runtime.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((s/'build_verification.json').read_text());q=json.loads((s/'verification.json').read_text());x=json.loads((s/'extended_verification.json').read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert q['checks']==491158 and x['checks']==121451 and q['images_compared']+x['images_compared']==100
    assert q['build_report_sha256']==x['build_report_sha256']==sha(s/'build_verification.json') and q['DLL_sha256']==b['DLL_sha256']==sha(Path(b['DLL']))
    for name,proof in [('verify_native_gpu.py',q),('verify_native_gpu_extended.py',x)]:assert sha(r/'outputs/sonic-adventure/jp_port_tools'/name)==proof['verifier_sha256']
    assert q['texture_Off_shader_reflection_has_no_texture_or_sampler_bindings'] and x['real_texture_shutdown_and_reinitialize_survived']
    d.mkdir(parents=True);member='jp_native_gpu.obj';assert sha(s/member)==b['GPU_object_sha256'];shutil.copyfile(s/member,d/member)
    librarian=a.compiler.parent/'llvm-lib.exe';library=d/'jp_native_gpu_support.lib';cmd=[str(librarian),'/nologo','/out:'+str(library),str(d/member)]
    run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not (d/'build.txt').stat().st_size
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;found=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:found.append(hashlib.sha256(body).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and found==[sha(d/member)]
    authored=dict(b['authored_source_sha256'])
    for path in [Path(__file__).resolve(),*[r/'outputs/sonic-adventure/jp_port_tools'/n for n in ['verify_native_gpu.py','verify_native_gpu_extended.py','native_gpu_test_support.py']]]:authored[path.relative_to(r).as_posix()]=sha(path)
    report=dict(status='authored_native_GPU_support_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),member_objects_sha256={member:sha(d/member)},
        component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=authored,qualification_reports_sha256={str(s/n):sha(s/n) for n in ['build_verification.json','verification.json','extended_verification.json']},
        exact_authored_members_verified=True,external_game_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        dependencies=['real frame owner','authored packet/texel support','D3D11','D3DCompiler_47','DXGUID'],qualification='incomplete synchronous offscreen GPU component; no game loop/window/flip/vsync/gameplay',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
