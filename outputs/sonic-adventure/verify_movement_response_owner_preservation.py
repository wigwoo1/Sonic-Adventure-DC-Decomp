"""Preserve prior source/proofs/registry/builds while adding lifecycle evidence."""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/movement_response_owner_preservation.json';assert not target.exists()
    baseline=W/'movement_response_owner_prior_source_manifest.json';prior=json.loads(baseline.read_text())['files'];assert len(prior)==16776
    mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json')}
    for row in prior:
        if row['path'] not in mutable:assert sha(R/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith((D/'outputs/sonic-adventure'/name).read_bytes())
    registry=P/'analysis/recovered_routines.json';assert registry.read_bytes()==(D/'outputs/sonic-adventure/analysis/recovered_routines.json').read_bytes()
    assert len(json.loads(registry.read_text())['routines'])==1634
    before=json.loads((D/'outputs/sonic-adventure/project_status.json').read_text());after=json.loads((P/'project_status.json').read_text())
    del after['verified_progress']['movement_response_owner_lifecycle'];assert after==before
    bm=W/'movement_response_owner_prior_build_manifest.json';builds=json.loads(bm.read_text())['files'];assert len(builds)==2781
    for row in builds:assert sha(R/row['path'])==row['sha256'],row['path']
    scripts=sorted(P.glob('*movement_response_owner*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    qp=P/'analysis/movement_response_owner_research.json';q=json.loads(qp.read_text())
    for path,value in q['sha256'].items():assert sha(path)==value,path
    report=dict(status='prior_source_proofs_entire_registry_and_all_builds_preserved',preserved_prior_immutable_selected_files=16773,preserved_prior_registry_rows=1634,current_registry_entries=1634,preserved_prior_builds=2781,python_scripts_parsed=len(scripts),native_sources_changed=0,new_native_builds=0,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,
        sha256={str(p):sha(p) for p in [Path(__file__),baseline,bm,qp,*scripts,P/'README.md',P/'SOURCE_HANDOFF.txt',P/'project_status.json',registry]})
    target.write_text(json.dumps(report,indent=2)+'\n');print('PRESERVED16773 immutable files/all1634registry rows/all2781builds;',len(scripts),'Python scripts parsed',flush=True)
if __name__=='__main__':main()
