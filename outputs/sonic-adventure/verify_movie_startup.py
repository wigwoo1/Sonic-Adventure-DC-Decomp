"""Original startup/platform instructions with explicit software register state."""
import collections,csv,ctypes,hashlib,itertools,json,re,struct,sys,types
from pathlib import Path
import verify_movie_heap as old
P=old.P;ROOT=old.ROOT;previous=old.previous
ENTRY,PLATFORM,EMPTY,TABLE,VECTORS,EMPTY2,TIMER,CACHE_HEAD,CACHE_TAIL,CACHE_SETUP=(0x8c010180,0x8c6024d0,0x8c02e720,0x8c602570,0x8c645de0,0x8c645f80,0x8c6025e8,0x8c5fc81c,0x8c5fc848,0x8c5fc06c)
SYSTEM=(0xffd00000,0xffd00004,0xffd00008,0xffd0000c,0xffd80000,0xffd80004,0xffd80008,0xffd8000c,0xffd80010)
WIDTHS=(2,2,2,2,1,1,4,4,2);BANK_WORDS=0x1500//4
CLOCK=0x8c60260e
def inherited(p):return p in (0x5f6800,0x5f6804,0x5f6808,0x5f688c,0x5f6900,0x5f6920,0x5f6924) or 0x5f7800<=p<0x5f78c0
def startup_slot(a):
 if a in SYSTEM:return 'system_registers',SYSTEM.index(a),WIDTHS[SYSTEM.index(a)],WIDTHS[SYSTEM.index(a)]
 p=a&0x1fffffff
 if 0x5f6800<=p<0x5f7d00 and not inherited(p):return 'startup_registers',(p-0x5f6800)//4,4,4
 if 0xf4000000<=a<0xf4004000:return 'cache_tags',(a-0xf4000000)//32,4,32
 return None
def startup_address(a):return startup_slot(a) is not None
def snapshot(startup,system,tags):
 values=list(startup)+list(system)+list(tags);return hashlib.sha256(struct.pack('<%dI'%len(values),*values)).digest()
MP=P/'analysis/movie_startup_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 if 'canonical_physical_entry' in routine:
  body={a+0x20000000:re.sub(r'0x8c5fc[0-9a-f]+',lambda m:hex(int(m[0],16)+0x20000000),t) for a,t in body.items()}
 ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
