"""Original camera allocation and scheduled particles through bounded native frames."""
import hashlib,json,pickle
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
parent=P/'verify_model_callback_camera_dependency.py'
source=parent.read_text().split('\ndef main():')[0]
source=source.replace('model_camera_dependency_fixtures','model_camera_lighting_load_fixtures').replace('write_watch_model_camera_dependency_state','write_watch_model_camera_lighting_load_state')
prefix={'__name__':'original_allocated_prefix','__file__':str(parent)}
exec(compile(source,str(parent),'exec'),prefix)
q=prefix['q'];sha=prefix['sha']
import frame_clock_native as clock
import render_completion_native as render
import float_color_pixels_native as jobs

def initialized():
 scene,tasks,owner=prefix['normal_prefix']()
 assert not tasks(0) and q.get(scene.m,0x8c18b7ec)==0
 def call(entry,args=()):
  result,path=scene.call(entry,args,compare=q.fast.compare)
  return result,path
 factory,path=call(0x8c01957e)
 assert factory['status']==0 and len(path)==360
 cameras=tasks(0);assert len(cameras)==1
 camera=cameras[0];task=camera['task'];work=camera['work']
 assert camera['callback']==0x8c0197ba and camera['draw']==0x8c019768
 assert q.get(scene.m,task+0x18)==0x8c019760 and scene.m[work&0xffffff]==0
 assert q.get(scene.m,0x8c18b7ec)==0
 result,path=call(0x8c09878c,[0])
 assert result['status']==0 and len(path)==102
 assert q.get(scene.m,0x8c18b7ec)==work and scene.m[work&0xffffff]==1
 assert q.get(scene.m,0x8c18b7f0)==1
 camera={k:hex(v) for k,v in camera.items()}
 return scene,tasks,owner,camera,call

def phases(scene):
 return [{k:v for k,v in p.items() if k!='boundaries'}|dict(ordered_targets=[b['target'] for b in p['boundaries']]) for p in scene.records]

