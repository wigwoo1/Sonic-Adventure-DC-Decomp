"""Capture the real queued fixture job and qualify unsupported pixel work honestly."""
import ctypes,hashlib,json,pickle
from pathlib import Path
from types import SimpleNamespace
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'outputs/sonic-adventure'))
from stage_render_end_verified_state import normal,sha,P,W
from model_scene_verified_state import Scene
import stream_pixel_jobs_native as jobs
import render_completion_native as completion
import pvr_pixels_native as pixels
from renderer_stream_pixel_jobs_state import ReferenceJobs

def main():
 target=P/'analysis/stage_render_end_frame_fixture_v2_verification.json';assert not target.exists()
 base,q=normal();prefix=W/'stage-render-end-bank1-fixture-particles';meta=json.loads((prefix/'provenance.json').read_text())
 assert not meta['faulted_state_saved'] and meta['CPU_entry_SR_is_explicit_fixture'] and not meta['bank1_fixture_canonical']
 assert meta['checkpoint_sha256']==sha(prefix/'normal_fixture_return.pkl')=='b650a3f39aa9e6df1b8441ff76e5540461f1b9789c6c3f70590e7dfe817d51f0'
 assert meta['DLL_sha256']==sha(q.v.effects.DLL)=='35bef98bc504ae45a5e7e5fad3c4dd335f132e2b6081dad1f24c903aba075293'
 assert len(meta['phases'])==2 and all(r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'] for r in meta['phases'])
 m,s,f,c=pickle.loads((prefix/'normal_fixture_return.pkl').read_bytes())
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f);scene.c=c;scene.records=[];scene.usable=True;scene.prefix=prefix/'provenance.json';q.v.bind_vectors(c['vbr'])
 assert scene.s.pc==scene.s.pr==0x8cfed000 and scene.s.sr&0x20000000
 r,path=scene.call(0x8c6050b0,compare=q.fast.compare)
 assert r['status']==0 and scene.usable and r['full_state_match'] and r['ordered_boundaries_match']
 seed=scene.c['pixel_job_seed'];state=jobs.State.from_buffer_copy(seed.state)

 print('Observed queued job:',seed.summary(),flush=True)
 print('Retired list:',scene.c['ta_list_seed']['last_count'],'active:',len(scene.c['graphics_in'].packets),'render:',{k:v for k,v in scene.c['render_completion_seed'].items() if k not in ('parameters','job')},flush=True)
 return
 folder=W/'stage-render-end-bank1-fixture-queued';folder.mkdir(exist_ok=False)
 raw=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5);(folder/'normal_fixture_return.pkl').write_bytes(raw)
 queued=dict(meta,status='normal_queued_job_under_bank1_fixture_NOT_CANONICAL_GAME',checkpoint_sha256=hashlib.sha256(raw).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),active_packets=0,retired_packets=7318,pixel_job_ready=True,pixel_job_id=5,phases=meta['phases']+scene.records)
 (folder/'provenance.json').write_text(json.dumps(queued,indent=2)+'\n');(folder/'NOT_CANONICAL.txt').write_text('Explicit CPU bank-1 API fixture; not a proven real-game boot continuation. Pending job 5 is unsupported by the pixel backend.\n')
 print('Original frame submission matched: pending job 5, 7,318 immutable packets, completed count still 4.',flush=True)
 render_seed=scene.c['render_completion_seed'];ref_render=SimpleNamespace(**render_seed)
 def forbidden_finish(*args):raise AssertionError('Unsupported pixel work must not finish a job')
 ref_render.finish=forbidden_finish
 reference=ReferenceJobs(seed);reference_before=reference.snapshot();ref_status=reference.execute(ref_render,scene.c['graphics_in'].vram)
 assert ref_status==-2 and reference.snapshot()==reference_before and reference.pixels==seed.pixels and reference.depth==seed.depth
 native=jobs.restored(seed);render=completion.restore(render_seed);before=jobs.snapshot(native);render_before=completion.snapshot(render)
 vram=(jobs.B*0x800000).from_buffer_copy(scene.c['graphics_in'].vram);view=(jobs.B*0x800000)();colour=(jobs.U*(640*480)).from_buffer_copy(seed.pixels);depth=(jobs.U*(640*480)).from_buffer_copy(seed.depth);options=pixels.Options(*seed.options)
 try:
  status=jobs.LIB.sa_stream_banked_pixel_job_execute(ctypes.byref(native),ctypes.byref(render),vram,len(vram),view,len(view),colour,depth,len(colour),ctypes.byref(options))
  assert status==ref_status==-2 and jobs.snapshot(native)==before==reference.snapshot() and completion.snapshot(render)==render_before
  assert bytes(colour)==seed.pixels and bytes(depth)==seed.depth
 finally:jobs.dispose_all()
 headers=[(i,w) for i,(_,_,w) in enumerate(seed.packets) if w[0]>>29==4]
 index,w=headers[0];assert index==0 and w[0]&0x300c5==4
 modes=dict(first_packet_index=index,first_header_words=[hex(x) for x in w[:4]],first_rejected_PCW_mask=hex(w[0]&0x300c5),specular_offset_colour_enabled=bool(w[0]&4),fog_mode=(w[2]>>22)&3,filter_mode=(w[2]>>13)&3,texture_format=(w[3]>>27)&7,VQ_enabled=bool(w[3]&(1<<30)),mipmap_enabled=bool(w[3]&(1<<31)),total_headers=len(headers),offset_colour_headers=sum(bool(t[0]&4) for _,t in headers),VQ_headers=sum(bool(t[3]&(1<<30)) for _,t in headers))
 files=[Path(__file__),P/'verify_stage_render_end_frame_fixture.py',W/'stage_render_end_frame_fixture_verification.log',prefix/'provenance.json',P/'stage_render_end_verified_state.py',q.v.effects.DLL,jobs.DLL,P/'renderer_stream_pixel_jobs_state.py',P/'pvr_pixels_stream_reference.py',P/'source/pvr_pixels_stream.c',P/'source/stream_pixel_jobs.c']
 report=dict(status='verified_queued_original_fixture_job_and_matching_unsupported_pixel_rejection',original_native_comparisons=len(queued['phases']),normal_returns=len(queued['phases']),host_stops=0,cases=queued['phases'],retained_prefix_comparisons=2,new_frame_submission_comparisons=1,independent_reference_native_pixel_rejection_comparisons=1,reference_pixel_status=ref_status,native_pixel_status=status,first_unsupported_header=modes,pixel_job_ready=True,pixel_job_id=5,queued_packets=7318,completed_native_jobs=4,new_completed_pixel_jobs=0,reference_and_native_completion_state_preserved=True,images_and_depth_preserved=True,first_verifier_and_failed_log_preserved=True,first_failure='The next query requires pixel work before CPU execution; its reference precondition correctly rejected unsupported polygon commands. This version captures the successful original frame submission and tests the negative backend result directly.',query_guest_instructions_executed=False,CPU_entry_SR_is_explicit_fixture=True,bank1_fixture_canonical=False,original_bank_mode_producer_verified=False,faulted_state_resumed_or_saved=False,normal_fixture_checkpoint=str(folder/'normal_fixture_return.pkl'),normal_fixture_checkpoint_sha256=queued['checkpoint_sha256'],normal_fixture_provenance_sha256=sha(folder/'provenance.json'),real_gameplay_or_complete_game_frame_qualified=False,new_registry_credit=0,overall_completion_percent=10,goal_paused=False,playable=False,documentation_sources=['https://kos-docs.dreamcast.wiki/group__pvr__bitmasks.html','https://kos-docs.dreamcast.wiki/group__pvr__primitives__headers.html','https://cadcdev.sourceforge.net/docs/kos-current/group__pvr__txr__fmts.html'],sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Queued fixture job and matching -2 pixel rejection verified; no completion synthesized.',flush=True)
if __name__=='__main__':main()