code=old.old.old.code
marker='tuple(queue_reference.transfer_registers)';assert code.count(marker)==1;code=code.replace(marker,marker+',queue_reference.startup_snapshot()')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if startup_address(a):\n    slot=startup_slot(a)\n    if n!=slot[2] or a&(slot[3]-1):stop(pc,r,sr,-3)\n    if c.get('null_bus'):stop(pc,r,sr,-2)")
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW,startup_address=startup_address,startup_slot=startup_slot)
exec(compile(code,'startup_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_startup.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
native_fn=effects.LIB.sa_movie_startup_connected;native_fn.argtypes=old.native_fn.argtypes;native_fn.restype=ctypes.c_int
initialized_fn=effects.LIB.sa_movie_startup_initialized_connected;initialized_fn.argtypes=native_fn.argtypes;initialized_fn.restype=ctypes.c_int;effects.sequence=False
def invoke(*args):return (initialized_fn if effects.sequence else native_fn)(*args)
effects.fn=invoke
ContextMemory=old._compare.__globals__['ContextMemory'];ContextReference=old._compare.__globals__['ContextReference']
class StartupMemory(ContextMemory):_fields_=[('context_bus',effects.Bus),('startup_registers',ctypes.c_uint32*BANK_WORDS),('system_registers',ctypes.c_uint32*9),('cache_tags',ctypes.c_uint32*512)]
effects.LIB.sa_startup_platform_bus_init.argtypes=[ctypes.POINTER(StartupMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus),ctypes.c_uint32,ctypes.c_uint32]
for name in ('sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes
effects.LIB.sa_hardware_transfer_set_samples.restype=ctypes.c_int;effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int;effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
def seed(c,name,count):
 values=[(c[name]^i)&0xffffffff for i in range(count)] if name in c else [0]*count
 if name=='system_seed':values=[v&((1<<(WIDTHS[i]*8))-1) for i,v in enumerate(values)]
 return values
class StartupReference(ContextReference):
 def __init__(self,ticks,capacity,c):
  super().__init__(ticks,capacity,c);self.startup_registers=seed(c,'startup_seed',BANK_WORDS);self.system_registers=seed(c,'system_seed',9);self.cache_tags=seed(c,'tag_seed',512)
 def startup_snapshot(self):return snapshot(self.startup_registers,self.system_registers,self.cache_tags)
 def __contains__(self,a):return startup_address(a) or super().__contains__(a)
 def __getitem__(self,a):
  # The inherited CPU clock-read producer owns this address. Reset writes are
  # stored separately; reading it must still consume the raw observation stream.
  if a==0xffd8000c:return super().__getitem__(a)
  slot=startup_slot(a)
  if slot is None:return super().__getitem__(a)
  name,i,n,alignment=slot;value=getattr(self,name)[i];self.events.append((27,a,value,n));return value
 def __setitem__(self,a,value):
  slot=startup_slot(a)
  if slot is None:return super().__setitem__(a,value)
  name,i,n,alignment=slot;value&=(1<<(8*n))-1;getattr(self,name)[i]=value;self.events.append((28,a,value,n))
compare_code=old.old.old.compare_code.replace('ContextMemory','StartupMemory').replace('ContextReference','StartupReference').replace('sa_transfer_context_bus_init','sa_startup_platform_bus_init')
marker=" memory.transfer_registers[:]=c.get('transfer_registers',[0]*48)";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"\n memory.startup_registers[:]=seed(c,'startup_seed',BANK_WORDS);memory.system_registers[:]=seed(c,'system_seed',9);memory.cache_tags[:]=seed(c,'tag_seed',512)")
marker='tuple(memory.transfer_registers)))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'tuple(memory.transfer_registers),snapshot(memory.startup_registers,memory.system_registers,memory.cache_tags)))')
marker=' return dict(transfer_registers=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert list(memory.startup_registers)==ref.startup_registers and list(memory.system_registers)==ref.system_registers and list(memory.cache_tags)==ref.cache_tags\n return dict(startup_registers=list(memory.startup_registers),system_registers=list(memory.system_registers),cache_tags=list(memory.cache_tags),startup_events=[list(e) for e in ref.events if e[0] in (27,28)],purge_events=[list(e) for e in ref.events if e[0]==18],transfer_registers=")
exec(compile(compare_code,'startup_comparison','exec'),dict(old._compare.__globals__,effects=effects,namespace=namespace,StartupMemory=StartupMemory,StartupReference=StartupReference,seed=seed,BANK_WORDS=BANK_WORDS,snapshot=snapshot),local:={});_compare=local['compare']
load=old.load;store=old.store
def compare(m,s,f,c):
 effects.sequence=c.get('initialize_context',False);result=_compare(m,s,f,c);record,path,a,s,f=result
 record['heap_root_at_stop']=hex(load(a,old.ROOT_WORD));return result
def prepare(ram,c):
 m,s,f=old.old.prepare(ram,c)
 if c['entry']==VECTORS:s.r[4]=c.get('selector',0);s.r[5]=c.get('vector_argument',0x12345678)
 elif c['entry'] in (CACHE_SETUP,CACHE_SETUP+0x20000000):s.r[4]=c.get('cache_value',0x101)
 elif c['entry'] in (CACHE_TAIL,CACHE_TAIL+0x20000000):store(m,s.r[15],c.get('saved_mask',7))
 elif c['entry']==ENTRY:s.r[4]=c.get('config0',0);s.r[5]=c.get('config1',0);s.r[6]=c.get('config2',0)
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f
def cases(smoke=False):
 if smoke:
  return [dict(entry=TABLE,mode=0),dict(entry=VECTORS,mode=0),dict(entry=CACHE_SETUP+0x20000000,mode=0),dict(entry=ENTRY,mode=0)]
 out=[]
 for mode in (0,4):out.append(dict(entry=CLOCK,mode=mode,ticks=[100,200],system_seed=0x55aa))
 for entry,mode in itertools.product((TABLE,TIMER,EMPTY,EMPTY2,PLATFORM),(0,4)):out.append(dict(entry=entry,mode=mode,system_seed=0x55aa,tag_seed=0xa5a50000))
 for mode,selector,vbr in itertools.product((0,4),(0,0x80000000,0xffffffff),(0x8ce50000,0x8ce60000)):
  out.append(dict(entry=VECTORS,mode=mode,selector=selector,vbr=vbr,tag_seed=0xa5a50000))
 for entry,mode,ccr in itertools.product((CACHE_HEAD,CACHE_TAIL,CACHE_TAIL+0x20000000,CACHE_SETUP,CACHE_SETUP+0x20000000),(0,4),(0,0x20)):
  out.append(dict(entry=entry,mode=mode,ccr=ccr,tag_seed=0xa5a50000,saved_mask=3,cache_value=0x121))
 for mode in (0,4):out.append(dict(entry=ENTRY,mode=mode,tag_seed=0xa5a50000))
 for entry in (TABLE,TIMER,VECTORS,CACHE_HEAD,CACHE_SETUP,CACHE_SETUP+0x20000000):
  out.append(dict(entry=entry,mode=0,null_bus=True))
 for entry in (VECTORS,CACHE_HEAD,CACHE_SETUP,CACHE_SETUP+0x20000000,ENTRY):out.append(dict(entry=entry,mode=0,sr=0))
 for entry in (CACHE_HEAD,CACHE_SETUP,ENTRY):out.append(dict(entry=entry,mode=0,bad_stack=True))
 return out
