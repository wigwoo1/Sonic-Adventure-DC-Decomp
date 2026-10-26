"""Reproduce the 203-source integration and preserve the published 202-source stage."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='c03288673dbf2133fa67a270282792d3fb0f7892'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/n for n in ('source/sa_movie_render_buffer.c','analysis/movie_render_buffer_translation.json','source/sa_movie_render_buffer_connected.c','source/movie_render_buffer_probe.c','build_movie_render_buffer.ps1','analysis/movie_render_buffer_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_render_buffer.py','connect_movie_render_buffer.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes());subprocess.run([sys.executable,'-B',str(P/'register_movie_render_buffer.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 names=('recover_movie_render_buffer.py','connect_movie_render_buffer.py','movie_render_buffer_reference.py','write_watch_render_buffer.py','verify_movie_render_buffer_format.py','verify_movie_render_buffer_size_edges.py','verify_movie_render_buffer_allocations.py','verify_movie_render_buffer_fixed_allocations.py','verify_movie_render_buffer_cache.py','verify_movie_render_buffer_edge_cases.py','verify_movie_render_buffer_failure_prefix.py','verify_movie_render_buffer_retained.py','capture_movie_render_buffer_startup.py','verify_movie_render_buffer_startup.py','qualify_movie_render_buffer.py','register_movie_render_buffer.py','verify_movie_render_buffer_workflow.py');scripts=[P/n for n in names]
 for script in scripts:ast.parse(script.read_text())
 pins={}
 for name in ('translation','connection','format_verification','size_edges_verification','allocation_verification','fixed_verification','cache_verification','edges_verification','failure_verification','retained_verification','startup_verification','qualification'):
  report=json.loads((P/f'analysis/movie_render_buffer_{name}.json').read_text())
  for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_render_buffer_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['qualified_conditional_outcomes'],q['compiled_source_count'],q['fresh_original_native_comparisons'])==(15,1491,370,203,960)
 import movie_render_buffer_reference as reference
 constants=reference.INHERITED_INTEGER_CONSTANTS;assert constants and all(getattr(reference,name)==value for name,value in constants.items())
 assert reference.PAIR==reference.old.PAIR==0x8c6437f0 and reference.ACCOUNT_PAIR==0x8c653dc0
 old_bridge=(P/'source/sa_movie_graphics_handoff_connected.c').read_text();manifest=json.loads((P/'analysis/movie_render_buffer_translation.json').read_text());assert len(manifest['routines'])==19
 marker='#include "sh4_exception_state.h"';assert old_bridge.count(marker)==1
 expected=old_bridge.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker);marker=' return -2;';assert expected.count(marker)==1
 expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_graphics_handoff_'+suffix,'sa_movie_render_buffer_'+suffix)
 assert expected==(P/'source/sa_movie_render_buffer_connected.c').read_text()
 assert (P/'source/movie_render_buffer_probe.c').read_text()==(P/'source/movie_graphics_handoff_probe.c').read_text().replace('sa_movie_graphics_handoff_','sa_movie_render_buffer_')
 capture=(P/'capture_movie_render_buffer_startup.py').read_text();assert 'import write_watch_render_buffer as fast' in capture and "v.effects.DLL.name=='movie_render_buffer.dll'" in capture
 for profile in ('warm','cold'):
  r=json.loads((ROOT/f'work/sonic-adventure/movie_render_buffer_startup_{profile}_capture.json').read_text());assert r['dll_sha256']==q['dll_sha256'] and r['record']['stop_pc']=='0x8c65393a' and r['full_original_native_comparison']
  assert r['sha256'][str(P/'movie_render_buffer_reference.py')]==digest(P/'movie_render_buffer_reference.py')
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13158
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old_registry)==1433 and len(current)==1448 and current[:1433]==old_registry
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1448
  elif key=='recovery_coverage_audit':continue
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1448 if field=='recovered_buildable_routines' else data),field
  else:assert now[key]==value,key
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
  old_builds=json.loads(archive.read('BUILD_MANIFEST.json'))['files']
  for row in old_builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==archive.read(row['path'])
 assert len(old_builds)==2643
 ps=P/'build_movie_render_buffer.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(set(sources))==len(sources)==203
 powershell=[ps,P/'ghidra/export_movie_graphics_handoff_next.ps1']+[P/'ghidra'/('export_movie_render_buffer_'+suffix+'.ps1') for suffix in ('support','lower','sizing','next')]
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(constants),original_pair_constant_preserved=True,bridge_only_adds_nineteen_routes_and_export_names=True,probe_exact_prefix_renaming_only=True,legacy_17_18_19_20_argument_layouts_preserved=True,current_capture_module_dll_identity_guarded=True,strict_unbounded_sources_checked=203,compiled_source_count=203,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1433,preserved_build_files=2643,additional_build_files=['work/sonic-adventure/build/movie_render_buffer.dll','work/sonic-adventure/build/movie_render_buffer.lib'],newly_registered_entries=15,qualified_new_rows=1491,executed_rows=1491,executed_conditional_outcomes=369,proved_infeasible_conditional_outcomes=1,fresh_original_native_comparisons=960,fresh_independent_checks=16631,new_direct_original_native_comparisons=727,fresh_retained_original_native_comparisons=231,fresh_full_startup_comparisons=2,retained_202_source_comparisons=3873,total_evidence_original_native_comparisons=4833,actual_next_frontier='0x8c65393a',remaining_unqualified_graphics_root_outcomes=q['remaining_unqualified_graphics_root_outcomes'],dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+powershell},scope='203-source actualrender-buffer integration;960freshcomparisons. 15newownbodies1491rows/370outcomes;onecheckedconstant-boundproof. All19bodies1652rows393outcomes execute plusoneproveddead. Fourcanonicaladapters no duplicatecredit. Originalsource/proof/build/requirements preserved. Fullconfiguredstartupfrontier65393a;rootstillpartialtwooutcomes;researchlibrarynotgame;goalactive10%weighted.')
 (P/'analysis/movie_render_buffer_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})

if __name__=='__main__':main()
