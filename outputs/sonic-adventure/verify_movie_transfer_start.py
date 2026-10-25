"""Original transfer/cache/callback comparisons with independent state oracles."""
import collections,ctypes,gc,hashlib,itertools,json,sys
from pathlib import Path
import write_watch_transfer_start as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def cache_addresses(c):
 a=c.get('address',0x8cd70040);n=c.get('length',64)
 if not n:return []
 if n<65536:return [(a&0x9fffffe0)+i*32 for i in range(((a&31)+n+31)>>5)]
 start=a&0x1fffffe0;end=(a+n-1)&0x1fffffe0;sign=a&0x80000000;ccr=c.get('ccr',0)&~0x808;seed=c.get('tag_seed',0);out=[]
 for base in (0,0x2000):
  for index in range(base,base+(0x1000 if ccr&0x20 else 0x2000),32):
   line=((seed^(index//32))&0x1ffffc00)|index
   if base and ccr&0x80:line|=end
   if start<=line<=end:out.append(line|sign)
 return out

def cache_ram(m,before,c):
 expected=bytearray(m);sp=before.r[15]
 for offset,reg in ((4,14),(8,13),(12,12)):v.store(expected,sp-offset,before.r[reg])
 if c.get('length',64)>=65536:
  v.store(expected,sp-20,(before.sr>>4)&15,2)
  v.store(expected,sp-16,(c.get('ccr',0)>>7)&1)
  for offset,reg in ((24,11),(28,10),(32,9),(36,8)):v.store(expected,sp-offset,before.r[reg])
 return expected

def semantics(m,a,before,s,r,path,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match'];entry=c['entry']
 assert not any(r['sound_state']);checks+=1
 if r['status']:return checks
 if entry==v.CACHE:
  assert a==cache_ram(m,before,c);checks+=1
  addresses=r['cache_writeback_sequence'];assert addresses==cache_addresses(c);checks+=1
  assert s.r[15]==before.r[15] and s.pr==before.pr and [s.r[i] for i in (8,9,10,11,12,13,14)]==[before.r[i] for i in (8,9,10,11,12,13,14)];checks+=3
  assert s.sr&0xf0==before.sr&0xf0;checks+=1
  if c.get('length',64)>=65536:
   reads=[e for e in r['startup_events'] if 0xf4000000<=e[1]<0xf4004000 and e[0]==27]
   count=256 if c.get('ccr',0)&0x20 else 512;assert len(reads)==count;checks+=1
   assert len(r['ccr_events'])==1 and r['ccr_events'][0][2]==c.get('ccr',0)&~0x808;checks+=1
 if entry==v.START:
  descriptor=before.r[4];channel=v.load(m,descriptor);root=v.load(m,v.load(m,0x8c10fb34));table=v.load(m,root);slot=(table+channel*32)&0xffffffff
  if table==v.TABLE:
   expected=bytearray(m);sp=before.r[15]
   for offset,value in ((4,before.r[14]),(8,before.r[13]),(12,before.r[12]),(16,before.pr),(20,before.r[5]),(24,c.get('busy',0)&1)):v.store(expected,sp-offset,value)
   if not c.get('busy',0)&1:
    size=v.load(m,descriptor+12)&0x7fffffff
    if size<65536:
     for offset,value in ((28,descriptor),(32,slot),(36,0)):v.store(expected,sp-offset,value)
     for offset,value in ((0,v.load(m,descriptor+4)),(4,v.load(m,descriptor+8)),(8,v.load(m,descriptor+12)),(12,v.load(m,descriptor+16)),(16,v.load(m,descriptor+20)|(2 if before.r[5] else 0)),(20,1),(24,1)):v.store(expected,slot+offset,value)
     v.store(expected,descriptor+44,1);counter=root+v.half(m,0x8c10fb32)+channel*4;v.store(expected,counter,(v.load(m,counter)+1)&0xffffffff)
     assert a==expected;checks+=1
   else:assert a==expected;checks+=1
  assert s.r[0]==(v.load(m,0x8c10fb3c) if c.get('busy',0)&1 else 0);checks+=1
  assert s.r[15]==before.r[15] and s.pr==before.pr;checks+=2
 if entry==v.PENDING:
  expected=bytearray(m);sp=before.r[15];v.store(expected,sp-4,before.r[14]);v.store(expected,sp-8,before.pr);root=v.load(m,0x8c67c1d0)
  if root:v.store(expected,before.r[4],v.load(m,root+v.half(m,0x8c66ac2a)));error=0
  else:
   error=v.load(m,0x8c66ac30);v.store(expected,v.load(m,0x8c65acb8),v.load(m,0x8c66ac34));v.store(expected,v.load(m,0x8c65acbc),error)
  assert a==expected and s.r[0]==error and s.r[15]==sp and s.pr==before.pr;checks+=4
 if entry==v.CALLBACK:
  root=v.load(m,0x8c67c1d0);record=before.r[4]
  assert v.load(a,record+4)==1;checks+=1
  completed=v.half(m,0x8c668fdc);current=v.half(m,0x8c668fde)
  assert completed==current-4;checks+=1
  assert v.load(a,root+completed)==(v.load(m,root+completed)+2)&0xffffffff and v.load(a,root+current)==0;checks+=2
  pending=v.load(m,root+0x1a4);positive=ctypes.c_int32(pending).value>0
  assert v.load(a,root+0x1a4)==((pending-1)&0xffffffff if positive else pending);checks+=1
  if positive:assert v.load(a,v.old.QUEUE)==v.load(a,v.old.QUEUE+4)==0;checks+=1
  if c.get('callback',0xffffffff)==0xffffffff:assert v.load(a,record+24)==1;checks+=1
  assert s.r[15]==before.r[15] and s.pr==before.pr;checks+=2
 return checks

def cases(smoke=False):
 base=dict(mode=0)
 if smoke:
  for entry in (v.CACHE,v.START,v.PENDING,v.CALLBACK,v.CLIENT_START):yield v.Input(base|dict(entry=entry))
  yield v.Input(base|dict(entry=v.CACHE,length=65536,ccr=0xa0,tag_seed=0x0cd70000));return
 for length,alignment,mode in itertools.product((0,1,31,32,33,64,65535),(0,1,31),(0,1,4)):
  yield v.Input(base|dict(entry=v.CACHE,length=length,address=0x8cd70000+alignment,mode=mode))
 for ccr,seed,address,length in itertools.product((0,0x20,0x80,0xa0),(0,0x0cd70000,0x0cd70004,0x0cd80000),(0x8cd70000,0x8cd7001f),(65536,131072)):
  yield v.Input(base|dict(entry=v.CACHE,length=length,address=address,ccr=ccr,tag_seed=seed))
 for mask,bank in itertools.product(range(16),(0,1)):
  yield v.Input(base|dict(entry=v.CACHE,length=65536,address=0x8cd70000,ccr=0xa0,tag_seed=0x0cd70000,initial_sr=0x40008302|(mask<<4)|(bank<<29)))
 for channel,busy,argument,mode in itertools.product((0,1,3,5),(0,1,2,0xffffffff),(0,1),(0,1,4)):
  yield v.Input(base|dict(entry=v.START,channel=channel,busy=busy,argument=argument,mode=mode,counter=0xffffffff))
 for table,channel,argument in itertools.product((v.TABLE,0xa05f7800),(0,1,5),(0,1)):
  yield v.Input(base|dict(entry=v.START,table=table,channel=channel,argument=argument))
 for value in (0,1,0xffffffff,0x80000000):
  for enabled in (False,True):yield v.Input(base|dict(entry=v.PENDING,pending=value,enabled=enabled))
 for pending,sdk_mode,callback,mode in itertools.product((0,1,0xffffffff),(0,1,2),(0,0xffffffff,0x8c5ff2cc),(0,1,4)):
  yield v.Input(base|dict(entry=v.CALLBACK,pending=pending,sdk_mode=sdk_mode,callback=callback,mode=mode))
 for entry in (v.CACHE,v.START,v.PENDING,v.CALLBACK,v.CLIENT_START):
  for extra in (dict(stack=0x8c000000),dict(initial_sr=0),dict(null_bus=True)):
   yield v.Input(base|dict(entry=entry)|extra)
 for extra in (dict(address=0, length=32),dict(address=0x8cffffe0,length=32),dict(address=0x0cd70001,length=31),dict(address=0xacd7001f,length=64),dict(length=65536,initial_sr=0)):
  yield v.Input(base|dict(entry=v.CACHE)|extra)
 for extra in (dict(descriptor=0),dict(descriptor=0x8cfffffc),dict(channel=0xffffffff),dict(table=0x8d000000)):
  yield v.Input(base|dict(entry=v.START)|extra)
 for extra in (dict(enabled=False),dict(output=0),dict(output=0x8cfffffc),dict(output=0x8d000000)):
  yield v.Input(base|dict(entry=v.PENDING)|extra)
 for extra in (dict(record=0),dict(callback=0x8cfef000),dict(pending=1,sdk_mode=1,child_state=0,busy=1)):
  yield v.Input(base|dict(entry=v.CALLBACK)|extra)
 for mode,busy,state in itertools.product((0,1,4),(0,1),(0,2)):
  yield v.Input(base|dict(entry=v.CLIENT_START,mode=mode,busy=busy,state=state))

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,s,r,path,c);checks+=count;coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','cache_writebacks','cache_writeback_sequence','ccr_state','ccr_events','startup_events','sound_state','sound_ram_sha256','sound_trace_dropped','write_watch')
  records.append({k:r[k] for k in keys}|dict(independent_checks=count,guest_r0=hex(s.r[0]),guest_sr=hex(s.sr)));print(hex(c['entry']),r['status'],r['stop_pc'],len(r['cache_writebacks']),flush=True);del r,path,a;gc.collect()
 if '--smoke' in sys.argv:print('Transfer-start smoke passed',len(records));return
 rows=[]
 for routine in v.manifest['routines']:
  body=set(v.PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_transfer_start_connection.json',P/'analysis/movie_transfer_wait_translation.json',P/'analysis/movie_queue_family_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original transfer-start/cache/pending-count/callback paths with existing status getter retained. Full machine/RAM/device/ordered boundaries match the independent original executor. Independent cache address lists and stack bytes, start descriptor/table/counter/stack effects, pending query/error entire RAM and callback counters/queue behavior are checked. Cache tags and coherent writeback are explicit software policies; physical cache/DMA/IRQ acceptance and complete startup remain unproved.')
 (P/'analysis/movie_transfer_start_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Transfer-start comparisons',len(records),checks,report['status_counts']);print(rows)
if __name__=='__main__':main()
