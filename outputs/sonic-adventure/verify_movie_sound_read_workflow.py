"""Reproduce sound-read adapters and preserve the earlier public source/proof/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_sound_read.c','analysis/movie_sound_read_translation.json','source/sa_movie_sound_read_connected.c','source/movie_sound_read_probe.c','build_movie_sound_read.ps1','analysis/movie_sound_read_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_sound_read.py','connect_movie_sound_read.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_sound_read.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 names=('recover_movie_sound_read.py','connect_movie_sound_read.py','movie_sound_read_reference.py','write_watch_sound_read.py','verify_movie_sound_read.py','verify_movie_sound_read_irq_control.py','verify_movie_sound_read_retained.py','capture_movie_sound_read_startup.py','verify_movie_sound_read_startup.py','qualify_movie_sound_read.py','register_movie_sound_read.py','verify_movie_sound_read_workflow.py');scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_sound_read_translation','movie_sound_read_connection','movie_sound_read_verification','movie_sound_read_irq_control_verification','movie_sound_read_retained_verification','movie_sound_read_startup_verification','movie_sound_read_qualification','write_watch_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='02fa8e86466cea85e7b46b0b613f51d43a0349dc';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10;preserved=0
 for row in manifest['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert now[key][field]==data,field
  else:assert now[key]==value,key
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert old_registry==current and len(current)==1397
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 assert preserved==12609 and len(builds)==2623
 bridge=(P/'source/sa_movie_sound_read_connected.c').read_text()
 for definition in json.loads((P/'analysis/movie_sound_read_translation.json').read_text())['routines']:
  declaration=f'VDECL({definition["name"]});\n';route=f' if(target=={definition["address"]}u)return {definition["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert bridge.count(declaration)==bridge.count(route)==1;bridge=bridge.replace(declaration,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected'):bridge=bridge.replace('sa_movie_sound_read_'+suffix,'sa_movie_transfer_runtime_'+suffix)
 assert bridge==(P/'source/sa_movie_transfer_runtime_connected.c').read_text()
 ps=[P/'build_movie_sound_read.ps1',P/'ghidra/export_movie_sound_read.ps1',P/'ghidra/export_movie_startup_frontier.ps1']
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",ps[0].read_text());assert len(sources)==len(set(sources))==194
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=194,preserved_baseline_commit=baseline,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1397,preserved_build_files=len(builds),prior_native_bridge_retained_except_three_added_routes=True,newly_registered_entries=0,scope='Three existing70-row/6outcome getters connected to actual10d460/SoundRAM;854fresh original/native comparisons include two full earlier-startup captures. Cold executes full8MiBVRAMclear,two full2MiBSEGAfills,two actual33376-byte transfers/IRQ/RTE completions and all1065282Soundevents before genuine64bddcfrontier. No droppedtrace/checkpoint/helperresult/shortenedcopy. Verification-only request wait counter now resets at actual invocation entry; native production behavior unchanged. Canonical fields/source/proof/build bytes and requirements preserved,zero duplicate credit.',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_sound_read_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
