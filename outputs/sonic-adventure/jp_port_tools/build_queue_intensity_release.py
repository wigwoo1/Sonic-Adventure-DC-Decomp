"""Relink the qualified authored queue under a unique, correct DLL/import name.
Old published files are retained. No game source bodies enter these artifacts.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();bdir=a.baseline.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((bdir/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    assert json.loads((bdir/'intensity_verification.json').read_text())['checks']==36879
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(bdir/'queue.obj')==b['artifacts_sha256']['queue.obj'];d.mkdir(parents=True);shutil.copyfile(bdir/'queue.obj',d/'queue.obj')
    link=list(b['commands'][1]['command']);assert '-shared' in link and sha(Path(link[0]))==b['compiler_sha256']
    index=link.index(str(bdir/'queue.obj'));link[index]=str(d/'queue.obj');link[-1]=str(d/'jp_native_queue_intensity.dll')
    run=subprocess.run(link,capture_output=True);log=d/'link.txt';log.write_bytes(run.stdout+run.stderr)
    assert run.returncode==0 and not log.stat().st_size,log.read_text()[:5000]
    report=dict(b);report.update(status='native_queue_linked_behavior_unverified',commands=[dict(command=link,exit_code=0,diagnostic_bytes=0,log_sha256=sha(log))],
        baseline_build_report_sha256=sha(bdir/'build_verification.json'),baseline_intensity_proof_sha256=sha(bdir/'intensity_verification.json'),
        production_object_same_as_original_qualified_queue=True,unique_DLL_and_matching_import_library=True,game_source_linked=False,retail_assets_embedded=False,
        artifacts_sha256={f.name:sha(f) for f in d.iterdir() if f.suffix in ['.obj','.dll','.lib']})
    report['authored_source_sha256'][Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Built uniquely named authored queue DLL/import library from the exact qualified production object; zero diagnostics.')

if __name__=='__main__':main()
