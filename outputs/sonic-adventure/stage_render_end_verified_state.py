"""Load the normal textured camera state before renderer end recovery."""
import hashlib,json,pickle
from dataclasses import fields
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def normal():
 inherited.prefix.checkpoint()
 proof_path=P/'analysis/stage_texture_world_chain_verification.json'
 proof=json.loads(proof_path.read_text())
 for name,digest in proof['sha256'].items():assert sha(name)==digest,name
 folder=W/'stage-texture-camera-normal';meta=json.loads((folder/'provenance.json').read_text())
 assert sha(folder/'provenance.json')=='fc266f68573ec295404f72a968bcc2c8c0ff95c6b30c47a9f4408aa11b3eb3ad'
 assert not meta['faulted_state_saved'] and meta['completed_native_jobs']==4 and meta['active_packets']==0
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']=='55ae68eb5b981d223947e5fff94e8cf4e839a21cebccc19254e6326c6f4eede3'
 assert sha(meta['DLL_path'])==meta['DLL_sha256']=='ffd4f8d69aca7d4b91ca8c5423df834509a74e6fbb61c07193b30042fccda943'
 assert sha(meta['prefix_provenance_path'])==meta['prefix_provenance_sha256']
 assert all(row['status']==0 and row['stop_pc']=='0x8cfed000' for row in meta['phases'])
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import stage_render_end_fixtures as q
 from write_watch_stage_render_end_state import GraphicsState
 c['graphics_in']=GraphicsState(**{field.name:getattr(c['graphics_in'],field.name) for field in fields(GraphicsState)})
 assert not c['graphics_in'].packets and c['pixel_job_seed'].summary()['processed']==4
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f)
 scene.c=c;scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json'
 assert scene.s.pc==scene.s.pr==0x8cfed000
 q.v.bind_vectors(c['vbr'])
 return scene,q
