"""Qualify additional player/land decisions without fabricating mid-call results."""
import json
from pathlib import Path
from verify_model_world_writers import normal,clone,setup,read,put,floats,reference_position,P,W,COUNT,NEAR_COUNT,LAND_COUNT,LAND_PTR,ENABLED
from model_world_writers_verified_state import sha

def main():
 target=P/'analysis/model_world_writer_coverage_verification.json';assert not target.exists()
 initial=P/'analysis/model_world_writers_verification.json';proof=json.loads(initial.read_text())
 for name,digest in proof['sha256'].items():assert sha(name)==digest,name
 base,q=normal();reached=set();records=[]
 retained=[('near_plain',1,(),{}),('near_turned',1,(),dict(turned=True)),
           ('near_far',1,(),dict(far=True)),('near_ineligible',1,(),dict(eligible=False)),
           ('near_boundary_equal',1,(),dict(threshold=450.)),('near_boundary_inside',1,(),dict(threshold=449.)),
           ('near_second_player',1,(),dict(dual=True)),('near_land',0,(1,0,2),{}),('near_capacity',129,(),{})]
 extra=[('only_first_player',1,(),{}),('only_second_player',1,(),{}),
        ('land_second_player',0,(1,),dict(dual=True)),('land_far',0,(1,),{}),
        ('land_capacity',0,(1,)*129,{}),('land_disabled',0,(1,),{}),('land_enabled_two',0,(1,),{}),
        ('combined',1,(1,2),{}),('land_negative_count',0,(1,),{}),
        ('land_boundary_equal',0,(1,),{}),('land_boundary_inside',0,(1,),{})]
 for kind,n,flags,options in retained+extra:
  scene=clone(base,q)
  if kind in ('only_first_player','only_second_player'):
   players=read(scene,0x8c052608);work=0x8ce80000
   put(scene,players,work if kind=='only_first_player' else 0)
   put(scene,players+4,work if kind=='only_second_player' else 0)
   floats(scene,work+0x20,(100.,20.,30.))
  setup(scene,n,flags,**options)
  if kind in ('land_far','land_boundary_equal','land_boundary_inside'):
   pos=reference_position(scene);offset={'land_far':10000.,'land_boundary_equal':450.,'land_boundary_inside':449.}[kind]
   floats(scene,0x8cea0000,(pos[0]+offset,pos[1],pos[2]))
  if kind in ('land_disabled','land_enabled_two'):
   put(scene,ENABLED,0 if kind=='land_disabled' else 2);put(scene,LAND_PTR,0x8cfffff0)
  if kind=='land_negative_count':put(scene,0x8ce90000,0xffff,2)
  result,path=scene.call(0x8c052518,compare=q.fast.compare);reached.update(path)
  assert result['status']==0 and scene.usable and result['full_state_match'] and result['ordered_boundaries_match'],kind
  added=0 if kind in ('near_far','near_ineligible','near_boundary_equal') else min(n,128)
  land=0 if kind in ('land_far','land_disabled','land_enabled_two','land_negative_count','land_boundary_equal') else min(sum(bool(flag&0x400003) for flag in flags),128)
  assert read(scene,NEAR_COUNT)==added and read(scene,LAND_COUNT)==land and read(scene,COUNT,2)==added+land,(kind,read(scene,COUNT,2),read(scene,LAND_COUNT))
  assert scene.c['pixel_job_seed']==base.c['pixel_job_seed'] and scene.c['ta_list_seed']==base.c['ta_list_seed']
  records.append(dict(case=kind,retained_case_repeated_for_coverage=kind in {x[0] for x in retained},steps=len(path),status=0,near_added=added,near_land=land,full_state_match=True,ordered_boundaries_match=True,declared_API_inputs=True))
 translation=P/'analysis/model_world_writers_translation.json';manifest=json.loads(translation.read_text())
 routine=next(row for row in manifest['routines'] if row['address']=='0x8c052518')
 static={int(pc,16) for pc in routine['reachable_addresses']};missing=sorted(static-reached)
 report=dict(status='verified_additional_world_collector_player_land_and_capacity_decisions',
  original_native_comparisons=len(records),normal_returns=len(records),expected_host_guard_stops=0,
  retained_case_repetitions=9,new_declared_API_cases=11,cases=records,
  collector_original_rows=len(static),collector_rows_reached=len(static&reached),unreached_collector_rows=list(map(hex,missing)),
  world_1024_guard_note='Entry resets candidate_count. In stable declared inputs, the original near-added and near-land caps permit at most 128+128 records. A 1024-count return is not forced with injected mid-call state; remaining rows are reported without an all-path claim.',
  real_populated_level_or_gameplay_qualified=False,geometry_or_callee_results_injected=False,
  completed_jobs_and_retired_generation_unchanged=True,faulted_state_resumed=False,
  new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,
  sha256={str(path):sha(path) for path in (Path(__file__),P/'verify_model_world_writers.py',initial,translation,q.v.effects.DLL)})
 target.write_text(json.dumps(report,indent=2)+'\n');print('WORLD COLLECTOR BRANCH CASES VERIFIED',len(records),'comparisons;',len(static&reached),'/',len(static),'rows',flush=True)
if __name__=='__main__':main()
