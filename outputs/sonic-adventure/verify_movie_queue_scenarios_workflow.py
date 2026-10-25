"""Current pins, original bytes and strict C checks for queue scenarios."""
import ast,hashlib,json,re,subprocess,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 scripts=[P/name for name in ('disc_queue_scenario_reference.py','verify_movie_queue_scenarios.py','verify_movie_queue_scenarios_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_queue_scenarios_verification','movie_mount_media','movie_mount_verification','movie_disc_paths_qualification','movie_disc_paths_connection'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 profiles=json.loads((P/'analysis/movie_mount_media.json').read_text())['profiles']
 for row in profiles.values():assert digest(ROOT/row['path'])==row['sha256']
 report=json.loads((P/'analysis/movie_queue_scenarios_verification.json').read_text())
 assert report['cases_passed']==33 and report['independent_checks']==333 and report['combined_covered_rows']==472 and report['combined_executed_outcomes']==49
 assert len(report['missing_rows'])==7 and len(report['missing_edges'])==9 and not report['qualified'] and report['new_registry_credit']==0
 assert report['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/movie_disc_paths.dll')
 baseline='8a6e51ab153dd3a2916f5b86449fd8ba8630c3ff';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 previous=json.loads(subprocess.check_output([git,'-C',str(checkout),'show',baseline+':PUBLICATION_MANIFEST.json']))
 preserved=0
 for row in previous['files']:
  path=row['path']
  if '/source/' not in path and not ('/analysis/' in path and any(x in Path(path).name for x in ('verification','qualification','translation','connection'))):continue
  if path.endswith('analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_disc_paths.ps1').read_text());assert len(sources)==len(set(sources))==171
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/name) for name in sources],check=True)
 result=dict(python_scripts_parsed=len(scripts),pinned_inputs_current=len(pins),media_profiles_checked=len(profiles),strict_unbounded_sources_checked=171,preserved_baseline_commit=baseline,preserved_source_and_proof_files=preserved,preserved_build_files=len(builds),native_c_changed=False,scope='Queue scenarios use the retained 171-source DLL and actual queue APIs. Existing C/build bytes and historical evidence remain unchanged; strict syntax is rechecked. No fresh DLL build or full accumulated suite replay is claimed.',sha256={str(p):digest(p) for p in scripts})
 (P/'analysis/movie_queue_scenarios_workflow_verification.json').write_text(json.dumps(result,indent=2)+'\n');print({k:v for k,v in result.items() if k!='sha256'})
if __name__=='__main__':main()
