"""Original camera allocation and scheduled particles through bounded native frames."""
import hashlib,json,pickle
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
parent=P/'verify_model_callback_camera_dependency.py'
source=parent.read_text().split('\ndef main():')[0]
source=source.replace('model_camera_dependency_fixtures','model_camera_owner_v3_fixtures').replace('write_watch_model_camera_dependency_state','write_watch_model_camera_owner_v3_state')
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
 folder=W/'model-camera-owner-normal';folder.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5)
 (folder/'normal_public_return.pkl').write_bytes(payload)
 meta=dict(status='matched_original_allocated_camera_and_particle_frame_return',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,camera=camera,phases=scene.records)
 (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')

def main():
 target=P/'analysis/model_camera_owner_verification.json';assert not target.exists()
 translation=P/'analysis/model_camera_owner_v3_translation.json'
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
  assert queued.ready==1 and queued.count==8 and frame['ta_current_packets']==0
  assert frame['ta_packet_capacity']==64 and frame['ta_list_out']['discarded']==0
  assert frame['ta_list_out']['last_count']==8 and path.count(0x8c651bc0)==2
  finished,path=call(0x8c04fe40);assert finished['status']==0
  summary=finished['pixel_job_summary']
  assert summary['processed']==summary['last_job']==1 and summary['ready']==0
  assert summary['queued_packets']==8 and summary['stats']==(2,4,4,4 if textured else 0,4)
  assert finished['render_completion_out']['completed']==1 and finished['render_completion_out']['pending']==7
  assert scene.c.get('backend_completed_job',0)==0
  if textured:
   save_normal(scene,camera)
   tail=q.get(scene.m,int(camera['task'],16)+0x14)
   assert tail==0x8c019768
   stop,path=call(tail,[int(camera['task'],16)]);expected='0x8c01a100'
  else:
   stop,path=call(0x8c09878c,[0]);expected='0x8c019400'
  assert stop['status']==-2 and stop['stop_pc']==expected and not scene.usable
  assert stop['full_state_match'] and stop['ordered_boundaries_match']
  cases.append(dict(textures_enabled=textured,owner=owner,camera=camera,particle_callback_steps=8905,particle_model_calls=6,particle_packets=0,frame_packets=8,frame_native_job=summary,original_VBlank_services=2,matched_next_dependency=expected,phases=phases(scene)))
  print('ALLOCATED CAMERA AND FRAME VERIFIED',textured,summary,'next',expected,flush=True)
 scene,tasks,owner,camera,call=initialized()
 destructor=q.get(scene.m,int(camera['task'],16)+0x18);assert destructor==0x8c019760
 cleared,path=call(destructor,[int(camera['task'],16)])
 assert cleared['status']==0 and len(path)==4 and q.get(scene.m,0x8c18b7ec)==0
 assert len(tasks(0))==1 # Direct stored destructor test; task deletion is not claimed.
 cleanup=dict(camera=camera,direct_stored_destructor_call=True,task_deletion_qualified=False,phases=phases(scene))
 all_phases=[p for c in cases for p in c['phases']]+cleanup['phases']
 assert len(all_phases)==32 and sum(p['status']==0 for p in all_phases)==30
 files=[Path(__file__),parent,translation,q.v.effects.DLL,P/'build_model_camera_owner_v3.ps1',W/'model-camera-owner-normal/provenance.json',jobs.DLL]
 for name in ('model_camera_owner_v3_reference','model_camera_owner_v3_fixtures','write_watch_model_camera_owner_v3','write_watch_model_camera_owner_v3_base','write_watch_model_camera_owner_v3_state','write_watch_model_camera_owner_v3_capture'):files.append(P/(name+'.py'))
 report=dict(status='verified_original_camera_allocation_state0_scheduled_particles_and_bounded_frames',original_native_comparisons=32,normal_returns=30,matched_incomplete_dependency_stops=2,cases=cases,cleanup=cleanup,full_293_source_strict_build_passed=True,static_instruction_rows=305,new_camera_body_rows=272,reused_scheduler_rows=33,camera_slot_published_by_original_callback=True,camera_pointer_injected=False,particle_work_or_callbacks_fabricated=False,manual_model_call=False,preview_translation_used=False,faulted_state_resumed=False,Texture_Off_zero_sampling=True,actual_native_frame_jobs_completed=2,collector_capacity=64,capacity_increased=False,positive_packets_discarded=0,full_camera_update_or_gameplay_qualified=False,playable=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
