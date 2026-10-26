"""Reproduce native integration and preserve the complete earlier public snapshot."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_transfer_runtime_connected.c','source/movie_transfer_runtime_probe.c','build_movie_transfer_runtime.ps1','analysis/movie_transfer_runtime_connection.json')];before={str(p):digest(p) for p in generated}
 subprocess.run([sys.executable,'-B',str(P/'connect_movie_transfer_runtime.py')],check=True);assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';old_bytes=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_transfer_runtime.py')],check=True);assert old_bytes==(registry.read_bytes(),status.read_bytes())
 names=('connect_movie_transfer_runtime.py','movie_transfer_runtime_reference.py','transfer_runtime_reference.py','transfer_runtime_reference_cpu.py','write_watch_transfer_runtime.py','verify_transfer_runtime.py','verify_movie_transfer_runtime.py','verify_movie_transfer_runtime_regression.py','verify_movie_transfer_runtime_compatibility.py','verify_movie_transfer_request_closure.py','qualify_movie_transfer_runtime.py','register_movie_transfer_runtime.py','verify_movie_transfer_runtime_workflow.py');scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_transfer_runtime_connection','transfer_runtime_verification','movie_transfer_runtime_verification','movie_transfer_runtime_regression_verification','movie_transfer_runtime_compatibility_verification','movie_transfer_request_closure_verification','movie_transfer_runtime_qualification','write_watch_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='803bca358c1eb4ec18b70984501db1c20b35af28';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
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
 old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert old==current and len(current)==1396
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 assert preserved==12574 and len(builds)==2621
 text=(P/'source/sa_movie_transfer_runtime_connected.c').read_text().split('\n/* Persistent platform state is supplied explicitly, never a guest checkpoint. */\n')[0]
 text=text.replace('sa_movie_transfer_runtime','sa_movie_interrupt_context').replace('#include "transfer_runtime.h"\n','')
 text=text.replace(';struct sa_transfer_runtime_binding *runtime;};',';};').replace('console,0,0};','console,0};').replace('console,exception,0};','console,exception};')
 start=text.index('static int dispatch(void*,uint32_t,');end=text.index('static int dispatch(void *opaque,',start);text=text[:start]+text[end:]
 new_route=' if(target==0x8c666e02u){int status=sa_movie_queue_family_8c666e02(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);return status?status:runtime_poll(c,m,f,sr,r,pr);}'
 old_route=' if(target==0x8c666e02u)return sa_movie_queue_family_8c666e02(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);'
 assert text.count(new_route)==1;text=text.replace(new_route,old_route);assert text==(P/'source/sa_movie_interrupt_context_connected.c').read_text()
 ps=P/'build_movie_transfer_runtime.ps1';command="$ErrorActionPreference='Stop';$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(ps)
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==193
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=4,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=1,pinned_inputs_current=len(pins),strict_unbounded_sources_checked=193,preserved_baseline_commit=baseline,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1396,preserved_build_files=len(builds),recovered_source_and_bridge_routes_preserved=True,newly_registered_entries=0,scope='193-source persistent platform connects real data and original IRQ/context return inside original REQUEST.699original/native comparisons,32continuous returned requests,59authored platform cases; no duplicate game recovery credit. Request own139rows execute but4outcomes remain unverified. All older source/proof/build bytes,canonical fields and requirements preserved; no fresh early-caller/full-copy/video-clear claim.',sha256={str(p):digest(p) for p in scripts+[ps]})
 (P/'analysis/movie_transfer_runtime_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
