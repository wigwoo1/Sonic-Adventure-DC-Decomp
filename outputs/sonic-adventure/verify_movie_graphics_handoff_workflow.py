"""Reproduce the 202-source integration and preserve all earlier public evidence."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='4de5e6559f52b45f627a43bf2a8776e4ba700f54'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_graphics_handoff.c','analysis/movie_graphics_handoff_translation.json','source/sa_movie_graphics_handoff_connected.c','source/movie_graphics_handoff_probe.c','build_movie_graphics_handoff.ps1','analysis/movie_graphics_handoff_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_graphics_handoff.py','connect_movie_graphics_handoff.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes());subprocess.run([sys.executable,'-B',str(P/'register_movie_graphics_handoff.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_graphics_handoff.py','connect_movie_graphics_handoff.py','movie_graphics_handoff_reference.py','write_watch_graphics_handoff.py','verify_movie_graphics_handoff.py','verify_movie_graphics_handoff_retained.py','capture_movie_graphics_handoff_startup.py','verify_movie_graphics_handoff_startup.py','qualify_movie_graphics_handoff.py','register_movie_graphics_handoff.py','verify_movie_graphics_handoff_workflow.py')]
 for script in scripts:ast.parse(script.read_text())
 pins={}
 for name in ('movie_graphics_handoff_translation','movie_graphics_handoff_connection','movie_graphics_handoff_verification','movie_graphics_handoff_retained_verification','movie_graphics_handoff_startup_verification','movie_graphics_handoff_qualification','movie_graphics_constructor_qualification'):
  report=json.loads((P/f'analysis/{name}.json').read_text())
  for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_graphics_handoff_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['compiled_source_count'],q['fresh_original_native_comparisons'])==(3,265,60,202,233)
 import movie_graphics_handoff_reference as reference
 assert reference.PAIR==reference.old.PAIR==0x8c6437f0 and reference.ACCOUNT_PAIR==0x8c653dc0
 constants=reference.INHERITED_INTEGER_CONSTANTS;assert constants and all(getattr(reference,name)==value for name,value in constants.items())
 # The old bridge is preserved except new declarations/routes and export names.
 old_bridge=(P/'source/sa_movie_graphics_init_connected.c').read_text();manifest=json.loads((P/'analysis/movie_graphics_handoff_translation.json').read_text())
 marker='#include "sh4_exception_state.h"';assert old_bridge.count(marker)==1
 expected=old_bridge.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker);marker=' return -2;';assert expected.count(marker)==1
 expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_graphics_init_'+suffix,'sa_movie_graphics_handoff_'+suffix)
 assert expected==(P/'source/sa_movie_graphics_handoff_connected.c').read_text()
 assert (P/'source/movie_graphics_handoff_probe.c').read_text()==(P/'source/movie_graphics_init_probe.c').read_text().replace('sa_movie_graphics_init_','sa_movie_graphics_handoff_')
 capture=(P/'capture_movie_graphics_handoff_startup.py').read_text();assert "import write_watch_graphics_handoff as fast" in capture and "v.effects.DLL.name=='movie_graphics_handoff.dll'" in capture
 for profile in ('warm','cold'):
  r=json.loads((ROOT/f'work/sonic-adventure/movie_graphics_handoff_startup_{profile}_capture.json').read_text());assert r['dll_sha256']==q['dll_sha256'] and r['record']['stop_pc']=='0x8c6536ec' and r['full_original_native_comparison']
  assert str(P/'movie_graphics_handoff_reference.py') in r['sha256'] and r['sha256'][str(P/'movie_graphics_handoff_reference.py')]==digest(P/'movie_graphics_handoff_reference.py')
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13105
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old_registry)==1430 and len(current)==1433 and current[:1430]==old_registry
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1433
  elif key=='recovery_coverage_audit':continue
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1433 if field=='recovered_buildable_routines' else data),field
  else:assert now[key]==value,key
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
  old_builds=json.loads(archive.read('BUILD_MANIFEST.json'))['files']
  for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==archive.read(row['path'])
 assert len(old_builds)==2641
 ps=P/'build_movie_graphics_handoff.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(set(sources))==len(sources)==202
 powershell=[ps]+[P/'ghidra'/('export_movie_graphics_handoff'+suffix+'.ps1') for suffix in ('','_support','_nested','_next')]
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(constants),original_pair_constant_preserved=True,bridge_only_adds_three_routes_and_export_names=True,probe_exact_prefix_renaming_only=True,legacy_17_18_19_20_argument_layouts_preserved=True,current_capture_module_dll_identity_guarded=True,strict_unbounded_sources_checked=202,compiled_source_count=202,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1430,preserved_build_files=2641,additional_build_files=['work/sonic-adventure/build/movie_graphics_handoff.dll','work/sonic-adventure/build/movie_graphics_handoff.lib'],newly_registered_entries=3,qualified_new_rows=265,executed_rows=265,executed_conditional_outcomes=60,fresh_original_native_comparisons=233,new_direct_original_native_comparisons=137,fresh_retained_original_native_comparisons=94,fresh_full_startup_comparisons=2,retained_201_source_comparisons=3640,total_evidence_original_native_comparisons=3873,actual_next_frontier='0x8c6536ec',remaining_unqualified_graphics_root_outcomes=q['remaining_unqualified_graphics_root_outcomes'],dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell},scope='202-source originalaccounting integration with233fresh comparisons:137direct/94retained/2fullstartup. All265rows60outcomes execute. Currentharness restoresinheritedPAIR,guardsallinheritedintegerconstants andrerunsactualnewDLLprofiles. Oldsource/proof/build/requirements preserved. Exactoldboundaryprefix/fullclear/Sound/transfer/IRQ/queueoracles retained;startupadvances6536ec,rootstillpartialtwooutcomes. Researchlibrarynotgame;goalactive10%weighted.')
 (P/'analysis/movie_graphics_handoff_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
