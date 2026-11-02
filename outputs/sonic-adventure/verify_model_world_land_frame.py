"""Carry an unpatched original prepared frame on the new land-capable CPU DLL."""
import hashlib,json,pickle
from verify_model_world_land_api_v2 import normal,sha,P,W

def main():
 target=P/'analysis/model_world_land_frame_verification.json';assert not target.exists()
 scene,q=normal();old=scene.c['pixel_job_seed'];records=[]
 for entry,args in [(0x8c09878c,(0,)),(0x8c09878c,(5,)),(0x8c6050b0,()),(0x8c04fe40,())]:
  result,path=scene.call(entry,args,compare=q.fast.compare)
  assert result['status']==0 and scene.usable and result['full_state_match'] and result['ordered_boundaries_match']
  records.append(dict(entry=hex(entry),arguments=list(args),status=0,steps=len(path),full_state_match=True,ordered_boundaries_match=True))
 seed=scene.c['pixel_job_seed'];assert seed.summary()['processed']==4 and old.summary()['processed']==3
 assert not scene.c['graphics_in'].packets and scene.c['ta_list_seed']['last_count']==189
 assert scene.c['render_completion_seed']['completed']==4 and scene.c['render_completion_seed']['pending']==7
 assert int.from_bytes(scene.m[0x754ef0:0x754ef2],'little')==0
 folder=W/'model-world-land-normal';folder.mkdir(exist_ok=False)
 raw=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5);(folder/'normal_public_return.pkl').write_bytes(raw)
 meta=dict(status='matched_original_prepared_frame_on_land_capable_DLL_not_populated_level',checkpoint_sha256=hashlib.sha256(raw).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,completed_native_jobs=4,retired_packets=189,active_packets=0,render_pending=7,world_candidate_count=0,phases=scene.records)
 (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
 from PIL import Image
 Image.frombytes('RGBA',(640,480),seed.pixels,'raw','BGRA').save(folder/'native_fourth_frame.png')
 files=[P/'verify_model_world_land_frame.py',P/'model_world_land_verified_state.py',q.v.effects.DLL]
 report=dict(status='verified_unpatched_original_prepared_frame_on_313_source_DLL',original_native_comparisons=4,normal_returns=4,expected_host_guard_stops=0,cases=records,actual_new_native_jobs=1,inherited_completed_jobs=3,completed_jobs=4,retired_packets=189,active_packets=0,render_pending=7,world_candidates=0,geometry_globals_or_callee_results_patched=False,manual_preview_translation=False,new_land_callback_scheduled=False,real_populated_level_qualified=False,overall_completion_percent=10,goal_paused=False,normal_checkpoint_sha256=meta['checkpoint_sha256'],normal_provenance_sha256=sha(folder/'provenance.json'),native_job=seed.summary(),sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Fourth prepared native frame verified; 189 retired packets, no populated level.',flush=True)
if __name__=='__main__':main()
