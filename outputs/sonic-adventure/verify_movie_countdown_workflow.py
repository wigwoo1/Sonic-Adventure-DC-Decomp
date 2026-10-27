"""Reproduce frame integration and preserve earlier source/proofs/rows/builds."""
import ast,csv,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='ee4df379484045f15d34fbac5c7654ebaf768203'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_countdown.c','analysis/movie_countdown_translation.json','source/sa_movie_countdown_connected.c','source/movie_countdown_probe.c','build_movie_countdown.ps1','analysis/movie_countdown_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_countdown.py','connect_movie_countdown.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=registry.read_bytes(),status.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_countdown.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_countdown.py','connect_movie_countdown.py','movie_countdown_reference.py','movie_countdown_fixtures.py','write_watch_countdown.py','verify_movie_countdown.py','verify_movie_countdown_irq.py','verify_movie_countdown_retained.py','capture_movie_countdown_startup.py','verify_movie_countdown_startup.py','qualify_movie_countdown.py','register_movie_countdown.py','verify_movie_countdown_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('translation','connection','verification','irq_verification','retained_verification','startup_verification','qualification'):
  report=json.loads((P/f'analysis/movie_countdown_{name}.json').read_text())
  for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_countdown_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['compiled_source_count'],q['fresh_original_native_comparisons'])==(5,155,215,474)
 import movie_countdown_reference as reference
 import write_watch_countdown as fast
 assert reference.namespace['REGISTER_ROWS']==reference.old.namespace['REGISTER_ROWS']
 assert all(reference.namespace['GUARDS'][pc]==rule for pc,rule in reference.old.namespace['GUARDS'].items())
 assert reference.namespace['GUARDS'][0x8c051798]==(True,2)
 assert all(getattr(reference,name)==value for name,value in reference.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 old_bridge=(P/'source/sa_movie_frame_wait_connected.c').read_text();bridge=(P/'source/sa_movie_countdown_connected.c').read_text();manifest=json.loads((P/'analysis/movie_countdown_translation.json').read_text())
 marker='#include "sh4_exception_state.h"';expected=old_bridge.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
 marker=' return -2;';expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_frame_wait_'+suffix,'sa_movie_countdown_'+suffix)
 assert bridge==expected
 assert (P/'source/movie_countdown_probe.c').read_text()==(P/'source/movie_frame_wait_probe.c').read_text().replace('sa_movie_frame_wait_','sa_movie_countdown_')
 begin=old_bridge.index('static int runtime_poll(');end=old_bridge.index('static int dispatch(void *opaque',begin);assert old_bridge[begin:end] in bridge
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13696
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];new_registry=json.loads(registry.read_text())['routines'];assert new_registry[:1472]==old_registry and len(new_registry)==1477
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text());assert now['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert now['verified_progress'][field]==(1477 if field=='recovered_buildable_routines' else value),field
 receipt=ROOT/'work/sonic-adventure/countdown_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
   raw=archive.read('BUILD_MANIFEST.json');assert json.loads(raw)['source_manifest_sha256']=='2fe42eb354c3b93e209114c8bd8c0c185e09e67535ac841f9a250c0ed2bb2a0b';receipt.write_bytes(raw)
 old_builds=json.loads(receipt.read_text())['files'];assert len(old_builds)==2661
 for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_countdown.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==215
 powershell=[ps,P/'ghidra/export_movie_countdown.ps1',P/'ghidra/export_movie_countdown_lower.ps1',P/'ghidra/export_movie_countdown_support.ps1',P/'ghidra/export_movie_countdown_slot.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/countdown_unbounded_receipt.json';inputs={str(P/'source'/n):digest(P/'source'/n) for n in sources};strict=json.loads(strict_path.read_text());assert strict['source_sha256']==inputs and strict['exit_code']==0 and strict['flags']==['-fsyntax-only','-Wall','-Wextra','-Werror']
 assert reference.namespace['CD_FPU_ROWS']==set(reference.PER_BODY[0x8c652554]) and len(reference.namespace['CD_FPU_ROWS'])==49
 marker='  if pc in GUARD_ENTRY:'
 assert reference.code==reference.old.code.replace(marker,"  if pc in CD_FPU_ROWS and (w>>12==0xf or w&0xf0ff in (0x006a,0x406a)) and sr&0x8000:stop(pc,r,sr,-4)\n"+marker)
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,prior_registry_rows_identical=1472,registry_entries=1477,new_routine_credit=5,qualified_new_rows=155,qualified_new_outcomes=14,canonical_zero_credit_entries=9,canonical_integration_rows=134,partial_entries=q['partial_entries'],python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(reference.INHERITED_INTEGER_CONSTANTS),inherited_SR_FD_guards_preserved=True,reached_FPU_guard_after_entry_boundary=True,positive_counter_wait_observed_without_automatic_progress=True,requested_fixture_values_checked_after_setup=True,observer_only_adds_final_MACL_read=True,exact_bridge_extension_checked=True,probe_exact_prefix_renaming_only=True,legacy_G2_CH2_runtime_unchanged=True,legacy_17_18_19_20_argument_layouts_preserved=True,compiled_source_count=215,strict_unbounded_sources_checked=215,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,preserved_build_files=2661,additional_build_files=['work/sonic-adventure/build/movie_countdown.dll','work/sonic-adventure/build/movie_countdown.lib'],original_requirements_preserved=True,fresh_original_native_comparisons=474,fresh_independent_checks=5405,total_evidence_original_native_comparisons=9598,authored_irq_admission_cases=16,actual_wait_pc='0x8c051798',frame_counter_unchanged=2,warm_boundaries=4709,fresh_cold_profile=False,historical_cold_source_stage=211,dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell+[strict_path]},scope='215-source countdown/audio/request/FPU wrapper integration;five complete bodies155rows14outcomes;nine canonical zero credit;four new partial bodies. Eight untimed original-vector IRQ chains count2->1->0 and restore interrupted state throughRTE. Explicit software admission/coherent driver/inactive input only. Automatic frame-wait progress,active input,largerhandlers and game/ports partial. Prior sources/proofs/registry/builds/requirements/runtime/API preserved. Goal continuation authorized;10%estimate.')
 (P/'analysis/movie_countdown_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('COUNTDOWN WORKFLOW',{k:v for k,v in report.items() if k not in ('sha256','partial_entries')})
if __name__=='__main__':main()
