"""Original prepared-scene world draw with natively loaded STG00 data."""
import hashlib,json,pickle
from pathlib import Path
from stage_world_render_gate_verified_state import normal,sha,P,W

def read(scene,a,n=4):return int.from_bytes(scene.m[a&0xffffff:(a&0xffffff)+n],'little')
def main():
 target=P/'analysis/stage_world_render_chain_verification.json';assert not target.exists()
 scene,q=normal();main=(W/'extracted/1ST_READ.BIN').read_bytes();records=[];reached=set();stopped=None;saved=[]
 def save(name):
  assert scene.usable and scene.s.pc==scene.s.pr==0x8cfed000
  folder=W/name;folder.mkdir(exist_ok=False);raw=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5)
  (folder/'normal_public_return.pkl').write_bytes(raw)
  meta=dict(status='matched_prepared_scene_with_original_loaded_stage_data_not_full_game',checkpoint_sha256=hashlib.sha256(raw).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,completed_native_jobs=scene.c['pixel_job_seed'].summary()['processed'],retired_packets=scene.c['ta_list_seed']['last_count'],active_packets=len(scene.c['graphics_in'].packets),render_pending=scene.c['render_completion_seed']['pending'],world_candidate_count=read(scene,0x8c754ef0,2),drawn_land_count=read(scene,0x8c75a308,2),loaded_shared_bank='STG00.BIN',shared_bank_code_execution_qualified=False,manual_geometry_globals_or_callee_result_patch=False,phases=scene.records)
  (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n');saved.append(dict(folder=str(folder),checkpoint_sha256=meta['checkpoint_sha256'],provenance_sha256=sha(folder/'provenance.json')))
 def call(name,entry,args=()):
  nonlocal stopped
  result,path=scene.call(entry,args,compare=q.fast.compare);reached.update(path)
  shared=sorted({a for a in path if 0x900000<=(a&0xffffff)<0xa00000})
  assert not shared,('Shared-bank executable dispatch remains unqualified',shared[:8])
  changed=[]
  for a in set(path):
   o=a&0xffffff
   if 0x10000<=o<=0x10000+len(main)-2 and bytes(scene.m[o:o+2])!=main[o-0x10000:o-0x10000+2]:changed.append(hex(a))
  assert not changed,('Reached main code differs from original image',changed[:8])
  row=dict(name=name,entry=hex(entry),arguments=list(args),status=result['status'],stop_pc=result['stop_pc'],steps=len(path),full_state_match=result['full_state_match'],ordered_boundaries_match=result['ordered_boundaries_match'],boundaries=result['boundaries'],shared_bank_code_PCs=shared,main_code_byte_mismatches=changed,partial_command_packets=len(result['command_packets']),drawn_land_count=read(scene,0x8c75a308,2))
  records.append(row);print(name,result['status'],result['stop_pc'],len(path),'packets',row['partial_command_packets'],'drawn',row['drawn_land_count'],flush=True)
  if result['status']!=0:assert not scene.usable;stopped=row;return False
  assert scene.usable;return True
 if call('original_camera_level0',0x8c09878c,(0,)):
  save('stage-world-render-camera-normal')
  if call('original_world_level1',0x8c09878c,(1,)):
   save('stage-world-render-world-normal')
   if call('original_particles_level5',0x8c09878c,(5,)) and call('original_frame',0x8c6050b0) and call('original_query',0x8c04fe40):
    save('stage-world-render-frame-normal')
    from PIL import Image
    Image.frombytes('RGBA',(640,480),scene.c['pixel_job_seed'].pixels,'raw','BGRA').save(W/'stage-world-render-frame-normal/native_stage_frame.png')
 coverage=[]
 for stem in ['model_stage_resources_translation','stage_world_render_gate_translation']:
  for row in json.loads((P/('analysis/'+stem+'.json')).read_text())['routines']:
   addresses={int(a,16) for a in row['reachable_addresses']};coverage.append(dict(address=row['address'],rows=len(addresses),reached=len(addresses&reached),unreached=[hex(a) for a in sorted(addresses-reached)]))
 files=[Path(__file__),P/'stage_world_render_gate_verified_state.py',P/'stage_world_render_gate_reference.py',P/'analysis/stage_world_render_gate_translation.json',P/'analysis/model_stage_resource_chain_verification.json',q.v.effects.DLL]
 report=dict(status='verified_prepared_world_draw_to_recorded_boundary_not_full_game',original_native_comparisons=len(records),normal_returns=sum(x['status']==0 for x in records),host_stops=sum(x['status']!=0 for x in records),cases=records,stopped_at=stopped,normal_checkpoints=saved,static_instruction_coverage=coverage,loaded_stage_data_from_original_native_file_and_PRS_path=True,geometry_globals_or_callee_results_patched=False,manual_preview_translation=False,faulted_state_resumed_or_saved=False,whole_stage_load_or_start_executed=False,all_task_levels_or_real_player_qualified=False,shared_bank_executable_dispatch_qualified=False,all_paths_qualified=False,playable=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('WORLD RENDER CHAIN SEALED:',len(records),'comparisons;',len(saved),'normal states',flush=True)
if __name__=='__main__':main()
