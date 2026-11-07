"""Freeze sampled complete original event queues on the unchanged weld build."""
import argparse,ast,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from package_native_sound_placement import prior
from rewrite_sprite_float_operations import rewrite
import verify_native_event_queue as event_suite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
 ap=argparse.ArgumentParser()
 for name in ('workspace','upstream','build','events','output'):ap.add_argument('--'+name,type=Path,required=True)
 a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');previous=p/'analysis/jp_native_sonic_weld_package.json';seen=set();prior(r,previous,seen);assert len(seen)==39;old=read(previous);b=read(a.build/'build_verification.json');bh=sha(a.build/'build_verification.json');published_build=p/'analysis/jp_native_sonic_weld_build.json';assert bh==old['build_verification_sha256']==sha(published_build)
 for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
 for key,value in b.items():
  if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']
 recipe=b['event_pose_recipe']['units']['event_draw'];original=(a.upstream/'src/event/event.c').read_bytes();assert original==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(a.upstream),'show',PIN+':src/event/event.c']) and hashlib.sha256(original).hexdigest()==b['event_pose_recipe']['original_source_sha256']['src/event/event.c']
 command=next(row['command'] for row in b['commands'] if '-ast-dump=json' in row['command'] and Path(row['command'][-1]).name=='external_event_draw.c');folder=Path(command[-1]).parent;raw=(folder/'external_event_draw.c').read_bytes();assert owner_function(raw,'event_motions_draw')==owner_function(original,'event_motions_draw')
 x=subprocess.run(command,capture_output=True);assert not x.returncode and not x.stderr;adapted,stats=rewrite(raw,json.loads(x.stdout),['event_motions_draw']);assert adapted==(folder/'external_event_draw_rz.c').read_bytes() and stats==recipe['scalar_arithmetic_adaptation']
 for n,h in recipe['generated_source_sha256'].items():assert sha(folder/n)==h
 assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(folder/'event_draw.ll').read_text());assert sha(folder/'event_draw.obj')==recipe['object_sha256']
 link=next(row['command'] for row in b['commands'] if '-shared' in row['command'] and Path(row['command'][-1]).name=='jp_native_frames.dll');assert str(folder/'event_draw.obj') in link
 value=read(a.events/'verification.json');derived,parent_hash=event_suite.derive();assert value['verifier_sha256']==sha(event_suite.__file__) and value['parent_weld_verifier_sha256']==parent_hash and value['derived_source_sha256']==sha(a.events/'derived_event_queue_caller.py') and (a.events/'derived_event_queue_caller.py').read_text()==derived and value['build_verification_sha256']==bh
 assert value['checks']==1019169 and value['original_complete_event_calls']==288 and value['original_entry_frees']==36 and value['original_player_weld_calls']==176
 names={'no_event':1,'empty':1,'normal':3,'end_next':2,'repeat':2,'repeat_next':2,'blend4':5,'blend_repeat4':5,'blend1':2,'previous_no_blend':1};records=value['event_records'];assert len(records)==288
 for super_active in (False,True):
  for bank in (0,1):
   for slot in (None,0,7):
    for name,count in names.items():
     rows=[v for v in records if v['super_active']==super_active and v['bank']==bank and v['slot']==slot and v['scenario']==name];assert [v['step'] for v in rows]==list(range(count))
     for row in rows:
      early=name in ('no_event','empty');assert row['visible_models']==(0 if early else 28);assert row['original_palette_calls']==([] if early else [2,0]);assert len(row['original_player_weld_calls'])==(0 if early or slot is None else 1)
      for call in row['original_player_weld_calls']:assert call[0]==slot
      if not early:assert {'njGetkmPointer','njSetkmPointer'}<=set(row['original_calls_seen'])
 assert value['all_other_work_event_and_live_entry_bytes_unchanged'] and value['all_canonical_model_and_weld_payload_bytes_compared_after_every_step'] and value['actual_entries_and_weld_allocations_fully_retired'] and value['caller_matrix_stack_preserved'] and not value['original_game_function_hooks_used'] and value['empty_texture_list_selection_only'] and value['model_rasterization_skipped_by_actual_original_frame_step_policy']
 regressions={}
 for name,count in (('init',530),('blend',519),('frames',382)):
  path=p/'analysis'/('jp_native_sonic_weld_'+name+'.json');v=read(path);assert v['checks']==count and v['build_verification_sha256']==bh;regressions[path.relative_to(r).as_posix()]=sha(path)
 d.mkdir();audit=dict(status='pinned_complete_event_body_RZ_native_object_derived_fixture_case_matrix_and_identical_build_regression_proofs_audited',build_verification_sha256=bh,auditor_sha256=sha(__file__),prior_packages_verified=39,native_build_unchanged_since_qualified_welding=True,original_function_body_sha256=hashlib.sha256(owner_function(original,'event_motions_draw')).hexdigest(),native_event_object_sha256=recipe['object_sha256'],scenario_families=10,actor_or_player_roles=3,model_forms=2,FPU_banks=2,original_game_function_hooks_used=False,rasterization_skipped_by_original_policy=True,production_actor_callback_or_gameplay_verified=False,playable=False,overall_completion_percent=10);(d/'audit.json').write_text(json.dumps(audit,indent=2)+chr(10))
 files=['jp_port_tools/verify_native_event_queue.py','jp_port_tools/qualify_native_event_queue.py','JP_NATIVE_EVENT_QUEUE.txt']
 for name in files:
  if name.endswith('.py'):ast.parse((p/name).read_text())
 target=p/'analysis/jp_native_event_queue_package.json';assert not target.exists();proof={}
 for name,path in (('events',a.events/'verification.json'),('audit',d/'audit.json')):
  to=p/'analysis'/('jp_native_event_queue_'+name+'.json');assert not to.exists();shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
 out=dict(status='sampled_original_event_queue_transitions_welding_pose_state_and_actual_entry_heap_free_scoped_qualification',new_checks=1019169,selected_regression_checks=1431,selected_regressions_reused_from_identical_native_build=True,combined_qualified_scoped_checks=old['combined_qualified_scoped_checks']+1019169,original_complete_event_calls=288,original_entry_frees=36,original_player_weld_calls=176,scenario_families=10,player_slots=[0,7],nonplayer_actor_cases=True,ordinary_and_Super_Sonic=True,both_SH4_register_banks=True,all_model_bytes_weld_payloads_live_entry_bytes_other_caller_bytes_and_guards_checked=True,palette_reset_and_texture_list_selection_verified=True,texture_selection_uses_valid_empty_list_only=True,model_rasterization_skipped_by_original_frame_step_policy=True,native_build_unchanged_since_qualified_welding=True,event_actor_step_ground_shadow_collision_original_Sonic_callback_or_gameplay_verified=False,render_FPS_decoupling_or_full_game_timing_verified=False,arbitrary_blend_durations_speeds_invalid_inputs_or_heap_exhaustion_verified=False,prior_qualifications_preserved={'sonic_weld':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in files},proof_files_sha256=proof,identical_native_build=dict(path=published_build.relative_to(r).as_posix(),sha256=sha(published_build)),selected_regression_proof_sha256=regressions,build_verification_sha256=bh,private_frames_DLL_sha256=b['frames_DLL_sha256'],retail_generated_source_captures_or_combined_DLLs_publishable=False,original_source_and_proofs_preserved=True,public_research_payloads_changed=False,playable=False,overall_completion_percent=10,goal_active=True)
 assert out['combined_qualified_scoped_checks']==2982524;(d/'qualification.json').write_text(json.dumps(out,indent=2)+chr(10));shutil.copyfile(d/'qualification.json',target);print(out['status'],'1019169 new checks;288 original event calls',flush=True)
if __name__=='__main__':main()
