"""Original tick/submission branches with explicit RAM and peripheral controls."""
import dataclasses,hashlib,itertools,json
from pathlib import Path
import renderer_input_tick_fixtures as q
from verify_renderer_filesystem_irq import carry
P=Path(__file__).resolve().parent;R=P.parents[1]
TICK,TAIL,NOP=0x8c646024,0x8c6463ac,0x8c05180c

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/renderer_input_tick_verification.json';assert not target.exists()
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0))
 s.r[15]=0x8cfff000;s.sr&=~0xf0;c['vbr']=0x8ce50000;q.v.bind_vectors(c['vbr']);setup=[]
 for entry,args,stack in ((0x8c645de0,[0,0x8ce40000],None),(0x8c602128,[0x8cb00000,0x100000],None),(0x8c604702,[1,0,1,15],None),(0x8c604dce,[-1,0,0x20000,0],0),(0x8c601a34,[0,q.get(m,0x8c0102cc),q.get(m,0x8c0102c8)],None)):
  sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,a in enumerate(args):s.r[4+i]=a&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  result,path,m,s,f=q.fast.compare(m,s,f,c)
  assert result['status']==0 and result['full_state_match'] and result['ordered_boundaries_match'] and s.r[15]==sp and s.pc==s.pr==0x8cfed000
  carry(c,result);setup.append(dict(entry=hex(entry),steps=len(path)));print('Original setup',hex(entry),'returned',flush=True)
 root=q.get(m,0x8c8a83b4);assert root==0x8c8a72b8
 baseline=(bytes(m),bytes(s),bytes(f),dict(c));seen=set();edges=set();records=[];checks=0
 def mmio(cfg,address,value):
  arrays=list(cfg['graphics_in'].arrays)
  bank,offset=(0,address-0x5f8000) if 0x5f8000<=address<0x5f8200 else (4,address-0x5f6800)
  words=list(arrays[bank]);words[offset//4]=value;arrays[bank]=tuple(words)
  cfg['graphics_in']=dataclasses.replace(cfg['graphics_in'],arrays=tuple(arrays))
 cases=[]
 for gate,callback,counter,bank,t in itertools.product((0,1),(0,NOP),(0,0xffffffff),(0,0x200000),(0,1)):
  cases.append(dict(kind='tick_gate',entry=TICK,gate=gate,callback=callback,counter=counter,bank=bank,t=t))
 cases.append(dict(kind='disabled_submission',entry=TAIL,enabled=0))
 for status,error,callback,dirty,bank in itertools.product((0,1),(0,0xf00),(0,NOP),(0,1),(0,1)):
  cases.append(dict(kind='submission',entry=TAIL,status=status,error=error,callback=callback,dirty=dirty,buffer_bank=bank))
 for busy,lock,error,callback in itertools.product((0,1),(0,1),(0,0xf00),(0,NOP)):
  cases.append(dict(kind='submission_readiness',entry=TAIL,status=1,busy=busy,lock=lock,error=error,callback=callback))
 for case in cases:
  m=bytearray(baseline[0]);s=q.v.effects.State.from_buffer_copy(baseline[1]);f=q.v.effects.Fpu.from_buffer_copy(baseline[2]);c=dict(baseline[3])
  s.pc=c['entry']=case['entry'];s.pr=0x8cfed000;sp=s.r[15]
  f.fpscr=(f.fpscr&~0x200000)|case.get('bank',0);s.sr=(s.sr&~1)|case.get('t',0)
  q.put(m,0x8c894098,case.get('gate',0));q.put(m,0x8c893e48,case.get('callback',0));q.put(m,0x8c893e18,case.get('counter',0))
  if 'enabled' in case:q.put(m,0x8c894090,case['enabled'])
  if case['kind'].startswith('submission'):
   q.put(m,root+0xfc0,case.get('status',1));q.put(m,0x8c8940a4,case.get('dirty',0));q.put(m,0x8c8940a0,case.get('dirty',0));q.put(m,0x8c894094,case.get('buffer_bank',0))
   # These are declared device/register observations, not game-service results.
   mmio(c,0x5f6908,case.get('error',0));mmio(c,0x5f6c18,case.get('busy',0));mmio(c,0x5f80c4,0xdeadbeef);mmio(c,0x5f810c,0x12345678)
   lock_pointer=q.get(m,q.get(m,0x8c65f154))
   assert lock_pointer&0x1f000000==0x0c000000
   if case.get('lock'):q.put(m,lock_pointer,case['lock'],1)
  r,path,a,after,fp=q.fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  assert r['status']==0 and after.pc==after.pr==0x8cfed000 and after.r[15]==sp;checks+=3
  assert bytes(fp)==bytes(f);checks+=1
  if case['entry']==TICK:
   assert q.get(a,0x8c893e18)==(case['counter']+1)&0xffffffff;checks+=1
   callbacks=[b for b in r['boundaries'] if b['target']==hex(NOP)]
   if case['gate']:
    assert (TAIL in path)==False and len(callbacks)==int(bool(case['callback']));checks+=2
    if callbacks:assert callbacks[0]['arguments']==[0,0xffffffff,(case['counter']+1)&0xffffffff,0];checks+=1
   else:assert TAIL in path;checks+=1
  seen.update(path)
  for i,pc in enumerate(path):
   text=q.v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  records.append(dict(input=case,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),return_value=hex(after.r[0]),boundaries=r['boundaries']))
  if len(records)%16==0:print('Input tick cases',len(records),flush=True)
 manifest=json.loads((P/'analysis/renderer_input_tick_translation.json').read_text());coverage=[]
 for body in manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-{(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],instruction_rows=body['instruction_rows'],missing_rows=sorted(missing),missing_conditional_outcomes=sorted(missing_edges)))
 files=[Path(__file__),P/'renderer_input_tick_reference.py',P/'renderer_input_tick_fixtures.py',P/'write_watch_renderer_input_tick.py',P/'analysis/renderer_input_tick_translation.json',P/'analysis/renderer_input_tick_connection.json',R/'work/sonic-adventure/build/renderer_input_tick.dll']
 report=dict(status='original_input_tick_and_submission_declared_controls_matched',original_native_comparisons=len(setup)+len(records),normal_returns=len(setup)+len(records),cases=len(records),checks=checks,setup=setup,coverage=coverage,controls=records,new_original_routine_credit=0,game_callee_outputs_supplied=0,allocator_outputs_supplied=0,private_execution_frames_supplied=0,whole_game_qualified=False,physical_GPU_pixels_qualified=False,overall_completion_percent=10,scope='Fresh cases clone only actual normal-return constructor outputs. Declared RAM flags/counters/callback bindings and peripheral registers exercise tick and submission decisions. Callback target 05180c is the real retained original no-op routine, not a host stub. All game callees including input preflight, packet builder and submission execute natively and in original reference. Entire CPU/FPU/RAM/VRAM/device and ordered boundary comparisons remain mandatory. Coverage gaps are listed explicitly; this report alone grants no recovery credit.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Verified',len(records),'cases',checks,'checks',json.dumps(coverage),flush=True)

if __name__=='__main__':main()
