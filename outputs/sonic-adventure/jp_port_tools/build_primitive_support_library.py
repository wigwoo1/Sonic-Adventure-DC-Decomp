"""Package only two authored support objects; exclude all game/model bodies."""
import argparse,collections,hashlib,json,shutil,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','objects','runtime','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();s=a.objects.resolve();d=a.output.resolve();runtime=a.runtime.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((s/'build_verification.json').read_text());qpath=runtime/'qualified_contract_verification.json';q=json.loads(qpath.read_text())
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler) and q['checks']==371722 and not q['original_dependency_hooks_used']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    d.mkdir(parents=True);members={}
    for n in ['primitive_textures.obj','primitive_numeric.obj']:
        assert sha(s/n)==b['production_objects_sha256'][n];shutil.copyfile(s/n,d/n);members[n]=sha(d/n)
    librarian=a.compiler.parent/'llvm-lib.exe';lib=d/'jp_native_primitive_support.lib'
    cmd=[str(librarian),'/nologo','/out:'+str(lib),*[str(d/n) for n in members]];run=subprocess.run(cmd,capture_output=True);log=d/'build.txt';log.write_bytes(run.stdout+run.stderr)
    assert run.returncode==0 and not log.stat().st_size,log.read_text()
    raw=lib.read_bytes();assert raw[:8]==b'!<arch>\n';at=8;found=[]
    while at<len(raw):
        header=raw[at:at+60];assert header[58:]==b'`\n';size=int(header[48:58]);name=header[:16].decode().strip();body=raw[at+60:at+60+size]
        if name not in ['/','//','/<ECSYMBOLS>/']:found.append(hashlib.sha256(body).hexdigest())
        at+=60+size+(size&1)
    assert at==len(raw) and collections.Counter(found)==collections.Counter(members.values())
    authored=dict(b['authored_source_sha256']);authored[Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    report=dict(status='authored_primitive_support_library_members_verified',exit_code=0,diagnostic_bytes=0,command=cmd,log_sha256=sha(log),librarian_sha256=sha(librarian),
        compiler_sha256=sha(a.compiler),authored_source_sha256=authored,member_objects_sha256=members,component_library=str(lib),component_library_sha256=sha(lib),
        source_object_build_report_sha256=sha(s/'build_verification.json'),qualification_report_sha256=sha(qpath),library_members_byte_exact=True,
        actual_game_primitive_bodies_packaged=False,point_vector_model_code_packaged=False,retail_assets_embedded=False,artifact_publishable=True,
        qualification='incomplete support component, requires actual queue/texture/math providers; no renderer or playable game',overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],'two exact authored objects; zero diagnostics')
if __name__=='__main__':main()
