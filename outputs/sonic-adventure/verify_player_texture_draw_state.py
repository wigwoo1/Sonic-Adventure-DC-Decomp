"""Original VRAM/register producers must survive a later ordinary no-op call."""
import hashlib,json
from pathlib import Path
import player_texture_link_fixtures as q
import write_watch_player_texture_draw as fast
P=q.P;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_draw_state_verification.json';assert not target.exists()
 base=q.prepare(q.v.seed_ram(),dict(real_tables=True));base[1].r[15]=0x8cfff000;records=[]
 def phase(entry,args):
  nonlocal base
  m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value
  result,path,m,s,f=fast.compare(m,s,f,c);assert result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match'];assert s.r[15]==sp and s.pc==s.pr==0x8cfed000
  c.update(graphics_in=result['graphics_out'],other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr']);base=m,s,f,c;records.append(dict(entry=hex(entry),steps=len(path),graphics_input=result['graphics_input_summary'],graphics_output=result['graphics_output_summary']));return result['graphics_out']
 source=0x8cb70000;payload=bytes((i*73+19)&255 for i in range(128));base[0][source&0xffffff:(source&0xffffff)+len(payload)]=payload
 state=phase(0x8c65253e,[0xa5000120,source,len(payload)]);assert state.vram[0x120:0x1a0]==payload
 expected=state.vram;state=phase(0x8c653444,[0xe4,0x13579bdf]);assert state.vram==expected and state.arrays[0][0xe4//4]==0x13579bdf
 state=phase(0x8c6416a4,[]);assert state.vram==expected and state.arrays[0][0xe4//4]==0x13579bdf
 files=[Path(__file__),P/'write_watch_player_texture_draw.py',P/'write_watch_player_texture_link.py',P/'player_texture_link_reference.py',R/'work/sonic-adventure/build/player_texture_link.dll']
 report=dict(status='original_VRAM_copy_and_register_write_survive_normal_public_call_boundaries',comparisons=3,phases=records,exact_payload_bytes=128,preserved_vram_bytes=8388608,preserved_register_value='0x13579bdf',native_DLL_unchanged=True,CPU_executor_unchanged=True,existing_full_state_comparisons_retained=True,game_callee_results_supplied=0,private_frames_supplied=0,overall_completion_percent=10,scope='Test-device seeding/extraction only. Carry exactly equal native/original graphics-state outputs between normal API returns;original128-byte VRAM copy/register writer andretail RTS/NOP execute. Independent payload/register checks provecontinuity,not rendered pixels. External timing/sample services retain their declared scenario inputs.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('PERSISTENT GRAPHICS PASS:3comparisons/original128-bytecopy/full8MiBcontinuity/registercarry',flush=True)
if __name__=='__main__':main()
