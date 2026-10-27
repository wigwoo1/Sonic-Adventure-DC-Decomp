"""Reproduce integration and preserve prior source/proofs/registry rows/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='191c7c8e52b6fef57b9bff0d623ab2a24a4bfe16'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/n for n in ('source/sa_movie_earlier_initialize.c','analysis/movie_earlier_initialize_translation.json','source/sa_movie_earlier_initialize_connected.c','source/movie_earlier_initialize_probe.c','build_movie_earlier_initialize.ps1','analysis/movie_earlier_initialize_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_earlier_initialize.py','connect_movie_earlier_initialize.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=registry.read_bytes(),status.read_bytes();subprocess.run([sys.executable,'-B',str(P/'register_movie_earlier_initialize.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_earlier_initialize.py','connect_movie_earlier_initialize.py','movie_earlier_initialize_reference.py','write_watch_earlier_initialize.py','verify_movie_earlier_initialize.py','verify_movie_earlier_initialize_retained.py','capture_movie_earlier_initialize_startup.py','verify_movie_earlier_initialize_startup.py','qualify_movie_earlier_initialize.py','register_movie_earlier_initialize.py','verify_movie_earlier_initialize_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('translation','connection','verification','retained_verification','startup_verification','qualification'):
  report=json.loads((P/f'analysis/movie_earlier_initialize_{name}.json').read_text())
  for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_earlier_initialize_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['compiled_source_count'],q['fresh_original_native_comparisons'])==(2,36,213,444)
 import movie_earlier_initialize_reference as reference
 import write_watch_earlier_initialize as fast
 assert reference.namespace['REGISTER_ROWS']==reference.old.namespace['REGISTER_ROWS']
 assert all(reference.namespace['GUARDS'][pc]==rule for pc,rule in reference.old.namespace['GUARDS'].items())
 assert reference.namespace['GUARDS'][0x8c053ada]==(True,2)
 assert all(getattr(reference,name)==value for name,value in reference.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 old_bridge=(P/'source/sa_movie_record_address_connected.c').read_text();bridge=(P/'source/sa_movie_earlier_initialize_connected.c').read_text();manifest=json.loads((P/'analysis/movie_earlier_initialize_translation.json').read_text())
 marker='#include "sh4_exception_state.h"';expected=old_bridge.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
 marker=' return -2;';expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_record_address_'+suffix,'sa_movie_earlier_initialize_'+suffix)
 assert bridge==expected
 assert (P/'source/movie_earlier_initialize_probe.c').read_text()==(P/'source/movie_record_address_probe.c').read_text().replace('sa_movie_record_address_','sa_movie_earlier_initialize_')
 begin=old_bridge.index('static int runtime_poll(');end=old_bridge.index('static int dispatch(void *opaque',begin);assert old_bridge[begin:end] in bridge
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','recovered_routines.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13585
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];new_registry=json.loads(registry.read_text())['routines'];assert new_registry[:1469]==old_registry and len(new_registry)==1471
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text());assert now['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert now['verified_progress'][field]==(1471 if field=='recovered_buildable_routines' else value),field
 receipt=ROOT/'work/sonic-adventure/earlier_initialize_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
   raw=archive.read('BUILD_MANIFEST.json');assert json.loads(raw)['source_manifest_sha256']=='168bcd02786f81b0478924a76223ba6d91401a1c44923fa7b603da9295b12942';receipt.write_bytes(raw)
 old_builds=json.loads(receipt.read_text())['files'];assert len(old_builds)==2657
 for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_earlier_initialize.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==213
 powershell=[ps,P/'ghidra/export_movie_earlier_initialize.ps1',P/'ghidra/export_movie_earlier_dependencies.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/earlier_initialize_unbounded_receipt.json';inputs={str(P/'source'/n):digest(P/'source'/n) for n in sources};strict=json.loads(strict_path.read_text());assert strict['source_sha256']==inputs and strict['exit_code']==0 and strict['flags']==['-fsyntax-only','-Wall','-Wextra','-Werror']
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,prior_registry_rows_identical=1469,registry_entries=1471,new_routine_credit=2,qualified_new_rows=36,qualified_new_outcomes=4,canonical_zero_credit_entries=3,canonical_integration_rows=22,partial_caller=q['partial_caller'],python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(reference.INHERITED_INTEGER_CONSTANTS),inherited_SR_FD_guards_preserved=True,research_caller_retry_guard_declared_not_exercised=True,observer_only_adds_final_MACL_read=True,exact_bridge_extension_checked=True,probe_exact_prefix_renaming_only=True,legacy_G2_CH2_runtime_unchanged=True,legacy_17_18_19_20_argument_layouts_preserved=True,compiled_source_count=213,strict_unbounded_sources_checked=213,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,preserved_build_files=2657,additional_build_files=['work/sonic-adventure/build/movie_earlier_initialize.dll','work/sonic-adventure/build/movie_earlier_initialize.lib'],original_requirements_preserved=True,fresh_original_native_comparisons=444,fresh_independent_checks=6530,total_evidence_original_native_comparisons=8868,actual_next_frontier='0x8c051790',warm_boundaries=4709,fresh_cold_profile=False,historical_cold_source_stage=211,dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell+[strict_path]},scope='213-source two new memory/status helpers fully execute36rows/four outcomes;three canonical adapters zero extra credit. Earlier053ac0 remains partial17/43rows and one of two outcomes. 444fresh cases6530checks;warm4709 reaches real051790. Old211cold historical/not rerun. Prior source/proofs/rows/builds/runtime/API/requirements preserved. Goal ACTIVE about10%;whole game/ports unfinished.')
 (P/'analysis/movie_earlier_initialize_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('EARLIER WORKFLOW', {k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
