"""Reproduce proposals, check unbounded C and preserve earlier public evidence."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/n for n in ('source/sa_movie_packet_startup.c','analysis/movie_packet_startup_translation.json','source/sa_movie_packet_startup_connected.c','source/movie_packet_startup_probe.c','build_movie_packet_startup.ps1','analysis/movie_packet_startup_connection.json','source/sa_movie_packet_flash_connected.c','source/movie_packet_flash_probe.c','build_movie_packet_flash.ps1','analysis/movie_packet_flash_connection.json')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_packet_startup.py','connect_movie_packet_startup.py','connect_movie_packet_flash.py'):
  subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_packet_flash.py')],check=True)
 assert saved==(registry.read_bytes(),status.read_bytes())
 names=('recover_movie_packet_startup.py','connect_movie_packet_startup.py','connect_movie_packet_flash.py','flash_storage_reference.py','verify_flash_storage.py','movie_packet_startup_reference.py','write_watch_packet_startup.py','movie_packet_flash_reference.py','write_watch_packet_flash.py','verify_movie_packet_flash.py','verify_movie_packet_flash_errors.py','verify_movie_packet_flash_retained.py','capture_movie_packet_flash_startup.py','capture_movie_packet_flash_configured_startup.py','qualify_movie_packet_flash.py','register_movie_packet_flash.py','verify_movie_packet_flash_workflow.py')
 scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_packet_startup_translation','movie_packet_startup_connection','movie_packet_flash_connection','movie_packet_flash_verification','movie_packet_flash_errors_verification','movie_packet_flash_retained_verification','movie_packet_flash_qualification','flash_storage_verification','write_watch_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='595c630f7c6a2392eb2029dee143dca4732feef0';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'))
 mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in manifest['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1405
  elif key=='recovery_coverage_audit':continue
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1405 if field=='recovered_buildable_routines' else data),field
  else:assert now[key]==value,key
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines']
 assert len(old_registry)==1397 and len(current)==1405 and current[:1397]==old_registry
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 assert preserved==12653 and len(builds)==2625
 bridge=(P/'source/sa_movie_packet_flash_connected.c').read_text()
 marker='\n/* Explicit mutable host storage; older ABIs retain read-only console policy. */\n';assert bridge.count(marker)==1
 bridge=bridge.split(marker)[0].removeprefix('#include "flash_storage.h"\n')
 addition=' if(target==SA_NATIVE_FLASH_TARGET&&c->flash){uint32_t result=r[0];int rc=sa_flash_storage_service(c->flash,m,r[7],r[4],r[5],r[6],&result);if(!rc)r[0]=result;return rc;}\n'
 assert bridge.count(addition)==1;bridge=bridge.replace(addition,'')
 bridge=bridge.replace('struct sa_transfer_runtime_binding *runtime;struct sa_flash_storage *flash;};','struct sa_transfer_runtime_binding *runtime;};')
 for before,after in ((',console,0,0,0};',',console,0,0};'),(',console,exception,0,0};',',console,exception,0};'),(',console,exception,&runtime_memory,0};',',console,exception,&runtime_memory};')):
  assert bridge.count(before)==1;bridge=bridge.replace(before,after)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected'):
  bridge=bridge.replace('sa_movie_packet_flash_'+suffix,'sa_movie_packet_startup_'+suffix)
 assert bridge==(P/'source/sa_movie_packet_startup_connected.c').read_text()
 ps=[P/n for n in ('build_movie_packet_startup.ps1','build_movie_packet_flash.ps1','build_flash_storage.ps1')]+sorted((P/'ghidra').glob('export_movie_packet*.ps1'))+[P/'ghidra/export_movie_post_flash_frontier.ps1']
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_packet_flash.ps1').read_text());assert len(sources)==len(set(sources))==196
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=10,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=196,preserved_baseline_commit=baseline,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1397,preserved_build_files=len(builds),prior_195_source_bridge_retained_except_explicit_storage_context_and_dispatch=True,newly_registered_entries=8,qualified_new_rows=610,qualified_new_conditional_outcomes=98,partial_settings_unregistered=True,scope='423fresh direct and711fresh retained original/native cases;338authored storage cases. Actual storage bytes,CRC,append,compaction and failure state;all1011integrated rows execute. Settings64bddc113rows/25of28outcomes remains partial. Explicit20-argument storage entry preserves17/18/19ABIs. This workflow does not claim completion of ongoing startup captures or of the full game. Prior public canonical/source/proof/build bytes and requirements preserved.',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_packet_flash_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
