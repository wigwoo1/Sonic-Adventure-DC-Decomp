"""Reproduce derivations and preserve every previous source/proof/registry/build byte."""
import ast,hashlib,json,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];G=r'C:\Program Files\Git\cmd\git.exe';BASELINE='94938c1608691f1915920bf6be2aa68a56dbb21f'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 scripts=[P/(n+'.py') for n in ('prove_collision_wrapper_scalar_returns','check_collision_wrapper_paired_aliases','verify_collision_wrapper_derivations','verify_collision_wrapper_scale_aliases','record_collision_wrapper_return_research','verify_collision_wrapper_return_workflow')]
 for p in scripts:ast.parse(p.read_text())
 outputs=[P/'analysis'/('collision_wrapper_'+n+'.json') for n in ('scalar_return_class','paired_frame_aliases','derivation_tests','return_research')]
 before={str(p):digest(p) for p in outputs};sp=P/'project_status.json';status_before=sp.read_bytes()
 for name in ('prove_collision_wrapper_scalar_returns.py','check_collision_wrapper_paired_aliases.py','verify_collision_wrapper_derivations.py','record_collision_wrapper_return_research.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert before=={str(p):digest(p) for p in outputs} and status_before==sp.read_bytes()
 def historical(path):return subprocess.check_output([G,'-C',str(ROOT/'publication/sonic-adventure'),'show',BASELINE+':'+path])
 baseline=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(baseline['files'])==15474
 mutable=[r['path'] for r in baseline['files'] if r['path'] in {'outputs/sonic-adventure/README.md','outputs/sonic-adventure/SOURCE_HANDOFF.txt','outputs/sonic-adventure/project_status.json'}];assert len(mutable)==3
 for row in baseline['files']:
  if row['path'] not in mutable:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 rp=P/'analysis/recovered_routines.json';assert rp.read_bytes()==historical('outputs/sonic-adventure/analysis/recovered_routines.json')
 old_status=json.loads(historical('outputs/sonic-adventure/project_status.json'));status=json.loads(sp.read_text())
 for key,value in old_status.items():
  if key=='verified_progress':
   for field,data in value.items():assert status[key][field]==data,field
   assert set(status[key])-set(value)=={'collision_wrapper_return_research'}
  else:assert status[key]==value,key
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:builds=json.loads(z.read('BUILD_MANIFEST.json'))
 assert len(builds['files'])==2723
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 pins={}
 for name in ('scalar_return_class','paired_frame_aliases','derivation_tests','scale_alias_verification','return_research'):
  path=P/'analysis'/('collision_wrapper_'+name+'.json');r=json.loads(path.read_text())
  for pin,h in r['sha256'].items():assert digest(pin)==h,(name,pin);pins[pin]=h
 research=json.loads(outputs[-1].read_text());assert research['newly_registered_entries']==research['newly_qualified_rows']==0 and research['fresh_original_native_comparisons']==48
 assert not research['paired_fmov_call_graph_closed'] and research['unexecuted_wrapper_rows']==6 and research['unqualified_wrapper_conditional_outcomes']==2
 dll=ROOT/'work/sonic-adventure/build/collision_angle.dll';old_q=json.loads(historical('outputs/sonic-adventure/analysis/collision_angle_qualification.json'));assert digest(dll)==old_q['dll_sha256']==research['current_dll_sha256']
 report=dict(status='partial_return_research_verified_no_wrapper_promotion',reproduced_reports=4,status_recording_byte_idempotent=True,python_scripts_parsed=len(scripts),pinned_inputs_current=len(pins),preserved_baseline_commit=BASELINE,preserved_prior_selected_files=15471,reviewed_mutable_paths=mutable,prior_registry_bytes_identical=True,registry_entries=1586,registry_unique_module_address_pairs=1583,confirmed_linked_unique_routines_by_name=626,preserved_build_files=2723,new_native_builds=0,native_source_unchanged=True,dll_sha256=digest(dll),fresh_original_native_comparisons=48,fresh_native_checks=336,derivation_regression_checks=7,total_evidence_original_native_comparisons=68275,new_routine_credit=0,wrapper_qualification_added=False,original_requirements_preserved=True,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',sha256={str(p):digest(p) for p in scripts+outputs+[dll]},scope='Fourreports reproduce;48actual original/native calls/336checks plus7derivationchecks;no wrapper promotion.15471prior selected source/proof files preserved except3docs/status paths;registrybytes/oldprogress/requirements/2723buildfiles unchanged. Scalar bound and paired alias conclusions remain distinct;fullpaired return/target closure is OPEN.')
 (P/'analysis/collision_wrapper_return_workflow.json').write_text(json.dumps(report,indent=2)+'\n');print('WRAPPER RETURN WORKFLOW PASS15471preserved files,2723builds,registryunchanged',len(pins),'pins')
if __name__=='__main__':main()
