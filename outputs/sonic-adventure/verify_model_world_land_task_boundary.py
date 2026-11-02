"""Preserve the original stage-style callback prefix at its actual resource guard."""
import json
from verify_model_world_land_api_v2 import normal,clone,read,sha,P

def main():
 target=P/'analysis/model_world_land_task_boundary_verification.json';assert not target.exists()
 base,q=normal();scene=clone(base,q);records=[];reached=set()
 for name,entry,args,status in [('task_create',0x8c09852e,(2,1,0x8c0519c0),0),('scheduler_level1',0x8c09878c,(1,),-3)]:
  result,path=scene.call(entry,args,compare=q.fast.compare);reached.update(path)
  assert result['status']==status and scene.usable==(status==0)
  assert result['full_state_match'] and result['ordered_boundaries_match']
  records.append(dict(name=name,entry=hex(entry),arguments=list(args),status=status,steps=len(path),stop_pc=result['stop_pc'],boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True))
 assert result['stop_pc']=='0x8c0521b2'
 assert scene.s.r[14]==0xfffb2f86 and (scene.s.r[14]+0x1c)&0xffffffff==0xfffb2fa2
 assert scene.c['pixel_job_seed']==base.c['pixel_job_seed'] and scene.c['ta_list_seed']==base.c['ta_list_seed']
 report=dict(status='verified_original_callback_prefix_to_actual_resource_RAM_guard',
  original_native_comparisons=2,normal_returns=1,expected_host_guard_stops=1,cases=records,
  reached_addresses=[hex(a) for a in sorted(reached)],
  selected_land_pointer=hex(read(scene,0x8c7596f4)),faulting_read='0xfffb2fa2',
  faulting_instruction='mov.l @(0x1c,r14),r2',
  interpretation='The selected table has an out-of-RAM entry pointer in this prepared input context. The complete stage/resource producer has not run. This is a matched boundary stop, not successful world rendering.',
  task_create_arguments_from_original_stage_start=True,whole_stage_start_executed=False,
  geometry_globals_or_callee_results_patched=False,faulted_state_resumed=False,
  faulted_state_saved=False,original_source_bytes_saved_as_assets=False,
  original_header_file_correspondence_unresolved=True,
  next_original_stage_loader='8c04e490',next_caller_reference='8c04e540',
  real_populated_level_qualified=False,new_original_routine_credit=0,
  overall_completion_percent=10,goal_paused=False,
  sha256={str(f):sha(f) for f in [P/'verify_model_world_land_api_v2.py',P/'model_world_land_verified_state.py',P/'verify_model_world_land_task_boundary.py',q.v.effects.DLL]})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Original callback prefix matched through 8c0521b2; guarded state discarded.',flush=True)
if __name__=='__main__':main()
