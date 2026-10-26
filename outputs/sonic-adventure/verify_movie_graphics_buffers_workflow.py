"""Validate proof-only recovery and preserve the complete published baseline."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='e118862fb711386274e243bf3366c5e9fcfa9cfd'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_graphics_init.c','analysis/movie_graphics_init_translation.json','source/sa_movie_graphics_init_connected.c','source/movie_graphics_init_probe.c','build_movie_graphics_init.ps1','analysis/movie_graphics_init_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_graphics_init.py','connect_movie_graphics_init.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes());subprocess.run([sys.executable,'-B',str(P/'register_movie_graphics_buffers.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('verify_movie_graphics_buffers.py','verify_movie_graphics_allocations.py','verify_movie_graphics_descriptor.py','verify_movie_graphics_display_infeasible.py','verify_movie_graphics_buffer_abi.py','verify_movie_graphics_buffers_rebuild.py','qualify_movie_graphics_buffers.py','register_movie_graphics_buffers.py','verify_movie_graphics_buffers_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_graphics_buffers_verification','movie_graphics_allocations_verification','movie_graphics_descriptor_verification','movie_graphics_display_infeasible_verification','movie_graphics_buffer_abi_verification','movie_graphics_buffers_rebuild_verification','movie_graphics_buffers_qualification','movie_graphics_leaves_qualification','movie_graphics_init_startup_verification'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text())
  for file,h in r['sha256'].items():assert digest(file)==h,(name,file);pins[file]=h
 q=json.loads((P/'analysis/movie_graphics_buffers_qualification.json').read_text());assert q['newly_registered_entries']==7 and q['newly_qualified_rows']==918 and q['fresh_original_native_comparisons']==706 and q['fresh_independent_checks']==9731 and q['retained_original_native_comparisons']==2473
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in manifest['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13050
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old_registry)==1416 and len(current)==1423 and current[:1416]==old_registry
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1423
  elif key=='recovery_coverage_audit':continue
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1423 if field=='recovered_buildable_routines' else data),field
  else:assert now[key]==value,key
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
  builds=json.loads(archive.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==archive.read(row['path'])
 assert len(builds)==2641,'Preserve current baseline builds before preparing the next archive'
 print('Preserved13050immutable sources/proofs,1416canonicalrows,2641builds',flush=True)
 ps=P/'build_movie_graphics_init.ps1';command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(ps);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==201
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 rebuild=json.loads((P/'analysis/movie_graphics_buffers_rebuild_verification.json').read_text());assert rebuild['fresh_build_and_link_passed'] and rebuild['all_other_pe_bytes_equal'] and rebuild['published_dll_sha256']==q['dll_sha256']
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=1,pinned_inputs_current=len(pins),strict_unbounded_sources_checked=201,fresh_isolated_build_link_passed=True,rebuilt_complete_pe_equal_except_coff_timestamp=True,native_source_or_published_build_changed=False,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1416,preserved_build_files=len(builds),newly_registered_entries=7,qualified_new_rows=918,executed_conditional_outcomes=150,proved_infeasible_conditional_outcomes=2,fresh_original_native_comparisons=706,fresh_independent_checks=9731,retained_original_native_comparisons=2473,retained_cases_rerun=False,total_evidence_original_native_comparisons=3179,remaining_unqualified_larger_bodies=7,reused_additional_credit=0,dll_sha256=q['dll_sha256'],scope='Proof-only qualification of six connected graphics builders plus descriptor provider. No native source/build changes. Fresh private rebuild matches every PEbyte exceptCOFFtimestamp;all201sources pass strictunboundedsyntax. Original source/proofs/builds/registry/statusrequirements preserved before nextpublication preparation. Earlier2473runtime/startup comparisons retained/hashchecked,not freshly rerun. No fullgame/rendering/physicalhardware claim;overall10%weighted,not measuredcoverage.',sha256={str(p):digest(p) for p in scripts+[ps]})
 (P/'analysis/movie_graphics_buffers_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
