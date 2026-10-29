"""Confirm prior published sources, original input and archived builds remain intact."""
import ast,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
G=r'C:\Program Files\Git\cmd\git.exe';BASE='9e3716d632dc95f2e62d4eef0777338eb0ee1286'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASE+':'+path])
def main():
    manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(manifest['files'])==16415
    mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
    for row in manifest['files']:
        if row['path'] not in mutable:
            path=ROOT/row['path'];assert path.stat().st_size==row['bytes'] and digest(path)==row['sha256'],row['path']
    for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(historical('outputs/sonic-adventure/'+name))
    old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'));now=json.loads((P/'analysis/recovered_routines.json').read_text());assert len(old['routines'])==1621 and now['routines'][:1621]==old['routines']
    for k,v in old.items():
        if k!='routines':assert now[k]==v,k
    old=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads((P/'project_status.json').read_text())
    for key,value in old.items():
        if key=='verified_progress':
            for field,data in value.items():
                if field!='recovered_buildable_routines':assert now[key][field]==data,field
        elif key!='recovered_buildable_routines':assert now[key]==value,key
    bp=P/'analysis/movement_contact_candidates_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2761
    for row in builds['files']:assert (ROOT/row['path']).stat().st_size==row['bytes'] and digest(ROOT/row['path'])==row['sha256'],row['path']
    raw=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';assert digest(raw)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    scripts=sorted(P.glob('*movement_contact_candidates*.py'))
    for path in scripts:ast.parse(path.read_text(),filename=str(path))
    ps=[P/'build_movement_contact_candidates.ps1',P/'ghidra/export_movement_contact_candidates.ps1']
    for path in ps:
        cmd="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(path)
        subprocess.run(['powershell.exe','-NoProfile','-Command',cmd],check=True)
    report=dict(status='previous_sources_original_inputs_builds_preserved',baseline_commit=BASE,preserved_previous_selected_files_outside_four_mutables=len(manifest['files'])-4,preserved_prior_registry_rows=1621,preserved_archived_builds=2761,original_input_sha256=digest(raw),goal_requirements_and_all_old_progress_fields_preserved=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',new_original_routine_credit=0,sha256={str(path):digest(path) for path in scripts+ps+[bp]})
    (P/'analysis/movement_contact_candidates_preservation.json').write_text(json.dumps(report,indent=2)+'\n');print('ORIENTATION PRESERVATION PASS16411oldfiles/1621oldregistryrows/2761oldbuilds')
if __name__=='__main__':main()
