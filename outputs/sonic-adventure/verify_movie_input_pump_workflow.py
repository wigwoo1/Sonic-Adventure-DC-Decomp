"""Reproduce new artifacts and preserve the published baseline's source/proofs/builds."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='1987edb6b6e3407bcbd49636cda61e405063556f'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/n for n in ('source/sa_movie_input_pump.c','analysis/movie_input_pump_translation.json','analysis/movie_input_pump_returns.json','source/sa_movie_input_pump_connected.c','source/movie_input_pump_probe.c','build_movie_input_pump.ps1','analysis/movie_input_pump_connection.json','analysis/movie_input_pump_audit.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_input_pump.py','connect_movie_input_pump.py','audit_movie_input_pump.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_input_pump.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
 names=('recover_movie_input_pump','connect_movie_input_pump','input_pump_reference_cpu','movie_input_pump_reference','write_watch_input_pump','movie_input_pump_fixtures','movie_input_pump_task_fixtures','check_movie_input_pump_unbounded','verify_movie_input_pump_leaves','verify_movie_input_pump_responses','verify_movie_input_pump_controls','verify_movie_input_pump_notifications','verify_movie_input_pump_subdevices','verify_movie_input_pump_subdevice_edges','verify_movie_input_pump_api','verify_movie_input_pump_swaps','verify_movie_input_pump_publication','verify_movie_input_pump_retained','audit_movie_input_pump','qualify_movie_input_pump','register_movie_input_pump','verify_movie_input_pump_workflow')
 scripts=[P/(n+'.py') for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={};reports=('translation','connection','leaves_verification','responses_verification','controls_verification','notifications_verification','subdevices_verification','subdevice_edges_verification','api_verification','swaps_verification','publication_verification','retained_verification','audit','qualification')
 for name in reports:
  r=json.loads((P/f'analysis/movie_input_pump_{name}.json').read_text())
  for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 # The two bookkeeping corrections are auditable without pretending a test rerun.
 for filename in ('movie_input_pump_metadata_correction.json','movie_input_pump_catalog_baseline_correction.json'):
  receipt=json.loads((P/'analysis'/filename).read_text());assert receipt['native_source_bridge_probe_build_returns_byte_identical'] and receipt['all_behavioral_case_records_unchanged']
  for original,row in receipt['preserved_before_files'].items():
   prior=P/row['preserved_copy'];assert digest(prior)==row['before_sha256']==row['copy_sha256']
   current=Path(original)
   if current.suffix=='.json':
    a=json.loads(prior.read_text());b=json.loads(current.read_text());strip=lambda r:{k:v for k,v in r.items() if k not in ('scope','sha256')};assert strip(a)==strip(b),original
   elif current.suffix in ('.c','.ps1'):assert digest(current)==row['before_sha256'],original
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==14144
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==14134
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now=json.loads(rp.read_text())['routines'];assert len(old_registry)==1493 and now[:1493]==old_registry and len(now)==1502
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
 for key,value in old_status['verified_progress'].items():assert status['verified_progress'][key]==(1502 if key=='recovered_buildable_routines' else value),key
 previous=json.loads((P/'analysis/movie_input_pump_previous_build_manifest.json').read_text());assert len(previous['files'])==2677
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_input_pump.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==223
 powershell=[ps,P/'ghidra/export_movie_input_pump_canonical.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict=P/'analysis/movie_input_pump_unbounded.json';receipt=json.loads(strict.read_text());assert receipt['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and receipt['exit_code']==0
 coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed'] and coverage['registered_routines']==1502
 q=json.loads((P/'analysis/movie_input_pump_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['fresh_original_native_comparisons'],q['fresh_independent_checks'])==(9,706,95,2601,36399)
 files=scripts+powershell+[strict,P/'analysis/movie_input_pump_qualification.json']
 report=dict(reproduced_generated_artifacts=8,registration_byte_idempotent=True,prior_registry_rows_identical=1493,registry_entries=1502,new_routine_credit=9,qualified_new_rows=706,qualified_new_outcomes=95,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),prior_runtime_ABI_SRFD_guards_preserved=True,original_flat_execution_code_unchanged=True,word_loop_observation_only_checked=True,metadata_corrections_preserve_all_native_and_behavior_bytes=True,compiled_source_count=223,strict_unbounded_sources_checked=223,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2677,additional_build_files=['work/sonic-adventure/build/movie_input_pump.dll','work/sonic-adventure/build/movie_input_pump.lib'],fresh_original_native_comparisons=2601,fresh_independent_checks=36399,total_evidence_original_native_comparisons=17227,canonical_duplicate_credit=0,physical_submission_complete=False,natural_response_to_task_parser_complete=False,arbitrary_resume_graphs_complete=False,overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eightartifactsreproduceafterregistrationwithfrozencanonicalbaseline;223strictCsources/22Python/2PS parse. All14134priorselectedsource/proofs and2677buildspreservedexcept10reviewedcatalog/status/docpaths. Actualresponse/pump/API/callbackbehavior scoped;canonicalsubmit65efda/65eeda remainsnextintegration. Metadata-only corrections preservepreviousfiles/hashes andallbehavioralrecords. Goalactive10%.')
 (P/'analysis/movie_input_pump_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP WORKFLOW PASS',preserved,'preservedsource/proofs;',len(pins),'pins')
if __name__=='__main__':main()
