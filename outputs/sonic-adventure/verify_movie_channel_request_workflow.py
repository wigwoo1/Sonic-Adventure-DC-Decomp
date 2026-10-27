"""Reproducible new artifacts and preservation of published source/proofs/builds."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='e85fc8a1b6f9f577689ad20237a806b22ea8347d'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/n for n in ('source/sa_movie_channel_request.c','analysis/movie_channel_request_translation.json','analysis/movie_channel_request_returns.json','source/sa_movie_channel_request_connected.c','source/movie_channel_request_probe.c','build_movie_channel_request.ps1','analysis/movie_channel_request_connection.json','analysis/movie_channel_request_audit.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_channel_request.py','connect_movie_channel_request.py','audit_movie_channel_request.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_channel_request.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
 names=('recover_movie_channel_request','connect_movie_channel_request','channel_request_reference_cpu','movie_channel_request_reference','write_watch_channel_request','movie_channel_request_fixtures','movie_channel_request_task_fixtures','check_movie_channel_request_unbounded','verify_movie_channel_request','verify_movie_channel_request_api','verify_movie_channel_request_tasks','verify_movie_channel_request_retained','audit_movie_channel_request','qualify_movie_channel_request','register_movie_channel_request','verify_movie_channel_request_workflow')
 scripts=[P/(n+'.py') for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('translation','connection','verification','api_verification','tasks_verification','retained_verification','audit','qualification'):
  r=json.loads((P/f'analysis/movie_channel_request_{name}.json').read_text())
  for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==14100
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==14090
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now=json.loads(rp.read_text())['routines'];assert len(old_registry)==1489 and now[:1489]==old_registry and len(now)==1493
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
 for key,value in old_status['verified_progress'].items():assert status['verified_progress'][key]==(1493 if key=='recovered_buildable_routines' else value),key
 previous=json.loads((P/'analysis/movie_channel_request_previous_build_manifest.json').read_text());assert len(previous['files'])==2675
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_channel_request.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==222
 powershell=[ps,P/'ghidra/export_movie_channel_request_report.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict=P/'analysis/movie_channel_request_unbounded.json';receipt=json.loads(strict.read_text());assert receipt['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and receipt['exit_code']==0
 coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed'] and coverage['registered_routines']==1493
 q=json.loads((P/'analysis/movie_channel_request_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['fresh_original_native_comparisons'],q['fresh_independent_checks'])==(4,89,4,1771,23451)
 files=scripts+powershell+[strict,P/'analysis/movie_channel_request_qualification.json']
 report=dict(reproduced_generated_artifacts=8,registration_byte_idempotent=True,prior_registry_rows_identical=1489,registry_entries=1493,new_routine_credit=4,qualified_new_rows=89,qualified_new_outcomes=4,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),prior_runtime_ABI_SRFD_guards_preserved=True,compiled_source_count=222,strict_unbounded_sources_checked=222,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2675,additional_build_files=['work/sonic-adventure/build/movie_channel_request.dll','work/sonic-adventure/build/movie_channel_request.lib'],fresh_original_native_comparisons=1771,fresh_independent_checks=23451,total_evidence_original_native_comparisons=14626,canonical_duplicate_credit=0,physical_response_complete=False,successful_active_parser_complete=False,arbitrary_resume_graphs_complete=False,overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Eightartifacts reproduce;registrationidempotent;222strictunbounded Csourcechecks. All14090priorselectedsource/proofs and2675priorbuildsbytepreservedexcept10reviewedcatalog/status/docpaths. Prior1489registryrowsunchangedplusfour89rowrequests. Actualpendingtask/packet/diagnosticbehavior scoped;canonicalpump/preflightalreadyregisteredbutnotyetintegrated. Goalactive10%.')
 (P/'analysis/movie_channel_request_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('REQUEST WORKFLOW PASS',preserved,'preservedsource/proofs;',len(pins),'pins')
if __name__=='__main__':main()
