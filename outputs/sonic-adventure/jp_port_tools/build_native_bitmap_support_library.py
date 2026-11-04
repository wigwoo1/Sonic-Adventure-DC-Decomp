"""Package three authored native bitmap upload/owner members, no game bodies."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path
from build_native_gpu_jobs_support_library import archive_members
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','audit','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((a.build/'build_verification.json').read_text());audit=json.loads(a.audit.read_text());q=json.loads((a.build/'verification.json').read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert q['checks']==722 and q['actual_GPU_images_compared']==2 and q['original_frame_wait_uses_real_bitmap_queue']
    counts={}
    for suite,expected in [('textures',1947),('frames',8572),('video',103700),('present',None)]:
        proof=json.loads((a.build/(suite+'-regression-qualified/qualification.json')).read_text());counts[suite]=proof['checks']
        if expected is not None:assert proof['checks']==expected
    assert audit['combined_remaining_import_count']==36 and audit['satisfied_presentation_import_count']==65 and not audit['test_fixture_or_dummy_services_linked']
    paths=[a.build.resolve()/n for n in ['textures_upload.obj','memory_upload.obj','bitmap_source.obj']];members={}
    for path in paths:assert sha(path)==audit['linked_objects_sha256'][str(path.resolve())];members[path.name]=sha(path)
    d.mkdir();library=d/'jp_native_bitmap_upload_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';cmd=[str(librarian),'/nologo','/out:'+str(library),*map(str,paths)]
    run=subprocess.run(cmd,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    assert archive_members(library)==collections.Counter(members.values())
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_bitmap_support_library.py','verify_native_bitmap_upload.py','verify_native_bitmap_regressions.py','audit_native_bitmap_upload_link.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    guide=r/'outputs/sonic-adventure/JP_NATIVE_BITMAP_UPLOAD.txt';sources[guide.relative_to(r).as_posix()]=sha(guide)
    result=dict(status='authored_native_bitmap_upload_support_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),
        member_objects_sha256=members,component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,exact_authored_members_verified=True,
        external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        production_link_audit_sha256=sha(a.audit),native_bitmap_report_sha256=sha(a.build/'verification.json'),integration_checks=722,regression_checks=counts,total_checks=722+sum(counts.values()),
        dependencies='REPLACE the retained texture owner with textures_upload.obj; never link both. Link real external bitmap queue/list/record services, native frame/video owners, GPU-jobs/presenter support and exactly one clock provider. No standalone game.',
        qualification='incomplete authored three-object bitmap/memory upload component; real owned surfaces and original queue integration; no full-gameplay claim',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['total_checks'],'checks including regressions')
if __name__=='__main__':main()
