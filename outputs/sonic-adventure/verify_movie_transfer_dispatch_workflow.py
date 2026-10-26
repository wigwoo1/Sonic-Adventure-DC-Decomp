"""Reproduce request-state integration and retain previous source/proofs/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_transfer_dispatch.c','analysis/movie_transfer_dispatch_translation.json','source/sa_movie_transfer_dispatch_connected.c','source/movie_transfer_dispatch_probe.c','build_movie_transfer_dispatch.ps1','analysis/movie_transfer_dispatch_connection.json')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_transfer_dispatch.py','connect_movie_transfer_dispatch.py'):
  subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';before_register=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_transfer_dispatch.py')],check=True);assert before_register==(registry.read_bytes(),status.read_bytes())
 names=('recover_movie_transfer_dispatch.py','connect_movie_transfer_dispatch.py','movie_transfer_dispatch_reference.py','write_watch_transfer_dispatch.py','write_watch_transfer_dispatch_sound.py','verify_movie_transfer_dispatch.py','verify_movie_transfer_dispatch_lifecycle.py','verify_movie_transfer_dispatch_regression.py','qualify_movie_transfer_dispatch.py','register_movie_transfer_dispatch.py','verify_movie_transfer_dispatch_workflow.py')
 scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_transfer_dispatch_translation','movie_transfer_dispatch_connection','movie_transfer_dispatch_verification','movie_transfer_dispatch_lifecycle_verification','movie_transfer_dispatch_regression_verification','movie_transfer_dispatch_qualification','write_watch_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='2c018bf7ae70fc05bea67a046c85eae814c045e3';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 old_manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in old_manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 for row in old_manifest['files']:
  path=row['path']
  if path in mutable:continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now_status=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now_status[key]==1394
  elif key=='verified_progress':
   for field,data in value.items():assert now_status[key][field]==(1394 if field=='recovered_buildable_routines' else data),field
  else:assert now_status[key]==value,key
 old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old)==1392 and len(current)==1394
 index={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in old:
  now=index[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for key,value in row.items():assert now[key]==value,(row['address'],key)
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  build=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in build:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 ps=[P/'build_movie_transfer_dispatch.ps1',P/'ghidra/export_movie_transfer_dispatch.ps1']
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_transfer_dispatch.ps1').read_text());assert len(sources)==len(set(sources))==190
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=len(generated),registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=len(sources),preserved_baseline_commit=baseline,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=len(old),preserved_build_files=len(build),newly_registered_entries=2,scope='New190-source DLL; original generic/primary/secondary/ASIC dispatch141rows and22outcomes executed;137direct+72lifecycle+352regression comparisons. Existing authored G2 support performs actual data movement with independent byte checks in24cases; no recovered-code credit for host support. Two new bodies,104rows; two existing dispatch bodies,37rows,zero duplicatecredit. Lifecycle explicitly carries register masks/data and supplies successful software pending bit after real copy. No physical exception/IRQ timing,automatic runtime pumping or fresh earliest-caller/full-copy/video-clear proof claimed. All older source/proof/build bytes,canonical fields and user requirements preserved except ten reviewed derived paths',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_transfer_dispatch_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
