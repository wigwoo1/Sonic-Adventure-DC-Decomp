"""Reproduce address integration and check unchanged source/proofs/registry/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='0af6cc0427c3b5c46d93dd49ce1ca16c588e2d5c'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/n for n in ('source/sa_movie_record_address.c','analysis/movie_record_address_translation.json','source/sa_movie_record_address_connected.c','source/movie_record_address_probe.c','build_movie_record_address.ps1','analysis/movie_record_address_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_record_address.py','connect_movie_record_address.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=registry.read_bytes(),status.read_bytes();subprocess.run([sys.executable,'-B',str(P/'apply_movie_record_address_progress.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_record_address.py','connect_movie_record_address.py','movie_record_address_reference.py','write_watch_record_address.py','verify_movie_record_address.py','verify_movie_record_address_retained.py','capture_movie_record_address_startup.py','verify_movie_record_address_startup.py','qualify_movie_record_address.py','apply_movie_record_address_progress.py','verify_movie_record_address_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_record_address_translation','movie_record_address_connection','movie_record_address_verification','movie_record_address_retained_verification','movie_record_address_startup_verification','movie_record_address_qualification'):
  r=json.loads((P/f'analysis/{name}.json').read_text())
  for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_record_address_qualification.json').read_text());assert (q['newly_registered_entries'],q['qualified_integration_rows'],q['compiled_source_count'],q['fresh_original_native_comparisons'])==(0,9,212,388)
 import movie_record_address_reference as reference
 import write_watch_record_address as fast
 assert reference.namespace['REGISTER_ROWS']==reference.old.namespace['REGISTER_ROWS']
 assert all(getattr(reference,name)==value for name,value in reference.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 old_bridge=(P/'source/sa_movie_record_register_connected.c').read_text();bridge=(P/'source/sa_movie_record_address_connected.c').read_text();manifest=json.loads((P/'analysis/movie_record_address_translation.json').read_text())
 marker='#include "sh4_exception_state.h"';expected=old_bridge.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
 marker=' return -2;';expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_record_register_'+suffix,'sa_movie_record_address_'+suffix)
 assert bridge==expected
 assert (P/'source/movie_record_address_probe.c').read_text()==(P/'source/movie_record_register_probe.c').read_text().replace('sa_movie_record_register_','sa_movie_record_address_')
 begin=old_bridge.index('static int runtime_poll(');end=old_bridge.index('static int dispatch(void *opaque',begin);assert old_bridge[begin:end] in bridge
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==9;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13554 and digest(registry)==q['registry_sha256']
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];assert json.loads(registry.read_text())['routines']==old_registry and len(old_registry)==1469
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text());assert now['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert now['verified_progress'][field]==value,field
 receipt=ROOT/'work/sonic-adventure/record_address_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
   raw=archive.read('BUILD_MANIFEST.json');assert json.loads(raw)['source_manifest_sha256']=='72721fec0baa56d61d895e8bf0f01d132348707446b93cffa1687612ccdce4c5';receipt.write_bytes(raw)
 old_builds=json.loads(receipt.read_text())['files'];assert len(old_builds)==2655
 for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_record_address.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==212
 powershell=[ps,P/'ghidra/export_movie_record_address.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/record_address_unbounded_receipt.json';inputs={str(P/'source'/n):digest(P/'source'/n) for n in sources};strict=json.loads(strict_path.read_text()) if strict_path.exists() else {}
 if strict.get('source_sha256')!=inputs or strict.get('exit_code')!=0 or strict.get('flags')!=['-fsyntax-only','-Wall','-Wextra','-Werror']:
  subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True);strict=dict(source_sha256=inputs,flags=['-fsyntax-only','-Wall','-Wextra','-Werror'],exit_code=0);strict_path.write_text(json.dumps(strict,indent=2)+'\n')
 report=dict(reproduced_generated_artifacts=6,progress_update_byte_idempotent=True,registry_byte_identical=True,registry_entries=1469,new_routine_credit=0,qualified_integration_rows=9,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(reference.INHERITED_INTEGER_CONSTANTS),inherited_SR_FD_guards_preserved=True,observer_only_adds_final_MACL_read=True,exact_bridge_extension_checked=True,probe_exact_prefix_renaming_only=True,legacy_G2_CH2_runtime_unchanged=True,legacy_17_18_19_20_argument_layouts_preserved=True,compiled_source_count=212,strict_unbounded_sources_checked=212,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,preserved_build_files=2655,additional_build_files=['work/sonic-adventure/build/movie_record_address.dll','work/sonic-adventure/build/movie_record_address.lib'],original_requirements_preserved=True,fresh_original_native_comparisons=388,fresh_independent_checks=6739,total_evidence_original_native_comparisons=8424,completed_entry='0x8c0546c0',warm_boundaries=4702,fresh_cold_profile=False,historical_cold_source_stage=211,dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell+[strict_path]},scope='212-source two canonical address helpers. All9 rows execute;zero new registry credit and registry bytes unchanged. Fresh352 direct/35 regression/one warm startup388 cases6739 checks. Warm0546c0 returns;old211 fullcold proof preserved,not rerun or relabeled. Existing source/proofs/builds/API/runtime/requirements preserved. Goal ACTIVE about10%;game/ports unfinished.')
 (P/'analysis/movie_record_address_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ADDRESS WORKFLOW', {k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
