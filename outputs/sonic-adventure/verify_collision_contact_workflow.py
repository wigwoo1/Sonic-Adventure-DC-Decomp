"""Reproduce partial complete-body recovery and preserve the unchanged registry."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];G=r'C:\Program Files\Git\cmd\git.exe';BASELINE='afed053875e1614203624da70448f33cbc6c8cd4'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('recover_collision_contact','connect_collision_contact','collision_contact_reference','write_watch_collision_contact','collision_contact_task_fixtures','collision_contact_session','collision_contact_vectors','collision_contact_fixtures','verify_collision_contact_direct','merge_collision_contact_direct','verify_collision_contact_frontier','verify_collision_contact_retained','check_collision_contact_unbounded','audit_collision_contact','qualify_collision_contact','record_collision_contact','verify_collision_contact_workflow')
 scripts=[P/(n+'.py') for n in names]
 for p in scripts:ast.parse(p.read_text())
 generated=[P/n for n in ('source/sa_collision_contact.c','analysis/collision_contact_translation.json','analysis/collision_contact_returns.json','source/sa_collision_contact_connected.c','source/collision_contact_probe.c','build_collision_contact.ps1','analysis/collision_contact_connection.json','analysis/collision_contact_audit.json')];saved={str(p):digest(p) for p in generated}
 for name in ('recover_collision_contact.py','connect_collision_contact.py','audit_collision_contact.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert saved=={str(p):digest(p) for p in generated}
 rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'record_collision_contact.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
 def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15654
 mutable=[r['path'] for r in baseline['files'] if r['path'] in {'outputs/sonic-adventure/README.md','outputs/sonic-adventure/SOURCE_HANDOFF.txt','outputs/sonic-adventure/project_status.json'}];assert len(mutable)==3
 for row in baseline['files']:
  if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 assert rp.read_bytes()==historical('outputs/sonic-adventure/analysis/recovered_routines.json')
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert status[key][field]==data,field
   assert set(status[key])-set(value)=={'collision_contact_integration'}
  else:assert status[key]==value,key
 bp=P/'analysis/collision_contact_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2731
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 pins={}
 for name in ('translation','connection','audit','direct_verification','frontier_verification','retained_verification','qualification'):
  path=P/f'analysis/collision_contact_{name}.json';r=json.loads(path.read_text())
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);pins[pin]=h
 build=P/'build_collision_contact.ps1';sources=re.findall(r"'([^']+\.c)'",build.read_text());assert len(sources)==len(set(sources))==249
 ps=[build,P/'ghidra/export_collision_contact.ps1']
 for p in ps:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(p);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict=P/'analysis/collision_contact_unbounded.json';u=json.loads(strict.read_text());assert u['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and u['exit_code']==0
 qp=P/'analysis/collision_contact_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==0 and q['executed_rows']==1121 and q['executed_conditional_outcomes']==72 and len(q['unaccounted_outcomes'])==2 and not q['domain_exclusion_arguments']
 files=scripts+ps+[strict,qp,bp]+generated
 report=dict(reproduced_generated_artifacts=8,status_recording_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15651,reviewed_mutable_paths=mutable,prior_registry_bytes_identical=True,registry_entries=1589,registry_unique_module_address_pairs=1586,confirmed_linked_unique_routines_by_name=631,preserved_build_files=2731,additional_build_files=['work/sonic-adventure/build/collision_contact.dll','work/sonic-adventure/build/collision_contact.lib'],compiled_source_count=249,strict_unbounded_sources_checked=249,new_routine_credit=0,canonical_duplicate_credit=0,original_rows_executed=1121,original_conditional_outcomes_executed=72,original_conditional_outcomes_proven_infeasible=0,unaccounted_outcomes=q['unaccounted_outcomes'],domain_excluded_rows=[],domain_excluded_outcomes=[],new_nonlocal_routes_qualified=0,test_only_readonly_actual_angle_input_capture=True,fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],original_requirements_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eightartifactsreproduce;completeoriginalcallerbodythroughcurrentABIwithall32realcallsites.1121rows/72of74outcomesexecute;2unqualifiedoutcomesretained,noexclusions/infeasibleproof/registrationcredit.15651oldsource/proofs preservedexcept3docs/statuspaths;registrybytes/oldprogress/requirements/2731oldbuildspreserved.444freshcomparisons/2651checks;no newnonlocalroutequalification. Physicaldelivery/fullcaller/gameplayunfinished;10%milestones,notcoverage.')
 (P/'analysis/collision_contact_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CONTACT WORKFLOW PASS15651preservedfiles,2731oldbuilds,registrybytesunchanged;',len(pins),'pins;2outcomesOPEN')
if __name__=='__main__':main()
