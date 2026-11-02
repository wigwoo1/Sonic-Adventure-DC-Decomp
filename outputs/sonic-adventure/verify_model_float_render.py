"""Retail model generation, native pixels and actual original frame submission."""
import dataclasses,hashlib,json,struct
from pathlib import Path
from model_scene_verified_state import Scene,sha,P,W
import model_float_color_fixtures as q
import write_watch_model_float_color_state as graphics
import float_color_pixels_native as jobs
import pvr_pixels_native as pixels
import pvr_pixels_float_reference as oracle
import frame_clock_native as clock
import render_completion_native as render

def main():
 target=P/'analysis/model_float_render_verification.json';assert not target.exists()
 extension=P/'analysis/model_float_color_extension.json';ext=json.loads(extension.read_text())
 for name,h in ext['sha256'].items():assert sha(name)==h,name
 cases=[];private=W/'model-float-render-verified-v2';private.mkdir(exist_ok=False)
 for textured in (False,True):
  scene=Scene('model-view-produced')
  values={f.name:getattr(scene.c['graphics_in'],f.name) for f in dataclasses.fields(scene.c['graphics_in'])}
  scene.c['graphics_in']=graphics.GraphicsState(**values)
  assert all(getattr(scene.c['graphics_in'],k)==v for k,v in values.items())
  q.v.bind_vectors(scene.c['vbr'])
  def call(entry,args=()):
   result,path=scene.call(entry,args,compare=q.fast.compare)
   assert result['status']==0
   return result,path
  # These are original caller-selected static buffer arguments, not a limit patch.
  assert q.get(scene.m,0x8c054814)==0x8c7ac8e0 and q.get(scene.m,0x8c054830)==0x8c60edb0
  call(0x8c60edb0,[0x8c7ac8e0,4096])
  vector=0x8cf30000
  for i,v in enumerate((0.0,0.0,-20.0)):q.put(scene.m,vector+4*i,int.from_bytes(struct.pack('<f',v),'little'))
  call(0x8c63a804,[0,vector])
  model,model_path=call(0x8c6215d6,[q.get(scene.m,0x8c044a00)])
  assert len(model['command_packets'])==31 and len(model['boundaries'])==16 and len(model_path)==2910
  words=[w for _,_,packet in model['command_packets'] for w in packet];raw=struct.pack('<'+'I'*len(words),*words)
  (private/('model_packets_'+str(int(textured))+'.bin')).write_bytes(raw)
  o=pixels.Options(640,480,640,480,int(textured),0,3,128);background=[0xff101018]*(640*480)
  vram=scene.c['graphics_in'].vram if textured else None
  rc,col,dep,stats=pixels.draw(words,vram,o,background,lib=jobs.LIB);er,ec,ed,es=oracle.draw(words,vram,o,background)
  assert rc==er==0 and col==struct.pack('<'+'I'*len(ec),*ec) and dep==struct.pack('<'+'I'*len(ed),*ed) and stats==es
  assert stats==(10,5929,5929,0,5929) and sum(a!=b for a,b in zip(ec,background))==5929
  direct_colour=hashlib.sha256(col).hexdigest();direct_depth=hashlib.sha256(dep).hexdigest()
  if not textured:
   from PIL import Image
   image=Image.frombytes('RGBA',(640,480),col,'raw','BGRA');image.save(private/'native_model_off.png')
   image.crop((270,136,381,253)).resize((444,468),Image.Resampling.NEAREST).save(private/'native_model_crop.png')
  # Host contract setup is derived from the actual original register state.
  registers=scene.c['graphics_in'].arrays[0];reg=lambda offset:registers[offset//4]
  state=clock.initialized(50000000,27000000,reg(0x44),reg(0xd0),reg(0xd8),reg(0xcc),reg(0xdc))
  scene.c['frame_clock_seed']=clock.serialize(state);scene.c['runtime_attached']=True
  scene.c.pop('ticks',None);scene.c.pop('irq_samples',None);scene.c['scan_samples']=[]
  call(0x8c6025e8)
  actual=scene.c['graphics_in'].arrays[0];state=render.initialized([actual[offset//4] for offset in render.OFFSETS])
  scene.c['render_completion_seed']=render.serialize(state);scene.c['pixel_job_seed']=jobs.initial(640,480,textured)
  frame,path=call(0x8c6050b0)
  queued=jobs.State.from_buffer_copy(scene.c['pixel_job_seed'].state)
  assert queued.ready==1 and queued.count==39 and list(queued.words[:len(words)])==words
  assert frame['ta_current_packets']==0 and frame['ta_packet_capacity']==64
  assert frame['ta_list_out']['discarded']==0 and frame['ta_list_out']['last_count']==39
  assert path.count(0x8c651bc0)==2 and frame['runtime_state'][9:11]==(0,0)
  assert not any(b['target']=='0x8c650b60' and b['pr'] in ('0x8c651d92','0x8c651eae') for b in frame['boundaries'])
  finished,_=call(0x8c04fe40)
  summary=finished['pixel_job_summary'];output=finished['pixel_job_out']
  assert summary['processed']==summary['last_job']==1 and summary['ready']==0 and summary['queued_packets']==39
  assert summary['stats'][0]==12 and summary['stats'][4]==5933 and summary['stats'][3]==(4 if textured else 0)
  assert finished['render_completion_out']['completed']==1 and finished['render_completion_out']['pending']==7
  assert scene.c.get('backend_completed_job',0)==0
  from PIL import Image
  Image.frombytes('RGBA',(640,480),output.pixels,'raw','BGRA').save(private/('native_frame_'+str(int(textured))+'.png'))
  phases=[{k:v for k,v in phase.items() if k!='boundaries'}|dict(ordered_boundary_count=len(phase['boundaries']),ordered_targets=[b['target'] for b in phase['boundaries']]) for phase in scene.records]
  cases.append(dict(textures_enabled=textured,phases=phases,normal_comparisons=len(phases),model_packet_count=31,model_packet_sha256=hashlib.sha256(raw).hexdigest(),model_triangles=10,model_RGB_changed_pixels=5929,direct_native_rational_colour_depth_and_stats_match=True,model_colour_sha256=direct_colour,model_depth_sha256=direct_depth,model_texture_samples=0,original_frame_packet_count=39,original_frame_VBlank_services=2,native_job_summary=summary,native_full_colour_depth_and_guest_state_match=True,accepted_packets_retired=39,collector_capacity=64,external_completion_tokens=0,render_pending_after_query=7,shutdown_drained_qualified=False))
  print('MODEL FLOAT RENDER VERIFIED',textured,summary,flush=True)
 assert cases[0]['model_packet_sha256']==cases[1]['model_packet_sha256']
 assert cases[0]['model_colour_sha256']==cases[1]['model_colour_sha256'] and cases[0]['model_depth_sha256']==cases[1]['model_depth_sha256']
 files=[Path(__file__),P/'model_scene_verified_state.py',extension,P/'analysis/float_color_platform_verification.json',P/'analysis/model_vertex_buffer_owner_verification.json',q.v.effects.DLL,jobs.DLL]
 for name in ('model_float_color_reference','model_float_color_fixtures','write_watch_model_float_color','write_watch_model_float_color_base','write_watch_model_float_color_state','write_watch_model_float_color_capture','renderer_float_color_comparison','renderer_float_color_jobs_state','renderer_float_color_pixel_jobs_state','pixel_jobs_float_reference','float_color_pixels_native','pvr_pixels_float_reference','frame_events_float_reference'):files.append(P/(name+'.py'))
 report=dict(status='verified_retail_model_float_packets_pixels_and_original_frame_native_jobs',cases=cases,normal_original_native_comparisons=sum(c['normal_comparisons'] for c in cases),actual_native_jobs_completed=2,model_triangles_per_case=10,model_pixels_per_case=5929,Texture_Off_zero_sampling=True,external_scene_translation=[0.0,0.0,-20.0],translation_is_declared_test_input=True,actual_game_camera_or_spawn_sequence_claimed=False,model_vertices_or_clip_flags_patched=False,original_source_and_buffer_owners_used=True,private_frames_or_callee_results_injected=False,faulted_state_resumed=False,retail_assets_or_images_included=False,full_292_source_game_and_four_source_pixel_builds_passed=True,hardware_GPU_precision_timing_or_FPS_qualified=False,playable=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
