"""Reproduce new yield files and preserve old proof/source/build/ABI contracts."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='ea33714b176d24f7b0d4c05c4facc4ab67c460ed'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_task_owner.c','analysis/movie_task_owner_translation.json','source/sa_movie_task_owner_connected.c','source/movie_task_owner_probe.c','build_movie_task_owner.ps1','analysis/movie_task_owner_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_task_owner.py','connect_movie_task_owner.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 audit_path=P/'analysis/movie_task_owner_contract_audit.json';audit_before=digest(audit_path);subprocess.run([sys.executable,'-B',str(P/'audit_movie_task_owner_contract.py')],check=True);assert digest(audit_path)==audit_before
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=registry.read_bytes(),status.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_task_owner.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_task_owner.py','connect_movie_task_owner.py','movie_task_owner_reference.py','movie_task_owner_fixtures.py','write_watch_task_owner.py','verify_movie_task_owner.py','verify_movie_task_owner_callers.py','audit_movie_task_owner_contract.py','verify_movie_task_owner_irq.py','verify_movie_task_owner_retained.py','qualify_movie_task_owner.py','register_movie_task_owner.py','verify_movie_task_owner_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('translation','connection','verification','callers_verification','contract_audit','irq_verification','retained_verification','qualification'):
  r=json.loads((P/f'analysis/movie_task_owner_{name}.json').read_text())
  for p,h in r['sha256'].items():assert digest(p)==h,(name,p);pins[p]=h
 q=json.loads((P/'analysis/movie_task_owner_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['fresh_original_native_comparisons'],q['fresh_independent_checks'])==(2,53,8,209,2305)
 import movie_task_owner_reference as reference
 import write_watch_task_owner as fast
 assert reference.code==reference.old.code
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY'):assert reference.namespace[name]==reference.old.namespace[name]
 assert all(getattr(reference,n)==x for n,x in reference.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 manifest=json.loads((P/'analysis/movie_task_owner_translation.json').read_text());old=(P/'source/sa_movie_task_yield_connected.c').read_text();bridge=(P/'source/sa_movie_task_owner_connected.c').read_text();marker='#include "sh4_exception_state.h"'
 expected=old.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
 marker=' return -2;';expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_task_yield_'+suffix,'sa_movie_task_owner_'+suffix)

 assert bridge==expected
 assert (P/'source/movie_task_owner_probe.c').read_text()==(P/'source/movie_task_yield_probe.c').read_text().replace('sa_movie_task_yield_','sa_movie_task_owner_')
 begin=old.index('static int runtime_poll(');end=old.index('static int dispatch(void *opaque',begin);assert old[begin:end] in bridge
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13858
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];new_registry=json.loads(registry.read_text())['routines'];assert new_registry[:1483]==old_registry and len(new_registry)==1485
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text());assert now['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert now['verified_progress'][field]==(1485 if field=='recovered_buildable_routines' else value),field
 receipt=ROOT/'work/sonic-adventure/task_owner_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
   raw=z.read('BUILD_MANIFEST.json');assert json.loads(raw)['source_manifest_sha256']=='14a57603c852ca9bf560515e4f78323fcc852a5defbf5b7f085a0294ad3c8f7c';receipt.write_bytes(raw)
 old_builds=json.loads(receipt.read_text())['files'];assert len(old_builds)==2667
 for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_task_owner.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==218
 powershell=[ps,P/'ghidra/export_movie_task_admission.ps1',P/'ghidra/export_movie_task_admission_lower.ps1',P/'ghidra/export_movie_task_input_entries.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/task_owner_unbounded_receipt.json';strict=json.loads(strict_path.read_text());assert strict['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and strict['exit_code']==0 and strict['flags']==['-fsyntax-only','-Wall','-Wextra','-Werror']
 report=dict(reproduced_generated_artifacts=7,registration_byte_idempotent=True,prior_registry_rows_identical=1483,registry_entries=1485,new_routine_credit=2,qualified_new_rows=53,qualified_new_outcomes=8,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(reference.INHERITED_INTEGER_CONSTANTS),original_model_execution_code_identical=True,inherited_SR_FD_guards_preserved=True,existing_continuation_routes_preserved=True,observer_only_adds_final_MACL_read=True,exact_bridge_extension_checked=True,legacy_G2_CH2_runtime_unchanged=True,legacy_17_18_19_20_argument_layouts_preserved=True,compiled_source_count=218,strict_unbounded_sources_checked=218,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,preserved_build_files=2667,additional_build_files=['work/sonic-adventure/build/movie_task_owner.dll','work/sonic-adventure/build/movie_task_owner.lib'],original_requirements_preserved=True,fresh_original_native_comparisons=209,fresh_independent_checks=2305,total_evidence_original_native_comparisons=10518,owner_counter_semantics_qualified=False,bounded_counter_observations_verified=True,worker_research_credit=0,fresh_warm_startup_comparisons=0,historical_warm_source_stage=216,historical_cold_source_stage=211,dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell+[strict_path]},scope='Two owner/poolteardown bodies53rows/eightoutcomes;actual0..8channeladmission/counter/cleanup behavior andtwoactualworkerfrontiers checked. Staticcounteraudit bounded;fieldmeaning/globalwriters remainundetermined. No guestcounter fix. Workerexports areunqualified researchonly. No freshwarm/cold rerun. Priorcode/model/guards/runtime/ABI/requirements/source/proof/buildbytes preserved;goalactive10%.')
 (P/'analysis/movie_task_owner_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('TASK OWNER WORKFLOW PASS',preserved,'preserved source/proof files;',len(pins),'pins')
if __name__=='__main__':main()
