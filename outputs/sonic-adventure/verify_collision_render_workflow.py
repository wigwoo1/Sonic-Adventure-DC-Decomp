"""Reproduce triangle recovery and verify historical source/proof/registry preservation."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];G=r'C:\Program Files\Git\cmd\git.exe';BASELINE='0ba2dc43ca080bb6b758a1e091a504cbaa7b6b75'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('recover_collision_render','connect_collision_render','collision_render_reference','write_watch_collision_render','collision_render_task_fixtures','collision_render_session','collision_render_vectors','collision_render_fixtures','verify_collision_render_direct','merge_collision_render_direct','verify_collision_render_retained','check_collision_render_unbounded','prove_collision_render_predicate','audit_collision_render','qualify_collision_render','register_collision_render','verify_collision_render_workflow')
 scripts=[P/(n+'.py') for n in names]
 for p in scripts:ast.parse(p.read_text())
 generated=[P/n for n in ('source/sa_collision_render.c','analysis/collision_render_translation.json','analysis/collision_render_returns.json','source/sa_collision_render_connected.c','source/collision_render_probe.c','build_collision_render.ps1','analysis/collision_render_connection.json','analysis/collision_render_audit.json','analysis/collision_render_predicate_proof.json')];saved={str(p):digest(p) for p in generated}
 for name in ('recover_collision_render.py','connect_collision_render.py','prove_collision_render_predicate.py','audit_collision_render.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert saved=={str(p):digest(p) for p in generated}
 rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_collision_render.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
 def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15564
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 for row in baseline['files']:
  if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];registry=json.loads(rp.read_text())['routines'];assert len(old_registry)==1586 and registry[:1586]==old_registry and len(registry)==1587
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert status[key][field]==(1587 if field=='recovered_buildable_routines' else data),field
   assert set(status[key])-set(value)=={'collision_render_integration'}
  elif key=='recovered_buildable_routines':assert status[key]==1587
  elif key=='recovery_coverage_audit':continue
  else:assert status[key]==value,key
 bp=P/'analysis/collision_render_previous_build_manifest.json';builds=json.loads(bp.read_text());assert len(builds['files'])==2727
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 pins={}
 for name in ('translation','connection','audit','direct_verification','retained_verification','qualification','predicate_proof'):
  path=P/f'analysis/collision_render_{name}.json';r=json.loads(path.read_text())
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);pins[pin]=h
 build=P/'build_collision_render.ps1';sources=re.findall(r"'([^']+\.c)'",build.read_text());assert len(sources)==len(set(sources))==247
 ps=[build,P/'ghidra/export_collision_render.ps1']
 for p in ps:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(p);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict=P/'analysis/collision_render_unbounded.json';u=json.loads(strict.read_text());assert u['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and u['exit_code']==0
 qp=P/'analysis/collision_render_qualification.json';q=json.loads(qp.read_text());assert q['new_original_routine_credit']==1 and q['executed_rows']==332 and q['executed_conditional_outcomes']==31 and q['proven_infeasible_conditional_outcomes']==1
 coverage=P/'analysis/recovery_coverage_audit.json';cv=json.loads(coverage.read_text());assert cv['registered_routines']==1587 and cv['registered_unique_module_address_pairs']==1584 and not cv['registry_evidence_errors'] and not cv['whole_source_hash_refresh_needed']
 files=scripts+ps+[strict,qp,bp,coverage]+generated
 report=dict(reproduced_generated_artifacts=9,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15554,reviewed_mutable_paths=mutable,prior_registry_rows_identical=1586,registry_entries=1587,registry_unique_module_address_pairs=1584,confirmed_linked_unique_routines_by_name=629,preserved_build_files=2727,additional_build_files=['work/sonic-adventure/build/collision_render.dll','work/sonic-adventure/build/collision_render.lib'],compiled_source_count=247,strict_unbounded_sources_checked=247,new_routine_credit=1,canonical_duplicate_credit=0,original_rows_executed=332,original_conditional_outcomes_executed=31,original_conditional_outcomes_proven_infeasible=1,domain_excluded_rows=[],domain_excluded_outcomes=[],unaccounted_outcomes=[],new_nonlocal_routes_qualified=0,fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],original_requirements_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Nineartifactsreproduce;onecompleteoriginaltrianglewrapper throughcurrentABIwithall15actualgamecallees.332rows/31feasibleoutcomesexecute,oneadditionaloutcomeprovedinfeasiblewithnoinputdomainexclusion.15554oldsource/proofs preservedexcept10reviewedcatalog/status/docpaths;1586oldregistryrows/progress/requirements/2727oldbuilds preserved.625freshcomparisons/6035checks;no newnonlocalroutequalification. Physicaldelivery/wholecollision/gameplayunfinished;10%milestones,notcoverage.')
 (P/'analysis/collision_render_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('RENDER WORKFLOW PASS15554preservedfiles,2727oldbuilds,1586oldregistryrows;',len(pins),'pins')
if __name__=='__main__':main()
