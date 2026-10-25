"""Reproduce setup generation and retain all published source/proof/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/name for name in ('source/sa_movie_heap.c','analysis/movie_heap_translation.json','source/sa_movie_heap_connected.c','analysis/movie_heap_connection.json','build_movie_heap.ps1','analysis/movie_heap_frontier.json')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_heap.py','connect_movie_heap.py','inventory_movie_heap_frontier.py'):
  subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';old=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_heap.py')],check=True);assert old==(registry.read_bytes(),status.read_bytes())
 scripts=[P/name for name in ('recover_movie_heap.py','connect_movie_heap.py','verify_movie_heap.py','verify_movie_heap_initialization.py','probe_movie_heap_state.py','verify_movie_heap_prefix.py','inventory_movie_heap_frontier.py','qualify_movie_heap.py','register_movie_heap.py','verify_movie_heap_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('movie_heap_translation','movie_heap_connection','movie_heap_verification','movie_heap_prefix_verification','movie_heap_qualification','movie_heap_frontier','movie_heap_initialization_verification','movie_heap_state_probe','movie_render_setup_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():
   assert digest(path)==h,path
   if path in pins:assert pins[path]==h,path
   pins[path]=h
 baseline='ffb3e355e4bb55d58b164fc754f47e6c517d08ba';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
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
 ps=[P/'ghidra/export_movie_heap_initialization.ps1',P/'ghidra/export_movie_heap_caller.ps1',P/'build_movie_heap.ps1']
 command="""$ErrorActionPreference='Stop'
 foreach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}
 """%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_heap.ps1').read_text());assert len(sources)==len(set(sources))==160
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/name) for name in sources],check=True)
 report=dict(reproduced_generated_artifacts=len(generated),registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=len(sources),
  preserved_baseline_commit=baseline,preserved_original_source_and_proof_files=preserved,preserved_original_registry_entries=len(previous),preserved_research_build_files=len(build['files']),newly_registered_entries=3,
  scope='Focused integration workflow, new 160-source DLL compiled by its build recipe and tested in direct/startup comparisons; all older build/source/proof bytes retained. Full accumulated project suite and natural boot are not claimed.',
  sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_heap_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
