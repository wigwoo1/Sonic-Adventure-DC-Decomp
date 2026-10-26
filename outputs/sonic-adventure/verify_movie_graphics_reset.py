"""Original graphics reset with real interrupt callback and timeout/count oracles."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_graphics_init as fast
v=fast.old;P=v.P;ROOT=v.ROOT;ENTRY=0x8c656860;IRQ=0x8c651bc0
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cases():
 for mode,renderer,fmt in itertools.product((0,1,4),(0,1,2,0xffffffff),(0,32,0xffffffff)):
  yield dict(entry=ENTRY,mode=mode,gfx_arguments=[renderer,fmt],transfer_busy=0,counter=0xffffffff)
 for mode,timeout,tag in itertools.product((0,1,4),(0,2,10,20,None),(0,0x12345678)):
  yield dict(entry=ENTRY,mode=mode,gfx_arguments=[1,33],transfer_busy=1,states=[2,-1,-1,-1] if timeout is not None else [-1]*4,timeout_b=timeout or 0,gfx_global_words=[(0x8c8a2fc0,tag)],expected_timeout=timeout,counter=0xffffffff)
 for mode in (0,4):yield dict(entry=ENTRY,mode=mode,gfx_arguments=[2,0xffffffff],transfer_busy=1,gfx_global_words=[(0x8c8a2fbc,0x8cfef000)],unknown_callback=True)
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'];n=2
  if c.get('unknown_callback'):
   assert r['status']==-2 and r['stop_pc']=='0x8cfef000' and v.load(a,0x8c8a30bc)==0;n+=3
   for address in (0x8c8a3090,0x8c8a316c,0x8c8a3170):assert v.load(a,address)==v.load(m,address);n+=1
  else:
   assert r['status']==0 and s.r[0]==0 and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;n+=5
   timeout=c.get('expected_timeout');count=0 if not c['transfer_busy'] else 11 if timeout is None else min(11,timeout+1)
   assert v.load(a,0x8c8a30bc)==count and sum(b['target']==hex(IRQ) for b in r['boundaries'])==count;n+=2
   assert all(b['arguments'][0]==v.load(m,0x8c8a2fc0) for b in r['boundaries'] if b['target']==hex(IRQ));n+=1
   expected_busy=int(bool(c['transfer_busy']) and (timeout is None or timeout>=11))
   assert v.load(a,0x8c8a2f60)==expected_busy;n+=1
   assert r['ack_writes']==[[12,0xa05f6900,8,4]]*count;n+=1
   assert r['irq_status']==(c.get('irq_status',8)&~8 if count else c.get('irq_status',8));n+=1
   renderer,fmt=c['gfx_arguments']
   for address,value in ((0x8c8a3090,renderer),(0x8c8a316c,fmt),(0x8c8a3170,fmt&15),(0x8c8a3098,0),(0x8c8a309c,0xffffffff),(0x8c8a30a0,0xffffffff),(0x8c8a30a4,0xffffffff),(0x8c8a30a8,0),(0x8c8a30ac,0),(0x8c8a30b0,0)):
    assert v.load(a,address)==value;n+=1
  assert not any(r['sound_state']) and r['runtime_state'] is None;n+=2
  checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],independent_checks=n,final_counter=v.load(a,0x8c8a30bc),final_busy=v.load(a,0x8c8a2f60),interrupt_callbacks=sum(b['target']==hex(IRQ) for b in r['boundaries'])));print(i,r['status'],records[-1]['final_counter'],records[-1]['final_busy'],flush=True)
 body=next(b for b in v.manifest['routines'] if b['address']==hex(ENTRY));want=set(body['reachable_addresses']);actual={hex(p) for p in seen};want_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(a),hex(b)) for a,b in edges}
 coverage=dict(address=hex(ENTRY),static_rows=len(want),covered_rows=len(want&actual),conditional_outcomes=len(want_edges),covered_outcomes=len(want_edges&actual_edges),missing_rows=sorted(want-actual),missing_outcomes=[list(e) for e in sorted(want_edges-actual_edges)])
 assert not coverage['missing_rows'] and not coverage['missing_outcomes']
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_graphics_init_connection.json',P/'verify_boot_interrupt.py',P/'analysis/boot-interrupt/8c651bc0_instructions.tsv']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=[coverage],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original reset57instructions6outcomes with actual existing651bc0interrupt callback. Initial busy/counter/queue/timeout metadata and IRQstatus observations are explicit inputs;real callback increments counter and clears pending state on its original timeout path. Independent count=min(11,timeout+1),counter reset,busy,ACK/callback argument andfinal reset-word oracles supplement fullmachine/memory/device/orderedboundary comparisons. Unknown callback preserves genuine reached-2 andprior writes. This is explicit softwarecallback execution,not physical IRQacceptance/timing or a fabricated completion flag;othercallbacks/concurrency/hardware/gameplay remain unqualified.')
 (P/'analysis/movie_graphics_reset_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Reset cases',len(records),'checks',checks,coverage)
if __name__=='__main__':main()
