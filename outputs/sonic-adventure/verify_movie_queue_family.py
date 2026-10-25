"""Original circular queue primitives with independent exact RAM effects."""
import collections,ctypes,gc,hashlib,itertools,json,sys,time
from pathlib import Path
import write_watch_queue_family as fast
v=fast.old;P=v.P;ROOT=v.ROOT
def signed(x):return ctypes.c_int32(x).value
def stack_image(m,before,entry):
 expected=bytearray(m);sp=before.r[15]
 values=(before.r[14],before.pr) if entry==v.ENQUEUE else (before.r[14],before.r[13],before.pr)
 for i,value in enumerate(values,1):v.store(expected,sp-i*4,value)
 v.store(expected,sp-(len(values)+1)*4,(before.sr>>4)&15)
 return expected

def semantics(m,a,before,s,r,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match'];entry=c['entry']
 if entry==v.EXECUTE:
  assert a==m and list(s.r)==list(before.r) and s.sr==before.sr and s.pr==before.pr and s.pc==before.pr;return checks+5
 if entry==v.CONFIG:
  if r['status']:return checks
  expected=bytearray(m);record=before.r[4];sp=before.r[15]
  for offset,value in ((4,before.r[5]&v.load(m,0x8c10fa54)),(8,before.r[6]&v.load(m,0x8c10fa54)),(12,(before.r[7]&v.load(m,0x8c10fa58))|v.load(m,0x8c10fa5c)),(16,v.load(m,sp)&1),(28,v.load(m,sp+12)),(32,v.load(m,sp+16)),(48,0)):v.store(expected,record+offset,value)
  flags=v.load(m,record+20);flags=(flags|1) if v.load(m,sp+4)==1 else (flags&0xfffffffd);flags=(flags|4) if v.load(m,sp+8)==1 else (flags&0xfffffffe);v.store(expected,record+20,flags&7)
  assert a==expected and s.r[0]==0 and s.sr==((before.sr&~1)|(v.load(m,sp+8)==1)) and s.r[15]==sp and s.pr==before.pr;return checks+5
 if entry not in (v.ENQUEUE,v.DEQUEUE):return checks
 if r['status']:
  if c.get('partial_pair_fault'):
   expected=stack_image(m,before,entry)
   if entry==v.ENQUEUE:v.store(expected,0x8cfffffc,before.r[4])
   else:v.store(expected,before.r[4],v.load(m,0x8cfffffc))
   assert a==expected and r['status']==-3;checks+=2
  if entry==v.DEQUEUE and c.get('output2')==0x8d000000:
   expected=stack_image(m,before,entry);root=v.load(m,0x8c67c1d0);slot=(v.load(m,root+0x194)+v.load(m,root+0x1a0)*8)&0xffffffff
   v.store(expected,before.r[4],v.load(m,slot));assert a==expected and r['status']==-3;checks+=2
  return checks
 expected=stack_image(m,before,entry);root=v.load(m,0x8c67c1d0);error=0
 if not root:error=v.load(m,0x8c667140 if entry==v.ENQUEUE else 0x8c667074)
 elif entry==v.ENQUEUE:
  count=v.load(expected,root+0x1a4);capacity=v.load(expected,root+0x198)
  if signed(capacity)<=signed(count):error=v.load(m,0x8c667144)
  else:
   tail=v.load(expected,root+0x19c);slot=(v.load(expected,root+0x194)+(tail*8&0xffffffff))&0xffffffff
   v.store(expected,slot,before.r[4]);v.store(expected,(slot+4)&0xffffffff,before.r[5]);tail=(tail+1)&0xffffffff
   v.store(expected,root+0x19c,0 if tail==capacity else tail);v.store(expected,root+0x1a4,(count+1)&0xffffffff)
 else:
  head=v.load(expected,root+0x1a0);slot=(v.load(expected,root+0x194)+(head*8&0xffffffff))&0xffffffff
  v.store(expected,before.r[4],v.load(expected,slot));v.store(expected,before.r[5],v.load(expected,(slot+4)&0xffffffff))
  v.store(expected,slot,0);v.store(expected,(slot+4)&0xffffffff,0);head=(head+1)&0xffffffff
  v.store(expected,root+0x1a0,0 if head==v.load(expected,root+0x198) else head);v.store(expected,root+0x1a4,(v.load(expected,root+0x1a4)-1)&0xffffffff)
 if error:
  v.store(expected,v.load(m,0x8c65acb8),v.load(m,0x8c667148 if entry==v.ENQUEUE else 0x8c667078));v.store(expected,v.load(m,0x8c65acbc),error)
 assert a==expected,('exact queue/stack/error RAM',hex(entry),c);checks+=1
 assert s.r[0]==error and s.r[15]==before.r[15] and s.pr==before.pr and s.r[14]==before.r[14];checks+=4
 assert s.sr==((before.sr&~1)|(0 if error else 1));checks+=1
 return checks

def cases(smoke=False):
 base=dict(mode=0)
 if smoke:
  for entry in (v.ENQUEUE,v.DEQUEUE,v.EXECUTE,v.START,v.SUBMIT):yield v.Input(base|dict(entry=entry))
  return
 for entry,mode,mask,bank in itertools.product((v.ENQUEUE,v.DEQUEUE),(0,1,4),range(16),(0,1)):
  yield v.Input(base|dict(entry=entry,mode=mode,initial_sr=0x40008302|(mask<<4)|(bank<<29)))
 for capacity in (1,2,4,8):
  for count in (0,capacity-1,capacity,capacity+1,0xffffffff,0x80000000,0x7fffffff):
   yield v.Input(base|dict(entry=v.ENQUEUE,capacity=capacity,count=count,tail=capacity-1))
  for head in (0,capacity-1):
   for count in (0,1,0xffffffff):yield v.Input(base|dict(entry=v.DEQUEUE,capacity=capacity,head=head,count=count))
 for entry in (v.ENQUEUE,v.DEQUEUE):
  for alias in (0x0c000000,0xac000000):yield v.Input(base|dict(entry=entry,alias=alias))
  for index in (0xffffffff,0xfffffffe,0x7fffffff,0x80000000):yield v.Input(base|dict(entry=entry,tail=index,head=index,count=1))
  for extra in (dict(enabled=False),dict(queue_pointer=0x8d000000),dict(stack=0x8c000000),dict(initial_sr=0),dict(queue_pointer=0x8cfffffc,partial_pair_fault=True)):
   yield v.Input(base|dict(entry=entry)|extra)
 for first,second in ((0,0),(0xffffffff,0x80000000),(1,0xffffffff)):
  yield v.Input(base|dict(entry=v.ENQUEUE,first=first,second=second,count=0))
 for extra in (dict(output1=0),dict(output2=0),dict(output2=0x8d000000),dict(output1=v.OUT1,output2=v.OUT1),dict(output1=v.QUEUE+4),dict(output2=v.QUEUE)):
  yield v.Input(base|dict(entry=v.DEQUEUE)|extra)
 for entry,mode in itertools.product((v.START,v.SUBMIT),(0,1,4)):
  for extra in (dict(enabled=False),dict(record=0),dict(state=2),dict(state=0)):yield v.Input(base|dict(entry=entry,mode=mode)|extra)
 for mode in (0,1,4):yield v.Input(base|dict(entry=v.EXECUTE,mode=mode))
 for mode,flags,flag_a,flag_b in itertools.product((0,1,4),(0,1,2,4,7,0xffffffff),(0,1,2),(0,1,0xffffffff)):
  yield v.Input(base|dict(entry=v.CONFIG,mode=mode,flags=flags,flag_a=flag_a,flag_b=flag_b))
 for value in (0,1,3,4,31,32,63,64,0x7fffffff,0x80000000,0xffffffff):
  yield v.Input(base|dict(entry=v.CONFIG,source=value,destination=value,transfer_size=value,direction=value,callback=value,callback_argument=value))
 for extra in (dict(record=0),dict(record=0x8d000000),dict(record=0x8cfffff8),dict(stack=0x8cffffec)):
  yield v.Input(base|dict(entry=v.CONFIG)|extra)

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,s,r,c);checks+=count;coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','sound_ram_sha256','sound_trace_dropped','write_watch')
  records.append({k:r[k] for k in keys}|dict(independent_checks=count,guest_r0=hex(s.r[0]),guest_sr=hex(s.sr)));print(hex(c['entry']),r['status'],r['stop_pc'],flush=True);gc.collect()
 if '--smoke' in sys.argv:print('Queue smoke passed',len(records));return
 rows=[]
 for routine in v.manifest['routines']:
  body=set(v.PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_queue_family_connection.json',P/'analysis/movie_bulk_family_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original queue/dequeue and actual retail empty-server adapter, with request-start/synchronous submission proposals left partial. Independent entire RAM oracle covers queue fields, data, output aliasing, every stack/error write, original signed comparison and counter wrap; dequeue has no empty guard, so zero-count underflow is preserved. Invalid pointers/privilege and partial pair-store faults retain actual state. No helper result or completion is supplied.')
 (P/'analysis/movie_queue_family_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Queue comparisons',len(records),checks,report['status_counts']);print([(r['address'],r['covered_rows'],r['static_rows'],r['conditional_edges_covered']) for r in rows])
if __name__=='__main__':main()
