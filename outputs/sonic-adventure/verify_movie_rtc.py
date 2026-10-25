"""Original two-word RTC reads with finite raw samples and exact state checks."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_maple as old
clear=old.clear;driver=old.driver;P=old.P;ROOT=old.ROOT
READ,LOG,TRANSFER,STABLE=0x8c10be78,0x8c10c018,0x8c10d460,0x8c10be0c
COHERENT,UPDATE=0x8c10bd96,0x8c10ba80
OUT,LOG_BUFFER,DEVICE,CHANNELS=0x8cdb0000,0x8cd40000,0x8cd60000,0x8cd64000
MP=P/'analysis/movie_rtc_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body

def rtc_address(a):return a&0x1fffffff in (0x710000,0x710004)

code=clear.code;marker='queue_reference.driver_snapshot()';assert code.count(marker)==1;code=code.replace(marker,marker+',queue_reference.rtc_snapshot()')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if rtc_address(a):\n    if n!=4:stop(pc,r,sr,-3)\n    if c.get('null_bus'):stop(pc,r,sr,-2)\n    if not text.split(' ',1)[1].split(',')[0].startswith('@'):stop(pc,r,sr,-3)\n    if queue_reference.rtc_samples is None:stop(pc,r,sr,-2)\n    if queue_reference.rtc_cursor>=len(queue_reference.rtc_samples):stop(pc,r,sr,-4)")
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW,rtc_address=rtc_address)
exec(compile(code,'rtc_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_rtc.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
observed=effects.LIB.sa_movie_rtc_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=clear.Stats()
def invoke(*args):return observed(*args,ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
class RtcMemory(driver.DriverMemory):_fields_=[('driver_bus',effects.Bus),('rtc_samples',ctypes.POINTER(ctypes.c_uint32)),('rtc_count',ctypes.c_uint32),('rtc_cursor',ctypes.c_uint32),('rtc_configured',ctypes.c_uint32)]
effects.LIB.sa_rtc_sample_bus_init.argtypes=[ctypes.POINTER(RtcMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus),ctypes.c_uint32,ctypes.c_uint32]
effects.LIB.sa_rtc_set_samples.argtypes=[ctypes.POINTER(RtcMemory),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];effects.LIB.sa_rtc_set_samples.restype=ctypes.c_int
class RtcReference(clear.Reference):
 def __init__(self,ticks,capacity,c):
  super().__init__(ticks,capacity,c);self.rtc_samples=None if c.get('unconfigured_rtc') else [x&0xffffffff for x in c.get('rtc_samples',[])];self.rtc_cursor=0
 def rtc_snapshot(self):return int(self.rtc_samples is not None),0 if self.rtc_samples is None else len(self.rtc_samples),self.rtc_cursor
 def __contains__(self,a):return rtc_address(a) or super().__contains__(a)
 def __getitem__(self,a):
  if rtc_address(a):
   value=self.rtc_samples[self.rtc_cursor];self.rtc_cursor+=1;self.events.append((31,a,value,4));return value
  return super().__getitem__(a)
 def __setitem__(self,a,value):
  assert not rtc_address(a),'RTC sample input is read-only'
  return super().__setitem__(a,value)
compare_code=clear.compare_code.replace('DriverMemory()','RtcMemory()').replace('ref=Reference(','ref=RtcReference(').replace('effects.LIB.sa_driver_signal_bus_init(','effects.LIB.sa_rtc_sample_bus_init(')
marker=" memory.transfer_registers[:]=";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," rtc_values=[x&0xffffffff for x in c.get('rtc_samples',[])];rtc_array=(ctypes.c_uint32*len(rtc_values))(*rtc_values)\n if not c.get('unconfigured_rtc'):assert effects.LIB.sa_rtc_set_samples(ctypes.byref(memory),rtc_array,len(rtc_values))==0\n"+marker)
marker='(memory.irq_configured,memory.irq_count,memory.irq_cursor,memory.dma_operation)))';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker[:-2]+',(memory.rtc_configured,memory.rtc_count,memory.rtc_cursor)))')
marker=' return dict(clear_native=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert (memory.rtc_configured,memory.rtc_count,memory.rtc_cursor)==ref.rtc_snapshot() and list(rtc_array)==rtc_values\n return dict(rtc_snapshot=ref.rtc_snapshot(),rtc_samples_unchanged=True,rtc_reads=[list(e) for e in ref.events.retained() if e[0]==31],clear_native=")
exec(compile(compare_code,'rtc_comparison','exec'),dict(old.old._compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats,RtcReference=RtcReference,RtcMemory=RtcMemory),local:={});compare=local['compare']
load=driver.load;store=driver.store

def prepare(ram,c):
 m,s,f=old.prepare(ram,c)
 if c['entry'] in (READ,STABLE,TRANSFER,COHERENT,UPDATE):
  store(m,0x8c5fbb14,DEVICE);m[DEVICE&0xffffff:(DEVICE&0xffffff)+0x180]=bytes(0x180);store(m,DEVICE,CHANNELS);store(m,DEVICE+0xd4,c.get('mask_interrupts',1));store(m,0x8c7ac8c0,0)
  for i in range(4):store(m,DEVICE+4+i*52,i);store(m,CHANNELS+i*32+0x18,0)
  if c['entry']==READ:s.r[4]=c.get('destination',OUT);store(m,OUT,0xa5a55a5a)
  if c['entry']==TRANSFER:
   s.r[4]=c.get('source',0xa0710000);s.r[5]=OUT;s.r[6]=c.get('width',4);s.r[7]=c.get('count',2)&0xffffffff;store(m,s.r[15],1);store(m,s.r[15]+4,1)
  if c['entry']==COHERENT:s.r[4]=c.get('previous_timestamp',0x89abcdef)
  if c['entry']==UPDATE:
   store(m,load(m,0x8c10baf8),c.get('previous_timestamp',0x89abcdef));store(m,load(m,0x8c10bafc),c.get('clock_flags',0))
 if c['entry'] in (READ,STABLE,LOG,TRANSFER,COHERENT,UPDATE):
  store(m,0x8c7a7ff4,c.get('log_flags',0)|(c.get('clock_flags',0) if c['entry']==UPDATE else 0));store(m,0x8c7a8000,c.get('log_cursor',LOG_BUFFER));store(m,0x8c7a8004,c.get('log_limit',LOG_BUFFER+c.get('log_words',2)*4));m[(LOG_BUFFER-16)&0xffffff:(LOG_BUFFER&0xffffff)+80]=bytes([0xa5])*96
 if c['entry']==LOG:s.r[4]=c.get('value',0x89abcdef)
 return m,s,f

def cases(smoke=False):
 base=dict(entry=READ,mode=0,rtc_samples=[0x1234,0xabcd],render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},startup_overrides={0x5f689c:0xb},irq_samples=[0,8],fifo_samples=[0]*128,scan_samples=[0,1]*32)
 if smoke:items=[{},dict(entry=LOG,log_flags=8),dict(entry=STABLE,rtc_samples=[0x1234,0xabcd]*4)]
 else:
  items=[dict(mode=mode,rtc_samples=words,log_flags=flags,log_words=slots) for mode,words,flags,slots in itertools.product((0,1,4),([0,0],[0xffff,0xffff],[0x12345678,0x9abcdef0]),(0,8),(0,1,2))]
  items += [dict(entry=LOG,log_flags=flags,log_cursor=LOG_BUFFER+offset,log_words=slots,value=value) for flags,offset,slots,value in ((0,0,2,0),(8,0,2,0xffffffff),(9,0,1,0x12345678),(8,4,1,0),(8,8,1,0),(8,0,0,0))]
  items += [dict(entry=LOG,log_flags=8,log_cursor=0x8d000000,log_limit=0xffffffff)]
  items += [dict(entry=TRANSFER,source=prefix|0x710000) for prefix in (0,0x80000000,0xa0000000)]
  items += [dict(rtc_samples=words) for words in ([],[0x1234])]
  items += [dict(unconfigured_rtc=True),dict(null_bus=True),dict(destination=0x8d000000),dict(bad_stack=True)]
  items += [dict(fifo_samples=[]),dict(mask_interrupts=0,log_flags=8)]
  items += [dict(entry=STABLE,rtc_samples=words,log_flags=flags,log_words=16) for words,flags in (([0x1234,0xabcd]*4,0),([0,0]*4,8),([1,2,1,3]+[1,2]*3,8),([1,2,1,3]*3,0),([],0))]
  items += [dict(entry=COHERENT,rtc_samples=words,log_flags=8,log_words=8) for words in ([0x1234,0xabcd]*2,[0xffffffff,0xffffffff]*2,[1,2,1,3],[0xffff,0xffff,0,0],[],[1,2,1])]
  items += [dict(entry=UPDATE,clock_flags=flags,rtc_samples=[1,2]*8,log_flags=8,log_words=20) for flags in (0,1,2,3)]
  items += [dict(entry=0x8c010180,cold_library=True,rtc_samples=[0x1234,0xabcd]*16)]
  items += [c|dict(rtc_samples=[]) for c in old.cases() if (c['entry']==old.CTOR and c.get('mode')==4) or (c['entry']==old.COMMAND and c.get('busy')==0x80) or (c['entry']==old.COMMAND and c.get('serial')==0x7fffffff)]
 for c in items:yield base|c

def semantic(m,a,before,s,r,c):
 checks=1;assert r['rtc_samples_unchanged']
 if r['status']:return checks
 assert s.r[15]==before.r[15] and s.pr==before.pr;checks+=2
 if c['entry']==READ:
  words=c['rtc_samples'];assert load(a,before.r[4])==((words[0]&65535)<<16)|(words[1]&65535) and s.r[0]==0;checks+=2
  assert r['rtc_reads']==[[31,0xa0710000,words[0],4],[31,0xa0710004,words[1],4]];checks+=1
 if c['entry']==STABLE:
  assert s.r[0]==((c['rtc_samples'][0]&65535)<<16)|(c['rtc_samples'][1]&65535);checks+=1
 if c['entry']==COHERENT:
  words=c['rtc_samples'];first=((words[0]&65535)<<16)|(words[1]&65535);second=((words[2]&65535)<<16)|(words[3]&65535)
  assert s.r[0]==(first if first==second else before.r[4]);checks+=1
 if c['entry']==UPDATE:
  expected=c.get('previous_timestamp',0x89abcdef) if c.get('clock_flags',0)&1 and not c.get('clock_flags',0)&2 else 0x10002
  assert load(a,load(m,0x8c10baf8))==expected and load(a,load(m,0x8c10bafc))==load(m,load(m,0x8c10bafc));checks+=2
 if c['entry'] in (READ,STABLE,LOG,COHERENT,UPDATE):
  expected=bytearray(m);cursor=load(m,0x8c7a8000);limit=load(m,0x8c7a8004)
  words=[before.r[4]] if c['entry']==LOG else [e[2] for e in r['rtc_reads']]
  if load(m,0x8c7a7ff4)&8:
   for value in words:
    if cursor<limit:store(expected,cursor,value);cursor=(cursor+4)&0xffffffff
  assert load(a,0x8c7a8000)==cursor;checks+=1
  assert a[(LOG_BUFFER-16)&0xffffff:(LOG_BUFFER&0xffffff)+80]==expected[(LOG_BUFFER-16)&0xffffff:(LOG_BUFFER&0xffffff)+80];checks+=1
 return checks

def bus_checks():
 ram=bytearray(16<<20);view=effects.RAM_TYPE.from_buffer(ram);memory=RtcMemory();bus=effects.Bus();effects.LIB.sa_rtc_sample_bus_init(ctypes.byref(memory),view,ctypes.byref(bus),0,0);out=ctypes.c_uint32(0xabcdef01);count=0
 assert bus.read(bus.context,0xa0710000,4,ctypes.byref(out))==-2 and out.value==0xabcdef01;count+=1
 assert effects.LIB.sa_rtc_set_samples(None,None,0)==-3;count+=1
 assert effects.LIB.sa_rtc_set_samples(ctypes.byref(memory),None,1)==-3 and not memory.rtc_configured;count+=1
 samples=(ctypes.c_uint32*6)(0,1,0xffffffff,0x12345678,0xabcdef01,0x87654321);assert effects.LIB.sa_rtc_set_samples(ctypes.byref(memory),samples,6)==0;count+=1
 for prefix in (0,0x80000000,0xa0000000):
  for p in (0x710000,0x710004):
   before=memory.rtc_cursor
   for width in (1,2,8):assert bus.read(bus.context,prefix|p,width,ctypes.byref(out))==-3 and memory.rtc_cursor==before;count+=1
   assert bus.read(bus.context,prefix|p,4,None)==-3 and memory.rtc_cursor==before;count+=1
   assert bus.write(bus.context,prefix|p,0xffffffff,4)==-3 and memory.rtc_cursor==before;count+=1
   assert bus.read(bus.context,prefix|p,4,ctypes.byref(out))==0 and out.value==samples[before] and memory.rtc_cursor==before+1;count+=1
 assert list(samples)==[0,1,0xffffffff,0x12345678,0xabcdef01,0x87654321];count+=1
 value=out.value;assert bus.read(bus.context,0xa0710000,4,ctypes.byref(out))==-4 and memory.rtc_cursor==6 and out.value==value;count+=1
 assert effects.LIB.sa_rtc_set_samples(ctypes.byref(memory),None,0)==0 and memory.rtc_cursor==0 and bus.read(bus.context,0xa0710000,4,ctypes.byref(out))==-4;count+=1
 return count

def main():
 ram=driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c);r['independent_checks']=semantic(m,a,before,s,r,c);records.append(r);coverage.update(path)
  for j,pc in enumerate(path):
   t=ROWS.get(pc,'');distance=2 if t.startswith(('bt/s ','bf/s ')) else 1
   if t.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  print(hex(c['entry']),r['status'],r['stop_pc'],r['rtc_snapshot'],flush=True)
 if '--smoke' in sys.argv:return
 routines=list(manifest['routines'])+[r for r in old.old.manifest['routines'] if int(r['address'],16) in (STABLE,0x8c10ba48,0x8c10ba80)];rows=[]
 for routine in routines:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_rtc_connection.json',P/'analysis/movie_maple_qualification.json'];api=bus_checks()
 report=dict(cases_passed=len(records),independent_checks=sum(r['independent_checks'] for r in records),backend_checks=api,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Original RTC reads and diagnostic sink execute using explicit finite raw words, with no generated clock, helper return or callback result. Every reached original lower service executes or stops. Full-state/software-device and ordered ordinary boundary comparisons retain the earlier full-clear verification.')
 (P/'analysis/movie_rtc_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('RTC comparisons',len(records),report['status_counts'],report['independent_checks'],'backend',api);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
