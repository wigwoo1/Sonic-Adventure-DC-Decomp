"""Load the qualified job-3 scene before original USA land-producer calls."""
import hashlib,json,pickle
from dataclasses import fields
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def normal():
 inherited.prefix.checkpoint()
 proof_path=P/'analysis/model_world_writers_verification.json'
 proof=json.loads(proof_path.read_text())
 for name,digest in proof['sha256'].items():assert sha(name)==digest,name
 folder=W/'model-world-writers-normal';meta=json.loads((folder/'provenance.json').read_text())
 assert sha(folder/'provenance.json')=='15950e6c28ee0345b58f8e4647f3fe9f2d907974027a5a211a4cfe14463776c7'
 assert not meta['faulted_state_saved'] and meta['completed_native_jobs']==3 and meta['active_packets']==0
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']=='01e412eebad8e6d5644c7e67c49dd0368d2978aa6033115aef581a6f63677722'
 assert sha(meta['DLL_path'])==meta['DLL_sha256']=='7574f9c22443af38667c4a0f11bb1d27b2d090c2f23f43c84fb0d5cb15ac6d0a'
 assert sha(meta['prefix_provenance_path'])==meta['prefix_provenance_sha256']
 assert all(row['status']==0 and row['stop_pc']=='0x8cfed000' for row in meta['phases'])
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import model_world_land_fixtures as q
 from write_watch_model_world_land_state import GraphicsState
 c['graphics_in']=GraphicsState(**{field.name:getattr(c['graphics_in'],field.name) for field in fields(GraphicsState)})
 assert not c['graphics_in'].packets and c['pixel_job_seed'].summary()['processed']==3
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f)
 scene.c=c;scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json'
 assert scene.s.pc==scene.s.pr==0x8cfed000
 q.v.bind_vectors(c['vbr'])
 return scene,q