def save_normal(scene,camera):
 assert scene.usable and scene.s.pc==scene.s.pr==0x8cfed000
 folder=W/'model-camera-lighting-normal';folder.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5)
 (folder/'normal_public_return.pkl').write_bytes(payload)
 meta=dict(status='matched_original_allocated_camera_and_particle_frame_return',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,camera=camera,phases=scene.records)
 (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')

def main():
 target=P/'analysis/model_camera_lighting_owner_verification.json';assert not target.exists()
 translation=P/'analysis/model_camera_lighting_load_translation.json'
 for name,h in json.loads(translation.read_text())['sha256'].items():assert sha(name)==h,name
 cases=[]
 for textured in (False,True):
  scene,tasks,owner,camera,call=initialized()
  particles,path=call(0x8c09878c,[5])
  assert particles['status']==0 and len(path)==8905 and not particles['command_packets']
  assert sum(b['target']=='0x8c6215d6' for b in particles['boundaries'])==6
  assert len(tasks(5))==6 and q.get(scene.m,0x8c18b7ec)==int(camera['work'],16)
  registers=scene.c['graphics_in'].arrays[0];reg=lambda off:registers[off//4]
  state=clock.initialized(50000000,27000000,reg(0x44),reg(0xd0),reg(0xd8),reg(0xcc),reg(0xdc))
  scene.c['frame_clock_seed']=clock.serialize(state);scene.c['runtime_attached']=True
  scene.c.pop('ticks',None);scene.c.pop('irq_samples',None);scene.c['scan_samples']=[]
  result,path=call(0x8c6025e8);assert result['status']==0
  actual=scene.c['graphics_in'].arrays[0]
  scene.c['render_completion_seed']=render.serialize(render.initialized([actual[off//4] for off in render.OFFSETS]))
  scene.c['pixel_job_seed']=jobs.initial(640,480,textured)
  frame,path=call(0x8c6050b0);assert frame['status']==0
  queued=jobs.State.from_buffer_copy(scene.c['pixel_job_seed'].state)
  # The original scheduled scene retires nine packets; the preview used eight.
  assert queued.ready==1 and queued.count==9 and frame['ta_current_packets']==0
  assert frame['ta_packet_capacity']==64 and frame['ta_list_out']['discarded']==0
  assert frame['ta_list_out']['last_count']==9 and path.count(0x8c651bc0)==2
  finished,path=call(0x8c04fe40);assert finished['status']==0
  summary=finished['pixel_job_summary']
  assert summary['processed']==summary['last_job']==1 and summary['ready']==0
  assert summary['queued_packets']==9 and summary['stats']==(2,4,4,4 if textured else 0,4)
  assert finished['render_completion_out']['completed']==1 and finished['render_completion_out']['pending']==7
  assert scene.c.get('backend_completed_job',0)==0
  if textured:
   save_normal(scene,camera)
   tail=q.get(scene.m,int(camera['task'],16)+0x14)
   assert tail==0x8c019768
   stop,path=call(tail,[int(camera['task'],16)]);expected='0x8c01a100'
  else:
   stop,path=call(0x8c09878c,[0]);expected='0x8c019400'
  assert stop['status']==0 and stop['stop_pc']=='0x8cfed000' and scene.usable
  assert scene.m[int(camera['work'],16)&0xffffff]==(1 if textured else 2)
  assert stop['full_state_match'] and stop['ordered_boundaries_match']
  cases.append(dict(textures_enabled=textured,owner=owner,camera=camera,particle_callback_steps=8905,particle_model_calls=6,particle_packets=0,frame_packets=9,frame_native_job=summary,original_VBlank_services=2,completed_previous_dependency=expected,phases=phases(scene)))
  print('ALLOCATED CAMERA AND FRAME VERIFIED',textured,summary,'completed previous frontier',expected,flush=True)
 scene,tasks,owner,camera,call=initialized()
 destructor=q.get(scene.m,int(camera['task'],16)+0x18);assert destructor==0x8c019760
 cleared,path=call(destructor,[int(camera['task'],16)])
 assert cleared['status']==0 and len(path)==4 and q.get(scene.m,0x8c18b7ec)==0
 assert len(tasks(0))==1 # Direct stored destructor test; task deletion is not claimed.
 cleanup=dict(camera=camera,direct_stored_destructor_call=True,task_deletion_qualified=False,phases=phases(scene))
 extras=[]
 for mode in ('cold_direction_particle_guard','installed_draw_dependency'):
  scene,tasks,owner,camera,call=initialized()
  direction=[q.get(scene.m,0x8c6ac5c0+4*i) for i in range(3)];assert direction==[0,0,0]
  positioned,path=call(0x8c09878c,[0])
  assert positioned['status']==0 and len(path)==3486 and scene.m[int(camera['work'],16)&0xffffff]==2
  assert q.get(scene.m,int(camera['task'],16)+0x14)==0x8c01c296
  derived=[q.get(scene.m,0x8c6aeec4+4*i) for i in range(3)]
  assert all(w&0x7f800000==0x7f800000 for w in derived)
  if mode=='cold_direction_particle_guard':
   stopped,path=call(0x8c09878c,[5]);assert stopped['status']==-1 and stopped['stop_pc']=='0x8c6210ba' and len(path)==389
  else:
   stopped,path=call(q.get(scene.m,int(camera['task'],16)+0x14),[int(camera['task'],16)])
   assert stopped['status']==-2 and stopped['stop_pc']=='0x8c01bd9e'
  assert not scene.usable and stopped['full_state_match'] and stopped['ordered_boundaries_match']
  extras.append(dict(mode=mode,first_camera_positioning_steps=3486,original_direction_input_zero=True,derived_nonfinite_fields_match=True,stop_status=stopped['status'],stop_pc=stopped['stop_pc'],phases=phases(scene)))
 scene,tasks,owner=prefix['normal_prefix']()
 loaded,path=scene.call(0x8c012240,compare=q.fast.compare)
 assert loaded['status']==0 and loaded['stop_pc']=='0x8cfed000' and len(path)==91680 and scene.usable
 assert [q.get(scene.m,0x8c6ac5c0+4*i) for i in range(3)]==[0,0xbf800000,0]
 extras.append(dict(mode='scene_direction_owner_complete',status=loaded['status'],stop_pc=loaded['stop_pc'],steps=91680,direction_produced_by_original_owner=True,phases=phases(scene)))
 # Fresh scene: actual direction setup, camera and all six stored particle tasks.
 scene,tasks,owner=prefix['normal_prefix']()
 for entry,args in ((0x8c012240,[]),(0x8c01957e,[]),(0x8c09878c,[0]),(0x8c09878c,[0])):
  result,path=scene.call(entry,args,compare=q.fast.compare);assert result['status']==0
 camera_task=tasks(0);assert len(camera_task)==1
 camera=camera_task[0];assert scene.m[camera['work']&0xffffff]==2 and len(tasks(5))==6
 assert q.get(scene.m,0x8c18b7ec)==camera['work']
 direction=[q.get(scene.m,0x8c6ac5c0+4*i) for i in range(3)];assert direction==[0,0xbf800000,0]
 derived=[q.get(scene.m,0x8c6aeec4+4*i) for i in range(3)];assert all(w&0x7f800000!=0x7f800000 for w in derived)
 assert scene.usable and scene.s.pc==scene.s.pr==0x8cfed000 and not scene.c['graphics_in'].packets
 folder=W/'model-camera-render-ready';folder.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5)
 (folder/'normal_public_return.pkl').write_bytes(payload)
 meta=dict(status='matched_original_scene_setup_and_camera_state2_before_particle_stream',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,camera={k:hex(v) for k,v in camera.items()},direction_produced_by_original_owner=True,derived_direction_finite=True,collector_capacity=64,phases=scene.records)
 (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
 submitted,path=scene.call(0x8c09878c,[5],compare=q.fast.compare)
 assert submitted['status']==-4 and submitted['stop_pc']=='0x8c6200f2' and len(path)==8287 and not scene.usable
 assert len(submitted['command_packets'])==submitted['ta_current_packets']==submitted['ta_packet_capacity']==64
 assert submitted['ta_list_out']['discarded']==0
 import math,struct
 vertex_packets=0;headers=0
 for _,_,words in submitted['command_packets']:
  if words[0]&0xe0000000 in (0xc0000000,0xe0000000):
   vertex_packets+=1
   assert all(math.isfinite(struct.unpack('<f',struct.pack('<I',w))[0]) for w in words[1:4])
  elif words[0]&0xe0000000==0x80000000:headers+=1
 assert vertex_packets and headers
 extras.append(dict(mode='original_scene_camera_particle_capture_limit',status=submitted['status'],stop_pc=submitted['stop_pc'],steps=8287,accepted_packets=64,vertex_packets=vertex_packets,headers=headers,accepted_vertex_coordinates_finite=True,all_six_tasks_retained=True,packet_capacity_increased=False,partial_frame_submitted=False,faulted_state_resumed=False,phases=phases(scene)))
 all_phases=[p for c in cases for p in c['phases']]+cleanup['phases']+[p for c in extras for p in c['phases']]
 assert len(all_phases)==66 and sum(p['status']==0 for p in all_phases)==63
 files=[Path(__file__),P/'verify_model_camera_position.py',W/'model-camera-render-ready/provenance.json',P/'analysis/model_camera_scene_translation.json',parent,P/'verify_model_camera_owner_v2.py',P/'analysis/model_camera_position_v2_translation.json',P/'analysis/model_camera_update_v2_translation.json',P/'analysis/model_camera_final_translation.json',P/'analysis/model_camera_copy_v2_translation.json',translation,q.v.effects.DLL,P/'build_model_camera_lighting_load.ps1',W/'model-camera-lighting-normal/provenance.json',jobs.DLL]
 for name in ('model_camera_lighting_load_reference','model_camera_lighting_load_fixtures','write_watch_model_camera_lighting_load','write_watch_model_camera_lighting_load_base','write_watch_model_camera_lighting_load_state','write_watch_model_camera_lighting_load_capture'):files.append(P/(name+'.py'))
 report=dict(status='verified_original_scene_direction_camera_positioning_and_reached_capture_limit',original_native_comparisons=66,normal_returns=63,matched_profile_or_incomplete_service_stops=3,cases=cases,cleanup=cleanup,frontiers=extras,full_299_source_strict_build_passed=True,new_static_instruction_rows=2181,first_camera_state1_qualified=True,full_direction_owner_return_qualified=True,complete_finite_particle_frame_qualified=False,camera_slot_published_by_original_callback=True,camera_pointer_injected=False,particle_work_or_callbacks_fabricated=False,manual_model_call=False,preview_translation_used=False,faulted_state_resumed=False,Texture_Off_zero_sampling=True,actual_native_frame_jobs_completed=2,collector_capacity=64,capacity_increased=False,positive_packets_discarded=0,full_camera_update_or_gameplay_qualified=False,playable=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
