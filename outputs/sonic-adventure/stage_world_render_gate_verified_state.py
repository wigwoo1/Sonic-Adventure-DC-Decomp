"""Load the normal loaded-stage task before original world rendering."""
import hashlib,json,pickle
from dataclasses import fields
from pathlib import Path
from model_scene_verified_state import Scene,inherited
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure'
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def normal():
 inherited.prefix.checkpoint()
 proof_path=P/'analysis/model_stage_resource_chain_verification.json'
 proof=json.loads(proof_path.read_text())
 for name,digest in proof['sha256'].items():assert sha(name)==digest,name
 folder=W/'model-stage-resources-task-created-normal';meta=json.loads((folder/'provenance.json').read_text())
 assert sha(folder/'provenance.json')=='192a1381156fb8f03099ac91d29e41038f1c659a2e54114b49f6f1981a64cdeb'
 assert not meta['faulted_state_saved'] and meta['completed_native_jobs']==4 and meta['active_packets']==0
 assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']=='ceff80d3a031043c705da6afe417497ba6231922ceece52938b8115094dfbdfa'
 assert sha(meta['DLL_path'])==meta['DLL_sha256']=='7f0ff640dd5c91d166c9f321f684affe135a41ad9551d1b944806fce6bffc72e'
 assert sha(meta['prefix_provenance_path'])==meta['prefix_provenance_sha256']
 assert all(row['status']==0 and row['stop_pc']=='0x8cfed000' for row in meta['phases'])
 m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
 import stage_world_render_gate_fixtures as q
 from write_watch_stage_world_render_gate_state import GraphicsState
 c['graphics_in']=GraphicsState(**{field.name:getattr(c['graphics_in'],field.name) for field in fields(GraphicsState)})
 assert not c['graphics_in'].packets and c['pixel_job_seed'].summary()['processed']==4
 scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f)
 scene.c=c;scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json'
 assert scene.s.pc==scene.s.pr==0x8cfed000
 q.v.bind_vectors(c['vbr'])
 return scene,q
