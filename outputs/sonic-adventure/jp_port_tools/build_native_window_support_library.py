"""Package the coherent nine-object authored graphics/state/window component.
Original SDK/game setters and diagnostic providers remain external and private.
"""
import argparse,collections,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','frames','window','audit','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    fb=json.loads((a.frames/'build_verification.json').read_text());wb=json.loads((a.window/'build_verification.json').read_text());audit=json.loads(a.audit.read_text())
    sources={}
    for b in [fb,wb]:
        assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h;sources[n]=h
    q=json.loads((a.window/'verification.json').read_text());assert q['checks']==103700 and q['visible_successful_Presents']==4 and q['build_report_sha256']==sha(a.window/'build_verification.json')
    assert audit['combined_remaining_import_count']==25 and not audit['test_fixture_or_dummy_services_linked']
    for folder,name,n in [(a.frames/'regression','frame_ownership.json',8572),(a.window/'regression','verification.json',491158),(a.window/'regression','extended_verification.json',121451)]:assert json.loads((folder/name).read_text())['checks']==n
    paths=[a.frames/'frames.obj',a.frames/'video.obj',a.frames/'video_sdk.obj',a.window/'gpu_window.obj']
    paths += [Path(n) for n in wb['existing_objects_sha256']]
    for name in ['queue_frames.obj','textures_retained.obj']:
        found=[Path(n) for n in fb['kept_objects_sha256'] if Path(n).name==name];assert len(found)==1;paths+=found
    assert len(paths)==9 and len({p.name for p in paths})==9
    d.mkdir(parents=True);members={}
    for path in paths:
        assert sha(path)==audit['linked_objects_sha256'][str(path.resolve())],path
        shutil.copyfile(path,d/path.name);members[path.name]=sha(path)
    library=d/'jp_native_window_state_support.lib';librarian=a.compiler.parent/'llvm-lib.exe';cmd=[str(librarian),'/nologo','/out:'+str(library),*[str(d/n) for n in members]]
    run=subprocess.run(cmd,capture_output=True);(d/'build.txt').write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not (d/'build.txt').stat().st_size
    raw=library.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;found=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:found.append(hashlib.sha256(body).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and collections.Counter(found)==collections.Counter(members.values())
    for name in ['build_native_window_support_library.py','native_video_test_support.py','verify_native_video_window.py','verify_native_stateful_regressions.py','audit_native_window_link.py']:
        path=r/'outputs/sonic-adventure/jp_port_tools'/name;sources[path.relative_to(r).as_posix()]=sha(path)
    report=dict(status='authored_stateful_window_support_library_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),member_objects_sha256=members,
        component_library=str(library),component_library_sha256=sha(library),authored_source_sha256=sources,exact_authored_members_verified=True,
        external_game_or_SDK_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        production_link_audit_sha256=sha(a.audit),qualification_report_sha256=sha(a.window/'verification.json'),
        qualification='incomplete nine-object authored component; replaces older queue/texture/frame/GPU support libraries; original SDK setters and game producers stay external; no Kamui scheduling/gameplay',overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])
if __name__=='__main__':main()