def semantics(m,a,before,s,record,c):
 entry=c['entry'];checks=0
 if record['status']:
  if entry==ENTRY and record['status']==-2 and record['stop_pc']=='0x8c604702':
   pool=(((load(m,0x8c66c2f8)&0xe0000000)|0x0d000000)+0xfff00000)&0xffffffff
   assert load(a,old.ROOT_WORD)==0x8c88ef1c and load(a,0x8c88ef1c)==pool and load(a,pool+4)==0x100000//32;checks+=3
  return checks
 if entry==VECTORS:
  vbr=c.get('vbr',0x8ce50000)
  expected_source=bytearray(m);special=c.get('selector',0)==0x80000000
  handler=load(m,0x8c645e6c if special else 0x8c645e7c)
  for literal in (0x8c645e64,0x8c645e70):store(expected_source,load(m,literal),handler)
  store(expected_source,load(m,0x8c645e74),0);store(expected_source,load(m,0x8c645e78),c.get('vector_argument',0x12345678) if special else 0)
  for offset,start,end in ((0x100,load(m,0x8c645e80),load(m,0x8c645e84)),(0x400,load(m,0x8c645ec8),load(m,0x8c645ecc)),(0x600,load(m,0x8c645f10),load(m,0x8c645f14))):
   length=end-start+4;dst=(vbr+offset)&0xffffff;assert a[dst:dst+length]==expected_source[start&0xffffff:(start&0xffffff)+length];checks+=1
  # Purge bases are separate original literals (400/1000/1800), not
  # inferred from the vector-copy destinations (100/400/600).
  assert record['purge_events']==[[18,vbr+offset,0,0] for offset in (0x400,0x420,0x1000,0x1020,0x1800,0x1820)];checks+=1
 if entry in (CACHE_HEAD,CACHE_TAIL,CACHE_TAIL+0x20000000,CACHE_SETUP,CACHE_SETUP+0x20000000):
  wanted=seed(c,'tag_seed',512)
  for i in range(512):
   if not c.get('ccr',0)&0x20 or i<128 or 256<=i<384:wanted[i]=0
  assert record['cache_tags']==wanted;checks+=1
  assert s.r[15]==(before.r[15]+(4 if entry in (CACHE_TAIL,CACHE_TAIL+0x20000000) else 0))&0xffffffff;checks+=1
  if entry in (CACHE_SETUP,CACHE_SETUP+0x20000000):
   targets=[b['target'] for b in record['boundaries']]
   assert targets.count(hex(CACHE_HEAD))==1 and hex(CACHE_HEAD+0x20000000) not in targets and targets.count(hex(CACHE_TAIL+0x20000000))==1;checks+=3
 if entry in (EMPTY,EMPTY2):assert a==m and list(s.r)==list(before.r) and s.sr==before.sr;checks+=3
 if entry==CLOCK:
  assert record['clock_reads']==[[0xffd8000c,0xffffffff-c['ticks'][0],4]] and s.r[0]==c['ticks'][0]
  assert record['system_registers'][7]==seed(c,'system_seed',9)[7];checks+=3
 return checks
def main():
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();smoke='--smoke' in sys.argv
 for c in cases(smoke):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);record,path,a,s,f=compare(m,s,f,c)
  expected=-3 if c.get('bad_stack') else (-4 if 'sr' in c and not c['sr']&0x40000000 else (-2 if c.get('null_bus') or c['entry']==ENTRY else 0))
  assert record['status']==expected,(c,record['status'],record['stop_pc'])
  if c['entry']==ENTRY and expected==-2 and not c.get('null_bus'):assert record['stop_pc']=='0x8c604702'
  record['independent_checks']=semantics(m,a,before,s,record,c);records.append(record);coverage.update(path)
  print(hex(c['entry']),record['status'],record['stop_pc'],len(path),flush=True)
  for j,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 if smoke:print('Startup smoke comparisons passed',len(records));return
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_startup_connection.json',P/'analysis/movie_heap_qualification.json']
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,routines=manifest['routines'],
  dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,
  scope='Original early caller/platform/vector/cache paths with full CPU/RAM/FPU/banked/special and ordered software register/cache-array state. Raw storage, coherent cache operations and inherited runtime/VBR fixtures are explicit platform policies. No peripheral completion or game-service return is supplied.')
 (P/'analysis/movie_startup_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
