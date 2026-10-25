"""Reproducible media, current evidence pins and preservation of the baseline."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 manifest=P/'analysis/movie_mount_media.json';media=json.loads(manifest.read_text())
 generated=[manifest]+[ROOT/r['path'] for r in media['profiles'].values()];before={str(p):digest(p) for p in generated}
 subprocess.run([sys.executable,'-B',str(P/'prepare_movie_mount_media.py')],check=True)
 assert all(digest(p)==h for p,h in before.items())
 scripts=[P/n for n in ('prepare_movie_mount_media.py','disc_mount_platform_reference.py','verify_movie_mount.py','verify_movie_mount_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('movie_mount_media','movie_mount_verification','movie_disc_paths_qualification','movie_disc_paths_connection'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 evidence=json.loads((P/'analysis/movie_mount_verification.json').read_text())
 assert evidence['cases_passed']==13 and evidence['independent_checks']==171 and evidence['combined_covered_rows']==463 and evidence['combined_executed_outcomes']==44
 assert len(evidence['missing_rows'])==16 and len(evidence['missing_edges'])==14 and not evidence['qualified'] and evidence['new_registry_credit']==0
 assert digest(ROOT/'work/sonic-adventure/build/movie_disc_paths.dll')==evidence['dll_sha256']
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe';baseline='626a1beae16969de32cebf311dba74c614284348'
 original=json.loads(subprocess.check_output([git,'-C',str(checkout),'show',baseline+':PUBLICATION_MANIFEST.json']))
 preserved=0
 for row in original['files']:
  path=row['path']
  if '/source/' not in path and not ('/analysis/' in path and any(x in Path(path).name for x in ('verification','qualification','translation','connection'))):continue
  if path.endswith('analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_disc_paths.ps1').read_text());assert len(sources)==len(set(sources))==171
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/name) for name in sources],check=True)
 report=dict(reproduced_generated_artifacts=len(generated),python_scripts_parsed=len(scripts),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=171,preserved_baseline_commit=baseline,preserved_source_and_proof_files=preserved,preserved_build_files=len(builds),native_c_changed=False,scope='New initializer comparisons reuse the previously compiled 171-source DLL; its hash and original bytes are unchanged. Strict C syntax is rechecked; no new C build or complete suite replay is claimed.',sha256={str(p):digest(p) for p in scripts})
 (P/'analysis/movie_mount_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
