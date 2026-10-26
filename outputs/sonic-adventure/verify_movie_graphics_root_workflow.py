"""Check fresh root evidence and preserve the entire prior published baseline."""
import ast,hashlib,json,re,subprocess,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='4cc23d09076628a3c6b8ed98108e862cec645b45'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=['verify_movie_graphics_root_option.py','verify_movie_graphics_root_arithmetic.py','verify_movie_graphics_root_rebuild.py','verify_movie_graphics_root_workflow.py']
 scripts=[P/n for n in names]
 for script in scripts:ast.parse(script.read_text())
 reports={name:json.loads((P/f'analysis/{name}.json').read_text()) for name in ('movie_graphics_root_option_verification','movie_graphics_root_arithmetic_verification','movie_graphics_root_rebuild_verification','movie_graphics_modes_qualification')};pins={}
 for report in reports.values():
  for path,h in report['sha256'].items():assert digest(path)==h,path;pins[path]=h
 option=reports['movie_graphics_root_option_verification'];arithmetic=reports['movie_graphics_root_arithmetic_verification'];rebuild=reports['movie_graphics_root_rebuild_verification']
 assert option['helper_result']==0 and option['unknown_register_reads']==0 and option['stack_restored'] and len(option['infeasible_rows'])==7 and len(option['infeasible_edges'])==3
 assert arithmetic['cases_passed']==66 and any(c['capacity_failure'] for c in arithmetic['cases']) and all(c['status'] in (0,-3) for c in arithmetic['cases'])
 assert arithmetic['independent_checks']==1334 and arithmetic['coverage']['covered_rows']==622 and arithmetic['coverage']['covered_outcomes']==69 and arithmetic['coverage']['missing_rows']==option['infeasible_rows']
 remaining=set(map(tuple,arithmetic['coverage']['missing_outcomes']))-set(map(tuple,option['infeasible_edges']));assert remaining=={('0x8c656cf4','0x8c656d06'),('0x8c656e90','0x8c656f38')}
 assert rebuild['fresh_build_and_link_passed'] and rebuild['all_other_pe_bytes_equal'] and rebuild['compiled_source_count']==201 and arithmetic['dll_sha256']==rebuild['published_dll_sha256']
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable=['outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json')];preserved=0
 for row in baseline['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13091
 old_registry=historical('outputs/sonic-adventure/analysis/recovered_routines.json');assert (P/'analysis/recovered_routines.json').read_bytes()==old_registry
 registry=json.loads(old_registry)['routines'];assert len(registry)==1429
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));new_status=json.loads((P/'project_status.json').read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert new_status[key][field]==data,field
  else:assert new_status[key]==value,key
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as archive:
  build=json.loads(archive.read('BUILD_MANIFEST.json'))
  for row in build['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==archive.read(row['path'])
 assert len(build['files'])==2641
 ps=P/'build_movie_graphics_init.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(set(sources))==len(sources)==201
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/s) for s in sources],check=True)
 report=dict(fresh_original_native_comparisons=arithmetic['cases_passed'],fresh_independent_checks=arithmetic['independent_checks'],retained_original_native_comparisons=3546,total_evidence_original_native_comparisons=3612,original_option_helper_steps=option['original_helper_steps'],executed_root_rows=622,executed_root_outcomes=69,proved_infeasible_root_rows=7,proved_infeasible_root_outcomes=3,remaining_unqualified_root_outcomes=[list(e) for e in sorted(remaining)],root_qualification_complete=False,newly_registered_entries=0,preserved_original_registry_entries=1429,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,preserved_build_files=2641,prior_reports_preserved=True,prior_cases_rerun=False,native_source_changed=False,python_scripts_parsed=4,pinned_inputs_current=len(pins),strict_unbounded_sources_checked=201,fresh_build_link_passed=True,complete_pe_equal_except_coff_timestamp=True,dll_sha256=arithmetic['dll_sha256'],overall_completion_percent=10,estimate_method='weighted milestones; not measured code coverage',sha256={str(p):digest(p) for p in scripts+[ps]},scope='Root arithmetic/capacity and immutable option proof extend actual full-entry evidence. Root/constructor remain partial,registry byte-unchanged. All prior source/proof/build/requirements bytes preserved except three reviewed documentation/status files. Fresh201-sourcebuild completePEmatchesexcepttimestamp. Older runtime/startup comparisons retained,not rerun. No new startup/pixels/gameplay/physical timing claim.')
 (P/'analysis/movie_graphics_root_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
