"""Read the reached material/texture pointers without patching or resuming faults."""
import json,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'outputs/sonic-adventure';W=R/'work/sonic-adventure';sys.path.insert(0,str(P))
from stage_world_render_gate_verified_state import normal,sha
def read(s,a,n=4):
 assert a&0x1f000000==0x0c000000 and (a&0xffffff)<=len(s.m)-n
 return int.from_bytes(s.m[a&0xffffff:(a&0xffffff)+n],'little')
scene,q=normal();first,path=scene.call(0x8c09878c,(0,),compare=q.fast.compare);assert first['status']==0
result,path=scene.call(0x8c09878c,(1,),compare=q.fast.compare)
assert result['status']==-3 and result['stop_pc']=='0x8c03764a' and not scene.usable
slot=read(scene,0x8c037670);texlist=read(scene,slot);names=read(scene,texlist);offset=scene.s.r[0];entry=names+offset
j=dict(status='matched_original_material_texture_pointer_boundary',stop_pc=result['stop_pc'],host_status=result['status'],faulting_R1=hex(scene.s.r[1]),texture_list_global=hex(slot),texture_list=hex(texlist),texture_names=hex(names),texture_count=read(scene,texlist+4),material_texture_byte_offset=offset,material_texture_index=offset//12,texture_name_entry=hex(entry),texture_entry_words=[read(scene,entry+4*i) for i in range(3)],world_drawn_count=read(scene,0x8c75a308,2),full_state_match=result['full_state_match'],ordered_boundaries_match=result['ordered_boundaries_match'],geometry_globals_or_callee_results_patched=False,faulted_state_resumed_or_saved=False,DLL_sha256=sha(q.v.effects.DLL),next_original_stage_texture_loader='8c099220',next_original_act_texture_loader='8c0995e0',overall_completion_percent=10,goal_paused=False)
target=W/'stage_texture_boundary_diagnostic.json';assert not target.exists();target.write_text(json.dumps(j,indent=2)+'\n');print(json.dumps(j,indent=2),flush=True)
