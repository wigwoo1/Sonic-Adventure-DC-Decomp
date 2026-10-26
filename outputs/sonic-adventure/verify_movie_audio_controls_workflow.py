"""Reproduce audio adapters and preserve published canonical source/proof/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 generated=[P/n for n in ('source/sa_movie_audio_controls.c','analysis/movie_audio_controls_translation.json','source/sa_movie_audio_controls_connected.c','source/movie_audio_controls_probe.c','build_movie_audio_controls.ps1','analysis/movie_audio_controls_connection.json')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_audio_controls.py','connect_movie_audio_controls.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_audio_controls.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_audio_controls.py','connect_movie_audio_controls.py','movie_audio_controls_reference.py','write_watch_audio_mode.py','verify_movie_audio_controls.py','verify_movie_audio_controls_retained.py','verify_movie_audio_controls_packet_regression.py','capture_movie_audio_controls_startup.py','verify_movie_audio_controls_startup.py','qualify_movie_audio_controls.py','register_movie_audio_controls.py','verify_movie_audio_controls_workflow.py','verify_movie_audio_controls_mode_regression.py','register_movie_audio_controls_startup.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_audio_controls_translation','movie_audio_controls_connection','movie_audio_controls_verification','movie_audio_controls_retained_verification','movie_audio_controls_packet_regression_summary','movie_audio_controls_mode_regression_verification','movie_audio_controls_qualification','write_watch_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='f298a57770c855a3b12e6f563c4534ab71c3c47c';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in manifest['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1410
  elif key=='recovery_coverage_audit':continue
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1410 if field=='recovered_buildable_routines' else data),field
  else:assert now[key]==value,key
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines']
 assert len(old_registry)==1407 and len(current)==1410 and current[:1407]==old_registry
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 assert preserved==12813 and len(builds)==2633
 bridge=(P/'source/sa_movie_audio_controls_connected.c').read_text()
 for definition in json.loads((P/'analysis/movie_audio_controls_translation.json').read_text())['routines']:
  declaration=f'VDECL({definition["name"]});\n';route=f' if(target=={definition["address"]}u)return {definition["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert bridge.count(declaration)==bridge.count(route)==1;bridge=bridge.replace(declaration,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
  bridge=bridge.replace('sa_movie_audio_controls_'+suffix,'sa_movie_audio_mode_'+suffix)
 assert bridge==(P/'source/sa_movie_audio_mode_connected.c').read_text()
 ps=[P/'build_movie_audio_controls.ps1']+[P/'ghidra'/n for n in ('export_movie_audio_controls.ps1','export_movie_post_audio_controls_frontier.ps1')]
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",ps[0].read_text());assert len(sources)==len(set(sources))==198
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=198,preserved_baseline_commit=baseline,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1407,preserved_build_files=len(builds),prior_native_bridge_retained_except_three_added_routes=True,newly_registered_entries=3,qualified_new_rows=147,qualified_new_conditional_outcomes=26,shared_copy_additional_credit=0,scope='Three original command controls qualify147rows/26outcomes;1555fresh direct/retained comparisons. Old17/18/19/20ABIs,all old canonical entries/source/proof/build bytes and requirements retained. Reused support gets zero credit;startup is separate;no ARM/audio/gameplay claim.',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_audio_controls_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
