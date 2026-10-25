"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_queue_family_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_transfer_start_translation.json';manifest=json.loads(MP.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
ROWS=dict(old.ROWS);PER_BODY=dict(old.PER_BODY)
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(old.namespace,ROWS=ROWS,RETURNS=old.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=old.namespace['LEGACY']-NEW)
code=old.code
marker=' def trace(pc,w,r,sr,pr,fp):';assert code.count(marker)==1
code=code.replace(marker,' completion_wait_counts={}\n'+marker)
marker='   a,n=access';assert code.count(marker)==1
marker='  assert pc in ROWS,hex(pc);path.append(pc);text=ROWS[pc]'
# Place the guard after the original trace has obtained the current opcode text.
lines=code.splitlines();positions=[i for i,line in enumerate(lines) if line.startswith(marker)];assert len(positions)==1
at=positions[0]+1
lines[at:at]=["  if pc==0x8c65a6ec and not sr&1:","   completion_wait_counts[pc]=completion_wait_counts.get(pc,0)+1","   if completion_wait_counts[pc]>=2:stop(pc,r,sr,-4)"]
lines[at:at]=["  if text.startswith('ocbwb '):","   address=r[int(text.split('r')[-1])]","   if c.get('null_bus'):stop(pc,r,sr,-2)","   if address&0x1f000000!=0x0c000000:stop(pc,r,sr,-3)"]
code='\n'.join(lines)+'\n'

exec(compile(code,'init_records_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=old.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_transfer_start.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_transfer_start_observed;observed.argtypes=old.observed.argtypes;observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.compare_code
marker='independent_checks=0,boundaries=boundaries';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,"independent_checks=0,cache_writebacks=[list(e) for e in ref.events.retained() if e[0]==6],boundaries=boundaries")
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
START,STATUS,CACHE,PENDING,CALLBACK,CLIENT_START=(0x8c10faa4,0x8c1101f0,0x8c5fc58a,0x8c66abfc,0x8c668f40,0x8c666d10)
ROOT_CONTEXT,TABLE,DESCRIPTOR,RECORD,CHILD,OUT1=(old.ROOT_CONTEXT,0x8cdc0000,0x8cde0000,old.RECORD,0x8cda0000,old.OUT1)
def half(m,a):return ctypes.c_int16(int.from_bytes(sample(m,a,2),'little')).value

def prepare(ram,c):
 entry=c['entry'];context=Input(c|dict(entry=old.ENQUEUE,count=c.get('pending',0)));m,s,f=old.prepare(ram,context);context['entry']=entry;c.update(context);s.pc=entry
 root=c.get('root',ROOT_CONTEXT);table=c.get('table',TABLE);channel=c.get('channel',0);store(m,0x8c67c1d0,root if c.get('enabled',True) else 0)
 store(m,load(m,0x8c10fb34),root);store(m,ROOT_CONTEXT,table)
 for i in range(48):store(m,TABLE+i*4,0x35350000+i)
 if 0<=channel<6:
  if table==TABLE:store(m,TABLE+channel*32+24,c.get('busy',0))
  elif table==0xa05f7800:
   values=c.get('transfer_registers',[0x35350000+i for i in range(48)]);values=list(values);values[channel*8+6]=c.get('busy',0);c['transfer_registers']=values
 for offset,value in ((0,channel),(4,c.get('source',0xa0800000)),(8,c.get('destination',0x8cd70040)),(12,c.get('length',64)),(16,c.get('direction',0)),(20,c.get('flags',5)),(24,0xdeadbeef),(28,0x8c668f40),(32,RECORD),(44,0x11112222),(48,0x33334444)):store(m,DESCRIPTOR+offset,value)
 if channel<6:store(m,ROOT_CONTEXT+half(m,0x8c10fb32)+channel*4,c.get('counter',0x12345678))
 store(m,ROOT_CONTEXT+half(m,0x8c666e12),c.get('channel_pointer',DESCRIPTOR))
 if entry in (START,STATUS):s.r[4]=c.get('descriptor',DESCRIPTOR);s.r[5]=c.get('argument',0) if entry==START else c.get('output',OUT1)
 if entry==CACHE:s.r[4]=c.get('address',0x8cd70040);s.r[5]=c.get('length',64)
 if entry==PENDING:s.r[4]=c.get('output',OUT1)
 if entry in (CALLBACK,CLIENT_START):
  s.r[4]=c.get('record',RECORD);s.r[5]=c.get('argument',0)
  for offset,value in ((4,c.get('state',0)),(8,c.get('destination',0x8cd70040)),(12,c.get('length',64)),(16,c.get('callback',0xffffffff)),(20,c.get('callback_argument',0x2468ace0)),(24,0xdeadbeef)):store(m,RECORD+offset,value)
  store(m,ROOT_CONTEXT+half(m,0x8c668fe0),c.get('sdk_mode',2))
  store(m,ROOT_CONTEXT+half(m,0x8c668fdc),c.get('completed_count',0x11223344))
  store(m,ROOT_CONTEXT+half(m,0x8c668fde),c.get('current_record',RECORD))
  store(m,ROOT_CONTEXT+half(m,0x8c668fde)-4,c.get('callback_count',0x55667788))
  store(m,old.QUEUE,CHILD);store(m,old.QUEUE+4,c.get('child_argument',0))
  for offset,value in ((4,c.get('child_state',2)),(8,0x8cd70080),(12,32),(16,0xffffffff),(20,0),(24,0xdeadbeef)):store(m,CHILD+offset,value)
 return m,s,f

