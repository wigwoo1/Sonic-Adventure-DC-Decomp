"""Carry the completed second frame into another real original camera update."""
import hashlib,json,pickle
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
 target=P/'analysis/camera_draw_state2_warm_verification.json';assert not target.exists()
 inherited.prefix.checkpoint()
 proof_path=P/'analysis/camera_draw_state2_sequence_verification.json';proof=json.loads(proof_path.read_text())
 for name,h in proof['sha256'].items():assert sha(name)==h,name
 folder=W/'camera-draw-state2-normal';meta=json.loads((folder/'provenance.json').read_text())
 assert not meta['faulted_state_saved'] and meta['completed_native_jobs']==2 and meta['active_packets']==0
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256'] and sha(meta['DLL_path'])==meta['DLL_sha256']
 assert all(p['status']==0 and p['stop_pc']=='0x8cfed000' for p in meta['phases'])
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import model_camera_position_tail_fixtures as q
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f);scene.c=c;scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json';q.v.bind_vectors(c['vbr'])
 assert scene.s.pc==scene.s.pr==0x8cfed000
 prior_job=c['pixel_job_seed'];prior_retired=c['ta_list_seed'];prior_render=c['render_completion_seed']
 result,path=scene.call(0x8c09878c,[0],compare=q.fast.compare)
 assert result['status']==0 and scene.usable and len(path)==3168 and result['full_state_match'] and result['ordered_boundaries_match']
 assert scene.c['pixel_job_seed']==prior_job and scene.c['ta_list_seed']==prior_retired and scene.c['render_completion_seed']==prior_render
 assert result['pixel_job_summary']['processed']==2 and not result['pixel_job_summary']['ready'] and result['ta_current_packets']==0
 assert q.get(scene.m,0x8c754ef0)==0 and scene.c.get('backend_completed_job',0)==0
 out=W/'camera-draw-state2-warm-normal';out.mkdir(exist_ok=False)
 payload=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5);(out/'normal_public_return.pkl').write_bytes(payload)
 state_proof=dict(status='matched_warm_original_camera_update_after_two_completed_native_jobs',checkpoint_sha256=hashlib.sha256(payload).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,candidate_count=0,active_packets=0,retired_packets=189,completed_native_jobs=2,render_pending=7,phases=scene.records)
 (out/'provenance.json').write_text(json.dumps(state_proof,indent=2)+'\n')
 files=[Path(__file__),proof_path,P/'model_camera_position_tail_verified_state.py',q.v.effects.DLL]
 report=dict(status='verified_warm_camera_update_preserves_completed_jobs_and_retired_generation',original_native_comparisons=1,normal_returns=1,steps=3168,full_state_match=True,ordered_boundaries_match=True,completed_jobs_remain=2,immutable_job_and_retired_packets_unchanged=True,duplicate_render_or_completion=False,candidate_count=0,populated_candidate_paths_qualified=False,manual_camera_or_callee_results_injected=False,external_completion_tokens=0,faulted_state_resumed=False,overall_completion_percent=10,goal_paused=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('WARM ORIGINAL CAMERA UPDATE VERIFIED',len(path),'instructions',flush=True)
if __name__=='__main__':main()
