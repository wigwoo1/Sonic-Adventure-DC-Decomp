"""Freeze scoped original weld lifecycle and same-owner canonical data proofs."""
import argparse,ast,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
from build_native_event_pose import comdat_any
from build_native_player_weld import layout_source
from package_native_sound_placement import prior
from rewrite_sprite_float_operations import rewrite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def linked(b,name):return next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name==name)
def main():
 ap=argparse.ArgumentParser()
 for n in ('workspace','upstream','baseline','runtime','build','inputs','weld','tree','init','blend','frames','output'):ap.add_argument('--'+n,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();runtime=a.runtime.resolve();build=a.build.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');old=read(base/'build_verification.json');rt=read(runtime/'build_verification.json');b=read(build/'build_verification.json');bh=sha(build/'build_verification.json');previous=p/'analysis/jp_native_event_pose_package.json';seen=set();prior(r,previous,seen);assert len(seen)==38
 for meta in (old,rt,b):
  for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h
  for key,value in meta.items():
   if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==meta[key+'_sha256']
 assert rt['player_weld_baseline_sha256']==sha(base/'build_verification.json') and b['sonic_weld_data_baseline_sha256']==sha(runtime/'build_verification.json');recipe=rt['player_weld_recipe'];originals={}
 for n,h in recipe['original_source_sha256'].items():
  raw=(u/n).read_bytes();assert hashlib.sha256(raw).hexdigest()==h and raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n]);originals[n]=raw
 body=owner_function(originals['src/player/player.c'],'player_update_welds');assert hashlib.sha256(body).hexdigest()==recipe['original_function_sha256']['player_update_welds'];states=[]
 for n in ('player_weld_matrix_a','player_weld_matrix_b','player_weld_inverse_a','player_weld_inverse_b'):
  rows=re.findall(rb'^static NJS_MATRIX '+n.encode()+rb';\r?\n',originals['src/player/player.c'],re.M);assert len(rows)==1;states+=rows
 assert [hashlib.sha256(x).hexdigest() for x in states]==recipe['original_private_matrix_definition_sha256'];expected=b'#include <ninja.h>\n#include <sg_xpt.h>\n#include "draw/pose.h"\n#include "player/player.h"\n#include "system/task.h"\n#include "system/util.h"\n#define GCC 1\n#include "jp_native_sprite_numeric.h"\n'+b''.join(states)+body.replace(b'struct task_work *work,',b'struct task_work *work __attribute__((unused)),').replace(b'struct task_motion *motion,',b'struct task_motion *motion __attribute__((unused)),');assert expected==(runtime/'external_player_update_welds.c').read_bytes()
 expected_tree=b'#include <ninja.h>\nstatic NJS_OBJECT *tree_find_node(NJS_OBJECT *,NJS_OBJECT *,int *);\n'+owner_function(originals['src/system/util.c'],'tree_node_index')+owner_function(originals['src/system/util.c'],'tree_find_node');assert expected_tree==(runtime/'external_tree_index.c').read_bytes()
 x=subprocess.run(rt['commands'][0]['command'],capture_output=True);assert not x.returncode and not x.stderr;adapted,stats=rewrite(expected,json.loads(x.stdout),['player_update_welds']);assert adapted==(runtime/'external_player_update_welds_rz.c').read_bytes() and stats==recipe['scalar_arithmetic_adaptation'] and stats['total_operators_rewritten']==9;assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(runtime/'player_update_welds.ll').read_text());names,layout=layout_source();assert layout==(runtime/'external_weld_layouts.c').read_text() and len(names)==recipe['layout_constants_per_ABI']==62
 for n,h in recipe['generated_source_sha256'].items():assert sha(runtime/n)==h
 for n,h in recipe['new_object_sha256'].items():assert sha(runtime/n)==h
 for meta,count in ((rt,7),(b,2)):
  for row in meta['commands'][:count]:
   assert row['exit_code']==row['diagnostic_bytes']==0
   if 'response_file' in row:assert sha(row['response_file'])==row['response_file_sha256'] and shlex.split(Path(row['response_file']).read_text())==row['command'][1:]
 before={n for n in linked(old,'jp_native_frames.dll') if n.endswith('.obj')};after={n for n in linked(rt,'jp_native_frames.dll') if n.endswith('.obj')};assert not before-after and after-before=={str(runtime/'player_update_welds.obj'),str(runtime/'tree_index.obj')} and {n:sha(n) for n in before}==rt['player_weld_kept_frame_objects_sha256'];providers={s['name'] for n in before for s in coff_symbols(Path(n)) if s['section']>0};defined=set(providers);undefined=set()
 for n in sorted(after-before):
  symbols=coff_symbols(Path(n));duplicate={s['name'] for s in symbols if s['section']>0}&defined
  for sym in symbols:
   if sym['name'] in duplicate:assert re.fullmatch('__real@[0-9a-f]+',sym['name']) and comdat_any(Path(n),sym['section'])
  defined|={s['name'] for s in symbols if s['section']>0};undefined|={s['name'] for s in symbols if s['section']==0}
 assert not undefined-defined-{'_fltused'}
 assert {x.name for x in base.glob('*.dll') if sha(x)!=sha(runtime/x.name)}=={'jp_native_frames.dll'} and {x.name for x in runtime.glob('*.dll') if sha(x)!=sha(build/x.name)}=={'jp_native_motion_data.dll'}
 old_objects={n for n in linked(rt,'jp_native_motion_data.dll') if n.endswith('.obj')};new_objects={n for n in linked(b,'jp_native_motion_data.dll') if n.endswith('.obj')};assert len(old_objects-new_objects)==len(new_objects-old_objects)==1 and all(Path(n).name=='models_with_toes.obj' for n in (old_objects-new_objects)|(new_objects-old_objects));oldsource=Path(next(iter(old_objects-new_objects))).with_suffix('.c');assert sha(oldsource)==b['sonic_weld_data_old_source_sha256'];old_names=re.findall(r'^\{"([^"\n]+)",',oldsource.read_text(),re.M);now_names=re.findall(r'^\{"([^"\n]+)",',(build/'models_with_toes.c').read_text(),re.M);assert len(old_names)==2876 and now_names[:2876]==old_names==b['sonic_weld_data_old_record_names'] and len(now_names)==3322 and len(set(now_names))==3322 and len(b['sonic_weld_data_added_registry'])==446;assert sha(build/'models_with_toes.c')==b['sonic_weld_data_generated_source_sha256'] and b['sonic_weld_data_weld_table_rows']==32 and len(b['sonic_weld_data_object_roots_added'])==26
 inputs=read(a.inputs/'inputs.json');assert inputs['actual_supplied_JP_disc_module_verified'] and inputs['source_script_sha256']==sha(p/'jp_port_tools/prepare_native_sonic_weld_inputs.py') and b['sonic_weld_data_module_inputs_sha256']==sha(a.inputs/'inputs.json')
 for n,row in inputs['files'].items():assert sha(a.inputs/n)==row['sha256']
 reader=Path(rt['commands'][0]['command'][0]).with_name('llvm-readobj.exe')
 def pe(flag,path):
  x=subprocess.run([str(reader),flag,str(path)],capture_output=True);assert not x.returncode and not x.stderr;return set(re.findall(r'^  Name: (.+)$',x.stdout.decode(),re.M))
 assert pe('--coff-exports',rt['frames_DLL'])==pe('--coff-exports',old['frames_DLL'])|{'player_update_welds','tree_node_index'} and pe('--coff-imports',rt['frames_DLL'])==pe('--coff-imports',old['frames_DLL']);assert pe('--coff-exports',b['motion_DLL'])==pe('--coff-exports',rt['motion_DLL']) and pe('--coff-imports',b['motion_DLL'])==pe('--coff-imports',rt['motion_DLL'])
 evidence={'runtime_build':runtime/'build_verification.json','build':build/'build_verification.json','inputs':a.inputs/'inputs.json'};reports={}
 for n,tool in (('weld','verify_native_sonic_weld_lifecycle.py'),('tree','verify_native_weld_tree_index.py'),('init','verify_native_game_init.py'),('blend','verify_native_action_link_poses.py'),('frames','verify_native_action_link_frames.py')):
  path=getattr(a,n)/'verification.json';v=read(path);assert v['build_verification_sha256']==bh and v['verifier_sha256']==sha(p/'jp_port_tools'/tool);evidence[n]=path;reports[n]=v
 weld=reports['weld'];tree=reports['tree'];assert weld['checks']==454265 and len(weld['original_calls'])==144 and weld['canonical_records']==3322 and weld['player_weld_direct_pose_verified'] and weld['all_weld_kinds_OFF_BOTH_TO_A_TO_B_and_six_MARK_outputs_exercised'];assert len(weld['records'])==12 and {(x['super_active'],x['variant'],x['bank']) for x in weld['records']}=={(s,v,k) for s in (False,True) for v in ('genuine','both','to_b') for k in (0,1)} and all(x['all_canonical_records_restored_before_owner_shutdown'] for x in weld['records']);assert tree['checks']==10952 and tree['original_complete_calls']==5475
 assert reports['init']['checks']==530 and reports['blend']['checks']==519 and reports['frames']['checks']==382;frames=reports['frames'];assert frames['actual_window_presents']==10 and frames['texture_Off_backend_bindings_and_uploads_zero']
 for row in frames['screenshots']+frames['rendered_blend_samples']+[frames['animated_preview']]:assert sha(row['path'])==row['sha256']
 d.mkdir();audit=dict(status='original_weld_sources_RZ_same_owners_canonical_index_preservation_actual_disc_inputs_and_regressions_audited',build_verification_sha256=bh,auditor_sha256=sha(__file__),prior_public_packages_verified=38,all_previous_Frame_objects_preserved=True,all_previous_model_indices_preserved=2876,canonical_records=3322,model_records_appended=446,original_weld_rows=32,ordinary_and_Super_stand_actions_shared_with_same_model_owner=True,new_runtime_exports=['player_update_welds','tree_node_index'],module_inputs_are_actual_supplied_JP_disc_data=True,old_DLLs_changed_only_in_explicit_Frame_then_Motion_steps=True,original_source_or_proofs_modified=False,production_player_event_queue_or_gameplay_verified=False,playable=False,overall_completion_percent=10);(d/'audit.json').write_text(json.dumps(audit,indent=2)+'\n');evidence['audit']=d/'audit.json'
 files=['jp_port_tools/'+n for n in ('prepare_native_sonic_weld_inputs.py','build_native_player_weld.py','build_native_sonic_weld_data.py','verify_native_sonic_weld_lifecycle.py','verify_native_weld_tree_index.py','qualify_native_sonic_weld.py')]+['JP_NATIVE_SONIC_WELD.txt'];target=p/'analysis/jp_native_sonic_weld_package.json';assert not target.exists()
 for n in files:
  if n.endswith('.py'):ast.parse((p/n).read_text())
 proof={}
 for n,path in evidence.items():
  to=p/'analysis'/('jp_native_sonic_weld_'+n+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
 new=weld['checks']+tree['checks'];assert new==465217;out=dict(status='original_Sonic_Super_Sonic_weld_make_update_free_direct_pose_and_tree_index_scoped_qualification',new_checks=new,selected_regression_checks=1431,combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+new,original_weld_and_heap_calls=144,original_tree_index_calls=5475,original_weld_rows=32,canonical_records=3322,previous_model_indices_preserved=2876,new_model_records_appended=446,actual_Frame_heap_allocations_and_complete_coalescing_verified=True,all_canonical_bytes_restored_before_Motion_shutdown=True,ordinary_and_Super_stand_actions_verified=True,valid_generic_kind_variants_are_not_default_Sonic_config=True,all_six_marker_outputs_exercised=True,blended_player_anim_state_or_heap_exhaustion_verified=False,event_queue_execution_and_production_player_task_texture_lifetimes_verified=False,original_player_callback_FSM_mainloop_or_gameplay_verified=False,actual_GPU_regression_Presents=10,texture_Off_zero_bindings_and_uploads=True,prior_qualifications_preserved={'event_pose':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in files},proof_files_sha256=proof,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],private_motion_DLL_sha256=b['motion_DLL_sha256'],retail_generated_external_source_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_build_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True);(d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n');shutil.copyfile(d/'qualification.json',target);print(out['status'],new,'new checks;1431 regressions',flush=True)
if __name__=='__main__':main()
