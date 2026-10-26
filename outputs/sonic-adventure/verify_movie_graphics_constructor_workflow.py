"""Reproduce constructor qualification plumbing and preserve previous publication."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='8627150bb83adf9c1b5d8643ebbb768667a713d1'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_graphics_init.c','analysis/movie_graphics_init_translation.json','source/sa_movie_graphics_init_connected.c','source/movie_graphics_init_probe.c','build_movie_graphics_init.ps1','analysis/movie_graphics_init_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_graphics_init.py','connect_movie_graphics_init.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes());subprocess.run([sys.executable,'-B',str(P/'register_movie_graphics_constructor.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('verify_movie_graphics_constructor.py','verify_movie_graphics_constructor_infeasible.py','verify_movie_graphics_constructor_rebuild.py','qualify_movie_graphics_constructor.py','register_movie_graphics_constructor.py','verify_movie_graphics_constructor_workflow.py')]
 for script in scripts:ast.parse(script.read_text())
 pins={}
 for name in ('movie_graphics_constructor_verification','movie_graphics_constructor_infeasible_verification','movie_graphics_constructor_rebuild_verification','movie_graphics_constructor_qualification','movie_graphics_root_workflow_verification'):
  report=json.loads((P/f'analysis/{name}.json').read_text())
  for path,h in report['sha256'].items():assert digest(path)==h,(name,path);pins[path]=h
 q=json.loads((P/'analysis/movie_graphics_constructor_qualification.json').read_text());assert (q['newly_registered_entries'],q['newly_qualified_rows'],q['executed_rows'],q['proved_infeasible_rows'],q['executed_conditional_outcomes'],q['proved_infeasible_conditional_outcomes'],q['fresh_original_native_comparisons'],q['fresh_independent_checks'])==(1,159,124,35,7,5,28,588)
 assert not q['graphics_root_complete'] and q['remaining_unqualified_root_outcomes']==[['0x8c656cf4','0x8c656d06'],['0x8c656e90','0x8c656f38']]
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'};mutable=[r['path'] for r in baseline['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13093
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old_registry)==1429 and len(current)==1430 and current[:1429]==old_registry
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1430
  elif key=='recovery_coverage_audit':continue
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1430 if field=='recovered_buildable_routines' else data),field
  else:assert now[key]==value,key
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
  builds=json.loads(archive.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==archive.read(row['path'])
 assert len(builds)==2641
 ps=P/'build_movie_graphics_init.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(set(sources))==len(sources)==201
 command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(ps);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 rebuild=json.loads((P/'analysis/movie_graphics_constructor_rebuild_verification.json').read_text());assert rebuild['fresh_build_and_link_passed'] and rebuild['all_other_pe_bytes_equal'] and rebuild['published_dll_sha256']==q['dll_sha256']
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,python_scripts_parsed=6,powershell_scripts_parsed=1,pinned_inputs_current=len(pins),strict_unbounded_sources_checked=201,fresh_isolated_build_link_passed=True,rebuilt_complete_pe_equal_except_coff_timestamp=True,native_source_or_published_build_changed=False,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1429,preserved_build_files=2641,newly_registered_entries=1,qualified_new_rows=159,executed_rows=124,proved_infeasible_rows=35,executed_conditional_outcomes=7,proved_infeasible_conditional_outcomes=5,fresh_original_native_comparisons=28,fresh_independent_checks=588,retained_original_native_comparisons=3612,retained_cases_rerun=False,total_evidence_original_native_comparisons=3640,reused_additional_credit=0,graphics_root_complete=False,remaining_unqualified_root_outcomes=q['remaining_unqualified_root_outcomes'],dll_sha256=q['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+[ps]},scope='Original constructor qualifies159rows:124execute35infeasible,7executed+5infeasibleoutcomes.28freshcomparisons588checks. Alloldsource/proof/build/requirements bytes preserved;fresh201sourcebuild completePEexcepttimestamp matches. Rootstillpartial2outcomes,oldstartupretainednotrerun;goalactive10%weighted.')
 (P/'analysis/movie_graphics_constructor_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
