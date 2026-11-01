"""Check the published baseline bytes,registry history,builds and new script syntax."""
import ast,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    baseline=W/'player_work_lifecycle_prior_source_manifest.json';prior=json.loads(baseline.read_text())['files'];assert len(prior)==16541
    mutable={'outputs/sonic-adventure/'+x for x in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in prior:
        if row['path'] not in mutable:assert sha(R/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith((D/'outputs/sonic-adventure'/name).read_bytes())
    old=json.loads((D/'outputs/sonic-adventure/analysis/recovered_routines.json').read_text());new=json.loads((P/'analysis/recovered_routines.json').read_text())
    assert len(old['routines'])==1623 and len(new['routines'])==1626 and new['routines'][:1623]==old['routines']
    old_status=json.loads((D/'outputs/sonic-adventure/project_status.json').read_text());new_status=json.loads((P/'project_status.json').read_text())
    del new_status['verified_progress']['player_work_lifecycle_integration']
    new_status['recovered_buildable_routines']=old_status['recovered_buildable_routines']
    new_status['verified_progress']['recovered_buildable_routines']=old_status['verified_progress']['recovered_buildable_routines']
    assert new_status==old_status
    builds=json.loads((W/'player_work_lifecycle_prior_build_manifest.json').read_text())['files'];assert len(builds)==2765
    for row in builds:assert sha(R/row['path'])==row['sha256'],row['path']
    scripts=sorted(P.glob('*player_work_*.py'))
    for file in scripts:ast.parse(file.read_text(encoding='utf-8-sig'),filename=str(file))
    ps=[P/'ghidra'/f'export_player_work_{x}.ps1' for x in ('initialize','dependencies','allocation')]+[P/f'build_player_work_{x}.ps1' for x in ('initialize','dependencies','allocation')]
    for file in ps:
        code="$taskParseTokens=$null;$taskParseErrors=$null;[System.Management.Automation.Language.Parser]::ParseFile('"+str(file).replace("'","''")+"',[ref]$taskParseTokens,[ref]$taskParseErrors)|Out-Null;if($taskParseErrors.Count){throw ($taskParseErrors|Out-String)}"
        subprocess.run(['powershell','-NoProfile','-Command',code],check=True,capture_output=True)
    result=dict(status='published_history_and_build_bytes_preserved',preserved_prior_immutable_selected_files=16537,allowed_mutable_paths=sorted(mutable),preserved_prior_registry_rows=1623,current_registry_entries=1626,preserved_prior_builds=2765,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),overall_completion_percent=10,goal_paused=False,
                sha256={str(p):sha(p) for p in [Path(__file__),baseline,*scripts,*ps,P/'analysis/recovered_routines.json',P/'project_status.json',P/'README.md',P/'SOURCE_HANDOFF.txt']})
    target=P/'analysis/player_work_lifecycle_preservation.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('PRESERVED16537immutable files/1623registry rows/2765builds;',len(scripts),'Python and6PowerShell scripts parsed',flush=True)
if __name__=='__main__':main()
