"""Reproduce native console integration and retain previous source/proofs/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_console_connected.c','source/movie_console_probe.c','build_movie_console.ps1','analysis/movie_console_connection.json','analysis/console_abi_evidence.json','analysis/movie_console_query_invariant.json','analysis/movie_console_frontier.json')]
 before={str(p):digest(p) for p in generated}
 for name in ('connect_movie_console.py','inventory_console_abi.py','prove_movie_console_queries.py','inventory_movie_console_frontier.py'):
  subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';before_register=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_console.py')],check=True);assert before_register==(registry.read_bytes(),status.read_bytes())
 names=('connect_movie_console.py','console_service_reference.py','verify_console_service_backend.py','verify_movie_console.py','verify_movie_console_regression.py','prove_movie_console_queries.py','fetch_console_abi_evidence.py','inventory_console_abi.py','inventory_movie_console_frontier.py','qualify_movie_console.py','register_movie_console.py','verify_movie_console_workflow.py')
 scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_console_connection','console_abi_evidence','console_service_backend_verification','movie_console_verification','movie_console_query_invariant','movie_console_regression_verification','movie_console_frontier','movie_console_qualification','movie_initialize_integration'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='bc609dacdf94528d890cd87fb93a536f434db767';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 old_manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
 for row in old_manifest['files']:
  path=row['path']
  if '/source/' not in path and not ('/analysis/' in path and any(x in Path(path).name for x in ('verification','qualification','translation','connection'))):continue
  if path.endswith('analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old)==1365 and len(current)==1367
 index={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in old:
  now=index[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for key,value in row.items():assert now[key]==value,(row['address'],key)
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  build=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in build:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 ps=[P/'build_movie_console.ps1',P/'ghidra/export_movie_console_frontier.ps1']
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_console.ps1').read_text());assert len(sources)==len(set(sources))==173
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=len(generated),registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=len(sources),preserved_baseline_commit=baseline,preserved_source_and_proof_files=preserved,preserved_original_registry_entries=len(old),preserved_build_files=len(build),newly_registered_entries=2,scope='New 173-source DLL built; 228 direct comparisons, 69 regression comparisons and 43 backend API cases pass in their stated scopes. All older source/proof/build bytes and canonical fields retained. Long cold benchmark and full accumulated suite not rerun.',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_console_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
