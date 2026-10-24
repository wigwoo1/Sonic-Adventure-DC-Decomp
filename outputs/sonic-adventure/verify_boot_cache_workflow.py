"""Check pinned evidence, deterministic generation and original-file preservation."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/p for p in ('source/sa_boot_cache.c','analysis/boot_cache_translation.json','source/sa_boot_cache_connected.c','analysis/boot_cache_connection.json','build_boot_cache.ps1')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_boot_cache.py','connect_boot_cache.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';old=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_boot_cache.py')],check=True)
 assert old==(registry.read_bytes(),status.read_bytes())
 scripts=[P/name for name in ('recover_boot_cache.py','connect_boot_cache.py','verify_boot_cache.py','verify_boot_cache_prefix.py','verify_boot_cache_backend.py','qualify_boot_cache.py','register_boot_cache.py','verify_boot_cache_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('boot_cache_translation','boot_cache_connection','boot_cache_verification','boot_cache_backend_verification','boot_cache_prefix_verification','boot_cache_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():
   assert digest(path)==h,path
   if path in pins:assert pins[path]==h,path
   pins[path]=h
 # The published baseline is immutable Git history; never refresh old hashes
 # from changed current source and mistake that for preservation.
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe';baseline='33ffac5403adbbec653af83be01f35580b7f10e5'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
 for row in manifest['files']:
  path=row['path'];qualifies='/source/' in path or ('/analysis/' in path and 'verification' in Path(path).name)
  if not qualifies or path.endswith('/analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines']
 lookup={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in old_registry:
  now=lookup[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for k,value in row.items():assert now[k]==value,(row['address'],k)
 build_preserved=0
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  for name in z.namelist():
   if name.startswith('work/'):
    assert (ROOT/name).read_bytes()==z.read(name),name;build_preserved+=1
 ps_scripts=list((P/'ghidra').glob('export_boot_cache*.ps1'))+[P/'build_boot_cache.ps1',P/'build_and_verify.ps1'];assert len(ps_scripts)==3
 # Parse the PowerShell workflows; compile all 147 C sources with the original
 # unbounded branches retained, independent of the bounded research DLL.
 command="""$ErrorActionPreference='Stop'
 $taskScripts=@(%s)
 foreach($taskPath in $taskScripts){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}
 Write-Output 'Three PowerShell workflows parsed'
 """%(','.join("'"+str(p)+"'" for p in ps_scripts))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_boot_cache.ps1').read_text());assert len(sources)==len(set(sources))==147
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/name) for name in sources],check=True)
 files=scripts+ps_scripts
 report=dict(reproduced_generated_artifacts=5,registration_byte_idempotent=True,newly_registered_entries=1,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps_scripts),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=147,preserved_baseline_commit=baseline,preserved_original_source_and_proof_files=preserved,preserved_original_registry_entries=len(old_registry),preserved_research_build_files=build_preserved,sha256={str(p):digest(p) for p in files})
 (P/'analysis/boot_cache_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))

if __name__=='__main__':main()
