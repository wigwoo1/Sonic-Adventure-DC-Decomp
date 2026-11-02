"""Observe the matched index-stream stop from the last normal textured scene."""
import hashlib,json,pickle,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'outputs/sonic-adventure';W=R/'work/sonic-adventure';sys.path.insert(0,str(P))
from stage_texture_load_verified_state import normal,sha
from model_scene_verified_state import Scene
base,q=normal();folder=W/'stage-texture-camera-normal';meta=json.loads((folder/'provenance.json').read_text())
assert not meta['faulted_state_saved'] and sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256'] and sha(meta['DLL_path'])==meta['DLL_sha256']==sha(q.v.effects.DLL)
m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes())
scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f);scene.c=c;scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json';q.v.bind_vectors(c['vbr'])
result,path=scene.call(0x8c09878c,(1,),compare=q.fast.compare)
assert result['status']==-3 and result['stop_pc']=='0x8c037d5a' and not scene.usable
models=[b for b in result['boundaries'] if b['target']=='0x8c037108']
def words(a,n):
 if a&0x1f000000!=0x0c000000 or (a&0xffffff)>0x1000000-4*n:return None
 return [int.from_bytes(scene.m[(a&0xffffff)+4*i:(a&0xffffff)+4*i+4],'little') for i in range(n)]
last=models[-1] if models else None;model=last['arguments'][0] if last else 0
j=dict(status='matched_original_primitive_index_pointer_guard',host_status=result['status'],stop_pc=result['stop_pc'],faulting_read=hex(scene.s.r[4]),remaining_strip_counter=hex(scene.s.r[12]),registers=[hex(x) for x in scene.s.r],last_model_boundary=last,last_model_words=words(model,12),last_boundaries=result['boundaries'][-12:],partial_packets=len(result['command_packets']),model_boundaries=len(models),full_state_match=result['full_state_match'],ordered_boundaries_match=result['ordered_boundaries_match'],faulted_state_resumed_or_saved=False,geometry_globals_or_callee_results_patched=False,source_checkpoint_sha256=meta['checkpoint_sha256'],DLL_sha256=sha(q.v.effects.DLL),overall_completion_percent=10,goal_paused=False)
target=W/'stage_primitive_boundary_diagnostic.json';assert not target.exists();target.write_text(json.dumps(j,indent=2)+'\n');print(json.dumps({k:v for k,v in j.items() if k!='last_boundaries'},indent=2),flush=True)
