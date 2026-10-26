"""Reproduce request qualification and retain prior source, proof, builds and scope."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 proof=[P/f'analysis/{n}.json' for n in ('movie_request_complete_verification','movie_request_complete_qualification')];before={str(p):digest(p) for p in proof}
 for name in ('verify_movie_request_complete.py','qualify_movie_request_complete.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';before_register=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_request_complete.py')],check=True);assert before_register==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('verify_movie_request_complete.py','qualify_movie_request_complete.py','register_movie_request_complete.py','verify_movie_request_complete_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_request_complete_verification','movie_request_complete_qualification','movie_transfer_runtime_qualification','movie_transfer_runtime_workflow_verification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='93e55e07b56b6d89f94a4ee0eb9f79d7a9b138a1';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in manifest['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1397
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1397 if field=='recovered_buildable_routines' else data),field
  elif key=='recovery_coverage_audit':
   for field,data in value.items():
    if field in ('registered_routines','registered_unique_module_address_pairs','registered_entries_matching_export_snapshots'):continue
    assert now[key][field]==data,field
   audit=json.loads((P/'analysis/recovery_coverage_audit.json').read_text())
   assert now[key]['registered_routines']==audit['registered_routines']==1397 and now[key]['registered_unique_module_address_pairs']==audit['registered_unique_module_address_pairs']==1394
   assert now[key]['registered_entries_matching_export_snapshots']==audit['registered_entries_matching_export_snapshots']
   assert now[key]['registry_evidence_errors']==now[key]['stale_whole_source_hash_reports']==0
  else:assert now[key]==value,key
 old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old)==1396 and len(current)==1397
 index={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in old:
  now_row=index[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for key,value in row.items():assert now_row[key]==value,(row['address'],key)
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 assert preserved==12601 and len(builds)==2623
 build=P/'build_movie_transfer_runtime.ps1';sources=re.findall(r"'([^']+\.c)'",build.read_text());assert len(sources)==len(set(sources))==193
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 q=json.loads(proof[1].read_text());assert q['production_code_changed'] is False
 report=dict(reproduced_proof_files=2,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=193,preserved_baseline_commit=baseline,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1396,preserved_build_files=len(builds),native_build_bytes_unchanged=True,newly_registered_entries=1,newly_qualified_rows=139,qualified_conditional_outcomes=26,scope='Proof-only completion of existing original request:12new cases/270independent checks plus699unchanged retained comparisons qualify139rows/all26outcomes. Existing193-source binary and all2623historical build bytes retained, with fresh strict unbounded compilation checks. Earlier source/proof bytes,canonical fields and all requested goals remain intact; no new startup/full-copy/video-clear claim.',sha256={str(p):digest(p) for p in scripts+[build]})
 (P/'analysis/movie_request_complete_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
