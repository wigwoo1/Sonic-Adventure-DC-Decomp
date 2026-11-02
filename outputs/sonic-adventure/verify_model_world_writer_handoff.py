"""Explicit missing FPU guard input and populated standalone writer/query handoff."""
import hashlib,json
from pathlib import Path
from verify_model_world_writers import normal,clone,setup,read,reference_position,P,W,COUNT,NEAR_COUNT,LAND_COUNT

def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
 target=P/'analysis/model_world_writer_handoff_verification.json';assert not target.exists()
 first=P/'analysis/model_world_writers_verification.json';proof=json.loads(first.read_text())
 for name,digest in proof['sha256'].items():assert sha(name)==digest,name
 base,q=normal();guard=clone(base,q)
 guard.f.fpscr=(guard.f.fpscr&~0x380000)|0x280000
 assert (guard.f.fpscr>>21)&1==1 and guard.f.fpscr&0x80000
 stopped,path=guard.call(0x8c051a16,compare=q.fast.compare)
 assert stopped['status']==-1 and stopped['stop_pc']=='0x8c051a30' and not guard.usable
 scene=clone(base,q);setup(scene,1,(1,0,2));position=reference_position(scene)
 collected,first_path=scene.call(0x8c052518,compare=q.fast.compare)
 assert collected['status']==0 and scene.usable and read(scene,COUNT,2)==3
 assert read(scene,NEAR_COUNT)==1 and read(scene,LAND_COUNT)==2
 import struct
 bank=(scene.f.fpscr>>21)&1
 for index,value in enumerate((*position,1.)):
  scene.f.banks[bank][4+index]=int.from_bytes(struct.pack('<f',value),'little')
 queried,last_path=scene.call(0x8c0522c0,compare=q.fast.compare)
 assert queried['status']==0 and scene.usable and read(scene,COUNT,2)==3
 assert read(scene,NEAR_COUNT)==1 and read(scene,LAND_COUNT)==2
 assert scene.c['pixel_job_seed']==base.c['pixel_job_seed']
 assert scene.c['ta_list_seed']==base.c['ta_list_seed']
 report=dict(status='verified_FR1_reached_guard_and_declared_populated_writer_query_handoff',
  original_native_comparisons=3,normal_returns=2,expected_host_guard_stops=1,
  explicit_FR1_PR1_guard=dict(input_FPSCR=hex((base.f.fpscr&~0x380000)|0x280000),status=-1,stop_pc=stopped['stop_pc'],steps=len(path),faulted_state_resumed=False),
  declared_API_handoff=dict(writer='0x8c052518',query='0x8c0522c0',writer_steps=len(first_path),query_steps=len(last_path),added_objects=1,land_entries=2,candidates=3,input_buffers_are_standalone_fixture=True),
  initial_report_metadata_correction="The initial report's second reset_reached_PR_guard case labels bank_input=1, but its source sets only PR on the original bank-0 scene. It is another FR0 guard case. This supplement explicitly sets FR1 and PR1. Both initial normal FR/SZ combinations were configured correctly. Initial source/report bytes are preserved.",
  original_report_preserved=True,full_state_and_ordered_boundaries_match=True,
  real_gameplay_or_populated_level_qualified=False,normal_scene_checkpoint_from_fixture_saved=False,
  completed_jobs_and_retired_generation_unchanged=True,new_original_routine_credit=0,
  overall_completion_percent=10,goal_paused=False,
  sha256={str(path):sha(path) for path in (Path(__file__),first,P/'verify_model_world_writers.py',q.v.effects.DLL)})
 target.write_text(json.dumps(report,indent=2)+'\n');print('FR1 GUARD AND POPULATED DECLARED WRITER/QUERY HANDOFF VERIFIED',len(first_path),len(last_path),flush=True)
if __name__=='__main__':main()
