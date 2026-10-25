"""Reproduce setup generation and retain all published source/proof/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/name for name in ('source/sa_movie_disc_paths.c','analysis/movie_disc_paths_translation.json','source/sa_movie_disc_paths_connected.c','analysis/movie_disc_paths_connection.json','build_movie_disc_paths.ps1','source/movie_disc_paths_probe.c')]
 generated += [P/'analysis/movie_disc_paths_media.json',P/'analysis/movie_disc_paths_invariant.json']+[ROOT/r['path'] for r in json.loads((P/'analysis/movie_disc_paths_media.json').read_text())['profiles'].values()]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_disc_paths.py','connect_movie_disc_paths.py','prepare_movie_disc_paths_media.py','prove_movie_disc_paths.py'):
  subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';old=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_disc_paths.py')],check=True);assert old==(registry.read_bytes(),status.read_bytes())
 scripts=[P/name for name in ('recover_movie_disc_paths.py','connect_movie_disc_paths.py','verify_movie_disc_paths.py','verify_movie_disc_paths_regression.py','prepare_movie_disc_paths_media.py','disc_paths_platform_reference.py','prove_movie_disc_paths.py','qualify_movie_disc_paths.py','register_movie_disc_paths.py','verify_movie_disc_paths_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('movie_disc_paths_translation','movie_disc_paths_connection','movie_disc_paths_verification','movie_disc_paths_invariant','movie_disc_paths_regression_verification','movie_disc_paths_media','movie_disc_paths_qualification','movie_disc_init_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():
   assert digest(path)==h,path
   if path in pins:assert pins[path]==h,path
   pins[path]=h
 baseline='d247898a6fac0a9b33a403aa8483fbe611105d5a';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
 for row in manifest['files']:
  path=row['path'];qualifies='/source/' in path or ('/analysis/' in path and any(x in Path(path).name for x in ('verification','qualification','translation','connection')))
  if not qualifies or path.endswith('/analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 previous=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines']
 lookup={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in previous:
  now=lookup[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for k,value in row.items():assert now[k]==value,(row['address'],k)
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  build=json.loads(z.read('BUILD_MANIFEST.json'))
  for row in build['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path']),row['path']
 ps=[P/'build_movie_disc_paths.ps1']+[P/'ghidra'/name for name in ('export_movie_disc_paths.ps1','export_movie_disc_paths_lower.ps1')]
 command="""$ErrorActionPreference='Stop'
 foreach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}
 """%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_disc_paths.ps1').read_text());assert len(sources)==len(set(sources))==171
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/name) for name in sources],check=True)
 report=dict(reproduced_generated_artifacts=len(generated),registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=len(sources),
  preserved_baseline_commit=baseline,preserved_original_source_and_proof_files=preserved,preserved_original_registry_entries=len(previous),preserved_research_build_files=len(build['files']),newly_registered_entries=4,
  scope='Focused integration workflow, new 171-source DLL compiled by its build recipe and tested in explicit path/directory/media/descriptor/fault comparisons and retained warm startup regression; prior cold proof is retained separately; all older build/source/proof bytes retained. Full accumulated project suite and natural boot are not claimed.',
  sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_disc_paths_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
