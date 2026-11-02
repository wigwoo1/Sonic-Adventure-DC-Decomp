"""Scoped render begin/end APIs and fresh world-call profiles; no boot-state patch."""
import json,pickle
from pathlib import Path
from stage_render_end_verified_state import normal,sha,P,W
from model_scene_verified_state import Scene

def clone(base,q):
 s=Scene.__new__(Scene);s.m=bytearray(base.m);s.s=q.v.effects.State.from_buffer_copy(bytes(base.s));s.f=q.v.effects.Fpu.from_buffer_copy(bytes(base.f));s.c=pickle.loads(pickle.dumps(base.c,protocol=5));s.records=[];s.usable=True;s.prefix=base.prefix;q.v.bind_vectors(s.c['vbr']);return s
def read(s,a):return int.from_bytes(s.m[a&0xffffff:(a&0xffffff)+4],'little')
def main():
 target=P/'analysis/stage_render_end_verification.json';assert not target.exists()
 base,q=normal();cases=[];reached=set();original=(W/'extracted/1ST_READ.BIN').read_bytes()
 def call(s,name,entry,args=(),expected=None,fixture=True):
  r,path=s.call(entry,args,compare=q.fast.compare);reached.update(path)
  assert r['full_state_match'] and r['ordered_boundaries_match'] and s.usable==(r['status']==0)
  if expected is not None:assert r['status']==expected,(name,r['status'],r['stop_pc'])
  assert not any(0x900000<=(a&0xffffff)<0xa00000 for a in path),'Executable overlay dispatch is unqualified'
  for a in set(path):
   off=a&0xffffff
   if 0x10000<=off<=0x10000+len(original)-2:assert bytes(s.m[off:off+2])==original[off-0x10000:off-0x10000+2],hex(a)
  row=dict(name=name,entry=hex(entry),arguments=list(args),status=r['status'],stop_pc=r['stop_pc'],steps=len(path),full_state_match=True,ordered_boundaries_match=True,boundaries=r['boundaries'],command_packets=len(r['command_packets']),declared_API_fixture=fixture,faulted_state_resumed_or_saved=False)
  cases.append(row);print(name,r['status'],r['stop_pc'],len(path),'packets',row['command_packets'],flush=True)
  return r,path
 for bank in (0,1):
  for mode in (0,0x380000):
   s=clone(base,q);s.s.sr=(s.s.sr&~0x20000000)|(bank<<29);s.f.fpscr=(s.f.fpscr&~0x380000)|mode
   call(s,f'begin_RB{bank}_FPU{mode:x}',0x8c038da0,expected=0)
   sp=s.s.r[15];fpu=bytes(s.f);other=s.c['other'];gbr=s.c['gbr'];pointer=read(s,0x8c037008);before=read(s,pointer+8)
   r,path=call(s,f'end_RB{bank}_FPU{mode:x}',0x8c038dfc,expected=0)
   assert [b['target'] for b in r['boundaries']]==['0x8c038dfc','0x8c036ffc','0x8c605e0a']
   assert r['boundaries'][1]['pr']=='0x8c038e04' and r['boundaries'][2]['pr']=='0x8cfed000'
   assert s.s.r[15]==sp and bytes(s.f)==fpu and s.c['gbr']==gbr and s.c['other']==other
   assert read(s,pointer+8)==before|0xc0
 s=clone(base,q);s.s.r[15]=0x8c000000
 r,path=call(s,'end_invalid_stack_entry_guard',0x8c038dfc,expected=-3)
 assert r['stop_pc']=='0x8c038dfc'
 s=clone(base,q)
 r,path=call(s,'unchanged_normal_profile_world',0x8c09878c,(1,),expected=-3,fixture=False)
 assert r['stop_pc']=='0x8c037d5a' and len(r['command_packets'])==2666
 s=clone(base,q);s.s.sr|=0x20000000
 r,path=call(s,'isolated_bank1_profile_world',0x8c09878c,(1,))
 # Even a normal result here is an explicit CPU-entry fixture, not a real boot
 # continuation. No fixture or failed CPU state is saved or made canonical.
 world_result=dict(status=r['status'],stop_pc=r['stop_pc'],packets=len(r['command_packets']),drawn_land_count=int.from_bytes(s.m[0x75a308:0x75a30a],'little'),final_SR=hex(s.s.sr))
 manifest=json.loads((P/'analysis/stage_render_end_translation.json').read_text());addresses={int(a,16) for row in manifest['routines'] for a in row['reachable_addresses']}
 assert addresses<=reached
 files=[Path(__file__),P/'stage_render_end_verified_state.py',P/'stage_render_end_reference.py',P/'analysis/stage_render_end_translation.json',q.v.effects.DLL]
 report=dict(status='verified_render_end_declared_APIs_and_recorded_world_profiles_not_real_game_boot',original_native_comparisons=len(cases),normal_returns=sum(c['status']==0 for c in cases),host_stops=sum(c['status']!=0 for c in cases),cases=cases,new_entry_reached_rows=len(addresses&reached),new_entry_total_rows=len(addresses),bank1_world_result=world_result,original_bank_mode_producer_verified=False,bank1_fixture_canonical=False,new_normal_checkpoint_saved=False,new_native_pixel_jobs=0,partial_packets_submitted_as_frame=False,shared_bank_code_PCs_reached=False,all_paths_qualified=False,real_gameplay_or_complete_frame_qualified=False,new_registry_credit=0,overall_completion_percent=10,goal_paused=False,playable=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('RENDER END VERIFIED:',len(cases),'comparisons; CPU-bank bootstrap remains unqualified.',flush=True)
if __name__=='__main__':main()
