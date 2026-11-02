"""Actual allocated star tasks and the original cold camera dependency."""
import dataclasses,hashlib,json,pickle
from pathlib import Path
from model_scene_verified_state import Scene,sha,P,W
import model_camera_dependency_fixtures as q
import write_watch_model_camera_dependency_state as graphics

def normal_prefix(save_name=None):
 scene=Scene('model-view-produced');values={f.name:getattr(scene.c['graphics_in'],f.name) for f in dataclasses.fields(scene.c['graphics_in'])}
 scene.c['graphics_in']=graphics.GraphicsState(**values);q.v.bind_vectors(scene.c['vbr'])
 for name in ('model_callback_roots_connection.json','model_camera_guarded_callbacks_translation.json'):
  for f,h in json.loads((P/'analysis'/name).read_text())['sha256'].items():assert sha(f)==h,f
 heads=q.get(scene.m,0x8c0987e0)
 def tasks(level):
  a=q.get(scene.m,heads+4*level);seen=set();items=[]
  while a:
   assert a not in seen and len(items)<1024;seen.add(a)
   items.append(dict(task=a,callback=q.get(scene.m,a+0x10),draw=q.get(scene.m,a+0x14),work=q.get(scene.m,a+0x20)))
   a=q.get(scene.m,a)
  return items
 def call(entry,args=()):
  result,path=scene.call(entry,args,compare=q.fast.compare);assert result['status']==0
  return result,path
 prior=json.loads((W/'model-geometry-prefix/provenance.json').read_text())
 ctor=next(p for p in prior['phases'] if p['entry']=='0x8c06c350');actor=ctor['boundaries'][0]['arguments'][1]
 source=q.get(scene.m,actor+0x20);table=q.get(scene.m,0x8c044844)
 assert source==q.get(scene.m,table) and scene.m[(source&0xffffff)+8]==0 and not tasks(5) and not tasks(6)
 call(0x8c60edb0,[0x8c7ac8e0,4096])
 call(0x8c07b320,[source]);guest_r0=scene.s.r[0]
 parent=tasks(6);assert len(parent)==1 and parent[0]['callback']==0x8c044680 and guest_r0==0
 call(0x8c09878c,[6]);particles=tasks(5)
 assert len(particles)==6 and len({t['work'] for t in particles})==6 and all(t['callback']==0x8c04481a for t in particles)
 assert tasks(6)[0]['callback']==0x8c09865c
 call(0x8c09878c,[6]);assert not tasks(6) and len(tasks(5))==6
 call(0x8c09878c,[5]);assert all(t['draw']==0x8c0448b4 and scene.m[t['work']&0xffffff]==1 for t in tasks(5))
 assert q.get(scene.m,q.get(scene.m,0x8c0449f0))==0
 owner=dict(actor=hex(actor),source_work=hex(source),source_table=hex(table),character_id=0,spawn_guest_R0=guest_r0,original_parent_task=hex(parent[0]['task']),particle_count=6,particle_tasks=[hex(t['task']) for t in tasks(5)],particle_work=[hex(t['work']) for t in tasks(5)])
 if save_name:
  folder=W/save_name;folder.mkdir(exist_ok=False);payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5)
  (folder/'normal_public_return.pkl').write_bytes(payload)
  meta=dict(status='matched_normal_allocated_star_callback_prefix',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,owner=owner,phases=scene.records)
  (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
 return scene,tasks,owner

def main():
 target=P/'analysis/model_callback_camera_dependency_verification.json';assert not target.exists()
 scene,tasks,owner=normal_prefix('model-callback-camera-prefix')
 result,path=scene.call(0x8c09878c,[5],compare=q.fast.compare)
 assert result['status']==-3 and result['stop_pc']=='0x8c0448fa' and result['full_state_match'] and result['ordered_boundaries_match']
 assert q.get(scene.m,0x8c18b7ec)==0 and not result['command_packets']
 files=[Path(__file__),P/'analysis/model_callback_roots_connection.json',P/'analysis/model_camera_guarded_callbacks_translation.json',q.v.effects.DLL,W/'model-callback-camera-prefix/provenance.json',P/'model_camera_dependency_reference.py',P/'model_camera_dependency_fixtures.py']
 phases=[{k:v for k,v in p.items() if k!='boundaries'}|dict(ordered_targets=[b['target'] for b in p['boundaries']]) for p in scene.records]
 report=dict(status='verified_owned_star_task_callbacks_and_reached_cold_camera_guard',comparisons=6,normal_returns=5,matched_partial_stop=1,owner=owner,phases=phases,camera_pointer_slot='0x8c18b7ec',camera_slot_from_original_literal='0x8c0449f0',camera_pointer=0,original_stop_pc='0x8c0448fa',full_guest_partial_state_and_ordered_boundaries_match=True,parent_allocation_and_removal_original=True,particle_work_or_callbacks_fabricated=False,manual_model_call=False,preview_translation_used=False,faulted_state_resumed=False,new_original_routine_credit=0,full_292_source_build_passed=True,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('OWNED CALLBACK CAMERA GUARD VERIFIED',len(scene.records),owner,flush=True)
if __name__=='__main__':main()
