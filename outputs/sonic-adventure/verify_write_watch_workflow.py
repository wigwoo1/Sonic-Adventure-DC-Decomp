"""Preserve all native/source/proof bytes while qualifying the new opt-in tool."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
BASE='bd7a7953c8fa21d15ce7147aa90b6fa83030b93b'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 names=('windows_write_watch.py','verify_windows_write_watch.py','write_watch_driver_control.py','verify_write_watch_driver_control.py','capture_write_watch_prefix.py','verify_write_watch_prefix.py','qualify_write_watch.py','verify_write_watch_workflow.py','inventory_movie_bulk_family.py')
 scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 inventory=P/'analysis/movie_bulk_family_inventory.json';before_inventory=digest(inventory)
 subprocess.run([sys.executable,'-B',str(P/'inventory_movie_bulk_family.py')],check=True);assert digest(inventory)==before_inventory
 ps=P/'ghidra/export_movie_bulk_family.ps1'
 command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('"+str(ps).replace("'","''")+"',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 pins={}
 for name in ('windows_write_watch_verification','write_watch_driver_control_verification','write_watch_prefix_verification','write_watch_qualification','movie_driver_control_qualification','movie_bulk_family_inventory'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 git=r'C:\Program Files\Git\cmd\git.exe';checkout=ROOT/'publication/sonic-adventure'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASE+':'+path])
 old_manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json'}
 mutable=[r['path'] for r in old_manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==3
 for row in old_manifest['files']:
  if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 old=json.loads(historical('outputs/sonic-adventure/project_status.json'));current=json.loads((P/'project_status.json').read_text())
 for key,value in old.items():
  if key=='verified_progress':
   for field,data in value.items():assert current[key][field]==data,field
  else:assert current[key]==value,key
 assert len(json.loads((P/'analysis/recovered_routines.json').read_text())['routines'])==1379
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  build=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in build:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_driver_control.ps1').read_text());assert len(sources)==len(set(sources))==179
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(python_scripts_parsed=len(scripts),powershell_scripts_parsed=1,next_family_inventory_reproduced=True,pinned_inputs_current=len(pins),strict_native_sources_checked=179,native_dll_rebuilt=False,preserved_baseline_commit=BASE,preserved_prior_selected_files=len(old_manifest['files'])-3,reviewed_mutable_paths=mutable,requirements_and_all_prior_status_fields_preserved=True,registry_bytes_unchanged=True,preserved_build_files=len(build),newly_registered_game_routines=0,scope='Verification-only opt-in page checker. All prior selected files except README/handoff/status retain exact bytes; the routine registry, native source, all proof files and all build bytes are unchanged. Strict compiler diagnostics cover unchanged 179-source C pipeline without overwriting frozen binaries. Unit/direct/full-startup gates are separate pinned reports; the complete accumulated historical suite is not rerun.',sha256={str(p):digest(p) for p in scripts+[ps]})
 (P/'analysis/write_watch_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
