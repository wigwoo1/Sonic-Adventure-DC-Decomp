"""Reproduce generation, check evidence and preserve the published baseline."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/p for p in ('source/sa_movie_hardware.c','analysis/movie_hardware_translation.json','source/sa_movie_hardware_connected.c','analysis/movie_hardware_connection.json','build_movie_hardware.ps1','analysis/movie_hardware_context_frontier.json')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_hardware.py','connect_movie_hardware.py','inventory_movie_hardware_context.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';old=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_hardware.py')],check=True)
 assert old==(registry.read_bytes(),status.read_bytes())
 scripts=[P/name for name in ('recover_movie_hardware.py','connect_movie_hardware.py','verify_movie_hardware.py','verify_movie_hardware_edges.py','verify_movie_hardware_backend.py','verify_movie_hardware_prefix.py','qualify_movie_hardware.py','register_movie_hardware.py','verify_movie_hardware_workflow.py','inventory_movie_hardware_context.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('movie_hardware_translation','movie_hardware_connection','movie_hardware_verification','movie_hardware_edges_verification','movie_hardware_backend_verification','movie_hardware_prefix_verification','movie_hardware_qualification','movie_hardware_context_frontier'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():
   assert digest(path)==h,path
   if path in pins:assert pins[path]==h,path
   pins[path]=h
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe';baseline='92fc42c557a437ba10ac0782e4be0cbfb3d71b81'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
 for row in manifest['files']:
  path=row['path'];qualifies='/source/' in path or ('/analysis/' in path and 'verification' in Path(path).name)
  if not qualifies or path.endswith('/analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 previous=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines']
 lookup={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in previous:
  now=lookup[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for k,value in row.items():assert now[k]==value,(row['address'],k)
 build_preserved=0
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  for name in z.namelist():
   if name.startswith('work/'):
    assert (ROOT/name).read_bytes()==z.read(name),name;build_preserved+=1
 ps_scripts=[P/'ghidra'/name for name in ('export_movie_hardware_lower.ps1','export_movie_hardware_hal.ps1','export_movie_hardware_channel.ps1','export_movie_hardware_root.ps1','export_movie_hardware_initializer.ps1')]+[P/'build_movie_hardware.ps1',P/'build_and_verify.ps1']
 command="""$ErrorActionPreference='Stop'
 foreach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}
 Write-Output 'Seven PowerShell workflows parsed'
 """%(','.join("'"+str(p)+"'" for p in ps_scripts))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_hardware.ps1').read_text());assert len(sources)==len(set(sources))==155
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/name) for name in sources],check=True)
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,newly_registered_entries=4,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps_scripts),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=155,preserved_baseline_commit=baseline,preserved_original_source_and_proof_files=preserved,preserved_original_registry_entries=len(previous),preserved_research_build_files=build_preserved,sha256={str(p):digest(p) for p in scripts+ps_scripts})
 (P/'analysis/movie_hardware_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})

if __name__=='__main__':main()
