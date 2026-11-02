"""Original partial-state RAM bounds and opaque-pointer copy behavior."""
import json
from pathlib import Path
from verify_model_world_writers import normal,clone,setup,put,read,P,COUNT,OUT,ADDED,LAND_PTR
from model_world_writers_verified_state import sha

def main():
 target=P/'analysis/model_world_writer_pointer_guard_verification.json';assert not target.exists()
 base,q=normal();cases=[]
 for kind in ('owner','model','land_table','land_entries'):
  scene=clone(base,q)
  setup(scene,1 if kind in ('owner','model') else 0,() if kind in ('owner','model') else (1,))
  if kind=='owner':put(scene,ADDED+8,0x8cfffff0)
  if kind=='model':put(scene,0x8ce00000+4,0x8cfffff0)
  if kind=='land_table':put(scene,LAND_PTR,0x8cfffff8)
  if kind=='land_entries':put(scene,0x8ce90000+12,0x8cffffe0)
  result,path=scene.call(0x8c052518,compare=q.fast.compare)
  assert result['status']==-3 and not scene.usable
  assert result['full_state_match'] and result['ordered_boundaries_match']
  cases.append(dict(input=kind,status=-3,steps=len(path),stop_pc=result['stop_pc'],
                    declared_standalone_API_input=True,partial_full_state_match=True,
                    ordered_boundaries_match=True,faulted_state_resumed=False))
 scene=clone(base,q);setup(scene,3)
 opaque=[(0xffffffff,0,0x80000000),(2,0xffffffff,0),(1,0xdeadbeef,0xffffffff)]
 for i,record in enumerate(opaque):
  for j,value in enumerate(record):put(scene,ADDED+12*i+4*j,value)
 result,path=scene.call(0x8c052e30,compare=q.fast.compare)
 assert result['status']==0 and scene.usable and read(scene,COUNT,2)==3
 for i,record in enumerate(reversed(opaque)):
  assert tuple(read(scene,OUT+12*i+4*j) for j in range(3))==record
 assert result['full_state_match'] and result['ordered_boundaries_match']
 cases.append(dict(input='collect_all_copies_opaque_pointer_bits',status=0,steps=len(path),
                   pointer_values_dereferenced_or_validated=False,declared_standalone_API_input=True,
                   full_state_match=True,ordered_boundaries_match=True))
 report=dict(status='verified_world_writer_reached_RAM_guards_and_opaque_copy_values',
  original_native_comparisons=5,normal_returns=1,expected_host_guard_stops=4,cases=cases,
  faulted_states_resumed=False,physical_bus_fault_or_exception_claim=False,
  populated_game_level_qualified=False,new_original_routine_credit=0,
  overall_completion_percent=10,goal_paused=False,
  sha256={str(path):sha(path) for path in (Path(__file__),P/'verify_model_world_writers.py',q.v.effects.DLL)})
 target.write_text(json.dumps(report,indent=2)+'\n');print('WORLD POINTER GUARDS AND OPAQUE COPIES VERIFIED: 5 comparisons',flush=True)
if __name__=='__main__':main()
