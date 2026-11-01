"""Original frame runs continuously through a declared external VBlank stream IRQ."""
import hashlib,json
from pathlib import Path
import renderer_list_completion_fixtures as q
P=q.P;R=P.parents[1];VBR=0x8ce50000


def carry(c,r):
 c['frame_events_seed']=r['frame_events_state']
 c.update(graphics_in=r['graphics_out'],other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
 if r['runtime_state'] is not None:c['runtime_seed']=r['runtime_state']
 for name,value in zip(q.v.EXCEPTION_KEYS,r['exception_state']):c[name]=value


def main():
 target=P/'analysis/renderer_list_completion_state_verification.json';frontier=P/'analysis/renderer_list_completion_state_frontier.json';assert not target.exists() and not frontier.exists()
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0))
 s.r[15]=0x8cfff000;s.sr&=~0xf0;c['vbr']=VBR;q.v.bind_vectors(VBR);records=[]
 phases=[(0x8c645de0,[0,0x8ce40000],'actual original vector initialization',None),(0x8c602128,[0x8cb00000,0x100000],'original system heap',None),(0x8c604702,[1,0,1,15],'original display and graphics IRQ registration',None),(0x8c604dce,[-1,0,0x20000,0],'original allocated renderer context',0),(0x8c6050b0,[],'original whole frame with declared external VBlank stream',None)]
 for entry,args,label,stack in phases:
  sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  if entry==0x8c6050b0:
   assert q.get(m,VBR+0x224)==0x8c652210
   c.update(ticks=list(q.FRAME_TICKS),runtime_attached=True,irq_samples=[8]*64)
   device=q.v.runtime_model.Device();device.pending=0;c['runtime_seed']=device.snapshot()
  r,path,m,s,f=q.fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match']
  records.append(dict(frame_events_state=r['frame_events_state'],SDK_watchdog_calls=sum(b['target']=='0x8c650b60' and b['pr'] in ('0x8c651d92','0x8c651eae') for b in r['boundaries']),list_callback_calls=sum(b['target']=='0x8c651f1e' for b in r['boundaries']),label=label,entry=hex(entry),status=r['status'],stop_pc=r['stop_pc'],steps=len(path),boundaries=r['boundaries'],command_packets=r['command_packets'],runtime_state=r['runtime_state'],exception_state=r['exception_state'],graphics_input=r['graphics_input_summary'],graphics_output=r['graphics_output_summary'],IRQ_vector_executed=VBR+0x600 in path,graphics_handler_executed=0x8c652210 in path,VBlank_handler_executed=0x8c651bc0 in path,busy_flag=q.get(m,0x8c8a30c0),full_state_match=True,ordered_boundaries_match=True))
  print(label,r['status'],r['stop_pc'],len(path),'runtime',r['runtime_state'][4:] if r['runtime_state'] else None,flush=True)
  if r['status']:
   result=dict(status='original_native_frame_IRQ_typed_frontier_matched',comparisons=len(records),frontier_status=r['status'],frontier_pc=r['stop_pc'],phases=records,partial_fault_state_discarded=True,physical_GPU_pixels_qualified=False,new_original_routine_credit=0,overall_completion_percent=10)
   frontier.write_text(json.dumps(result,indent=2)+'\n');raise SystemExit(2)
  assert s.r[15]==sp and s.pc==s.pr==0x8cfed000
  carry(c,r)
 files=[Path(__file__),P/'renderer_list_completion_reference.py',P/'frame_irq_reference_cpu.py',P/'write_watch_renderer_list_completion.py',P/'analysis/renderer_list_completion_connection.json',R/'work/sonic-adventure/build/renderer_list_completion.dll']
 result=dict(status='original_empty_frame_returns_through_original_IRQ_and_RTE',comparisons=len(records),phases=records,physical_GPU_pixels_qualified=False,automatic_event_production=False,new_original_routine_credit=0,overall_completion_percent=10,scope='Raw game RAM plus original vector/display/context producers. A declared rawVBlank observation stream isconfigured only asan external device INPUT before original6050b0 begins. Actual6416ec RTS/delay boundary delivers320/priority6 throughoriginal copiedvector/context/registered652210/651bc0/RTE. No flags,helper results,private frames orstopped-state continuation supplied. Physical timing,TA list events,reset boot andpixels remain unqualified.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':main()
