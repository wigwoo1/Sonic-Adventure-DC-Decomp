"""Reproduction, strict compilation, ABI and published-byte preservation checks."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='e039794dbed8d22c154df58a8e942710eb656f84'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_buffer_initialize.c','analysis/movie_buffer_initialize_translation.json','source/sa_movie_buffer_initialize_connected.c','source/movie_buffer_initialize_probe.c','build_movie_buffer_initialize.ps1','analysis/movie_buffer_initialize_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_buffer_initialize.py','connect_movie_buffer_initialize.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes());subprocess.run([sys.executable,'-B',str(P/'register_movie_buffer_initialize.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 names=('recover_movie_buffer_initialize.py','connect_movie_buffer_initialize.py','movie_buffer_initialize_reference.py','write_watch_buffer_initialize.py','buffer_transfer_reference_cpu.py','vram_aperture_reference.py','pvr_dma_reference.py','verify_vram_aperture_bus.py','verify_pvr_dma_bus.py','verify_pvr_error_status.py','verify_movie_buffer_initialize.py','verify_movie_buffer_interrupt.py','verify_movie_buffer_initialize_retained.py','capture_movie_buffer_initialize_startup.py','verify_movie_buffer_initialize_startup.py','qualify_movie_buffer_initialize.py','register_movie_buffer_initialize.py','verify_movie_buffer_initialize_workflow.py');scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 reports=('movie_buffer_initialize_translation','movie_buffer_initialize_connection','movie_buffer_initialize_verification','movie_buffer_interrupt_verification','movie_buffer_initialize_retained_verification','movie_buffer_initialize_startup_verification','vram_aperture_bus_verification','pvr_dma_bus_verification','pvr_error_status_verification','movie_buffer_initialize_qualification');pins={}
 for name in reports:
  r=json.loads((P/f'analysis/{name}.json').read_text())
  for path,h in r['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_buffer_initialize_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['compiled_source_count'],q['fresh_original_native_comparisons'])==(5,252,26,206,332)
 import movie_buffer_initialize_reference as reference
 assert reference.INHERITED_INTEGER_CONSTANTS and all(getattr(reference,name)==value for name,value in reference.INHERITED_INTEGER_CONSTANTS.items())
 old_bridge=(P/'source/sa_movie_render_buffer_connected.c').read_text();bridge=(P/'source/sa_movie_buffer_initialize_connected.c').read_text();begin=old_bridge.index('static int runtime_poll(');end=old_bridge.index('static int dispatch(void *opaque',begin);assert old_bridge[begin:end] in bridge
 assert (P/'source/movie_buffer_initialize_probe.c').read_text()==(P/'source/movie_render_buffer_probe.c').read_text().replace('sa_movie_render_buffer_','sa_movie_buffer_initialize_')
 assert '0xa05f6930u' in bridge and '0x320u,6u' in bridge and 'runtime_poll_buffer(c,m,f,sr,r,pr)' in bridge
 for profile in ('warm','cold'):
  r=json.loads((ROOT/f'work/sonic-adventure/movie_buffer_initialize_startup_{profile}_capture.json').read_text());assert r['dll_sha256']==q['dll_sha256'] and r['record']['stop_pc']=='0x8c60edb0' and r['full_original_native_comparison']
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13246
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old_registry)==1448 and len(current)==1453 and current[:1448]==old_registry
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 assert now['requirements']==old_status['requirements']
 for field,value in old_status['verified_progress'].items():assert now['verified_progress'][field]==(1453 if field=='recovered_buildable_routines' else value),field
 previous_builds=ROOT/'work/sonic-adventure/buffer_initialize_previous_build_manifest.json'
 if not previous_builds.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
   old=archive.read('BUILD_MANIFEST.json');assert json.loads(old)['source_manifest_sha256']=='065339bffba1f52e78b0d2253b597636531c7de801188a7db08ff66dd4035924'
   previous_builds.write_bytes(old)
 old_builds=json.loads(previous_builds.read_text())['files'];assert len(old_builds)==2645
 for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_buffer_initialize.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(set(sources))==len(sources)==206
 powershell=[ps]+[P/'ghidra'/('export_movie_buffer_initialize_'+suffix+'.ps1') for suffix in ('support','transfer','cache','queue','interrupt','register')]+[P/'ghidra/export_movie_render_buffer_next.ps1']
 for script in powershell:
  assert script.is_file(),script
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 # Reuse this task's successful strict compiler pass only for exactly identical
 # inputs; a public checkout without this private receipt runs the compiler.
 receipt_path=ROOT/'work/sonic-adventure/buffer_initialize_unbounded_receipt.json'
 syntax_inputs={str(P/'source'/n):digest(P/'source'/n) for n in sources}
 receipt=json.loads(receipt_path.read_text()) if receipt_path.exists() else {}
 if receipt.get('source_sha256')!=syntax_inputs or receipt.get('flags')!=['-fsyntax-only','-Wall','-Wextra','-Werror'] or receipt.get('exit_code')!=0:
  subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
  receipt=dict(source_sha256=syntax_inputs,flags=['-fsyntax-only','-Wall','-Wextra','-Werror'],exit_code=0);receipt_path.write_text(json.dumps(receipt,indent=2)+'\n')
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(reference.INHERITED_INTEGER_CONSTANTS),legacy_G2_runtime_poll_unchanged=True,probe_exact_prefix_renaming_only=True,legacy_17_18_19_20_argument_layouts_preserved=True,strict_unbounded_sources_checked=206,compiled_source_count=206,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1448,preserved_build_files=2645,additional_build_files=['work/sonic-adventure/build/movie_buffer_initialize.dll','work/sonic-adventure/build/movie_buffer_initialize.lib'],newly_registered_entries=5,qualified_new_rows=252,executed_conditional_outcomes=26,fresh_original_native_comparisons=q['fresh_original_native_comparisons'],fresh_independent_checks=q['fresh_independent_checks'],total_evidence_original_native_comparisons=q['total_evidence_original_native_comparisons'],actual_next_frontier='0x8c60edb0',partial_unregistered_dispatcher='0x8c652210',dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell+[receipt_path]},scope='206-sourceupload/interruptpath. Fivefullycoveredownbodies;fourcanonicaladaptersnoextracredit;652210partial. Earlierselectedsource/proofs/builds/requirements preserved. Strictunboundedsyntax andgeneratorreproduction;researchlibrarynotgame;goalactive10%weighted.')
 (P/'analysis/movie_buffer_initialize_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('WORKFLOW', {k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
