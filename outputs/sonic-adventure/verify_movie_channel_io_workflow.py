"""Reproduce continuation artifacts; preserve prior source/proof/build evidence."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='017c800b4ddb6d251905c27d9618eaca0b04c717'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_channel_io.c','source/sa_movie_channel_io_decoder.c','source/sa_movie_channel_io_worker.c','analysis/movie_channel_io_translation.json','analysis/movie_channel_io_returns.json','source/sa_movie_channel_io_connected.c','source/movie_channel_io_probe.c','build_movie_channel_io.ps1','analysis/movie_channel_io_connection.json','analysis/movie_channel_io_contract_audit.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_channel_io.py','connect_movie_channel_io.py','audit_movie_channel_io.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_channel_io.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
 scripts=[P/n for n in ('channel_io_continuations.py','recover_movie_channel_io.py','connect_movie_channel_io.py','movie_channel_io_reference.py','write_watch_channel_io.py','movie_channel_io_fixtures.py','verify_movie_channel_io_units.py','verify_movie_channel_io_nested.py','verify_movie_channel_io_prefixes.py','verify_movie_channel_io_ordinary.py','verify_movie_channel_io_worker.py','verify_movie_channel_io_irq.py','verify_movie_channel_io_retained.py','audit_movie_channel_io.py','qualify_movie_channel_io.py','register_movie_channel_io.py','verify_movie_channel_io_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('translation','connection','units_verification','nested_verification','prefixes_verification','ordinary_regression','worker_regression','irq_verification','retained_verification','contract_audit','qualification'):
  report=json.loads((P/f'analysis/movie_channel_io_{name}.json').read_text())
  for p,h in report['sha256'].items():assert digest(p)==h,(name,p);pins[p]=h
 import movie_channel_io_reference as v
 import write_watch_channel_io as fast
 assert v.code==v.old.code
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY'):assert v.namespace[name]==v.old.namespace[name]
 assert all(getattr(v,n)==value for n,value in v.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 returns=json.loads((P/'analysis/movie_channel_io_returns.json').read_text());assert set(map(lambda x:int(x,16),returns['allowed_call_returns']))==v.namespace['RETURNS']
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==14021
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==14011
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now=json.loads(rp.read_text())['routines'];assert len(old_registry)==1488 and now[:1488]==old_registry and len(now)==1489
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert status['verified_progress'][field]==(1489 if field=='recovered_buildable_routines' else value),field
 previous=json.loads((ROOT/'work/sonic-adventure/channel_io_previous_build_manifest.json').read_text());assert len(previous['files'])==2673
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_channel_io.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==221
 powershell=[ps]+[P/'ghidra'/n for n in ('export_movie_channel_decoder_io.ps1','export_movie_channel_io_backend.ps1','export_movie_channel_io_status.ps1','export_movie_channel_io_request.ps1','export_movie_channel_io_request_lower.ps1')]
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/channel_io_unbounded_receipt.json';strict=json.loads(strict_path.read_text());assert strict['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and strict['exit_code']==0 and strict['flags']==['-fsyntax-only','-Wall','-Wextra','-Werror']
 q=json.loads((P/'analysis/movie_channel_io_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['fresh_original_native_comparisons'],q['fresh_independent_checks'])==(1,38,6,1139,10295)
 coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed'] and coverage['registered_routines']==1489
 files=scripts+powershell+[strict_path,P/'analysis/movie_channel_io_qualification.json']
 report=dict(reproduced_generated_artifacts=10,registration_byte_idempotent=True,prior_registry_rows_identical=1488,registry_entries=1489,new_routine_credit=1,qualified_new_rows=38,qualified_new_outcomes=6,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),original_model_execution_code_identical=True,inherited_SR_FD_guards_preserved=True,external_ABI_argument_layouts_preserved=True,internal_root_pr_host_metadata=True,original_decoder_worker_source_files_preserved=True,compiled_decoder_worker_forks_add_explicit_continuations=True,guard_membership_equals_flat_model_returns=True,explicit_resume_routes=82,behavior_qualified_nonlocal_routes=6,compiled_source_count=221,strict_unbounded_sources_checked=221,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2673,additional_build_files=['work/sonic-adventure/build/movie_channel_io.dll','work/sonic-adventure/build/movie_channel_io.lib'],fresh_original_native_comparisons=1139,fresh_independent_checks=10295,total_evidence_original_native_comparisons=12855,canonical_duplicate_credit=0,active_parser_complete=False,arbitrary_resume_graphs_complete=False,overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Tenartifacts reproducible;registrationidempotent;221strictunbounded Csourcescompile. Prior14011source/prooffilesand2673buildspreserved;10reviewedcatalog/status/docpathsupdated. Old1488registryrows unchangedplusreadiness38/6. Originalmodel/SRFD/runtime/externalABI unchanged;internalhostrootPR/exactresumedreturnguardand82explicitcontinuationroutes documented,sixqualifiedbyactualnestedfailurelifecycles. Sourceforksnotrelabelledasidenticaloldcompiledcode. No fakeparserresults/taskrepairs. Historicalstartupnotfresh;goalactive10%.')
 (P/'analysis/movie_channel_io_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('IO WORKFLOW PASS',preserved,'preservedsource/proofs;',len(pins),'pins')
if __name__=='__main__':main()
