"""Preserve prior published source/proofs/registry prefix/build bytes."""
import ast,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_child_callback_preservation.json';assert not target.exists()
    baseline=W/'player_child_callback_prior_source_manifest.json';prior=json.loads(baseline.read_text())['files'];assert len(prior)==16788
    mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in prior:
        if row['path'] not in mutable:assert sha(R/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith((D/'outputs/sonic-adventure'/name).read_bytes())
    old=json.loads((D/'outputs/sonic-adventure/analysis/recovered_routines.json').read_text())['routines'];new=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
    assert len(old)==1634 and len(new)==1636 and new[:1634]==old
    before=json.loads((D/'outputs/sonic-adventure/project_status.json').read_text());after=json.loads((P/'project_status.json').read_text())
    del after['verified_progress']['player_child_callback_integration']
    after['recovered_buildable_routines']=before['recovered_buildable_routines'];after['verified_progress']['recovered_buildable_routines']=before['verified_progress']['recovered_buildable_routines'];assert after==before
    bm=W/'player_child_callback_prior_build_manifest.json';builds=json.loads(bm.read_text())['files'];assert len(builds)==2781
    for row in builds:assert sha(R/row['path'])==row['sha256'],row['path']
    scripts=sorted(P.glob('*player_child_callback*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    ps=[P/'build_player_child_callback.ps1',P/'build_player_child_callback_dependencies.ps1',P/'ghidra/export_player_child_callback.ps1',P/'ghidra/export_player_child_callback_dependencies.ps1']
    for path in ps:
        code="$tokens=$null;$errors=$null;[System.Management.Automation.Language.Parser]::ParseFile('"+str(path).replace("'","''")+"',[ref]$tokens,[ref]$errors)|Out-Null;if($errors.Count){throw ($errors|Out-String)}"
        subprocess.run(['powershell','-NoProfile','-Command',code],check=True,capture_output=True)
    qp=P/'analysis/player_child_callback_qualification.json';q=json.loads(qp.read_text())
    for path,value in q['sha256'].items():assert sha(path)==value,path
    report=dict(status='prior_immutable_source_proofs_registry_prefix_and_all_builds_preserved',preserved_prior_immutable_selected_files=16784,preserved_prior_registry_rows=1634,current_registry_entries=1636,preserved_prior_builds=2781,python_scripts_parsed=len(scripts),powershell_scripts_parsed=4,overall_completion_percent=10,goal_paused=False,
        sha256={str(p):sha(p) for p in [Path(__file__),baseline,bm,qp,*scripts,*ps,P/'README.md',P/'SOURCE_HANDOFF.txt',P/'project_status.json',P/'analysis/recovered_routines.json']})
    target.write_text(json.dumps(report,indent=2)+'\n');print('PRESERVED16784immutable files/1634registry rows/2781builds;',len(scripts),'Python/4PowerShell scripts parsed',flush=True)
if __name__=='__main__':main()
