"""Preserve prior published bytes outside four authorized status files."""
import ast,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    target=P/'analysis/player_response_clip_preservation.json';assert not target.exists()
    baseline=W/'player_response_clip_prior_source_manifest.json';prior=json.loads(baseline.read_text())['files'];assert len(prior)==16731
    mutable={'outputs/sonic-adventure/'+x for x in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in prior:
        if row['path'] not in mutable:assert sha(R/row['path'])==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith((D/'outputs/sonic-adventure'/name).read_bytes())
    old=json.loads((D/'outputs/sonic-adventure/analysis/recovered_routines.json').read_text())['routines'];new=json.loads((P/'analysis/recovered_routines.json').read_text())['routines']
    assert len(old)==1631 and len(new)==1632 and new[:1631]==old
    before=json.loads((D/'outputs/sonic-adventure/project_status.json').read_text());after=json.loads((P/'project_status.json').read_text())
    del after['verified_progress']['player_response_clip_integration']
    after['recovered_buildable_routines']=before['recovered_buildable_routines'];after['verified_progress']['recovered_buildable_routines']=before['verified_progress']['recovered_buildable_routines'];assert after==before
    builds=json.loads((W/'player_response_clip_prior_build_manifest.json').read_text())['files'];assert len(builds)==2779
    for row in builds:assert sha(R/row['path'])==row['sha256'],row['path']
    scripts=sorted(P.glob('*player_response_clip*.py'))
    for path in scripts:ast.parse(path.read_text(encoding='utf-8-sig'),filename=str(path))
    ps=[P/'build_player_response_clip.ps1',P/'ghidra/export_player_response_clip.ps1']
    for path in ps:
        code="$tokens=$null;$errors=$null;[System.Management.Automation.Language.Parser]::ParseFile('"+str(path).replace("'","''")+"',[ref]$tokens,[ref]$errors)|Out-Null;if($errors.Count){throw ($errors|Out-String)}"
        subprocess.run(['powershell','-NoProfile','-Command',code],check=True,capture_output=True)
    q=json.loads((P/'analysis/player_response_clip_qualification.json').read_text())
    for path,expected in q['sha256'].items():assert sha(path)==expected,path
    result=dict(status='prior_immutable_source_proofs_registry_and_builds_preserved',preserved_prior_immutable_selected_files=16727,preserved_prior_registry_rows=1631,current_registry_entries=1632,preserved_prior_builds=2779,python_scripts_parsed=len(scripts),powershell_scripts_parsed=2,overall_completion_percent=10,goal_paused=False,
        sha256={str(p):sha(p) for p in [Path(__file__),baseline,*scripts,*ps,P/'analysis/recovered_routines.json',P/'project_status.json',P/'README.md',P/'SOURCE_HANDOFF.txt']})
    target.write_text(json.dumps(result,indent=2)+'\n');print('PRESERVED16727 immutable files/1631 registry rows/2779 builds;',len(scripts),'Python/2PowerShell scripts parsed',flush=True)
if __name__=='__main__':main()
