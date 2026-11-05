"""Package only the authored passive observation member, not game/SDK code."""
import argparse,collections,hashlib,json,subprocess
from pathlib import Path
from build_native_gpu_jobs_support_library import archive_members
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();b=read(a.build/'build_verification.json');d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(a.compiler)==b['compiler_sha256'] and b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    proofpaths=[a.build/n for n in ['verification.json','lifecycle-regression/verification.json','startup-regression.json','frames-regression/verification.json','present-regression/verification.json']]
    counts=[read(p)['checks'] for p in proofpaths];assert counts==[125,7601,8266,8572,131] and sum(counts)==24695
    assert read(proofpaths[0])['build_report_sha256']==sha(a.build/'build_verification.json')
    obj=a.build/'observer.obj';source=r/'outputs/sonic-adventure/native/jp_native_present_observer.c'
    command=next(x['command'] for x in b['commands'] if x['command'][-1]==str(obj.resolve()))
    assert Path(command[command.index('-c')+1]).resolve()==source.resolve()
    d.mkdir();library=d/'jp_native_presentation_observer_support.lib';librarian=a.compiler.parent/'llvm-lib.exe'
    cmd=[str(librarian),'/nologo','/out:'+str(library),str(obj.resolve())];run=subprocess.run(cmd,capture_output=True)
    log=d/'build.txt';log.write_bytes(run.stdout+run.stderr);assert run.returncode==0 and not log.stat().st_size
    assert archive_members(library)==collections.Counter([sha(obj)])
    authored=[source,source.with_suffix('.h'),Path(__file__).resolve(),r/'outputs/sonic-adventure/jp_port_tools/build_native_task_presentation_runtime.py',r/'outputs/sonic-adventure/jp_port_tools/build_native_task_logo_start.py',r/'outputs/sonic-adventure/jp_port_tools/verify_native_task_presentation.py']
    result=dict(status='authored_passive_presentation_observer_member_source_matched_and_verified',exit_code=0,diagnostic_bytes=0,command=cmd,librarian_sha256=sha(librarian),component_library=str(library),component_library_sha256=sha(library),member_objects_sha256={'observer.obj':sha(obj)},authored_source_sha256={p.resolve().relative_to(r).as_posix():sha(p) for p in authored},verification_files_sha256={str(p.resolve()):sha(p) for p in proofpaths},checks=counts,total_checks=sum(counts),exact_authored_member_verified=True,external_game_or_SDK_bodies_packaged=False,diagnostic_fixture_objects_packaged=False,clock_task_or_texture_owners_packaged=False,retail_assets_embedded=False,artifact_publishable=True,qualification='Optional passive tracing only; genuine task/presentation/clock providers are external dependencies. Incomplete research component, not a playable game.',playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],sum(counts),'checks')


if __name__=='__main__':main()
