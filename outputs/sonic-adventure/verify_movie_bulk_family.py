"""Full original transfer size, control/callback coverage and actual failure state."""
import collections,csv,gc,hashlib,itertools,json,sys,time
from pathlib import Path
import write_watch_bulk_family as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def cases(smoke=False):
 base=dict(mode=0,fifo_ready_count=0x40000,instruction_budget=0x4000000)
 if smoke:
  for entry in (v.RELEASE,v.CALLBACK,v.INFO,v.REQUEST):yield v.Input(base|dict(entry=entry))
  return
 for mode,mask,bank in itertools.product((0,1,4),range(16),(0,1)):
  yield v.Input(base|dict(entry=v.RELEASE,mode=mode,initial_sr=0x40008302|(bank<<29)|(mask<<4),control_word=(0x9e3779b9*(mask+1)+bank)&0xffffffff,mask_interrupts=mask&1))
 for cb,arg,record in ((0,0,v.RECORD),(0xffffffff,0,v.RECORD),(0x8c5ff2cc,0,v.RECORD),(0x8c5ff2cc,0x2468ace0,v.RECORD),(0x8cfef000,0x12345678,v.RECORD),(0,0,0)):
  for mode in (0,4):yield v.Input(base|dict(entry=v.CALLBACK,callback=cb,callback_argument=arg,record=record,mode=mode))
 for mode in (0,1):
  for options in (dict(root_enabled=False),dict(record=0),dict(state=2),dict(state=0)):
   yield v.Input(base|dict(entry=v.REQUEST,mode=mode)|options)
 for enabled in (False,True):yield v.Input(base|dict(entry=v.INFO,root_enabled=enabled))
 for entry,width,count,stride in itertools.product((0x8c10d460,0x8c10d770),(1,2,4),(0,1,3,17),(0,1,2)):
  options=dict(src=0xa0800100,dst=v.hardware.DST) if entry==0x8c10d460 else dict(src=v.hardware.SRC,dst=0xa0800100)
  yield v.Input(base|dict(entry=entry,width=width,count=count,source_stride=stride,dest_stride=stride,sound_pattern='ramp')|options)
 for entry in (v.RESET,v.RELEASE):
  for options in (dict(fifo_samples=[]),dict(fifo_samples=[0]*64),dict(stack=0x8c000000),dict(initial_sr=0),dict(sound_unconfigured=True)):
   yield v.Input(base|dict(entry=entry,fill_word=0x12345678)|options)
 if '--quick' not in sys.argv:
  # The natural cold caller separately covers the full original SEGA fill.
  # Retain a different full nonzero fill here to detect hardcoded fill stubs.
  for mode,word in ((4,0x89abcdef),):
   yield v.Input(base|dict(entry=v.RESET,mode=mode,fill_word=word,progress=True))

