"""Reproduce decoder artifacts and preserve prior source/proof/build contracts."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='e3b0c4de0c0b6e1ac847ddc30afd6fa78adf9fe9'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_channel_decoder.c','analysis/movie_channel_decoder_translation.json','source/sa_movie_channel_decoder_connected.c','source/movie_channel_decoder_probe.c','build_movie_channel_decoder.ps1','analysis/movie_channel_decoder_connection.json','analysis/movie_channel_decoder_contract_audit.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_channel_decoder.py','connect_movie_channel_decoder.py','audit_movie_channel_decoder.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 rp=P/'analysis/recovered_routines.json';sp=P/'project_status.json';saved=rp.read_bytes(),sp.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_channel_decoder.py')],check=True);assert saved==(rp.read_bytes(),sp.read_bytes())
 scripts=[P/n for n in ('recover_movie_channel_decoder.py','connect_movie_channel_decoder.py','movie_channel_decoder_reference.py','write_watch_channel_decoder.py','verify_movie_channel_decoder_leaves.py','verify_movie_channel_decoder_paths.py','verify_movie_channel_decoder_callers.py','verify_movie_channel_decoder_worker.py','verify_movie_channel_decoder_irq.py','verify_movie_channel_decoder_retained.py','audit_movie_channel_decoder.py','qualify_movie_channel_decoder.py','register_movie_channel_decoder.py','verify_movie_channel_decoder_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('translation','connection','leaves_verification','paths_verification','callers_verification','worker_regression','irq_verification','retained_verification','contract_audit','qualification'):
  report=json.loads((P/f'analysis/movie_channel_decoder_{name}.json').read_text())
  for p,h in report['sha256'].items():assert digest(p)==h,(name,p);pins[p]=h
 import movie_channel_decoder_reference as v
 import write_watch_channel_decoder as fast
 assert v.code==v.old.code
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY'):assert v.namespace[name]==v.old.namespace[name]
 assert all(getattr(v,n)==value for n,value in v.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==13969
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13959
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];now_registry=json.loads(rp.read_text())['routines'];assert len(old_registry)==1485 and now_registry[:1485]==old_registry and len(now_registry)==1488
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text());assert status['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert status['verified_progress'][field]==(1488 if field=='recovered_buildable_routines' else value),field
 previous=json.loads((ROOT/'work/sonic-adventure/channel_decoder_previous_build_manifest.json').read_text());assert len(previous['files'])==2671
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_channel_decoder.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==220
 powershell=[ps,P/'ghidra/export_movie_channel_decoders.ps1',P/'ghidra/export_movie_channel_decoder_lower.ps1',P/'ghidra/export_movie_channel_decoder_io.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/channel_decoder_unbounded_receipt.json';strict=json.loads(strict_path.read_text());assert strict['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and strict['exit_code']==0 and strict['flags']==['-fsyntax-only','-Wall','-Wextra','-Werror']
 q=json.loads((P/'analysis/movie_channel_decoder_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['fresh_original_native_comparisons'],q['fresh_independent_checks'])==(3,158,40,771,7207)
 coverage=json.loads((P/'analysis/recovery_coverage_audit.json').read_text());assert not coverage['registry_evidence_errors'] and not coverage['whole_source_hash_refresh_needed'] and coverage['registered_routines']==1488
 files=scripts+powershell+[strict_path,P/'analysis/movie_channel_decoder_qualification.json']
 report=dict(reproduced_generated_artifacts=7,registration_byte_idempotent=True,prior_registry_rows_identical=1485,registry_entries=1488,new_routine_credit=3,qualified_new_rows=158,qualified_new_outcomes=40,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(v.INHERITED_INTEGER_CONSTANTS),original_model_execution_code_identical=True,inherited_SR_FD_guards_preserved=True,existing_worker_and_continuation_sources_preserved=True,observer_only_adds_final_MACL_read=True,compiled_source_count=220,strict_unbounded_sources_checked=220,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_build_files=2671,additional_build_files=['work/sonic-adventure/build/movie_channel_decoder.dll','work/sonic-adventure/build/movie_channel_decoder.lib'],fresh_original_native_comparisons=771,fresh_independent_checks=7207,total_evidence_original_native_comparisons=11716,canonical_duplicate_credit=0,active_decoding_complete=False,whole_worker_complete=False,overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in files},scope='Sevenartifacts reproducible/registration idempotent;220strictunbounded Csourcescompile. Prior13959source/prooffilesand2671buildspreserved;10reviewedcatalog/status/doc paths mutable. Registryold1485rows unchangedplus3localbodies158rows40outcomes. Activecodecfamily/fullworkerremainpartial;alloldmodel/guards/ABI/runtime/continuations/source/proofs preserved. No fakehelperresults/counters/contexts. Historicalstartupnotfreshlyrerun;goalactive10%.')
 (P/'analysis/movie_channel_decoder_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('DECODER WORKFLOW PASS',preserved,'preservedsource/proofs;',len(pins),'pins')
if __name__=='__main__':main()
