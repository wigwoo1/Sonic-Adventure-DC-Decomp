"""Load the matched prepared frame before recovering the actual stored draw."""
import hashlib,json,pickle
from dataclasses import fields
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def normal():
 inherited.prefix.checkpoint()
 proof=json.loads((P/'analysis/model_ta_jobs_verification.json').read_text())
 for name,h in proof['sha256'].items():assert sha(name)==h,name
 folder=W/'model-ta-jobs-frame-normal';meta=json.loads((folder/'provenance.json').read_text())
 assert not meta['faulted_state_saved'] and meta['completed_native_jobs']==1 and meta['retired_packets']==194 and meta['active_packets']==0
 assert all(p['status']==0 and p['stop_pc']=='0x8cfed000' for p in meta['phases'])
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']
 assert sha(meta['DLL_path'])==meta['DLL_sha256']=='ec419b55f41c591f04bf62ac36d6562042f3db7cc69b2d9c56083979e9ca80c1'
 assert sha(meta['prefix_provenance_path'])==meta['prefix_provenance_sha256']
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import model_camera_history_vectors_fixtures as q
 from write_watch_model_camera_history_vectors_state import GraphicsState
 c['graphics_in']=GraphicsState(**{x.name:getattr(c['graphics_in'],x.name) for x in fields(GraphicsState)})
 assert not c['graphics_in'].packets and c['pixel_job_seed'].summary()['processed']==1
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f);scene.c=c
 assert scene.s.pc==scene.s.pr==0x8cfed000
 scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json';q.v.bind_vectors(c['vbr'])
 return scene,q
