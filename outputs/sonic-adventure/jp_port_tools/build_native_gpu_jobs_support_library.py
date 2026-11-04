"""Package the exact nine authored production members, including GPU queries."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def archive_members(path):
    raw=path.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;found=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:found.append(hashlib.sha256(body).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw);return collections.Counter(found)
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline-library','build','audit','compiler','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    prior=json.loads((a.baseline_library/'build_verification.json').read_text());b=json.loads((a.build/'build_verification.json').read_text());audit=json.loads(a.audit.read_text())
    assert prior['exact_authored_members_verified'] and not prior['external_game_or_SDK_bodies_packaged'] and not prior['diagnostic_objects_packaged']
    oldlib=Path(prior['component_library']);assert sha(oldlib)==prior['component_library_sha256'] and archive_members(oldlib)==collections.Counter(prior['member_objects_sha256'].values())
    assert b['compiler_sha256']==sha(a.compiler) and b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert audit['combined_remaining_import_count']==25 and not audit['test_fixture_or_dummy_services_linked']
    proofs=[a.build/'verification.json'];q=json.loads(proofs[0].read_text());assert q['checks']==4205 and q['real_pending_observations']>0 and q['four_live_targets_and_fifth_rejection_verified']
    for suite,count in [('gpu',1202),('gpu_extended',725),('stateful',198)]:
        path=a.build/'regression'/(suite+'_jobs_verification.json');q=json.loads(path.read_text());assert q['checks']==count and q['build_report_sha256']==sha(a.build/'build_verification.json');proofs.append(path)
    paths=[];members={}
    for name,oldhash in prior['member_objects_sha256'].items():
        matches=[Path(n) for n in audit['linked_objects_sha256'] if Path(n).name==name];assert len(matches)==1;path=matches[0]
        assert sha(path)==audit['linked_objects_sha256'][str(path)]
        assert sha(path)==(b['production_object_sha256'] if name=='gpu_window.obj' else oldhash)
        paths.append(path);members[name]=sha(path)
    assert len(paths)==len(members)==9
    d.mkdir();library=d/'jp_native_gpu_jobs_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';cmd=[str(librarian),'/nologo','/out:'+str(library),*map(str,paths)]
    run=subprocess.run(cmd,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    assert archive_members(library)==collections.Counter(members.values())
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_gpu_jobs_support_library.py','native_gpu_jobs_test_support.py','verify_native_gpu_jobs.py','verify_native_gpu_jobs_regressions.py','audit_native_direct_window_link.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    result=dict(status='authored_GPU_query_jobs_support_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),
        member_objects_sha256=members,component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,
        exact_authored_members_verified=True,external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        verification_files_sha256={str(path):sha(path) for path in proofs+[a.audit]},production_link_audit_sha256=sha(a.audit),
        qualification='incomplete nine-object authored component with actual GPU query ownership; replaces overlapping older graphics libraries; original SDK/game bodies external; no native game waits or Kamui scheduler yet',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
