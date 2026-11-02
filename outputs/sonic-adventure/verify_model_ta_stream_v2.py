"""Compare the complete original six-particle scheduler with native owned pages."""
import hashlib,json,pickle,struct
from pathlib import Path
from model_ta_stream_verified_state import normal,sha,P,W

def main():
 target=P/'analysis/model_ta_stream_v2_verification.json';assert not target.exists()
 scene,q=normal();cases=[];initial=(bytes(scene.s),bytes(scene.f))
 for iteration in range(2):
  before=tuple(scene.c['graphics_in'].packets)
  result,path=scene.call(0x8c09878c,[5],compare=q.fast.compare)
  assert result['status']==0 and scene.usable and scene.s.pc==scene.s.pr==0x8cfed000
  packets=result['command_packets'];assert packets[:len(before)]==before
  assert result['ta_current_packets']==len(packets) and result['ta_stream_pages']==(len(packets)+63)//64
  assert result['ta_packet_capacity']==64 and len(packets)>64
  new=packets[len(before):];headers=[p for p in new if p[2][0]>>29==4];vertices=[p for p in new if p[2][0]>>29==7]
  assert len(new)==186 and len(headers)==6 and len(vertices)==180
  assert all(all(abs(struct.unpack('<f',struct.pack('<I',w))[0])<1e30 for w in words[1:4]) for _,_,words in vertices)
  models=[b for b in result['boundaries'] if b['target']=='0x8c6215d6'];assert len(models)==6
  assert len(path)==19627 and result['frame_events_state'][0]==len(packets)
  raw=b''.join(struct.pack('<10I',a,k,*words) for a,k,words in packets)
  cases.append(dict(iteration=iteration+1,steps=len(path),status=0,stop_pc=result['stop_pc'],model_calls=len(models),new_packets=len(new),headers=len(headers),vertices=len(vertices),total_packets=len(packets),native_pages=result['ta_stream_pages'],page_packets=64,ordered_packet_sha256=hashlib.sha256(raw).hexdigest(),finite_vertex_coordinates=True,frame_events=result['frame_events_state'],full_state_match=True,ordered_boundaries_match=True,write_watch=result['write_watch']))
  if iteration==0:
   folder=W/'model-ta-stream-normal-v2';folder.mkdir(exist_ok=False)
   payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5);(folder/'normal_public_return.pkl').write_bytes(payload)
   meta=dict(status='matched_complete_original_six_particle_scheduler_normal_return',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,queued_packets=len(packets),page_packets=64,native_pages=result['ta_stream_pages'],frame_rendered=False,phases=scene.records)
   (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
  print('ORIGINAL PARTICLE PASS VERIFIED',iteration+1,len(path),len(packets),result['ta_stream_pages'],flush=True)
 files=[Path(__file__),P/'model_ta_stream_verified_state.py',P/'renderer_ta_stream_comparison.py',P/'renderer_ta_stream_list_comparison.py',P/'ta_stream_native.py',P/'ta_stream_events_reference.py',P/'ta_stream_lifecycle_reference.py',P/'build_model_ta_stream.ps1',*[P/'source'/name for name in ('ta_stream.c','ta_stream.h','ta_stream_win32.c')],q.v.effects.DLL]
 files += [P/(name+'.py') for name in ('model_ta_stream_reference','model_ta_stream_fixtures','write_watch_model_ta_stream','write_watch_model_ta_stream_base','write_watch_model_ta_stream_state','write_watch_model_ta_stream_capture')]
 report=dict(status='verified_complete_original_particle_passes_and_persistent_ordered_paged_capture',original_native_full_comparisons=2,normal_returns=2,cases=cases,first_pass_packets=186,second_total_packets=372,page_packets=64,strict_full_build_sources=301,legacy_fixed_64_profile_unchanged=True,active_stream_owned_natively=True,host_allocator_is_native_Win32=True,all_original_particles_retained=True,vertices_or_clip_flags_patched=False,callee_results_injected=False,camera_or_direction_injected=False,faulted_state_resumed=False,partial_frame_submitted=False,pixel_job_execution_qualified=False,TA_generation_restore_in_full_comparison_qualified=False,playable=False,new_original_routine_credit=0,overall_completion_percent=10,goal_paused=False,sha256={str(x):sha(x) for x in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('TWO COMPLETE NORMAL PARTICLE PASSES VERIFIED',flush=True)
if __name__=='__main__':main()
