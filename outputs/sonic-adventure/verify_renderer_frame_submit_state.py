"""Probe actual original empty-frame submission before repeating asset loading."""
import hashlib,json
from pathlib import Path
import renderer_frame_submit_clock_fixtures as q
P=q.P;R=P.parents[1]


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_frame_submit_state_verification.json';frontier=P/'analysis/renderer_frame_submit_state_frontier.json';assert not target.exists() and not frontier.exists()
 base=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0));base[1].r[15]=0x8cfff000;records=[]
 for entry,args,label,stack in [(0x8c602128,[0x8cb00000,0x100000],'original system heap',None),(0x8c604702,[1,0,1,15],'original display/config record',None),(0x8c604dce,[-1,0,0x20000,0],'original allocated renderer context',0),(0x8c6050b0,[],'original whole frame wrapper onactual empty context',None)]:
  m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,x in enumerate(args):s.r[4+i]=x&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  if entry==0x8c6050b0:c['ticks']=list(q.FRAME_TICKS)
  r,path,m,s,f=q.fast.compare(m,s,f,c);assert r['full_state_match'] and r['ordered_boundaries_match']
  records.append(dict(label=label,entry=hex(entry),status=r['status'],stop_pc=r['stop_pc'],return_value=hex(s.r[0]),steps=len(path),boundaries=r['boundaries'],command_packets=r['command_packets'],graphics_input=r['graphics_input_summary'],graphics_output=r['graphics_output_summary'],full_state_match=True,ordered_boundaries_match=True))
  print(label,r['status'],r['stop_pc'],len(path),len(r['command_packets']),flush=True)
  if r['status']:
   report=dict(status='actual_empty_frame_wrapper_original_native_matched_typed_frontier',comparisons=len(records),frontier_status=r['status'],frontier_pc=r['stop_pc'],phases=records,partial_fault_state_discarded=True,physical_GPU_pixels_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):sha(p) for p in [Path(__file__),P/'renderer_frame_submit_clock_fixtures.py',P/'write_watch_renderer_frame_submit.py',R/'work/sonic-adventure/build/renderer_context_owner.dll']});frontier.write_text(json.dumps(report,indent=2)+'\n');raise SystemExit(2)
  assert s.r[15]==sp and s.pc==s.pr==0x8cfed000
  c.update(graphics_in=r['graphics_out'],other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr']);base=m,s,f,c
 report=dict(status='actual_empty_frame_wrapper_returns_with_complete_original_native_state_match',comparisons=len(records),phases=records,physical_GPU_pixels_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):sha(p) for p in [Path(__file__),P/'renderer_frame_submit_clock_fixtures.py',P/'write_watch_renderer_frame_submit.py',R/'work/sonic-adventure/build/renderer_context_owner.dll']});target.write_text(json.dumps(report,indent=2)+'\n')


if __name__=='__main__':main()
