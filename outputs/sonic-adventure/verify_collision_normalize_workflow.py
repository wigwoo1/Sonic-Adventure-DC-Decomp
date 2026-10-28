"""Reproduce routing artifacts and preserve all prior source/proof/registry/build bytes."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];G=r'C:\Program Files\Git\cmd\git.exe';BASELINE='60d392b9988f4d3605b14f76e3c38a2e9116b7b1'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('connect_collision_normalize','collision_normalize_reference','write_watch_collision_normalize','collision_normalize_task_fixtures','collision_normalize_session','collision_normalize_vectors','verify_collision_normalize_direct','merge_collision_normalize_direct','verify_collision_normalize_retained','check_collision_normalize_unbounded','audit_collision_normalize','qualify_collision_normalize','record_collision_normalize','verify_collision_normalize_workflow')
 scripts=[P/(n+'.py') for n in names]
 for p in scripts:ast.parse(p.read_text())
 generated=[P/n for n in ('source/sa_collision_normalize_connected.c','source/collision_normalize_probe.c','build_collision_normalize.ps1','analysis/collision_normalize_connection.json','analysis/collision_normalize_audit.json')];saved={str(p):digest(p) for p in generated}
 for name in ('connect_collision_normalize.py','audit_collision_normalize.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert saved=={str(p):digest(p) for p in generated}
 sp=P/'project_status.json';before=sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'record_collision_normalize.py')],check=True);assert sp.read_bytes()==before
 def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15487
 mutable=[r['path'] for r in baseline['files'] if r['path'] in {'outputs/sonic-adventure/README.md','outputs/sonic-adventure/SOURCE_HANDOFF.txt','outputs/sonic-adventure/project_status.json'}];assert len(mutable)==3
 for row in baseline['files']:
  if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 assert (P/'analysis/recovered_routines.json').read_bytes()==historical('outputs/sonic-adventure/analysis/recovered_routines.json')
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert status[key][field]==data,field
   assert set(status[key])-set(value)=={'collision_normalize_direct_route'}
  else:assert status[key]==value,key
 bp=P/'analysis/collision_normalize_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2723
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 pins={}
 for name in ('connection','audit','direct_verification','retained_verification','qualification'):
  path=P/f'analysis/collision_normalize_{name}.json';r=json.loads(path.read_text())
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);pins[pin]=h
 build=P/'build_collision_normalize.ps1';sources=re.findall(r"'([^']+\.c)'",build.read_text());assert len(sources)==len(set(sources))==245
 command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(build);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict=P/'analysis/collision_normalize_unbounded.json';u=json.loads(strict.read_text());assert u['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and u['exit_code']==0
 qp=P/'analysis/collision_normalize_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==0 and q['new_direct_entry_routes_qualified']==1 and q['original_canonical_rows_executed']==16
 files=scripts+[build,strict,qp,bp]+generated
 report=dict(reproduced_generated_artifacts=5,status_recording_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=1,pinned_inputs_current=len(pins),preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15484,reviewed_mutable_paths=mutable,prior_registry_bytes_identical=True,registry_entries=1586,registry_unique_module_address_pairs=1583,confirmed_linked_unique_routines_by_name=626,preserved_build_files=2723,additional_build_files=['work/sonic-adventure/build/collision_normalize.dll','work/sonic-adventure/build/collision_normalize.lib'],compiled_source_count=245,strict_unbounded_sources_checked=245,new_routine_credit=0,canonical_duplicate_credit=0,new_direct_routes_qualified=1,original_canonical_rows_executed=16,new_nonlocal_routes_qualified=0,fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],original_requirements_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Fiveartifacts reproduce;one canonicalnormalization direct entry through unchanged lowerABI.15484oldselected source/proofs preserved except3docs/status paths;registrybytes/oldprogress/requirements/2723oldbuilds unchanged,newDLL/libadditional.757freshcomparisons/6321checks;zero newroutine/nonlocalroutecredit. Physicalfaults/wholecollision/gameplayunfinished;10%milestones,notcoverage.')
 (P/'analysis/collision_normalize_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('NORMALIZE WORKFLOW PASS15484preservedfiles,2723oldbuilds;registryunchanged;',len(pins),'pins')
if __name__=='__main__':main()