def initial_sound(c):return bytearray(bytes(range(256))*(0x200000//256) if c.get('sound_pattern')=='ramp' else bytes([c.get('sound_seed',0xa5)])*0x200000)

def semantics(m,a,before,s,r,path,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match'];entry=c['entry'];wanted_sound=initial_sound(c)
 actual_sound=fast.namespace['journal'].regions['sound_ram']['native'].read()
 if entry==v.RESET:
  words=r['sound_state'][1];word=c.get('fill_word',v.load(m,0x8c67c1c4));wanted_sound[:words*4]=word.to_bytes(4,'little')*words
  assert actual_sound==wanted_sound and r['sound_state'][3]==words*4 and r['sound_trace_dropped']==0;checks+=3
  if not r['status']:
   assert words==0x80000 and r['sound_trace_events']==0x80000 and s.r[0]==0;checks+=3
   assert r['control_word']==(c.get('control_word',0)|1) and v.load(a,0x8c8a3408)==r['control_word'];checks+=2
   copies=[b for b in r['boundaries'] if b['target']=='0x8c10d770' and b['arguments'][3]==0x80000]
   assert len(copies)==1 and copies[0]['arguments']==[0x8c67c1c4,0xa0800000,4,0x80000];checks+=2
 if entry==v.RELEASE and not r['status']:
  value=c.get('control_word',0)&0xfffffffe
  assert r['control_word']==value and v.load(a,v.load(m,0x8c65a2ec))==value and s.r[0]==0 and s.r[4]==0;checks+=4
  assert s.sr==(before.sr|1) and path.count(0x8c65a2d8)==256;checks+=2
 if entry in (v.RESET,v.RELEASE) and not r['status']:
  assert s.r[15]==before.r[15] and s.pr==before.pr and s.r[14]==before.r[14] and s.sr&0xf0==before.sr&0xf0;checks+=4
 if entry==v.CALLBACK and not r['status']:
  expected=bytearray(m);sp=before.r[15]
  for offset,value in ((4,before.r[14]),(8,before.r[13]),(12,before.pr)):v.store(expected,sp-offset,value)
  status=0
  if not before.r[4]:
   status=v.load(m,0x8c65a55c);v.store(expected,v.load(m,0x8c65acb8),v.load(m,0x8c65a560));v.store(expected,v.load(m,0x8c65acbc),status)
  elif c.get('callback',0xffffffff)==0xffffffff:v.store(expected,before.r[4]+24,1)
  elif c.get('callback',0xffffffff)==0x8c5ff2cc and c.get('callback_argument',0x2468ace0):v.store(expected,0x8c67340c,c.get('callback_argument',0x2468ace0))
  assert a==expected and s.r[0]==status and s.r[15]==before.r[15] and s.pr==before.pr;checks+=4
 if entry in (0x8c10d460,0x8c10d770) and not r['status']:
  width=c.get('width',4);count=c.get('count',1);step={0:0,1:width,2:-width};source=c['src'];dest=c['dst'];wanted_ram=bytearray(m)
  for i in range(max(0,count)):
   if entry==0x8c10d460:
    start=(source&0x1fffffff)-0x800000;value=wanted_sound[start:start+width];target=dest&0xffffff;wanted_ram[target:target+width]=value
   else:
    start=source&0xffffff;value=wanted_ram[start:start+width];target=(dest&0x1fffffff)-0x800000;wanted_sound[target:target+width]=value
   source+=step[c['source_stride']];dest+=step[c['dest_stride']]
  assert actual_sound==wanted_sound;checks+=1
  if entry==0x8c10d460:
   lo=(v.hardware.DST&0xffffff)-128;hi=(v.hardware.DST&0xffffff)+256;assert a[lo:hi]==wanted_ram[lo:hi];checks+=1
 if entry!=v.RESET and entry not in (0x8c10d460,0x8c10d770):assert actual_sound==wanted_sound;checks+=1
 return checks

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0;started=time.perf_counter()
 for c in cases('--smoke' in sys.argv):
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,s,r,path,c);checks+=count;coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','sound_ram_sha256','sound_trace_sha256','sound_trace_events','sound_trace_dropped','write_watch')
  records.append({k:r[k] for k in keys}|dict(independent_checks=count,guest_r0=hex(s.r[0]),guest_sr=hex(s.sr)));print(hex(c['entry']),r['status'],r['stop_pc'],r['sound_state'],flush=True)
  del path,r,a;gc.collect()
 if '--smoke' in sys.argv:print('Bulk-family smoke passed',len(records));return
 if '--quick' in sys.argv:print('Bulk-family small-input preflight passed',len(records));return
 rows=[]
 for routine in v.manifest['routines']:
  body=set(v.PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'sound_ram_reference.py',v.MP,P/'analysis/movie_bulk_transfer_translation.json',P/'analysis/movie_bulk_family_connection.json',P/'analysis/sound_ram_bus_verification.json',P/'analysis/write_watch_qualification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,full_sized_transfer_cases=sum(r['input']['entry']==v.RESET and r['status']==0 for r in records),original_transfer_words=0x80000,original_transfer_bytes=0x200000,seconds=time.perf_counter()-started,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),cases=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original full-sized reset/clear wrapper and related control/callback/request/information proposals. Complete 2-MiB transfers retain all 524288 original writes, full exact memory, complete ordered sound-bus trace and original instruction execution; only the verification watchdog budget increases. Finite zero FIFO words are declared raw input via an explicit count recipe, not generated game/helper results. Repeated immutable graphics-register snapshots share storage only after exact tuple equality. Partial dependency stops remain real; routine qualification is separate.')
 (P/'analysis/movie_bulk_family_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Bulk-family comparisons',len(records),checks,report['status_counts']);print(rows)
if __name__=='__main__':main()
