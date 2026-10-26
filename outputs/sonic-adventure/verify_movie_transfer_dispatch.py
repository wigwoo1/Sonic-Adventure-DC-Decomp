"""Original event traversal and ASIC snapshot dispatch, without supplied callbacks."""
import collections,gc,hashlib,itertools,json,sys
from pathlib import Path
import write_watch_transfer_dispatch as fast
v=fast.old;P=v.P;ROOT=v.ROOT
EVENTS=[0xa00+32*i for i in range(7)]+[0xb00+32*i for i in range(4)]

def selected_events(c):
 if c['entry']!=v.ASIC:return [c.get('event',0xa60)]
 event=c.get('event',0x360)&0xffff
 if event&0x8000:event-=0x10000
 if event>>4!=0x36:return []
 mask=c.get('masks',[0,0]);normal=(c.get('irq_status',0)&mask[0])>>12
 if normal:return [0xa00+32*i for i in range(7) if normal&(1<<i)]
 error=c.get('startup_overrides',{}).get(0x5f6904,0)&mask[1]
 return [0xb00+32*i for i in range(4) if error&(1<<i)]

def semantics(m,a,before,s,r,path,c):
 checks=3;assert r['full_state_match'] and r['ordered_boundaries_match'];assert not any(r['sound_state']);assert not r['cache_writeback_sequence']
 if r['status']:return checks
 chosen=selected_events(c);depth=c.get('depth',1);callback=c.get('dispatch_callback',v.COUNTER)
 if c['entry']==v.ASIC:
  calls=[b['arguments'][0] for b in r['boundaries'] if int(b['target'],16)==v.GENERIC]
  assert calls==chosen;checks+=1
 else:chosen=[c.get('event',0xa60)]
 if callback==v.COUNTER:
  address=v.load(m,0x8c666e4c);assert v.load(a,address)==(v.load(m,address)+len(chosen)*depth)&0xffffffff;checks+=1
 assert s.r[15]==before.r[15] and s.pr==before.pr and s.r[14]==before.r[14];checks+=3
 if c['entry'] in (v.PRIMARY,v.SECONDARY):assert s.sr&0xf0==0x60;checks+=1
 if c['entry']==v.ASIC:
  assert s.sr&0xf0==(0x40 if (c.get('event',0x360)&0xffff)>>4==0x36 else before.sr&0xf0);checks+=1
  assert [s.r[i] for i in (10,11,12,13)]==[before.r[i] for i in (10,11,12,13)];checks+=1
 if c['entry']==v.GENERIC and not depth:
  expected=bytearray(m);v.store(expected,before.r[15]-4,before.r[14]);v.store(expected,before.r[15]-8,before.pr)
  assert a==expected;checks+=1
 return checks

def cases(smoke=False):
 base=dict(mode=0)
 if smoke:
  for entry in (v.GENERIC,v.PRIMARY,v.SECONDARY,v.ASIC):yield v.Input(base|dict(entry=entry,events=EVENTS,irq_status=1<<15,masks=[0xffffffff,0]))
  return
 for entry,depth,mode in itertools.product((v.GENERIC,v.PRIMARY,v.SECONDARY),range(4),(0,1,4)):
  yield v.Input(base|dict(entry=entry,depth=depth,mode=mode,error_counter=0xffffffff))
 for event in (0xa60,0xa6f,0x1a60,0xffff,0x8000,0,0x360,0x320,0x3a0,0x370):
  yield v.Input(base|dict(entry=v.GENERIC,event=event))
 for entry,mask,bank in itertools.product((v.PRIMARY,v.SECONDARY,v.ASIC),(0,6,15),(0,1)):
  yield v.Input(base|dict(entry=entry,initial_sr=0x40008303|(mask<<4)|(bank<<29),events=EVENTS,irq_status=(1<<15)|(1<<18),masks=[0xffffffff,0]))
 for status in (0,1<<11,1<<12,1<<13,1<<14,1<<15,1<<16,1<<17,1<<18,1<<19,0x7f000,0xffffffff):
  for mask in (0,0xffffffff):
   yield v.Input(base|dict(entry=v.ASIC,irq_status=status,masks=[mask,15],startup_overrides={0x5f6904:15},events=EVENTS))
 for error,mask in itertools.product((0,1,2,4,8,15,16,0xffffffff),(0,15)):
  yield v.Input(base|dict(entry=v.ASIC,irq_status=0,masks=[0,mask],startup_overrides={0x5f6904:error},events=EVENTS))
 for event in (0x320,0x321,0x3a0,0x370,0xffff):yield v.Input(base|dict(entry=v.ASIC,event=event,events=EVENTS,irq_status=0xffffffff,masks=[0xffffffff,15]))
 for entry,extra in itertools.product((v.GENERIC,v.PRIMARY,v.SECONDARY,v.ASIC),(dict(depth=0),dict(bad_next=True),dict(dispatch_callback=0x8cfef000),dict(dispatch_callback=0xffffffff),dict(initial_sr=0),dict(stack=0x8c000000),dict(null_bus=True))):
  yield v.Input(base|dict(entry=entry,events=EVENTS,irq_status=1<<15,masks=[0xffffffff,0])|extra)

def main():
 records=[];coverage=set();edges=set();checks=0;ram=v.seed_ram()
 for c in cases('--smoke' in sys.argv):
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,s,r,path,c);checks+=count;coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','irq_status','write_watch')
  records.append({k:r[k] for k in keys}|dict(independent_checks=count,guest_sr=hex(s.sr)));print(len(records),hex(c['entry']),r['status'],flush=True);del r,path,a;gc.collect()
 if '--smoke' in sys.argv:print('Dispatch smoke passed',len(records));return
 rows=[]
 for definition in v.manifest['routines']:
  body=set(v.PER_BODY[int(definition['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in definition['conditional_edges']}
  rows.append(dict(address=definition['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(a) for a in sorted(body-coverage)],conditional_outcomes=len(wanted),covered_outcomes=len(wanted&edges),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_transfer_dispatch_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Full original/native state and ordered boundaries for generic/primary/secondary/ASIC dispatch with actual existing counter callbacks. Independent selected event order, normal-vs-error snapshot priority, ignored bits, signed event lookup, counter count, empty entire RAM and preserved register/stack/mask effects. Software register inputs explicit; physical exception entry and asynchronous delivery not claimed.')
 (P/'analysis/movie_transfer_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Dispatch comparisons',len(records),checks);print(rows)
if __name__=='__main__':main()
