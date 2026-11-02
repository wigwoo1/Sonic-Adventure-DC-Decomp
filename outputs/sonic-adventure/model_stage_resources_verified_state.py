"""Load the qualified job-4 scene before original USA stage-resource calls."""
import hashlib,json,pickle
from dataclasses import fields
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def normal():
 inherited.prefix.checkpoint()
 proof_path=P/'analysis/model_world_land_frame_verification.json'
 proof=json.loads(proof_path.read_text())
 for name,digest in proof['sha256'].items():assert sha(name)==digest,name
 folder=W/'model-world-land-normal';meta=json.loads((folder/'provenance.json').read_text())
 assert sha(folder/'provenance.json')=='01c0e0b5ccc36aa562c9b5fbd79e534955eef1d2eed37c352b25a7f5557449b5'
 assert not meta['faulted_state_saved'] and meta['completed_native_jobs']==4 and meta['active_packets']==0
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']=='abc15b26035ca1446080911550ad790394af8512569976934833e3999a1ebba1'
 assert sha(meta['DLL_path'])==meta['DLL_sha256']=='e299197f0efd020d3819ca51b014cca10ff481868c66ae81f1c41368ce2512b5'
 assert sha(meta['prefix_provenance_path'])==meta['prefix_provenance_sha256']
 assert all(row['status']==0 and row['stop_pc']=='0x8cfed000' for row in meta['phases'])
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import model_stage_resources_fixtures as q
 from write_watch_model_stage_resources_state import GraphicsState
 c['graphics_in']=GraphicsState(**{field.name:getattr(c['graphics_in'],field.name) for field in fields(GraphicsState)})
 assert not c['graphics_in'].packets and c['pixel_job_seed'].summary()['processed']==4
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f)
 scene.c=c;scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json'
 assert scene.s.pc==scene.s.pr==0x8cfed000
 q.v.bind_vectors(c['vbr'])
 return scene,q
