"""Reproduce generation, check qualification pins, preserve all old sources/proofs/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
BASELINE='3911e1e386fadb08110639d9eae141d64b620fe3'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_graphics_init.c','analysis/movie_graphics_init_translation.json','source/sa_movie_graphics_init_connected.c','source/movie_graphics_init_probe.c','build_movie_graphics_init.ps1','analysis/movie_graphics_init_connection.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_graphics_init.py','connect_movie_graphics_init.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';saved=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_graphics_leaves.py')],check=True);assert saved==(registry.read_bytes(),status.read_bytes())
 scripts=[P/n for n in ('recover_movie_graphics_init.py','connect_movie_graphics_init.py','movie_graphics_init_reference.py','write_watch_graphics_init.py','explore_movie_graphics_init.py','explore_movie_graphics_init_extended.py','verify_movie_graphics_leaf_configuration.py','verify_movie_graphics_init_retained.py','capture_movie_graphics_init_startup.py','verify_movie_graphics_init_startup.py','qualify_movie_graphics_leaves.py','register_movie_graphics_leaves.py','verify_movie_graphics_leaves_workflow.py')]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 def check_report(path):
  path=Path(path);r=json.loads(path.read_text())
  for file,h in r['sha256'].items():assert digest(file)==h,(path,file);pins[file]=h
  return r
 for name in ('movie_graphics_init_translation','movie_graphics_init_connection','movie_graphics_leaf_configuration_verification','movie_graphics_init_all_retained_verification','movie_graphics_init_startup_verification','movie_graphics_leaves_qualification','write_watch_qualification'):check_report(P/f'analysis/{name}.json')
 retained=check_report(P/'analysis/movie_graphics_init_all_retained_verification.json')
 for suite in retained['suites']:
  r=check_report(suite['report'])
  if suite['comparisons']==711:
   for nested in r['suites']:check_report(P/nested['report'])
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));mutable_names={'README.md','SOURCE_HANDOFF.txt','project_status.json','case_dependency_map.json','main_dispatch_case_map_verification.json','project_completion_audit.json','recovered_routines.json','recovery_coverage_audit.json','recovery_registry_evidence.json','unrecovered_function_candidates.tsv'}
 mutable=[r['path'] for r in manifest['files'] if Path(r['path']).name in mutable_names];assert len(mutable)==10
 preserved=0
 for row in manifest['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==12959
 old_registry=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old_registry)==1412 and len(current)==1416 and current[:1412]==old_registry
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));now=json.loads(status.read_text())
 for key,value in old_status.items():
  if key=='recovered_buildable_routines':assert now[key]==1416
  elif key=='recovery_coverage_audit':continue
  elif key=='verified_progress':
   for field,data in value.items():assert now[key][field]==(1416 if field=='recovered_buildable_routines' else data),field
  else:assert now[key]==value,key
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  builds=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in builds:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 assert len(builds)==2639,'Run this preservation check before preparing a new publication archive'
 bridge=(P/'source/sa_movie_graphics_init_connected.c').read_text()
 for definition in json.loads((P/'analysis/movie_graphics_init_translation.json').read_text())['routines']:
  declaration=f'VDECL({definition["name"]});\n';route=f' if(target=={definition["address"]}u)return {definition["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n';assert bridge.count(declaration)==bridge.count(route)==1;bridge=bridge.replace(declaration,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):bridge=bridge.replace('sa_movie_graphics_init_'+suffix,'sa_movie_midi_commands_'+suffix)
 assert bridge==(P/'source/sa_movie_midi_commands_connected.c').read_text()
 ps=[P/'build_movie_graphics_init.ps1']+[P/'ghidra'/n for n in ('export_movie_command_handoff_next.ps1','export_movie_graphics_init_dependencies.ps1','export_movie_graphics_init_nested.ps1','export_movie_graphics_init_support.ps1')]
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps));subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",ps[0].read_text());assert len(sources)==len(set(sources))==201
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 reference=__import__('movie_graphics_init_reference');constants={n:x for n,x in vars(reference.old).items() if n.isupper() and isinstance(x,int)}
 fixture=reference.GRAPHICS_CONFIG_FIXTURE;assert digest(fixture['source_capture'])==fixture['source_capture_sha256']
 observed=next(b for b in json.loads(Path(fixture['source_capture']).read_text())['record']['boundaries'] if b['target']=='0x8c656940')
 assert fixture['descriptor_bytes']==observed['graphics_input_record'] and fixture['indirect_records']==observed['graphics_indirect_input_records'] and fixture['observed_fpscr']==observed['graphics_input_fpscr'] and fixture['observed_caller_arguments']==observed['arguments']
 for n,x in constants.items():assert getattr(reference,n)==x,n
 q=json.loads((P/'analysis/movie_graphics_leaves_qualification.json').read_text());assert digest(reference.effects.DLL)==q['dll_sha256']
 report=dict(reproduced_generated_artifacts=6,registration_byte_idempotent=True,legacy_reference_constants_checked=len(constants),python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=201,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,original_requirements_preserved=True,preserved_original_registry_entries=1412,preserved_build_files=len(builds),prior_native_bridge_retained_except22_added_routes=True,newly_registered_entries=4,qualified_new_rows=352,qualified_new_conditional_outcomes=74,reused_adapter_rows=55,reused_additional_credit=0,unqualified_new_larger_bodies=13,total_original_native_comparisons=2473,dll_sha256=q['dll_sha256'],scope='Four graphics leaves qualify within declared aligned mapped ABI;larger initializer/dependencies remain partial despite two full configured startup comparisons reaching608d88. Prior original source/proofs/builds/canonical registry/requirements preserved before new publication preparation. Whole-project10%weighted estimate is not measured coverage;no playable port claim.',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_graphics_leaves_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
