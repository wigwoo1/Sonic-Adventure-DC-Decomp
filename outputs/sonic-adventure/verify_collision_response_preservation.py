"""Preserve every prior published source/proof/build outside approved status updates."""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/collision_response_preservation.json';assert not target.exists()
    baseline=W/'collision_response_prior_source_manifest.json';prior=json.loads(baseline.read_text())['files'];assert len(prior)==16759
    mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in prior:
        if row['path'] not in mutable:assert sha(R/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith((D/'outputs/sonic-adventure'/name).read_bytes())
    old=json.loads((D/'outputs/sonic-adventure/analysis/recovered_routines.json').read_text())['routines'];new=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
    assert len(old)==1632 and len(new)==1634 and new[:1632]==old
    before=json.loads((D/'outputs/sonic-adventure/project_status.json').read_text());after=json.loads((P/'project_status.json').read_text())
    del after['verified_progress']['collision_response_qualification']
    after['recovered_buildable_routines']=before['recovered_buildable_routines'];after['verified_progress']['recovered_buildable_routines']=before['verified_progress']['recovered_buildable_routines'];assert after==before
    build_manifest=W/'collision_response_prior_build_manifest.json';builds=json.loads(build_manifest.read_text())['files'];assert len(builds)==2781
    for row in builds:assert sha(R/row['path'])==row['sha256'],row['path']
    scripts=sorted(P.glob('*collision_response*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    qp=P/'analysis/collision_response_qualification.json';q=json.loads(qp.read_text())
    for path,value in q['sha256'].items():assert sha(path)==value,path
    report=dict(status='prior_immutable_source_proofs_registry_build_bytes_preserved',preserved_prior_immutable_selected_files=16755,preserved_prior_registry_rows=1632,current_registry_entries=1634,preserved_prior_builds=2781,python_scripts_parsed=len(scripts),native_sources_changed=0,new_native_builds=0,overall_completion_percent=10,goal_paused=False,
        sha256={str(p):sha(p) for p in [Path(__file__),baseline,build_manifest,qp,*scripts,P/'README.md',P/'SOURCE_HANDOFF.txt',P/'project_status.json',P/'analysis/recovered_routines.json']})
    target.write_text(json.dumps(report,indent=2)+'\n');print('PRESERVED16755 immutable files/1632 prior registry rows/all2781 builds;',len(scripts),'Python scripts parsed',flush=True)
if __name__=='__main__':main()
