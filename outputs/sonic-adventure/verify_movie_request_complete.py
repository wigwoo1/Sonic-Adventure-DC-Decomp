"""Close four original request outcomes with real queue and callback states."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_transfer_runtime as fast
v=fast.old;P=v.P;ROOT=v.ROOT;cpu=v.old.old.old.old
SCENARIOS=(
 ('queue-full',dict(pending=4,capacity=4),0x8c65a66e,0x05000101),
 ('current-request',dict(current_record=v.RECORD),0x8c65a678,0),
 ('queued-start-error',dict(pending=1,tail=1,head=0,child_state=2),0x8c65a690,0x06000001),
 ('async-callback',dict(callback=v.COUNTER),0x8c65a6a8,0),
)
def main():
 records=[];coverage=set();edges=set();checks=0;ram=v.seed_ram();body=set(v.PER_BODY[cpu.REQUEST]);queue=cpu.old.old.QUEUE
 for mode,(kind,extra,branch,result) in itertools.product((0,1,4),SCENARIOS):
  c=v.Input(dict(entry=cpu.REQUEST,mode=mode,sdk_mode=1,current_record=0,length=64)|extra)
  m,s,f=cpu.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==0 and s.pc==before.pr and s.pr==before.pr and s.r[0]==result;checks+=4
  assert list(s.r[8:])==list(before.r[8:]) and s.sr&0xf0==before.sr&0xf0 and bytes(f)==fp;checks+=3
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['runtime_state'] is None;checks+=3
  assert not any(r['sound_state']) and v.HANDLER not in path and v.COUNTER not in path and v.load(a,v.RECORD+24)==0;checks+=4
  assert a[cpu.SRC&0xffffff:(cpu.SRC&0xffffff)+64]==m[cpu.SRC&0xffffff:(cpu.SRC&0xffffff)+64];checks+=1
  local_edges=set()
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if pc in body and text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):local_edges.add((pc,path[i+distance]))
  assert (branch,0x8c65a6ee) in local_edges;checks+=1
  root=v.ROOT_CONTEXT
  if kind=='queue-full':
   assert v.load(a,root+0x1a4)==4 and v.load(a,root+0x19c)==0 and v.load(a,root+0x1a0)==0;checks+=3
   assert a[queue&0xffffff:(queue&0xffffff)+32]==m[queue&0xffffff:(queue&0xffffff)+32] and cpu.CLIENT_START not in path;checks+=2
  elif kind=='current-request':
   assert v.load(a,root+0x190)==v.RECORD and v.load(a,root+0x1a4)==1 and v.load(a,root+0x19c)==1 and v.load(a,root+0x1a0)==0;checks+=4
   assert v.load(a,queue)==v.RECORD and cpu.CLIENT_START not in path;checks+=2
  elif kind=='queued-start-error':
   assert v.load(a,root+0x1a4)==1 and v.load(a,root+0x19c)==2 and v.load(a,root+0x1a0)==1;checks+=3
   calls=[b for b in r['boundaries'] if int(b['target'],16)==cpu.CLIENT_START]
   assert len(calls)==1 and calls[0]['arguments'][0]==cpu.old.CHILD and v.load(a,cpu.old.CHILD+4)==2;checks+=3
  else:
   assert v.load(a,root+0x1a4)==0 and v.load(a,root+0x19c)==1 and v.load(a,root+0x1a0)==1;checks+=3
   assert v.load(a,root+0x190)==v.RECORD and v.load(a,v.RECORD+4)==2 and v.load(a,v.RECORD+16)==v.COUNTER;checks+=3
   assert v.load(a,v.DESCRIPTOR+44)==1 and v.load(a,v.DESCRIPTOR+48)==0 and cpu.CLIENT_START in path;checks+=3
  coverage.update(body&set(path));edges.update(local_edges)
  records.append(dict(kind=kind,input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=hex(s.r[0]),covered_outcome=[hex(branch),'0x8c65a6ee'],boundaries=r['boundary_comparisons'],full_state_match=r['full_state_match'],ordered_boundaries_match=r['ordered_boundaries_match']))
  print('Request',mode,kind,hex(s.r[0]),flush=True)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'source/sa_movie_transfer_wait.c',P/'analysis/movie_transfer_runtime_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,request_executed_rows=[hex(a) for a in sorted(coverage)],request_executed_branch_outcomes=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Twelve fresh original/native comparisons close four request outcomes across three FPU modes: real queue-full rejection, existing active request after enqueue, genuine dequeued child state2 causing native SDK start error, and actual non-FFFFFFFF asynchronous callback skipping wait. Full machine/RAM/device/banked/FPU/boundary state and independent queue counts/indices/records/error codes/callee-saved registers checked. No queue helper result, checkpoint, transfer or completion flag is supplied. Asynchronous return does not claim completed DMA; descriptor remains active, completion0 and SoundRAM untouched.')
 (P/'analysis/movie_request_complete_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('New request outcomes',len(records),'checks',checks)
if __name__=='__main__':main()
