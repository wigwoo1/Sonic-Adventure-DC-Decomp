"""Reproduce the partial caller integration and preserve earlier canonical proof."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_initialize.c','analysis/movie_initialize_translation.json','source/sa_movie_initialize_connected.c','analysis/movie_initialize_connection.json','source/movie_initialize_probe.c','build_movie_initialize.ps1')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_initialize.py','connect_movie_initialize.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';before_register=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_initialize_integration.py')],check=True);assert before_register==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_initialize.py','connect_movie_initialize.py','verify_movie_initialize.py','verify_movie_initialize_regression.py','qualify_movie_initialize.py','register_movie_initialize_integration.py','verify_movie_initialize_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('movie_initialize_translation','movie_initialize_connection','movie_initialize_verification','movie_initialize_regression_verification','movie_initialize_integration','movie_disc_paths_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='b8a5d0e8c56f72abdb2c7f45f8e0737a94514865';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 prior=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
 for row in prior['files']:
  path=row['path']
  if '/source/' not in path and not ('/analysis/' in path and any(x in Path(path).name for x in ('verification','qualification','translation','connection'))):continue
  if path.endswith('analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old)==len(current)==1365
 indexed={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in old:
  now=indexed[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for key,value in row.items():assert now[key]==value,(row['address'],key)
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 ps=[P/'build_movie_initialize.ps1']+[P/'ghidra'/n for n in ('export_movie_boot_caller.ps1','export_movie_initialize_caller.ps1','export_movie_initialize_caller_lower.ps1','export_movie_initialize_queries.ps1','export_movie_initialize_region.ps1','export_movie_initialize_firmware_frontier.ps1')]
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_initialize.ps1').read_text());assert len(sources)==len(set(sources))==172
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=len(generated),registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=len(sources),preserved_baseline_commit=baseline,preserved_source_and_proof_files=preserved,preserved_canonical_registry_entries=len(old),preserved_build_files=len(builds),new_registry_credit=0,scope='New 172-source DLL built and reached-path comparisons passed; original caller and BIOS paths remain partial. Warm/full-clear regression rerun; cold proof retained separately. All older source/proof/build bytes and canonical registry fields preserved. Full accumulated suite not rerun.',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_initialize_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
