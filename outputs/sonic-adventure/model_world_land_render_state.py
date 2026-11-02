"""Carry the hash-pinned complete particle pass into the submitted-job backend."""
import hashlib,json,pickle
from dataclasses import fields
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def normal():
 inherited.prefix.checkpoint()
 proof=json.loads((P/'analysis/model_ta_stream_v3_verification.json').read_text())
 for name,h in proof['sha256'].items():assert sha(name)==h,name
 folder=W/'model-ta-stream-normal-v3';meta=json.loads((folder/'provenance.json').read_text())
 assert not meta['faulted_state_saved'] and meta['queued_packets']==186
 assert all(p['status']==0 and p['stop_pc']=='0x8cfed000' for p in meta['phases'])
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']
 assert sha(meta['DLL_path'])==meta['DLL_sha256']=='2a04b8e0618d788fb525326a24fa5eb95853fcf7651fd862e6615955f2337302'
 assert sha(meta['prefix_provenance_path'])==meta['prefix_provenance_sha256']
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import model_world_land_fixtures as q
 from write_watch_model_world_land_state import GraphicsState
 c['graphics_in']=GraphicsState(**{x.name:getattr(c['graphics_in'],x.name) for x in fields(GraphicsState)})
 assert len(c['graphics_in'].packets)==186 and c.get('pixel_job_seed') is None
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f);scene.c=c
 assert scene.s.pc==scene.s.pr==0x8cfed000
 scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json';q.v.bind_vectors(c['vbr'])
 return scene,q
