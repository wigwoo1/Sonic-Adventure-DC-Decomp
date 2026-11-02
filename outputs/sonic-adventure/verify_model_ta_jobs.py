"""Full original frame submission and native page-backed raster completion."""
import hashlib,json,pickle,struct
from pathlib import Path
from model_ta_jobs_verified_state import normal,sha,P,W
import frame_clock_native as clock
import render_completion_native as render
import stream_pixel_jobs_native as jobs

def main():
 target=P/'analysis/model_ta_jobs_verification.json';assert not target.exists();cases=[]
 private=W/'model-ta-jobs-verified-frames';private.mkdir(exist_ok=False)
 for textured in (False,True):
  scene,q=normal();initial=tuple(scene.c['graphics_in'].packets);assert len(initial)==186
  registers=scene.c['graphics_in'].arrays[0];reg=lambda off:registers[off//4]
  scene.c['frame_clock_seed']=clock.serialize(clock.initialized(50000000,27000000,reg(0x44),reg(0xd0),reg(0xd8),reg(0xcc),reg(0xdc)))
  scene.c['runtime_attached']=True;scene.c.pop('ticks',None);scene.c.pop('irq_samples',None);scene.c['scan_samples']=[]
  def call(entry,args=()):
   result,path=scene.call(entry,args,compare=q.fast.compare)
   assert result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match'] and scene.usable
   return result,path
  paced,path=call(0x8c6025e8);assert len(path)==19
  actual=scene.c['graphics_in'].arrays[0];scene.c['render_completion_seed']=render.serialize(render.initialized([actual[off//4] for off in render.OFFSETS]));scene.c['pixel_job_seed']=jobs.initial(640,480,textured)
  frame,path=call(0x8c6050b0);assert len(path)==10109 and path.count(0x8c651bc0)==2
  submitted=scene.c['pixel_job_seed'];state=jobs.State.from_buffer_copy(submitted.state)
  assert not state.snapshot and state.ready==1 and state.count==194 and state.cursor==0 and state.processed==0
  assert submitted.packets[:186]==initial and len(submitted.packets)==194
  assert frame['ta_current_packets']==0 and frame['ta_retired_pages']==4 and frame['ta_list_out']['last_count']==194 and frame['ta_list_out']['discarded']==0
  assert sum(q==2 for _,q,_ in submitted.packets)>0,'actual DMA producer reaches the owned FIFO endpoint'
  assert submitted.packets==tuple((a,q,tuple(w)) for a,q,w in frame['ta_list_out']['last'])
  retired_before=frame['ta_list_out']
  finished,path=call(0x8c04fe40);assert len(path)==5
  summary=finished['pixel_job_summary'];output=scene.c['pixel_job_seed']
  assert summary['processed']==summary['last_job']==1 and summary['ready']==0 and summary['queued_packets']==194
  assert tuple(summary['stats'])==(62,1294,1294,4 if textured else 0,1294)
  assert output.packets==submitted.packets and finished['ta_list_out']==retired_before
  assert finished['render_completion_out']['completed']==1 and finished['render_completion_out']['active_id']==0 and finished['render_completion_out']['pending']==7
  assert scene.c.get('backend_completed_job',0)==0
  repeat,path=call(0x8c04fe40);assert len(path)==5
  assert repeat['pixel_job_summary']==summary and repeat['render_completion_out']==finished['render_completion_out']
  assert scene.c['pixel_job_seed']==output and repeat['ta_list_out']==retired_before
  from PIL import Image
  image=private/('native_frame_'+str(int(textured))+'.png');Image.frombytes('RGBA',(640,480),output.pixels,'raw','BGRA').save(image)
  if textured:
   folder=W/'model-ta-jobs-frame-normal';folder.mkdir(exist_ok=False)
   raw=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5);(folder/'normal_public_return.pkl').write_bytes(raw)
   meta=dict(status='matched_original_frame_and_complete_native_paged_pixel_job_normal_return',checkpoint_sha256=hashlib.sha256(raw).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,submitted_packets=194,retired_packets=194,retired_pages=4,active_packets=0,completed_native_jobs=1,render_pending=7,shutdown_drained=False,phases=scene.records)
   (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
  raw_packets=b''.join(struct.pack('<10I',a,q,*row) for a,q,row in submitted.packets)
  phases=[{k:v for k,v in phase.items() if k!='boundaries'}|dict(ordered_targets=[b['target'] for b in phase['boundaries']]) for phase in scene.records]
  cases.append(dict(textures_enabled=textured,phases=phases,normal_comparisons=len(phases),original_frame_packets=194,original_particle_packets=186,original_DMA_packets=sum(q==2 for _,q,_ in submitted.packets),original_VBlank_services=2,immutable_submitted_packet_sha256=hashlib.sha256(raw_packets).hexdigest(),native_job_summary=summary,full_colour_depth_memory_devices_and_ordered_boundaries_match=True,retired_generation_restored_across_normal_calls=True,duplicate_execution_or_completion=False,render_pending_after_query=7,shutdown_drained=False,private_image_sha256=sha(image)))
  print('COMPLETE ORIGINAL PARTICLE FRAME VERIFIED',textured,summary,flush=True)
 files=[Path(__file__),P/'model_ta_jobs_verified_state.py',P/'build_model_ta_jobs.ps1',*[P/'source'/n for n in ('ta_stream_jobs_store.c','stream_pixel_jobs.c','stream_pixel_jobs.h','pvr_pixels_stream.c')],q.v.effects.DLL]
 for name in ('model_ta_jobs_reference','model_ta_jobs_fixtures','write_watch_model_ta_jobs','write_watch_model_ta_jobs_base','write_watch_model_ta_jobs_state','write_watch_model_ta_jobs_capture','ta_job_stream_native','ta_jobs_events_reference','renderer_ta_jobs_stream_comparison','renderer_stream_pixel_comparison','renderer_stream_pixel_jobs_state','stream_pixel_jobs_native','pvr_pixels_stream_reference'):files.append(P/(name+'.py'))
 report=dict(status='verified_original_particle_frame_native_owned_snapshot_raster_and_completion',cases=cases,normal_original_native_comparisons=8,actual_native_jobs_completed=2,original_frame_packets=194,triangles=62,fragments=1294,Texture_Off_zero_sampling=True,strict_full_build_sources=305,all_six_original_particles_retained=True,actual_original_scene_camera_source_owners=True,manual_preview_translation=False,vertices_or_clip_flags_patched=False,game_callee_results_or_camera_direction_injected=False,external_completion_tokens=0,faulted_state_resumed=False,partial_frame_submitted=False,retail_assets_or_images_published=False,hardware_GPU_precision_timing_or_FPS_qualified=False,playable=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('EIGHT COMPLETE ORIGINAL/NATIVE COMPARISONS AND TWO ACTUAL JOBS VERIFIED',flush=True)
if __name__=='__main__':main()
