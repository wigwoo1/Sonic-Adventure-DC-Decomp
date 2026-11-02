"""Isolated bank-1 entry-profile experiment; never a canonical game state."""
import json,pickle,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'outputs/sonic-adventure';W=R/'work/sonic-adventure';sys.path.insert(0,str(P))
from stage_texture_load_verified_state import normal,sha
from model_scene_verified_state import Scene
base,q=normal();folder=W/'stage-texture-camera-normal';meta=json.loads((folder/'provenance.json').read_text());assert sha(folder/'normal_public_return.pkl')==meta['checkpoint_sha256']
m,s,f,c=pickle.loads((folder/'normal_public_return.pkl').read_bytes());scene=Scene.__new__(Scene);scene.m=bytearray(m);scene.s=q.v.effects.State.from_buffer_copy(s);scene.f=q.v.effects.Fpu.from_buffer_copy(f);scene.c=c;scene.records=[];scene.usable=True;scene.prefix=folder/'provenance.json';q.v.bind_vectors(c['vbr'])
original_sr=scene.s.sr
# This defines a standalone privileged bank-1 call profile. It does not execute
# or replace the original game's missing mode producer, and is not saved.
scene.s.sr|=0x20000000
result,path=scene.call(0x8c09878c,(1,),compare=q.fast.compare)
j=dict(status='isolated_bank1_renderer_API_observation_not_canonical_game',original_SR=hex(original_sr),fixture_SR=hex(original_sr|0x20000000),host_status=result['status'],stop_pc=result['stop_pc'],steps=len(path),full_state_match=result['full_state_match'],ordered_boundaries_match=result['ordered_boundaries_match'],partial_packets=len(result['command_packets']),final_SR=hex(scene.s.sr),faulting_registers=[hex(x) for x in scene.s.r],last_boundaries=result['boundaries'][-10:],normal_game_state_saved=False,original_mode_producer_executed=False,geometry_or_callee_results_patched=False,CPU_entry_SR_is_explicit_fixture=True,DLL_sha256=sha(q.v.effects.DLL),overall_completion_percent=10,goal_paused=False)
target=W/'stage_render_bank_contract_observation.json';assert not target.exists();target.write_text(json.dumps(j,indent=2)+'\n');print(json.dumps({k:v for k,v in j.items() if k not in ['faulting_registers','last_boundaries']},indent=2),flush=True)
