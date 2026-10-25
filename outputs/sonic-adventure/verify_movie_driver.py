"""Original driver/library instructions with finite raw normal-status input."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_startup as old
P=old.P;ROOT=old.ROOT;previous=old.previous
DRIVER,SYNC,MODE,IDLE,LIB_INIT,LIB_RESET=(0x8c604702,0x8c6045b4,0x8c65262c,0x8c6045a8,0x8c6510a4,0x8c651384)
IRQ=0xa05f6900
def irq_address(a):return a&0x1fffffff==0x5f6900
MP=P/'analysis/movie_driver_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
code=old.code
marker='queue_reference.startup_snapshot()';assert code.count(marker)==1;code=code.replace(marker,marker+',queue_reference.driver_snapshot()')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if a==0xffa00040:\n    if n!=4:stop(pc,r,sr,-3)\n    if c.get('null_bus'):stop(pc,r,sr,-2)\n   if irq_address(a):\n    if n!=4 or a&3:stop(pc,r,sr,-3)\n    if c.get('null_bus'):stop(pc,r,sr,-2)\n    if queue_reference.irq_samples is not None and queue_reference.irq_cursor>=len(queue_reference.irq_samples) and text.split(' ',1)[1].split(',')[0].startswith('@'):stop(pc,r,sr,-4)")
marker='  if pc==STALL:';assert code.count(marker)==1
code=code.replace(marker,"  if pc==0x8c6656e2:guard_visits[0x8c665968]=0\n  if pc==0x8c665968:\n   guard_visits[pc]=guard_visits.get(pc,0)+1\n   if guard_visits[pc]>=2:stop(pc,r,sr,-4)\n"+marker)
GUARDS=dict(old.namespace['GUARDS']);GUARD_ENTRY=dict(old.namespace['GUARD_ENTRY'])
GUARDS[0x8c665998]=(False,2);GUARD_ENTRY[0x8c6656e2]=[0x8c665998]
namespace=dict(old.namespace,GUARDS=GUARDS,GUARD_ENTRY=GUARD_ENTRY,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW,irq_address=irq_address)
exec(compile(code,'driver_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_driver.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
native_fn=effects.LIB.sa_movie_driver_connected;native_fn.argtypes=old.native_fn.argtypes;native_fn.restype=ctypes.c_int
initialized_fn=effects.LIB.sa_movie_driver_initialized_connected;initialized_fn.argtypes=native_fn.argtypes;initialized_fn.restype=ctypes.c_int;effects.sequence=False
def invoke(*args):return (initialized_fn if effects.sequence else native_fn)(*args)
effects.fn=invoke
class DriverMemory(old.StartupMemory):_fields_=[('startup_bus',effects.Bus),('irq_samples',ctypes.POINTER(ctypes.c_uint32)),('irq_count',ctypes.c_uint32),('irq_cursor',ctypes.c_uint32),('irq_configured',ctypes.c_uint32),('dma_operation',ctypes.c_uint32)]
effects.LIB.sa_driver_signal_bus_init.argtypes=[ctypes.POINTER(DriverMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus),ctypes.c_uint32,ctypes.c_uint32]
effects.LIB.sa_driver_signal_set_samples.argtypes=[ctypes.POINTER(DriverMemory),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];effects.LIB.sa_driver_signal_set_samples.restype=ctypes.c_int
for name in ('sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes
effects.LIB.sa_hardware_transfer_set_samples.restype=ctypes.c_int;effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int;effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
class DriverReference(old.StartupReference):
 def __init__(self,ticks,capacity,c):
  super().__init__(ticks,capacity,c);self.irq_samples=None if 'irq_samples' not in c else [x&0xffffffff for x in c['irq_samples']];self.irq_cursor=0;self.dma_operation=c.get('dma_operation',0)
  self.registers.update({int(a):value&0xffffffff for a,value in c.get('render_seed',{}).items()})
  for a,value in c.get('startup_overrides',{}).items():self.startup_registers[(int(a)-0x5f6800)//4]=value&0xffffffff
 def driver_snapshot(self):return int(self.irq_samples is not None),0 if self.irq_samples is None else len(self.irq_samples),self.irq_cursor,self.dma_operation
 def __contains__(self,a):return a==0xffa00040 or super().__contains__(a)
 def __setitem__(self,a,value):
  if a==0xffa00040:self.dma_operation=value&0xffffffff;self.events.append((30,a,self.dma_operation,4));return
  return super().__setitem__(a,value)
 def __getitem__(self,a):
  if a==0xffa00040:self.events.append((29,a,self.dma_operation,4));return self.dma_operation
  if irq_address(a) and self.irq_samples is not None:self.normal_status=self.irq_samples[self.irq_cursor];self.irq_cursor+=1
  return super().__getitem__(a)
compare_code=old.compare_code.replace('StartupMemory','DriverMemory').replace('StartupReference','DriverReference').replace('sa_startup_platform_bus_init','sa_driver_signal_bus_init')
marker=" memory.startup_registers[:]=seed(c,'startup_seed',BANK_WORDS);memory.system_registers[:]=seed(c,'system_seed',9);memory.cache_tags[:]=seed(c,'tag_seed',512)";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"\n irq_values=None if 'irq_samples' not in c else [x&0xffffffff for x in c['irq_samples']];irq_array=None;irq_before=None\n if irq_values is not None:\n  irq_array=(ctypes.c_uint32*len(irq_values))(*irq_values);irq_before=bytes(irq_array)\n  assert effects.LIB.sa_driver_signal_set_samples(ctypes.byref(memory),irq_array,len(irq_values))==0")
marker=' irq_values=None';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," memory.dma_operation=c.get('dma_operation',0)\n for address,value in c.get('startup_overrides',{}).items():memory.startup_registers[(int(address)-0x5f6800)//4]=value&0xffffffff\n"+marker)
marker=' memory.normal_status=c.get';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," for address,value in c.get('render_seed',{}).items():memory.registers[(int(address)-0x5f8000)//4]=value&0xffffffff\n"+marker)
marker='snapshot(memory.startup_registers,memory.system_registers,memory.cache_tags)))';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,'snapshot(memory.startup_registers,memory.system_registers,memory.cache_tags),(memory.irq_configured,memory.irq_count,memory.irq_cursor,memory.dma_operation)))')
marker=' return dict(startup_registers=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert (memory.irq_configured,memory.irq_count,memory.irq_cursor,memory.dma_operation)==ref.driver_snapshot()\n assert irq_before is None or bytes(irq_array)==irq_before\n return dict(dma_operation=memory.dma_operation,driver_events=[list(e) for e in ref.events if e[0] in (29,30)],irq_cursor=memory.irq_cursor,irq_samples_unchanged=True,irq_reads=[list(e) for e in ref.events if e[0]==13],startup_registers=")
exec(compile(compare_code,'driver_comparison','exec'),dict(old._compare.__globals__,effects=effects,namespace=namespace,DriverMemory=DriverMemory,DriverReference=DriverReference),local:={});_compare=local['compare']
load=old.load;store=old.store
def compare(m,s,f,c):
 effects.sequence=c.get('initialize_context',False);record,path,a,s,f=_compare(m,s,f,c);record['heap_root_at_stop']=hex(load(a,0x8c88ef3c));record['library_root_at_stop']=hex(load(a,0x8c67c004));return record,path,a,s,f
def prepare(ram,c):
 m,s,f=old.prepare(ram,c)
 if c['entry']==DRIVER:s.r[4]=c.get('driver_flags',0);s.r[5]=c.get('pixel_format',0);s.r[6]=c.get('config2',0);s.r[7]=c.get('extra',0xf)
 elif c['entry']==SYNC:s.r[4]=c.get('sync_value',0)
 elif c['entry']==IDLE:s.r[4]=0x8cda0000;store(m,s.r[4],0xa5a5a5a5)
 elif c['entry']==MODE:s.r[4]=c.get('display_mode',0)
 if 'arguments' in c:s.r[4:8]=c['arguments']
 for address,value in c.get('ram_words',{}).items():store(m,int(address),value)
 if c.get('cold_library'):store(m,0x8c67c004,0)
 if c.get('indexed_sizes'):
  first=load(m,0x8c665ef0);second=load(m,0x8c665ef4)
  for i in range(5):store(m,first+i*4,i%4)
  store(m,second,2)
 if c['entry']==0x8c666706:
  store(m,s.r[15],c.get('format_flags',0));store(m,load(m,0x8c6668c4),c.get('frame_flags',0));store(m,load(m,0x8c6668c8),c.get('frame_mode',0))
 if c.get('linked_pools'):
  head=load(m,0x8c663120)
  for group,offset in enumerate((8,0,0x1c,0x14)):
   first=0x8c8a83e4+group*48;store(m,head+offset,first)
   for i in range(2):store(m,first+i*24,0xa5a50001);store(m,first+i*24+8,first+24 if not i else 0)
  store(m,load(m,0x8c663130),0x8c8c03e4)
  for i in range(2):store(m,0x8c8c03e4+i*76,0xa5a50001);store(m,0x8c8c03e4+i*76+4,0x8c8c03e4+76 if not i else 0)
 if c.get('pool_kind'):
  base,size,stride=(0x8c8a83e4,0x18000,24) if c['pool_kind']=='small' else (0x8c8c03e4,0x13000,76)
  occupied=c.get('occupied',0)
  for index in range(size//stride):m[(base+index*stride)&0xffffff:((base+index*stride)&0xffffff)+2]=(0xa5a1 if index<occupied else 0xa5a0).to_bytes(2,'little')
  if c.get('release_index') is not None:s.r[4]=base+c['release_index']*stride
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f
def cases(smoke=False):
 base=dict(mode=0,render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},irq_samples=[0,8],fifo_samples=[0]*32,scan_samples=[0,1]*32)
 if smoke:
  selected=[dict(entry=SYNC),dict(entry=MODE,display_mode=0,cold_library=True),dict(entry=DRIVER),dict(entry=old.ENTRY,cold_library=True)]
 else:
  selected=[dict(entry=a,arguments=[0,0,0,0]) for a in sorted(NEW) if a not in (SYNC,MODE,DRIVER)]
  selected += [dict(entry=0x8c665e00,arguments=[x,0,0,0],indexed_sizes=True) for x in (3,0xffffffff)]
  selected += [dict(entry=LIB_INIT,cold_library=True),dict(entry=0x8c663080,linked_pools=True)]
  selected += [dict(entry=0x8c666706,format_flags=flags,frame_flags=frame,frame_mode=mode) for flags,frame,mode in ((0,0,0),(1,0,0),(0x10000,0,0),(0,0x20,0),(0,0x20,1),(0x10001,0x20,1))]
  selected += [dict(entry=MODE,display_mode=x,cold_library=True) for x in (0,1,2,3,0xffffffff)]
  selected += [dict(entry=DRIVER,driver_flags=x,pixel_format=y) for x,y in ((0,0),(1,1),(2,3),(3,4),(0x10,5),(0x20,1),(0x30,1),(0x10000,1))]
  selected += [dict(entry=old.ENTRY,cold_library=True,mode=x) for x in (0,1)]
  for mask in (0,1,5,15):
   regs=[0]*48
   for i in range(4):regs[6+i*8]=(mask>>i)&1;regs[7+i*8]=0x1234+i
   selected.append(dict(entry=SYNC,sync_value=0x321,transfer_registers=regs,control_word=0xabcdef01,fifo_samples=[0x20,0]))
  selected += [dict(entry=IDLE,fifo_samples=[x]) for x in (0,0x20,0xffffffff,0x1234)]
  selected += [dict(entry=0x8c664ac0,pool_kind='small',occupied=x) for x in (0,1,4095,4096)]
  selected += [dict(entry=entry,pool_kind=kind,occupied=4096,release_index=i) for entry,kind in ((0x8c664aec,'small'),(0x8c665440,'large')) for i in (-1,0,1,4095 if kind=='small' else 1023)]
  selected += [dict(entry=0x8c6656e2,startup_overrides={0x5f689c:x}) for x in (0,1,2,3,4,5,6,8,9)]
  selected += [dict(entry=0x8c6656e2,render_seed={0x5f8000:0,0x5f8004:0x11}),dict(entry=0x8c6656e2,render_seed={0x5f8000:0x17fd11db,0x5f8004:1}),dict(entry=0x8c6656e2,render_seed={0x5f8000:0x17fd11db,0x5f8004:2})]
  selected += [dict(entry=SYNC,fifo_samples=[]),dict(entry=DRIVER,irq_samples=[]),dict(entry=DRIVER,irq_samples=[0])]
  selected += [dict(entry=x,null_bus=True) for x in (SYNC,DRIVER,0x8c653640,0x8c665a00)]
  selected += [dict(entry=x,sr=0) for x in (SYNC,0x8c653640)]
  selected += [dict(entry=x,bad_stack=True) for x in (SYNC,DRIVER,LIB_INIT,0x8c664aec)]
 for case in selected:yield base|case

def semantics(m,a,before,s,r,c):
 entry=c['entry'];checks=0;rc=r['status']
 expected=0
 if c.get('bad_stack'):expected=-3
 elif c.get('null_bus'):expected=-2
 elif c.get('sr')==0:expected=-4
 elif entry==IDLE and 'arguments' in c:expected=-3
 elif entry==DRIVER and c.get('driver_flags',0)&3==3:expected=-2
 elif entry in (DRIVER,LIB_RESET,0x8c6656e2,old.ENTRY) or entry==MODE and c.get('display_mode',0) in (0,1,2):expected=-4
 elif entry==SYNC and not c['fifo_samples']:expected=-4
 assert rc==expected,(hex(entry),rc,expected,r['stop_pc']);checks+=1
 if rc:
  if entry==DRIVER and not c.get('null_bus') and not c.get('bad_stack'):
   assert r['stop_pc']==('0x8c60487c' if len(c['irq_samples'])<2 else ('0x8c000000' if c.get('driver_flags',0)&3==3 else '0x8c665998'));checks+=1
  if entry==old.ENTRY:
   assert r['stop_pc']=='0x8c665998' and r['heap_root_at_stop']=='0x8c88ef1c' and r['library_root_at_stop']=='0x8c8a2d54';checks+=3
  if r['stop_pc']=='0x8c665998':
   assert r['vram_writes']==[[14,0xa5000000,0,4],[14,0xa5000004,0,4]];checks+=1
  return checks
 assert s.r[15]==before.r[15] and s.pr==before.pr;checks+=2
 if entry==SYNC:
  mask=int.from_bytes(m[0x60476c:0x60476e],'little',signed=True)&0xffffffff
  assert r['control_word']==((c.get('control_word',0)&mask)|c.get('sync_value',0));checks+=1
  original=c.get('transfer_registers',[0]*48)
  for i in range(4):assert r['transfer_registers'][7+i*8]==(1 if original[6+i*8]&1 else original[7+i*8]);checks+=1
  assert list(s.r)[12:15]==list(before.r)[12:15] and s.sr&0xf0==before.sr&0xf0;checks+=2
 if entry==IDLE:assert load(a,before.r[4])==int(not(c['fifo_samples'][0]&0x20));checks+=1
 if entry==LIB_INIT:
  assert load(a,0x8c67c004)==(load(m,0x8c6511e8) if c.get('cold_library') else load(m,0x8c67c004));checks+=1
  root=load(m,0x8c6511e8);version=load(m,0x8c6511f0)
  assert [load(a,version+4*i) for i in range(4)]==[3,2,6,2] and load(a,root)==version;checks+=2
 if entry==0x8c641160:
  center=load(m,0x8c641200);base=load(m,0x8c641204);assert load(a,center)==0;checks+=1
  for offset,literal in ((0,0x8c641208),(16,0x8c641210),(32,0x8c641218),(48,0x8c641220),(64,0x8c641228)):
   assert load(a,base+offset)==load(m,literal) and load(a,base+offset+4)==0;checks+=2
  for offset,literal in ((-0x6c,0x8c64120c),(-0x5c,0x8c641214),(-0x4c,0x8c64121c),(-0x3c,0x8c641224)):
   assert load(a,center+offset)==load(m,literal) and load(a,center+offset+4)==0;checks+=2
 if entry==0x8c650aa0:
  assert r['dma_operation']==0x8201;checks+=1
  base=load(m,0x8c650b5c);extent=int.from_bytes(m[0x650b22:0x650b24],'little')
  for i in range(0,extent,32):assert load(a,base+i+24)==0 and load(a,base+extent+i+24)==8;checks+=2
 if entry==0x8c653640:
  offset=int.from_bytes(m[0x6536a2:0x6536a4],'little',signed=True)
  assert load(a,c.get('vbr',0x8ce50000)+offset)==before.r[4] and s.sr&0xf0==before.sr&0xf0;checks+=2
 if entry==0x8c665e00:
  table=load(m,0x8c665eec);root=load(m,0x8c665ef0);other=load(m,0x8c665ef4)
  indices=[load(m,root+i) for i in (0,8,12,16)]+[load(m,other)]
  value=(sum(load(m,(table+4*i)&0xffffffff) for i in indices)*before.r[4])&0xffffffff
  assert s.r[0]==value*4&0xffffffff and r['macl']==value;checks+=2
 if entry in (0x8c64241a,0x8c653440):assert a==m and list(s.r)==list(before.r) and s.sr==before.sr;checks+=3
 if entry==0x8c663080:
  head=load(m,0x8c663120);pool=load(m,0x8c663128);size=load(m,0x8c66312c)
  assert all(load(a,head+i)==0 for i in range(0,40,4)) and load(a,load(m,0x8c663130))==0;checks+=2
  assert all(int.from_bytes(a[(pool+i)&0xffffff:((pool+i)&0xffffff)+2],'little')==0 for i in range(0,size,24));checks+=1
 if c.get('pool_kind'):
  base,size,stride=(0x8c8a83e4,0x18000,24) if c['pool_kind']=='small' else (0x8c8c03e4,0x13000,76)
  flags=lambda data,i:int.from_bytes(data[(base+i*stride)&0xffffff:((base+i*stride)&0xffffff)+2],'little')
  if entry==0x8c664ac0:
   chosen=c['occupied'];assert s.r[0]==(base+chosen*stride if chosen<size//stride else 0);checks+=1
   assert all(flags(a,i)==(1 if i==chosen else flags(m,i)) for i in range(size//stride));checks+=1
  else:
   assert all(flags(a,i)==(flags(m,i)&0xfffe if base+i*stride==before.r[4] else flags(m,i)) for i in range(size//stride));checks+=1
 return checks

def main():
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=cases('--smoke' in sys.argv)
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,s,record,c);records.append(record);coverage.update(path)
  print(hex(c['entry']),record['status'],record['stop_pc'],len(path),record['heap_root_at_stop'],record['library_root_at_stop'],flush=True)
  for j,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 rows=[]
 for routine in manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 if '--smoke' in sys.argv:print('Driver original/native smoke comparisons',len(records));return
 files=[Path(__file__),Path(old.__file__),MP,P/'analysis/movie_driver_connection.json']
 report=dict(independent_checks=sum(r['independent_checks'] for r in records),cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,routines=manifest['routines'],dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records)
 (P/'analysis/movie_driver_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks')});print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
