"""Reproduce inverse integration and preserve historical source/proof/registry/build bytes."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];G=r'C:\Program Files\Git\cmd\git.exe';BASELINE='dff3abb2438c6b090b091ed2726152b5db80247b'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('recover_collision_inverse','connect_collision_inverse','collision_inverse_reference','write_watch_collision_inverse','collision_inverse_task_fixtures','collision_inverse_session','collision_inverse_vectors','verify_collision_inverse_direct','merge_collision_inverse_direct','verify_collision_inverse_retained','check_collision_inverse_unbounded','audit_collision_inverse','qualify_collision_inverse','record_collision_inverse','verify_collision_inverse_workflow')
 scripts=[P/(n+'.py') for n in names]
 for p in scripts:ast.parse(p.read_text())
 generated=[P/n for n in ('source/sa_collision_inverse.c','analysis/collision_inverse_translation.json','analysis/collision_inverse_returns.json','source/sa_collision_inverse_connected.c','source/collision_inverse_probe.c','build_collision_inverse.ps1','analysis/collision_inverse_connection.json','analysis/collision_inverse_audit.json')];saved={str(p):digest(p) for p in generated}
 for name in ('recover_collision_inverse.py','connect_collision_inverse.py','audit_collision_inverse.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert saved=={str(p):digest(p) for p in generated}
 sp=P/'project_status.json';before=sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'record_collision_inverse.py')],check=True);assert sp.read_bytes()==before
 def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15520
 mutable=[r['path'] for r in baseline['files'] if r['path'] in {'outputs/sonic-adventure/README.md','outputs/sonic-adventure/SOURCE_HANDOFF.txt','outputs/sonic-adventure/project_status.json'}];assert len(mutable)==3
 for row in baseline['files']:
  if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 assert (P/'analysis/recovered_routines.json').read_bytes()==historical('outputs/sonic-adventure/analysis/recovered_routines.json')
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert status[key][field]==data,field
   assert set(status[key])-set(value)=={'collision_inverse_integration'}
  else:assert status[key]==value,key
 bp=P/'analysis/collision_inverse_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2725
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 pins={}
 for name in ('translation','connection','audit','direct_verification','retained_verification','qualification'):
  path=P/f'analysis/collision_inverse_{name}.json';r=json.loads(path.read_text())
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);pins[pin]=h
 build=P/'build_collision_inverse.ps1';sources=re.findall(r"'([^']+\.c)'",build.read_text());assert len(sources)==len(set(sources))==246
 ps=[build,P/'ghidra/export_collision_inverse.ps1']
 for p in ps:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(p);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict=P/'analysis/collision_inverse_unbounded.json';u=json.loads(strict.read_text());assert u['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and u['exit_code']==0
 qp=P/'analysis/collision_inverse_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==0 and q['new_direct_entry_routes_qualified']==2 and q['original_canonical_rows_executed']==1191 and q['qualified_conditional_outcomes']==8
 files=scripts+ps+[strict,qp,bp]+generated
 report=dict(reproduced_generated_artifacts=8,status_recording_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15517,reviewed_mutable_paths=mutable,prior_registry_bytes_identical=True,registry_entries=1586,registry_unique_module_address_pairs=1583,confirmed_linked_unique_routines_by_name=628,preserved_build_files=2725,additional_build_files=['work/sonic-adventure/build/collision_inverse.dll','work/sonic-adventure/build/collision_inverse.lib'],compiled_source_count=246,strict_unbounded_sources_checked=246,new_routine_credit=0,canonical_duplicate_credit=0,canonical_adapter_rows=1191,new_direct_routes_qualified=2,original_canonical_rows_executed=1191,qualified_original_conditional_outcomes=8,new_nonlocal_routes_qualified=0,fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],original_requirements_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eightartifacts reproduce;two canonical matrix dependencies through currentABI with actual determinant call.1191rows/8outcomes execute,no exclusions/duplicatecredit.15517oldsource/proofs preserved except3docs/statuspaths;registrybytes/oldprogress/requirements/2725oldbuilds unchanged,newDLL/libadditional.867freshcomparisons/7545checks;no newnonlocalroutequalification. Physicaldelivery/wholecollision/gameplayunfinished;10%milestones,notcoverage.')
 (P/'analysis/collision_inverse_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INVERSE WORKFLOW PASS15517preservedfiles,2725oldbuilds,registryunchanged;',len(pins),'pins')
if __name__=='__main__':main()
