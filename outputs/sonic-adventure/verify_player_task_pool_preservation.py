"""Preserve the published b3965dd5 snapshot and every previous binary byte."""
import ast,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    baseline=W/'player_task_pool_prior_source_manifest.json';prior=json.loads(baseline.read_text())['files'];assert len(prior)==16610
    mutable={'outputs/sonic-adventure/'+x for x in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in prior:
        if row['path'] not in mutable:assert sha(R/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith((D/'outputs/sonic-adventure'/name).read_bytes())
    before=json.loads((D/'outputs/sonic-adventure/analysis/recovered_routines.json').read_text())['routines'];after=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
    assert len(before)==1626 and len(after)==1627 and after[:1626]==before
    old_status=json.loads((D/'outputs/sonic-adventure/project_status.json').read_text());new_status=json.loads((P/'project_status.json').read_text())
    del new_status['verified_progress']['player_task_pool_integration']
    new_status['recovered_buildable_routines']=old_status['recovered_buildable_routines']
    new_status['verified_progress']['recovered_buildable_routines']=old_status['verified_progress']['recovered_buildable_routines']
    assert new_status==old_status
    builds=json.loads((W/'player_task_pool_prior_build_manifest.json').read_text())['files'];assert len(builds)==2771
    for row in builds:assert sha(R/row['path'])==row['sha256'],row['path']
    scripts=sorted(P.glob('*player_task_pool*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    ps=[P/'ghidra'/f'export_player_task_pool_{name}.ps1' for name in ('callers','owner_leads')]
    for path in ps:
        code="$taskParseTokens=$null;$taskParseErrors=$null;[System.Management.Automation.Language.Parser]::ParseFile('"+str(path).replace("'","''")+"',[ref]$taskParseTokens,[ref]$taskParseErrors)|Out-Null;if($taskParseErrors.Count){throw ($taskParseErrors|Out-String)}"
        subprocess.run(['powershell','-NoProfile','-Command',code],check=True,capture_output=True)
    for name in ('player_task_pool_qualification','player_task_pool_caller_leads'):
        report=json.loads((P/f'analysis/{name}.json').read_text())
        for path,expected in report['sha256'].items():assert sha(path)==expected,path
    result=dict(status='published_source_proofs_registry_and_all_build_bytes_preserved',preserved_prior_immutable_selected_files=16606,preserved_prior_registry_rows=1626,current_registry_entries=1627,preserved_prior_builds=2771,all_build_bytes_unchanged=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=2,overall_completion_percent=10,goal_paused=False,
                sha256={str(p):sha(p) for p in [Path(__file__),baseline,*scripts,*ps,P/'analysis/recovered_routines.json',P/'project_status.json',P/'README.md',P/'SOURCE_HANDOFF.txt']})
    target=P/'analysis/player_task_pool_preservation.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('PRESERVED16606immutable files/1626registry rows/2771builds;all binaries unchanged;',len(scripts),'Python scripts parsed',flush=True)
if __name__=='__main__':main()
