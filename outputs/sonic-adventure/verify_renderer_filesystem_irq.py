"""Actual filesystem construction, gate controls and real shared-worker execution."""
import hashlib,itertools,json,shlex
from pathlib import Path
import renderer_filesystem_irq_fixtures as q
P=q.P;R=P.parents[1];ENTRY=0x8c5ffcf0;WORKER=0x8c6005c2


def carry(c,r):
 c.update(frame_events_seed=r['frame_events_state'],graphics_in=r['graphics_out'],other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
 if r['runtime_state'] is not None:c['runtime_seed']=r['runtime_state']
 for name,value in zip(q.v.EXCEPTION_KEYS,r['exception_state']):c[name]=value


def main():
 target=P/'analysis/renderer_filesystem_irq_verification.json';assert not target.exists()
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0));s.r[15]=0x8cfff000;s.sr&=~0xf0;c['vbr']=0x8ce50000;q.v.bind_vectors(0x8ce50000);setup=[]
 def phase(entry,args,label,stack=None):
  nonlocal m,s,f,c
  sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  r,path,m,s,f=q.fast.compare(m,s,f,c)
  print(label,r['status'],r['stop_pc'],flush=True)
  assert r['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp
  assert r['full_state_match'] and r['ordered_boundaries_match'];carry(c,r)
  setup.append(dict(label=label,entry=hex(entry),steps=len(path),disc_read_attempts=r['disc_read_attempts'],return_value=hex(s.r[0])))
  print(label,'returned',hex(s.r[0]),flush=True);return r
 phase(0x8c645de0,[0,0x8ce40000],'original vector initialization')
 phase(0x8c602128,[0x8cb00000,0x100000],'original system heap initialization')
 phase(0x8c604702,[1,0,1,15],'original display and library initialization')
 phase(0x8c604dce,[-1,0,0x20000,0],'original allocated context initialization',stack=0)
 phase(0x8c098a10,[],'original game heap initialization')
 phase(0x8c098b0c,[1,0x4020],'actual filesystem-context allocation');context=(s.r[0]+31)&~31
 phase(0x8c098b0c,[1,0x30020],'actual directory allocation');directory=(s.r[0]+31)&~31
 q.put(m,q.get(m,0x8c6000e4),0);q.put(m,0x8c673420,0)
 inventory=json.loads((P/'disc_inventory.json').read_text());track=next(x for x in reversed([shlex.split(line) for line in inventory['gdi'].splitlines()[1:] if line.strip()]) if x[2]=='4');q.put(m,0x8c66c530,int(track[1])+166)
 r=phase(0x8c601618,[4,context,4096,directory],'original real-disc filesystem mount')
 assert s.r[0]==0 and r['disc_read_attempts']==21
 base=(bytes(m),bytes(s),bytes(f),dict(c));cases=[];covered=set();edges=set();checks=0
 for pointer,gate,bank,t in itertools.product((context,0x8cda0000),(0,1,2,0xffffffff),(0,0x200000),(0,1)):
  a=bytearray(base[0]);s=q.v.effects.State.from_buffer_copy(base[1]);f=q.v.effects.Fpu.from_buffer_copy(base[2]);c=dict(base[3])
  q.put(a,pointer+0x20,gate);s.pc=c['entry']=ENTRY;s.pr=0x8cfed000;s.r[4]=pointer;s.sr=(s.sr&~1)|t;f.fpscr=(f.fpscr&~0x200000)|bank
  sp=s.r[15];before=bytes(a);before_f=bytes(f)
  r,path,actual,s,f=q.fast.compare(a,s,f,c)
  assert r['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp;checks+=3
  assert r['full_state_match'] and r['ordered_boundaries_match'] and bytes(f)==before_f;checks+=3
  assert (WORKER in path)==(gate!=1);checks+=1
  if gate==1:assert s.r[0]==1 and s.sr&1 and bytes(actual)==before;checks+=3
  else:
   calls=[b for b in r['boundaries'] if b['target']==hex(WORKER)]
   assert len(calls)==1 and calls[0]['pr']=='0x8cfed000' and calls[0]['arguments'][0]==pointer;checks+=3
  assert r['disc_read_attempts']==0;checks+=1
  body=q.v.PER_BODY[ENTRY];covered.update(set(path)&set(body))
  for i,pc in enumerate(path):
   text=body.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  cases.append(dict(pointer=hex(pointer),gate=hex(gate),FPU_bank=bank>>21,initial_T=t,steps=len(path),return_value=hex(s.r[0]),worker_executed=WORKER in path,original_native_matched=True))
  print('Gate',hex(pointer),hex(gate),bank>>21,t,'actual worker',WORKER in path,flush=True)
 translation=json.loads((P/'analysis/renderer_filesystem_callback_translation.json').read_text());b=translation['routines'][0]
 assert covered=={int(x,16) for x in b['reachable_addresses']} and {tuple(int(x,16) for x in e) for e in b['conditional_edges']}<=edges
 files=[Path(__file__),P/'renderer_filesystem_irq_reference.py',P/'write_watch_renderer_filesystem_irq.py',P/'analysis/renderer_filesystem_callback_translation.json',P/'analysis/renderer_filesystem_callback_connection.json',R/'work/sonic-adventure/build/renderer_filesystem_callback.dll']
 report=dict(status='original_gate_and_real_idle_filesystem_tail_verified',original_native_comparisons=len(setup)+len(cases),setup_calls=len(setup),gate_cases=len(cases),checks=checks,gate_instruction_rows_executed=len(covered),gate_conditional_outcomes_executed=len(edges),shared_tail_new_credit=0,context_from_original_allocation_and_mount=True,context=hex(context),directory=hex(directory),setup=setup,cases=cases,backend_results_supplied=False,game_callee_outputs_supplied=0,allocator_outputs_supplied=0,private_execution_frames_supplied=0,new_original_routine_credit=0,scope='Original heap/calloc/real-disc mount produce context andglobal worker tables. Fresh normal-return input percase coversgate0/1/2/ffffffff,twoFPUbanks,bothTvalues,andactual/alternate callerrecords. Non1gatesexecute realexisting6005c2,not a hoststub;locked gatepreservesallRAM. TailPR/argument andfullmachine/RAM/VRAM/device/boundary equality checked. Declared callerflag/recordinputs andidle mounted filesystem only;sharedworker pending-I/O branches arecanonicalreuse,not newqualification/credit.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Callback',len(cases),'gate cases',checks,'checks',flush=True)


if __name__=='__main__':main()
