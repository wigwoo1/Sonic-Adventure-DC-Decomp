"""Package exactly the qualified authored production objects; keep game private."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-library','build','audit','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    prior=json.loads((a.baseline_library/'build_verification.json').read_text());b=json.loads((a.build/'build_verification.json').read_text());audit=json.loads(a.audit.read_text())
    assert prior['exact_authored_members_verified'] and not prior['external_game_or_SDK_bodies_packaged'] and not prior['diagnostic_objects_packaged']
    assert b['compiler_sha256']==sha(a.compiler) and b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert audit['combined_remaining_import_count']==25 and not audit['test_fixture_or_dummy_services_linked']
    proofs=[]
    for suite,minimum in [('gpu',1033),('gpu_extended',613),('stateful',191)]:
        path=a.build/'regression'/(suite+'_direct_verification.json');q=json.loads(path.read_text());assert q['checks']==minimum and q['build_report_sha256']==sha(a.build/'build_verification.json');proofs.append(path)
    path=a.build/'direct_failure_verification.json';q=json.loads(path.read_text());assert q['checks']==41 and q['frame_palette_texture_shutdown_before_readback_verified'];proofs.append(path)
    paths=[]
    for name,h in prior['member_objects_sha256'].items():
        path=a.baseline_library/name;assert sha(path)==h,name
        paths.append(a.build/'gpu_window.obj' if name=='gpu_window.obj' else path)
    assert len(paths)==len({path.name for path in paths})==9
    # The prior library's copied members must equal the actual production
    # providers by basename/hash, rather than inventing a second state owner.
    actual={Path(n).name:h for n,h in audit['linked_objects_sha256'].items()};members={}
    for path in paths:assert sha(path)==actual[path.name];members[path.name]=sha(path)
    d.mkdir();library=d/'jp_native_direct_window_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';cmd=[str(librarian),'/nologo','/out:'+str(library),*map(str,paths)]
    run=subprocess.run(cmd,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;found=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:found.append(hashlib.sha256(body).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and collections.Counter(found)==collections.Counter(members.values())
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_direct_window_support_library.py','verify_native_direct_window.py','verify_native_direct_window_failures.py','audit_native_direct_window_link.py','audit_native_render_flip_waits.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    result=dict(status='authored_direct_window_support_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),
        member_objects_sha256=members,component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,
        exact_authored_members_verified=True,external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        verification_files_sha256={str(path):sha(path) for path in proofs+[a.audit]},production_link_audit_sha256=sha(a.audit),
        qualification='incomplete nine-object authored component; replaces older queue/texture/frame/GPU support libraries; original SDK/game bodies external; native direct GPU target is not Kamui scheduling/gameplay',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
