"""Package exactly the three qualified authored owner/frame objects."""
import argparse,collections,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','runtime','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();s=a.runtime.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((s/'build_verification.json').read_text());qp=s/'final_frame_verification.json';q=json.loads(qp.read_text());mp=s/'qualified_model_retention.json';m=json.loads(mp.read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler) and q['checks']==8572 and m['original_model_checks']==200669 and m['checks']==7
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir(parents=True);members={}
    for n in ['queue_frames.obj','textures_retained.obj','frames.obj']:
        assert sha(s/n)==b['production_objects_sha256'][n];shutil.copyfile(s/n,d/n);members[n]=sha(d/n)
    librarian=a.compiler.parent/'llvm-lib.exe';lib=d/'jp_native_owned_frames_support.lib'
    cmd=[str(librarian),'/nologo','/out:'+str(lib),*[str(d/n) for n in members]];run=subprocess.run(cmd,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr)
    assert run.returncode==0 and not log.stat().st_size,log.read_text()
    raw=lib.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;found=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:found.append(hashlib.sha256(body).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and collections.Counter(found)==collections.Counter(members.values())
    authored=dict(b['authored_source_sha256']);authored[Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    report=dict(status='authored_owned_frame_support_library_members_verified',exit_code=0,diagnostic_bytes=0,command=cmd,log_sha256=sha(log),librarian_sha256=sha(librarian),compiler_sha256=sha(a.compiler),
        authored_source_sha256=authored,member_objects_sha256=members,component_library=str(lib),component_library_sha256=sha(lib),source_object_build_report_sha256=sha(s/'build_verification.json'),
        qualification_report_sha256=sha(qp),model_retention_report_sha256=sha(mp),library_members_byte_exact=True,external_game_bodies_packaged=False,diagnostic_objects_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        qualification='incomplete owner/frame support component; replaces earlier queue/texture owners, requires real SDK providers; no renderer or playable game',overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],'three exact authored objects; zero diagnostics',flush=True)
if __name__=='__main__':main()
