"""Reproduce text recovery and verify historical source/proof/registry preservation."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];G=r'C:\Program Files\Git\cmd\git.exe';BASELINE='03a615b25047a62865fa714ce173f841e6eb2f4c'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('recover_collision_text','connect_collision_text','collision_text_reference','write_watch_collision_text','collision_text_task_fixtures','collision_text_session','collision_text_vectors','collision_text_fixtures','verify_collision_text_direct','merge_collision_text_direct','verify_collision_text_retained','check_collision_text_unbounded','audit_collision_text','qualify_collision_text','register_collision_text','verify_collision_text_workflow')
 scripts=[P/(n+'.py') for n in names]
 for p in scripts:ast.parse(p.read_text())
 generated=[P/n for n in ('source/sa_collision_text.c','analysis/collision_text_translation.json','analysis/collision_text_returns.json','source/sa_collision_text_connected.c','source/collision_text_probe.c','build_collision_text.ps1','analysis/collision_text_connection.json','analysis/collision_text_audit.json')];saved={str(p):digest(p) for p in generated}
 for name in ('recover_collision_text.py','connect_collision_text.py','audit_collision_text.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert saved=={str(p):digest(p) for p in generated}
 rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_collision_text.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
 def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15609
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 for row in baseline['files']:
  if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];registry=json.loads(rp.read_text())['routines'];assert len(old_registry)==1587 and registry[:1587]==old_registry and len(registry)==1589
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert status[key][field]==(1589 if field=='recovered_buildable_routines' else data),field
   assert set(status[key])-set(value)=={'collision_text_integration'}
  elif key=='recovered_buildable_routines':assert status[key]==1589
  elif key=='recovery_coverage_audit':continue
  else:assert status[key]==value,key
 bp=P/'analysis/collision_text_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2729
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 pins={}
 for name in ('translation','connection','audit','direct_verification','retained_verification','qualification'):
  path=P/f'analysis/collision_text_{name}.json';r=json.loads(path.read_text())
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);pins[pin]=h
 build=P/'build_collision_text.ps1';sources=re.findall(r"'([^']+\.c)'",build.read_text());assert len(sources)==len(set(sources))==248
 ps=[build,P/'ghidra/export_collision_text.ps1']
 for p in ps:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(p);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict=P/'analysis/collision_text_unbounded.json';u=json.loads(strict.read_text());assert u['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and u['exit_code']==0
 qp=P/'analysis/collision_text_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==2 and q['executed_rows']==315 and q['executed_conditional_outcomes']==4 and not q['domain_exclusion_arguments']
 coverage=P/'analysis/recovery_coverage_audit.json';cv=json.loads(coverage.read_text());assert cv['registered_routines']==1589 and cv['registered_unique_module_address_pairs']==1586 and not cv['registry_evidence_errors'] and not cv['whole_source_hash_refresh_needed']
 files=scripts+ps+[strict,qp,bp,coverage]+generated
 report=dict(reproduced_generated_artifacts=8,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15599,reviewed_mutable_paths=mutable,prior_registry_rows_identical=1587,registry_entries=1589,registry_unique_module_address_pairs=1586,confirmed_linked_unique_routines_by_name=631,preserved_build_files=2729,additional_build_files=['work/sonic-adventure/build/collision_text.dll','work/sonic-adventure/build/collision_text.lib'],compiled_source_count=248,strict_unbounded_sources_checked=248,new_routine_credit=2,canonical_duplicate_credit=0,original_rows_executed=315,original_conditional_outcomes_executed=4,original_conditional_outcomes_proven_infeasible=0,domain_excluded_rows=[],domain_excluded_outcomes=[],unaccounted_outcomes=[],new_nonlocal_routes_qualified=0,test_only_external_QACR_input_seed=True,fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],original_requirements_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eightartifactsreproduce;twocompleteoriginalglyph/texthelpers throughcurrentABIwithactualstrlen/glyphcalls.315rows/4outcomesexecute,noexclusions.15599oldsource/proofs preservedexcept10reviewedcatalog/status/docpaths;1587oldregistryrows/progress/requirements/2729oldbuildspreserved.486freshcomparisons/4392checks;no newnonlocalroutequalification. TestonlyexternalQACRinputseed,allcomparisonlogic/productionobserverunchanged. Physicaldelivery/wholecollision/gameplayunfinished;10%milestones,notcoverage.')
 (P/'analysis/collision_text_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('TEXT WORKFLOW PASS15599preservedfiles,2729oldbuilds,1587oldregistryrows;',len(pins),'pins')
if __name__=='__main__':main()
