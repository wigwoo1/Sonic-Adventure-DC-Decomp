"""Original stored camera draw, state-2 update and the next native frame job."""
import hashlib,json,pickle,struct
from pathlib import Path
from model_camera_position_tail_verified_state import normal,sha,P,W

def bits(x):return int.from_bytes(struct.pack('<f',x),'little')
def tasks(scene,q,level):
 heads=q.get(scene.m,0x8c0987e0);address=q.get(scene.m,heads+4*level);seen=set();out=[]
 while address:
  assert address not in seen and len(out)<1024;seen.add(address)
  out.append(dict(task=address,callback=q.get(scene.m,address+0x10),draw=q.get(scene.m,address+0x14),work=q.get(scene.m,address+0x20)))
  address=q.get(scene.m,address)
 return out
def main():
 target=P/'analysis/camera_draw_state2_sequence_verification.json';assert not target.exists()
 manifests=['model_camera_draw_translation.json','model_camera_draw_update_translation.json','model_camera_history_vectors_translation.json','model_camera_state2_translation.json','model_camera_position_tail_translation.json'];entries=[]
 for name in manifests:
  report=json.loads((P/'analysis'/name).read_text());entries+=report['routines']
  for f,h in report['sha256'].items():assert sha(f)==h,(name,f)
 assert len(entries)==16 and sum(r['instruction_rows'] for r in entries)==1830
 units=[]
 for mode in ('separate','alias','overlap','invalid_source'):
  scene,q=normal();base=0x8ce00000;source=0x8ce01000;dest=base
  if mode=='separate':values=[1.0,2.0,3.0];other=[0.5,-2.0,0.25];expected=[0.5,4.0,2.75]
  elif mode=='alias':values=[1.0,2.0,3.0];other=values;source=dest;expected=[0.0,0.0,0.0]
  elif mode=='overlap':
   values=[1.0,2.0,4.0,8.0];other=None;source=base;dest=base+4;expected=[1.0,2.0,4.0]
  else:values=[1.0,2.0,3.0];other=None;source=0x8cfffffc;expected=None
  scene.m[base&0xffffff:(base&0xffffff)+4*len(values)]=struct.pack('<'+'f'*len(values),*values)
  if other is not None and source!=dest:scene.m[source&0xffffff:(source&0xffffff)+12]=struct.pack('<3f',*other)
  result,path=scene.call(0x8c63a7e4,[dest,source],compare=q.fast.compare)
  if expected is not None:
   assert result['status']==0 and scene.usable and len(path)==13
   assert [q.get(scene.m,dest+4*i) for i in range(3)]==[bits(x) for x in expected]
   assert scene.s.r[4]==dest and scene.s.r[5]==source+8
  else:assert result['status']==-3 and result['stop_pc']=='0x8c63a7e8' and len(path)==3 and not scene.usable
  units.append(dict(mode=mode,status=result['status'],stop_pc=result['stop_pc'],steps=len(path),full_state_match=result['full_state_match'],ordered_boundaries_match=result['ordered_boundaries_match'],declared_external_buffer_inputs=True,faulted_state_resumed=False))
 scene,q=normal();camera=tasks(scene,q,0);assert len(camera)==1
 camera=camera[0];assert camera['draw']==0x8c01c296 and camera['callback']==0x8c0197ba
 assert q.get(scene.m,0x8c18b7ec)==camera['work'] and scene.m[camera['work']&0xffffff]==2
 assert len(tasks(scene,q,5))==6 and q.get(scene.m,0x8c754ef0)==0
 old_job=scene.c['pixel_job_seed'];old_retired=scene.c['ta_list_seed'];old_pending=scene.c['render_completion_seed']['pending'];assert old_pending==7
 executed=set()
 def call(entry,args=()):
  result,path=scene.call(entry,args,compare=q.fast.compare);executed.update(path)
  assert result['status']==0 and scene.usable and result['full_state_match'] and result['ordered_boundaries_match']
  return result,path
 drawn,path=call(camera['draw'],[camera['task']]);assert len(path)==460
 assert scene.c['pixel_job_seed']==old_job and scene.c['ta_list_seed']==old_retired and not scene.c['graphics_in'].packets
 updated,path=call(0x8c09878c,[0]);assert len(path)==3232 and len(tasks(scene,q,0))==1 and len(tasks(scene,q,5))==6
 reached={b['target'] for b in updated['boundaries']};assert {'0x8c01b500','0x8c63a7e4','0x8c01b554','0x8c01995e','0x8c01bd9e','0x8c01b436','0x8c01b3e0','0x8c01c2d8','0x8c01a5ae'}<=reached
 assert scene.c['pixel_job_seed']==old_job and scene.c['ta_list_seed']==old_retired
 particles,path=call(0x8c09878c,[5]);assert len(path)==19789 and len(tasks(scene,q,5))==6 and particles['ta_current_packets']==186 and particles['ta_stream_pages']==3
 assert sum(b['target']=='0x8c6215d6' for b in particles['boundaries'])==6
 frame,path=call(0x8c6050b0);assert len(path)==8319
 queued=scene.c['pixel_job_seed'];queued_summary=queued.summary()
 assert queued_summary['job_id']==2 and queued_summary['processed']==1 and queued_summary['ready']==1 and queued_summary['queued_packets']==189
 assert frame['ta_current_packets']==0 and frame['ta_retired_pages']==3 and frame['ta_list_out']['last_count']==189 and frame['ta_list_out']['discarded']==0
 assert frame['render_completion_out']['submitted']==2 and frame['render_completion_out']['completed']==1 and frame['render_completion_out']['pending']==0
 acknowledged=0
 for _,address,value,width in frame['ack_writes']:
  if address&0x1fffffff==0x5f6900 and width==4:acknowledged|=value&7
 assert acknowledged==7,'the original IRQ path acknowledges the previous real job'
 finished,path=call(0x8c04fe40);assert len(path)==5
 summary=finished['pixel_job_summary'];output=scene.c['pixel_job_seed']
 assert summary['job_id']==summary['processed']==summary['last_job']==2 and summary['ready']==0 and summary['queued_packets']==189
 assert tuple(summary['stats'])==(60,1346,1312,0,1312)
 assert output.packets==queued.packets and finished['render_completion_out']['completed']==2 and finished['render_completion_out']['pending']==7
 repeated,path=call(0x8c04fe40);assert len(path)==5 and repeated['pixel_job_summary']==summary and scene.c['pixel_job_seed']==output
 assert scene.c.get('backend_completed_job',0)==0 and q.get(scene.m,0x8c754ef0)==0
 folder=W/'camera-draw-state2-normal';folder.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5);(folder/'normal_public_return.pkl').write_bytes(payload)
 meta=dict(status='matched_original_camera_state2_and_second_native_frame_normal_return',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,camera={k:hex(v) for k,v in camera.items()},candidate_count=0,submitted_packets=189,retired_packets=189,retired_pages=3,active_packets=0,completed_native_jobs=2,render_pending=7,phases=scene.records)
 (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
 from PIL import Image
 image=folder/'native_second_frame.png';Image.frombytes('RGBA',(640,480),output.pixels,'raw','BGRA').save(image)
 coverage=[dict(address=r['address'],static_rows=r['instruction_rows'],rows_reached_in_actual_sequence=len(executed&{int(a,16) for a in r['reachable_addresses']})) for r in entries]
 files=[Path(__file__),P/'model_camera_position_tail_verified_state.py',P/'build_model_camera_position_tail.ps1',q.v.effects.DLL]+[P/'analysis'/name for name in manifests]
 for name in ('model_camera_position_tail_reference','model_camera_position_tail_fixtures','write_watch_model_camera_position_tail','write_watch_model_camera_position_tail_base','write_watch_model_camera_position_tail_state','write_watch_model_camera_position_tail_capture'):files.append(P/(name+'.py'))
 report=dict(status='verified_reached_original_camera_draw_state2_helpers_and_next_native_frame',original_native_comparisons=10,normal_returns=9,matched_invalid_pointer_stops=1,actual_sequence_normal_comparisons=len(scene.records),declared_vector_API_cases=units,new_static_entry_bodies=16,new_static_instruction_rows=1830,actual_sequence_static_row_coverage=coverage,camera={k:hex(v) for k,v in camera.items()},candidate_table_count=0,populated_camera_candidate_paths_qualified=False,stored_draw_steps=460,state2_update_steps=3232,particle_pass_steps=19789,next_frame_steps=8319,next_frame_packets=189,actual_new_native_jobs_completed=1,inherited_completed_jobs=1,total_completed_jobs=2,native_job_summary=summary,old_completion_bits_acknowledged_by_original_path=True,manual_acknowledgment_or_state_clear=False,manual_preview_translation=False,all_six_original_particles_retained=True,camera_or_game_callee_results_injected=False,external_completion_tokens=0,faulted_state_resumed=False,render_pending_after_query=7,full_game_or_main_frame_loop_qualified=False,playable=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,private_native_image_sha256=sha(image),phases=[{k:v for k,v in r.items() if k!='boundaries'}|dict(ordered_targets=[b['target'] for b in r['boundaries']]) for r in scene.records],sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('CAMERA DRAW / STATE2 / NEXT NATIVE FRAME VERIFIED',summary,flush=True)
if __name__=='__main__':main()
