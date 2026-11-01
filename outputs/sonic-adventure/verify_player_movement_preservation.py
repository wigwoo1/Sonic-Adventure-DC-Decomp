"""Preserve every previous published byte outside the four approved status files."""
import ast,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    baseline=W/'player_movement_prior_source_manifest.json';prior=json.loads(baseline.read_text())['files'];assert len(prior)==16633
    mutable={'outputs/sonic-adventure/'+x for x in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in prior:
        if row['path'] not in mutable:assert sha(R/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith((D/'outputs/sonic-adventure'/name).read_bytes())
    old=json.loads((D/'outputs/sonic-adventure/analysis/recovered_routines.json').read_text())['routines'];new=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
    assert len(old)==1627 and len(new)==1631 and new[:1627]==old
    before=json.loads((D/'outputs/sonic-adventure/project_status.json').read_text());after=json.loads((P/'project_status.json').read_text())
    del after['verified_progress']['player_movement_path_integration']
    after['recovered_buildable_routines']=before['recovered_buildable_routines'];after['verified_progress']['recovered_buildable_routines']=before['verified_progress']['recovered_buildable_routines'];assert after==before
    builds=json.loads((W/'player_movement_prior_build_manifest.json').read_text())['files'];assert len(builds)==2771
    for row in builds:assert sha(R/row['path'])==row['sha256'],row['path']
    scripts=sorted(P.glob('*player_movement*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    ps=[P/f'build_player_movement_{x}.ps1' for x in ('owner','dependencies','path')]+[P/'ghidra'/f'export_player_movement_{x}.ps1' for x in ('owner_dependencies','shape_primitives')]
    for path in ps:
        code="$taskParseTokens=$null;$taskParseErrors=$null;[System.Management.Automation.Language.Parser]::ParseFile('"+str(path).replace("'","''")+"',[ref]$taskParseTokens,[ref]$taskParseErrors)|Out-Null;if($taskParseErrors.Count){throw ($taskParseErrors|Out-String)}"
        subprocess.run(['powershell','-NoProfile','-Command',code],check=True,capture_output=True)
    q=json.loads((P/'analysis/player_movement_path_qualification.json').read_text())
    for path,expected in q['sha256'].items():assert sha(path)==expected,path
    result=dict(status='all_previous_immutable_source_proofs_registry_and_build_bytes_preserved',preserved_prior_immutable_selected_files=16629,preserved_prior_registry_rows=1627,current_registry_entries=1631,preserved_prior_builds=2771,python_scripts_parsed=len(scripts),powershell_scripts_parsed=5,overall_completion_percent=10,goal_paused=False,
                sha256={str(p):sha(p) for p in [Path(__file__),baseline,*scripts,*ps,P/'analysis/recovered_routines.json',P/'project_status.json',P/'README.md',P/'SOURCE_HANDOFF.txt']})
    target=P/'analysis/player_movement_preservation.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('PRESERVED16629immutable files/1627registry rows/2771builds;',len(scripts),'Python and5PowerShell scripts parsed',flush=True)
if __name__=='__main__':main()
