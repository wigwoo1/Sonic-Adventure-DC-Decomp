"""Package four authored native wait/presenter members, excluding game bodies."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path
from build_native_gpu_jobs_support_library import archive_members
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','audit','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((a.build/'build_verification.json').read_text());audit=json.loads(a.audit.read_text());q=json.loads((a.build/'verification_extended.json').read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert q['checks']>=125 and q['visible_Kamui_Presents']==8 and q['original_frame_wait_calls_real_render_flip']
    for suite,count in [('wait',6943),('frame',8572),('video',103700),('clock',12873)]:assert json.loads((a.build/(suite+'-regression')/'qualification.json').read_text())['checks']==count
    assert audit['combined_remaining_import_count']==39 and audit['satisfied_presentation_import_count']==63 and not audit['test_fixture_or_dummy_services_linked']
    paths=[a.build.resolve()/n for n in ['wait_service.obj','kamui_present.obj','present_sdk.obj']]+[Path(b['windows_idle_object'])];members={}
    for path in paths:assert sha(path)==audit['linked_objects_sha256'][str(path.resolve())];members[path.name]=sha(path)
    assert len(members)==4;d.mkdir();library=d/'jp_native_present_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';cmd=[str(librarian),'/nologo','/out:'+str(library),*map(str,paths)]
    run=subprocess.run(cmd,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    assert archive_members(library)==collections.Counter(members.values())
    sources=dict(b['authored_source_sha256'])
    for name in ['build_native_present_support_library.py','native_present_test_support.py','verify_native_present.py','verify_native_present_regressions.py','audit_native_present_link.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    result=dict(status='authored_native_render_flip_support_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),
        member_objects_sha256=members,component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,exact_authored_members_verified=True,
        external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        production_link_audit_sha256=sha(a.audit),native_present_report_sha256=sha(a.build/'verification_extended.json'),
        dependencies='Replaces earlier wait support; link alongside GPU jobs support and exactly one native clock provider. Original frame/game-present objects remain external.',
        qualification='incomplete authored four-object native presenter; real render/flip/EOR/stats, no trace providers or full-gameplay claim',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
