"""Audit/freeze the shared math, original morphs, canonical data and toe control."""
import argparse,ast,hashlib,json,re,shlex,shutil,struct,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from package_native_sound_placement import prior
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def linked(b,name):return next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name==name)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--stage',choices=('audit','package'),required=True)
 for n in ('workspace','build','math-build','morph-build','toe-build','controller-build','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
 for n in ('baseline','upstream','inputs','math','morph','data','control','frames','poses','init','audit'):ap.add_argument('--'+n,type=Path)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
 folders=[a.math_build.resolve(),a.morph_build.resolve(),a.toe_build.resolve(),a.controller_build.resolve(),a.build.resolve()];metas=[read(x/'build_verification.json') for x in folders];b=metas[-1];bh=sha(folders[-1]/'build_verification.json');previous=p/'analysis/jp_native_draw_controls_package.json';seen=set();prior(r,previous,seen);assert len(seen)==35
 for folder,meta in zip(folders,metas):
  for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h,n
  for key,value in meta.items():
   if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==meta[key+'_sha256']
 if a.stage=='audit':
  base=a.baseline.resolve();old=read(base/'build_verification.json');u=a.upstream.resolve();inputs=a.inputs.resolve();chain=[base]+folders
  keys=('morph_math_baseline_sha256','model_morph_baseline_sha256','toe_models_baseline_sha256','sonic_toes_baseline_sha256','sonic_run_data_baseline_sha256')
  for i,key in enumerate(keys):assert metas[i][key]==sha(chain[i]/'build_verification.json')
  assert sha(inputs/'inputs.json')==read(p/'analysis/jp_native_motion_data_build.json')['private_inputs_manifest_sha256']
  for key,row in read(inputs/'inputs.json')['files'].items():bank,ext=key.split('/');assert sha(inputs/bank/('content.'+ext))==row['sha256']
  originals={}
  for name in ('src/system/util.c','src/player/sonic.c','sdk/ninja/njSin.c','sdk/ninja/njUnitVector.c'):
   raw=(u/name).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+name]);originals[name]=raw
  groups=[(metas[0],'morph_math_generated_source_sha256'),(metas[1]['model_morph_recipe'],'generated_source_sha256'),(metas[2],None),(metas[3],'sonic_toes_generated_source_sha256'),(metas[4],None)]
  for folder,(meta,key) in zip(folders,groups):
   if key:
    for name,h in meta[key].items():assert sha(folder/name)==h
  assert sha(folders[2]/'models_with_toes.c')==metas[2]['toe_models_generated_source_sha256'] and sha(folders[4]/'models_with_toes.c')==metas[4]['sonic_run_data_generated_source_sha256']
  for folder,meta,n in zip(folders,metas,(4,7,2,4,2)):
   for row in meta['commands'][:n]:
    assert row['exit_code']==row['diagnostic_bytes']==0
    if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
  sound_body=owner_function((p/'native/jp_native_sound_spatial.c').read_bytes(),'njUnitVector');assert sound_body==owner_function((p/'native/jp_native_morph_math.c').read_bytes(),'njUnitVector')
  assert sound_body not in (folders[0]/'sound_spatial_shared_math.c').read_bytes()
  morph=metas[1]['model_morph_recipe'];assert morph['scalar_arithmetic_adaptation']['total_operators_rewritten']==77 and morph['residual_native_FP_operations']==0
  for name,h in morph['original_function_sha256'].items():assert hashlib.sha256(owner_function(originals['src/system/util.c'],name)).hexdigest()==h
  bodies={name:owner_function(originals['src/system/util.c'],name) for name in morph['source_functions']}
  for name in ('points_blend','points_blend_sine','points_blend_square'):
   toward=b'*toward++' if name=='points_blend_sine' else b'*toward';before=b'(keep * *src) + ('+toward+b' * blend)';assert bodies[name].count(before)==1;bodies[name]=bodies[name].replace(before,b'jp_model_morph_fmac(keep, *src, '+toward+b' * blend)')
  util=originals['src/system/util.c'];macros=util[util.index(b'#define STRIP_LENGTH'):util.index(b'void model_calc_face_normals')]
  expected=b'#include <ninja.h>\n#include "system/util.h"\n#include "jp_native_sprite_numeric.h"\n#include "jp_native_model_morph_numeric.h"\n'+macros+b'\n'.join(bodies.values());assert expected==(folders[1]/'external_model_morph.c').read_bytes()
  for stage_folder,meta,name,functions in ((folders[1],metas[1],'external_model_morph_expanded.c',morph['source_functions']),(folders[3],metas[3],'external_sonic_toes.c',['sonic_blend_toes'])):
   command=next(row['command'] for row in meta['commands'] if '-ast-dump=json' in row['command']);run=subprocess.run(command,capture_output=True);assert not run.returncode and not run.stderr
   adapted,stats=rewrite((stage_folder/name).read_bytes(),json.loads(run.stdout),functions);target_name='external_model_morph_rz.c' if len(functions)==6 else 'external_sonic_toes_rz.c';assert adapted==(stage_folder/target_name).read_bytes()
   assert stats==(morph['scalar_arithmetic_adaptation'] if len(functions)==6 else meta['sonic_toes_arithmetic_adaptation'])
  # Original FMAC sites are also directly visible in the preserved JP image.
  jp=(r/'work/sonic-adventure/jp-ninja-math-reference/1ST_READ.BIN').read_bytes()
  for name,address in morph['original_instruction_FMAC_sites'].items():assert struct.unpack_from('<H',jp,(int(address,16)&0xffffff)-0x10000)[0]&0xf00f==0xf00e
  toe=metas[3];body=owner_function(originals['src/player/sonic.c'],'sonic_blend_toes');generated=owner_function((folders[3]/'external_sonic_toes.c').read_bytes(),'sonic_blend_toes').replace(b' __attribute__((unused))',b'');assert generated==body and hashlib.sha256(body).hexdigest()==toe['sonic_toes_original_body_sha256'];assert toe['sonic_toes_arithmetic_adaptation']['total_operators_rewritten']==14 and toe['sonic_toes_original_LP32_player_layout']==[512,292,304,148,152,40,52]
  for index,prefix,expected in ((2,'toe_models',56),(4,'sonic_run_data',27)):
   meta=metas[index];names=re.findall(r'^\{"([^"\n]+)",',(folders[index]/'models_with_toes.c').read_text(),re.M);before=meta[prefix+'_old_record_names'];added=meta[prefix+'_added_registry'];assert len(added)==expected and names==before+[x['name'] for x in added]
  assert len(metas[2]['toe_models_old_record_names'])==2793 and len(metas[4]['sonic_run_data_old_record_names'])==2849
  reader=Path(b['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
  def pe(flag,path):
   run=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not run.returncode and not run.stderr;return run.stdout.decode()
  def exports(path):return set(re.findall(r'^  Name: (.+)$',pe('--coff-exports',path),re.M))
  variants=[(base,old)]+list(zip(folders,metas));expected_changes=[{'jp_native_frames.dll','jp_native_game_sound.dll'},{'jp_native_frames.dll'},{'jp_native_motion_data.dll'},{'jp_native_frames.dll'},{'jp_native_motion_data.dll'}]
  for index,((left,l),(right,m)) in enumerate(zip(variants,variants[1:])):
   changed={x.name for x in left.glob('*.dll') if sha(x)!=sha(right/x.name)};assert changed==expected_changes[index]
   if index in (0,1,3):
    earlier={x for x in linked(l,'jp_native_frames.dll') if x.endswith('.obj')};now={x for x in linked(m,'jp_native_frames.dll') if x.endswith('.obj')};assert earlier<=now and len(now-earlier)==(2 if index==1 else 1)
   else:
    earlier={x for x in linked(l,'jp_native_motion_data.dll') if x.endswith('.obj')};now={x for x in linked(m,'jp_native_motion_data.dll') if x.endswith('.obj')};assert len(earlier-now)==len(now-earlier)==1
  assert exports(b['game_sound_DLL'])==exports(old['game_sound_DLL']) and 'ForwardedTo: jp_native_frames.njUnitVector' in pe('--coff-exports',b['game_sound_DLL'])
  extra={'njSin','njUnitVector','jp_model_morph_fmac','model_blend','points_blend','points_blend_sine','points_blend_square','model_calc_face_normals','model_calc_point_normals','jp_sonic_toes_bind','jp_sonic_toes_step','jp_sonic_toes_close','jp_sonic_toes_snapshot','jp_sonic_toes_layout'};assert exports(b['frames_DLL'])==exports(old['frames_DLL'])|extra
  assert exports(b['motion_DLL'])==exports(old['motion_DLL'])
  graph={x.name.lower():[n.lower() for n in re.findall(r'^  Name: (.+)$',pe('--coff-imports',x),re.M) if (folders[-1]/n).exists()] for x in folders[-1].glob('*.dll')};active=set();done=set()
  def visit(n):
   assert n not in active;active.add(n)
   for child in graph[n]:
    if child not in done:visit(child)
   active.remove(n);done.add(n)
  for n in graph:
   if n not in done:visit(n)
  unchanged={x.name:sha(x) for x in base.glob('*.dll') if sha(x)==sha(folders[-1]/x.name)};assert len(unchanged)==21
  out=dict(status='shared_math_original_FMAC_morphs_canonical_data_and_owned_original_toe_control_chain_audited',build_verification_sha256=bh,auditor_sha256=sha(__file__),stage_build_sha256={str(x):sha(x/'build_verification.json') for x in folders},prior_packages_checked=35,original_sources_pinned_to=PIN,unchanged_DLL_sha256=unchanged,all_previous_Frame_objects_preserved=True,sound_normalizer_body_moved_unchanged_and_export_forwarded=True,canonical_record_counts=[2793,2849,2876],original_FMAC_sites=morph['original_instruction_FMAC_sites'],morph_scalar_adaptations=77,toe_control_scalar_adaptations=14,LP32_player_layout=toe['sonic_toes_original_LP32_player_layout'],actual_local_PE_import_graph=graph,playable=False,overall_completion_percent=10)
  d.mkdir();(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],'35 previous packages;21 unchanged DLLs',flush=True);return
 reports={};evidence={name:folder/'build_verification.json' for name,folder in zip(('math_build','morph_build','toe_build','controller_build','build'),folders)}
 specs={'math':('verify_native_morph_math.py',bh),'morph':('verify_native_model_morph.py',sha(folders[1]/'build_verification.json')),'data':('verify_native_toe_models.py',sha(folders[2]/'build_verification.json')),'control':('verify_native_sonic_toes.py',bh),'frames':('verify_native_sonic_toes_frames.py',bh),'poses':('verify_native_sonic_run_poses.py',bh),'init':('verify_native_game_init.py',bh),'audit':('qualify_native_sonic_toes.py',bh)}
 for name,(tool,build_hash) in specs.items():
  path=getattr(a,name)/'verification.json';v=read(path);assert v['build_verification_sha256']==build_hash and v['auditor_sha256' if name=='audit' else 'verifier_sha256']==sha(p/'jp_port_tools'/tool);evidence[name]=path;reports[name]=v
 assert [reports[n]['checks'] for n in ('math','morph','data','control','frames','poses','init')]==[67586,1400,40068,120913,406,76,530]
 assert reports['control']['original_complete_toe_control_calls']==40 and reports['poses']['original_complete_action_calls']==14 and reports['frames']['native_original_toe_control_connected_to_real_rendering']
 f=reports['frames'];assert f['canonical_records']==2876 and f['visible_models']==28 and f['preflighted_model_variants']==34 and f['actual_window_presents']==10 and f['texture_Off_backend_bindings_and_uploads_zero']
 assert f['all_immutable_model_data_and_mutation_guards_preserved'] and not f['whole_model_data_unchanged'] and not f['original_player_callback_or_gameplay_verified']
 for row in f['screenshots']+f['rendered_original_action_frames']+[f['animated_preview']]:assert sha(row['path'])==row['sha256']
 files=['native/'+n for n in ('jp_native_morph_math.c','jp_native_morph_math.h','jp_native_model_morph_numeric.c','jp_native_model_morph_numeric.h','jp_native_sonic_toes.h','jp_native_sonic_toes.inc')]+['jp_port_tools/'+n for n in ('build_native_morph_math.py','verify_native_morph_math.py','build_native_model_morph.py','verify_native_model_morph.py','build_native_toe_models.py','verify_native_toe_models.py','build_native_sonic_toes.py','verify_native_sonic_toes.py','verify_native_sonic_toes_frames.py','build_native_sonic_run_data.py','verify_native_sonic_run_poses.py','qualify_native_sonic_toes.py')]+['JP_NATIVE_SONIC_TOES.txt'];assert len(files)==19
 for name in files:
  if name.endswith('.py'):ast.parse((p/name).read_text(encoding='utf-8-sig'))
 target=p/'analysis/jp_native_sonic_toes_package.json';assert not target.exists();d.mkdir();proof={}
 for name,path in evidence.items():
  to=p/'analysis'/('jp_native_sonic_toes_'+name+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
 new=sum(reports[n]['checks'] for n in ('math','morph','data','control','frames','poses'));assert new==230449
 out=dict(status='original_Sonic_toe_control_shared_math_complete_morphs_owned_canonical_data_full_speed_task_GPU_and_texture_Off_scoped_qualification',new_checks=new,selected_regression_checks=530,combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+new,original_toe_control_calls=40,original_full_speed_pose_calls=14,original_fixture_morph_calls=700,original_genuine_toe_morph_calls=12,original_math_machine_calls=2048,all_sixteen_bit_sine_inputs=65536,canonical_records=2876,previous_record_indices_preserved=2793,player_only_records_appended=83,owned_toe_model_node_normal_views=12,native_player_bytes=600,original_LP32_player_bytes=512,visible_models=28,preflighted_model_variants=34,actual_window_presents=10,real_full_speed_action_with_original_toe_control_rendered=True,texture_Off_zero_texture_sampler_bindings_and_uploads=True,model_morph_mutations_are_intentional_and_guarded=True,diagnostic_task_destructor_retires_native_toe_owner_after_GPU_and_frame_release=True,original_player_draw_callback_FSM_mainloop_or_gameplay_verified=False,full_automatic_production_task_and_texture_ownership=False,normalizer_new_machine_comparison_scope='finite nonzero vectors with finite nonzero squared length',console_hardware_or_complete_game_timing_validation=False,prior_qualifications_preserved={'draw_controls':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={(p/name).relative_to(r).as_posix():sha(p/name) for name in files},proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],private_motion_DLL_sha256=b['motion_DLL_sha256'],retail_external_source_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
 (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new scoped assertions;530 regressions',flush=True)
if __name__=='__main__':main()
