"""Reproduce frame integration and preserve earlier source/proofs/rows/builds."""
import ast,csv,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='b9a8f4058772a7d2932bea418dad673735acf510'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_active_input.c','analysis/movie_active_input_translation.json','source/sa_movie_active_input_connected.c','source/movie_active_input_probe.c','build_movie_active_input.ps1','analysis/movie_active_input_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_active_input.py','connect_movie_active_input.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=registry.read_bytes(),status.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_active_input.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_active_input.py','connect_movie_active_input.py','movie_active_input_reference.py','movie_active_input_fixtures.py','write_watch_active_input.py','verify_movie_active_input.py','verify_movie_active_input_callers.py','verify_movie_active_input_irq.py','verify_movie_active_input_retained.py','capture_movie_active_input_startup.py','verify_movie_active_input_startup.py','qualify_movie_active_input.py','register_movie_active_input.py','verify_movie_active_input_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('translation','connection','verification','callers_verification','irq_verification','retained_verification','startup_verification','qualification'):
  report=json.loads((P/f'analysis/movie_active_input_{name}.json').read_text())
  for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_active_input_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['compiled_source_count'],q['fresh_original_native_comparisons'])==(3,158,216,518)
 import movie_active_input_reference as reference
 import write_watch_active_input as fast
 assert reference.namespace['REGISTER_ROWS']==reference.old.namespace['REGISTER_ROWS']
 assert all(reference.namespace['GUARDS'][pc]==rule for pc,rule in reference.old.namespace['GUARDS'].items())
 assert reference.namespace['GUARDS'][0x8c051798]==(True,2)
 assert all(getattr(reference,name)==value for name,value in reference.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 old_bridge=(P/'source/sa_movie_countdown_connected.c').read_text();bridge=(P/'source/sa_movie_active_input_connected.c').read_text();manifest=json.loads((P/'analysis/movie_active_input_translation.json').read_text())
 marker='#include "sh4_exception_state.h"';expected=old_bridge.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
 marker=' return -2;';expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_countdown_'+suffix,'sa_movie_active_input_'+suffix)
 assert bridge==expected
 assert (P/'source/movie_active_input_probe.c').read_text()==(P/'source/movie_countdown_probe.c').read_text().replace('sa_movie_countdown_','sa_movie_active_input_')
 begin=old_bridge.index('static int runtime_poll(');end=old_bridge.index('static int dispatch(void *opaque',begin);assert old_bridge[begin:end] in bridge
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13773
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];new_registry=json.loads(registry.read_text())['routines'];assert new_registry[:1477]==old_registry and len(new_registry)==1480
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text());assert now['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert now['verified_progress'][field]==(1480 if field=='recovered_buildable_routines' else value),field
 receipt=ROOT/'work/sonic-adventure/active_input_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
   raw=archive.read('BUILD_MANIFEST.json');assert json.loads(raw)['source_manifest_sha256']=='40c42f9ad528fa285692ccd0c5c2147077aea7153b4fd935eb4be35ca986c67b';receipt.write_bytes(raw)
 old_builds=json.loads(receipt.read_text())['files'];assert len(old_builds)==2663
 for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_active_input.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==216
 powershell=[ps,P/'ghidra/export_movie_active_input.ps1',P/'ghidra/export_movie_active_input_lower.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/active_input_unbounded_receipt.json';inputs={str(P/'source'/n):digest(P/'source'/n) for n in sources};strict=json.loads(strict_path.read_text());assert strict['source_sha256']==inputs and strict['exit_code']==0 and strict['flags']==['-fsyntax-only','-Wall','-Wextra','-Werror']
 assert reference.namespace['CD_FPU_ROWS']==reference.old.namespace['CD_FPU_ROWS']
 assert reference.namespace['CONTEXT_ROWS']==reference.old.namespace['CONTEXT_ROWS']
 assert reference.namespace['AI_FPU_ROWS']==set(reference.PER_BODY[0x8c668e60])|set(reference.PER_BODY[0x8c1108e0])
 expected=reference.old.code.replace('if w==0xb and pr not in RETURNS|{entry_pr}:','if w==0xb and pc!=0x8c11094c and pr not in RETURNS|{entry_pr}:')
 marker='  if pc in GUARD_ENTRY:'
 expected=expected.replace(marker,"  if pc in AI_FPU_ROWS and (w>>12==0xf or w&0xf0ff in (0x006a,0x406a,0x405a)) and sr&0x8000:stop(pc,r,sr,-4)\n"+marker)
 assert reference.code==expected
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,prior_registry_rows_identical=1477,registry_entries=1480,new_routine_credit=3,qualified_new_rows=158,qualified_new_outcomes=16,canonical_context_save_zero_credit=True,partial_entries=q['partial_entries'],partial_parent_entries=q['partial_parent_entries'],python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(reference.INHERITED_INTEGER_CONSTANTS),inherited_SR_FD_guards_preserved=True,new_restore_RTS_only_bypasses_ordinary_return_contract=True,reached_FPU_guard_after_entry_boundary=True,positive_counter_wait_observed_without_automatic_progress=True,requested_fixture_values_checked_after_setup=True,observer_only_adds_final_MACL_read=True,exact_bridge_extension_checked=True,probe_exact_prefix_renaming_only=True,legacy_G2_CH2_runtime_unchanged=True,legacy_17_18_19_20_argument_layouts_preserved=True,compiled_source_count=216,strict_unbounded_sources_checked=216,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,preserved_build_files=2663,additional_build_files=['work/sonic-adventure/build/movie_active_input.dll','work/sonic-adventure/build/movie_active_input.lib'],original_requirements_preserved=True,fresh_original_native_comparisons=518,fresh_independent_checks=5100,total_evidence_original_native_comparisons=10116,authored_irq_admission_cases=16,actual_wait_pc='0x8c051798',frame_counter_unchanged=2,warm_boundaries=4709,fresh_cold_profile=False,historical_cold_source_stage=211,dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell+[strict_path]},scope='216-source active-input scheduler,cleanup,contextrestore:three complete bodies158rows16outcomes within explicit coherent task/context domains. 48real input/tickcaller cases and8untimedoriginal-vector IRQchains withinputenabled pass. Unknowncoroutinecallbacks remainactualmissingtargets;newrestoreRTShonorsguestPR and actual668e92continuation throughinternalunwind,without gueststate fixes. Automaticframewaittiming,acquiredcontroller events,natural taskstartup,arbitraryinteriorcontinuations andgame/ports partial. Prior source/proofs/registry/builds/requirements/runtime/API preserved. Goalactive10%.')
 (P/'analysis/movie_active_input_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COUNTDOWN WORKFLOW',{k:v for k,v in report.items() if k not in ('sha256','partial_entries')})
if __name__=='__main__':main()
