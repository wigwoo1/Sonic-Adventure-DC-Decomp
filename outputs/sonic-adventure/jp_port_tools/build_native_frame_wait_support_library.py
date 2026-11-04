"""Package only authored wait/message services, excluding original frame code."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path
from build_native_gpu_jobs_support_library import archive_members
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','audit','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((a.build/'build_verification.json').read_text());audit=json.loads(a.audit.read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    q=json.loads((a.build/'verification.json').read_text());clock=json.loads((a.build/'clock-regression/qualification.json').read_text())
    assert q['checks']==6943 and q['build_report_sha256']==sha(a.build/'build_verification.json') and clock['checks']==12873
    assert audit['combined_remaining_import_count']==43 and audit['satisfied_presentation_import_count']==60 and not audit['test_fixture_or_dummy_services_linked']
    paths=[a.build.resolve()/n for n in ['wait.obj','windows_idle.obj']];members={}
    for path in paths:
        assert sha(path)==b['production_objects_sha256'][path.name]==audit['linked_objects_sha256'][str(path)];members[path.name]=sha(path)
    d.mkdir();library=d/'jp_native_frame_wait_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';cmd=[str(librarian),'/nologo','/out:'+str(library),*map(str,paths)]
    run=subprocess.run(cmd,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    assert archive_members(library)==collections.Counter(members.values())
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_frame_wait_support_library.py','verify_native_frame_wait.py','verify_native_frame_wait_clock.py','audit_native_frame_wait_link.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    result=dict(status='authored_native_frame_wait_support_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),member_objects_sha256=members,
        component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,exact_authored_members_verified=True,
        external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        production_link_audit_sha256=sha(a.audit),native_wait_report_sha256=sha(a.build/'verification.json'),clock_qualification_sha256=sha(a.build/'clock-regression/qualification.json'),
        dependencies='Link alongside the qualified GPU support and exactly one native clock provider. Original adapted frame/present objects stay external; replaces SDK njSetVSyncFunction.',
        qualification='incomplete authored two-object wait/message service; no game/trace/clock duplication, no native render/flip backend or gameplay claim',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
