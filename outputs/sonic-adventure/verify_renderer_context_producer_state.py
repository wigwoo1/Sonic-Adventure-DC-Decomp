"""Original context allocation with independent heap/cursor/record oracles."""
import hashlib,itertools,json
from pathlib import Path
import renderer_context_producer_fixtures as q
P=q.P;R=P.parents[1]


def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_context_producer_state_verification.json';assert not target.exists()
 records=[];checks=oracles=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 for mode,rm in itertools.product((0,4),(0,1)):
  base=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=mode,rm=rm));base[1].r[15]=0x8cfff000
  exact(q.get(base[0],0x8c88fcd4)==q.get(base[0],0x8c88fcb4)==0,'both original roots initially null')
  def phase(entry,args,label,stack=None):
   nonlocal base
   m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
   for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
   if stack is not None:q.put(m,sp,stack)
   r,path,m,s,f=q.fast.compare(m,s,f,c)
   require(r['status']==0,(label,r['status'],r['stop_pc']));require(r['full_state_match'] and r['ordered_boundaries_match'],'complete RAM/CPU/FPU/hidden state andordered boundaries');require(s.pc==s.pr==0x8cfed000 and s.r[15]==sp,'balanced ordinary return')
   c.update(graphics_in=r['graphics_out'],other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr']);base=m,s,f,c
   records.append(dict(mode=mode,rounding=rm,label=label,entry=hex(entry),steps=len(path),status=0,boundaries=r['boundaries'],context_root=hex(q.get(m,0x8c88fcd4)),context_words=[q.get(m,q.get(m,0x8c88fcd4)+4*i) for i in range(5)] if q.get(m,0x8c88fcd4) else [],graphics_input=r['graphics_input_summary'],graphics_output=r['graphics_output_summary']))
   return r
  phase(0x8c602128,[0x8cb00000,0x100000],'actual system-heap initialization')
  exact(q.get(base[0],0x8cb00004)==0x100000//32,'independent arena units after real heap constructor')
  phase(0x8c604702,[1,0,1,15],'actual display and input-record constructor')
  exact(q.get(base[0],0x8c88fdb4)==96 and q.get(base[0],0x8c88fdd4)==0x8c88fcd4,'independent record size andoutput cell')
  exact(q.get(base[0],0x8c88fcd4)==0,'record constructor does not itself create context')
  r=phase(0x8c604dce,[-1,0,0x20000,0],'actual allocated context constructor',stack=0)
  m=base[0];request=4*(100+0+0x20000+0+0)+0x160
  calls=[b for b in r['boundaries'] if b['target']=='0x8c6021d6'];exact(len(calls)==1 and calls[0]['arguments'][0]==request,'independent normalized caller counts and allocation byte formula')
  units=(request+31)//32+1;header=0x8cc00000-units*32;owner=header+32
  exact(q.get(m,0x8c88fdd8)==(owner|0x20000000),'independent descending heap allocation andoriginal uncached alias')
  exact(q.get(m,header+4)==units and q.get(m,0x8cb00004)==(header-0x8cb00000)//32,'independent allocated andremaining free units')
  exact(q.get(m,0x8c88fcd4)==0x8c8a33c0,'independent indirect output root')
  exact([q.get(m,0x8c8a33c0+i*4) for i in range(5)]==[0x10000000,0,(owner+32)|0x20000000,0,0],'independent configured context flags andreal owned command cursor')
  exact(any(b['target']=='0x8c656940' and b['arguments'][0]==0x8c88fdb4 for b in r['boundaries']),'actual original wrapper passes its actual constructed record')
  print('CONTEXT PRODUCER',mode,rm,'PASS',hex(owner),flush=True)
 files=[Path(__file__),P/'renderer_context_producer_fixtures.py',P/'renderer_context_owner_fixtures.py',P/'renderer_context_owner_reference.py',P/'write_watch_renderer_context_owner_capture.py',P/'write_watch_renderer_context_owner_state.py',P/'analysis/renderer_context_owner_compile_checks.json',R/'work/sonic-adventure/build/renderer_context_owner.dll']
 report=dict(status='original_context_producers_match_with_independent_heap_record_and_cursor_oracles',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,phases=records,renderer_context_pointer_supplied=False,allocator_or_callee_results_supplied=0,private_execution_frames_supplied=0,initial_RAM_is_original_loaded_image=True,new_original_routine_credit=0,native_DLL_unchanged=True,native_wait_guards_changed=False,physical_GPU_pixels_qualified=False,full_game_qualified=False,overall_completion_percent=10,scope='Actual602128/604702/604dce/656940 ordinary producers;four declared FPU-bank/rounding profiles.Independent normalized allocation formula,free/allocated heap units,original uncached alias,constructed record/output pointer andcontext cursor. Declared GPU identity/display-mode/scan/IRQ inputs andcaller arena/counts remain component inputs. Not broad initializer domains,reset boot,physical hardware,frame consumption ordisplayed pixels. No new routine credit.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('CONTEXT STATE PROOF',len(records),checks,oracles,flush=True)


if __name__=='__main__':main()
