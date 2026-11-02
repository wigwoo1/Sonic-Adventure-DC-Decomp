"""Load the hash-pinned normal scene/camera prefix before any particle overflow."""
import hashlib,json,pickle
from dataclasses import fields
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def normal():
 inherited.prefix.checkpoint()
 proof=json.loads((P/'analysis/model_camera_lighting_owner_verification.json').read_text())
 for name,h in proof['sha256'].items():assert sha(name)==h,name
 folder=W/'model-camera-render-ready';meta=json.loads((folder/'provenance.json').read_text())
 assert not meta['faulted_state_saved'] and len(meta['phases'])==9
 assert all(p['status']==0 and p['stop_pc']=='0x8cfed000' for p in meta['phases'])
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']
 assert sha(meta['DLL_path'])==meta['DLL_sha256']=='27fdcbc8feebe0bf06f94808e1e80eb588144ec286d733260a0cc1490f07bfb1'
 assert sha(meta['prefix_provenance_path'])==meta['prefix_provenance_sha256']
 # Deserialize the older graphics-state class before selecting the new family.
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import model_ta_stream_fixtures as q
 from write_watch_model_ta_stream_state import GraphicsState
 c['graphics_in']=GraphicsState(**{x.name:getattr(c['graphics_in'],x.name) for x in fields(GraphicsState)})
 assert not c['graphics_in'].packets and c.get('pixel_job_seed') is None
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f);scene.c=c
 assert scene.s.pc==scene.s.pr==0x8cfed000
 scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json';q.v.bind_vectors(c['vbr'])
 return scene,q
