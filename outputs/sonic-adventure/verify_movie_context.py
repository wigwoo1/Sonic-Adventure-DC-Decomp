"""Run original context initialization and retain its state for later startup."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_movie_hardware as previous
from verify_overlay_selector_initializer import load,store
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/movie_context_translation.json';manifest=json.loads(MP.read_text());INIT,DESC,WINDOW,BOOT=0x8c10d5a8,0x8c10ed54,0x8c10ee64,0x8c10b840;CROOT,BASE=0x8c7ac7a0,0xa05f7800
NEW={INIT,DESC,WINDOW};ROWS=dict(previous.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
def context_address(a):return 0x005f7800<=a&0x1fffffff<0x005f78c0
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW,context_address=context_address)
code=previous.code;marker='queue_reference.control_word';assert code.count(marker)==1;code=code.replace(marker,marker+',tuple(queue_reference.transfer_registers)')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if context_address(a):\n    if c.get('null_bus'):stop(pc,r,sr,-2)\n    if n!=4 or a&3:stop(pc,r,sr,-3)\n    if c.get('context_write_status') and not text.split(' ',1)[1].split(',')[0].startswith('@'):stop(pc,r,sr,c['context_write_status'])")
exec(compile(code,'transfer_context_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.DLL=ROOT/'work/sonic-adventure/build/movie_context.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));native_fn=effects.LIB.sa_movie_context_connected;native_fn.argtypes=previous.effects.fn.argtypes;native_fn.restype=ctypes.c_int
class ContextMemory(previous.HardwareMemory):_fields_=[('hardware_bus',effects.Bus),('transfer_registers',ctypes.c_uint32*48)]
effects.LIB.sa_transfer_context_bus_init.argtypes=[ctypes.POINTER(ContextMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus),ctypes.c_uint32,ctypes.c_uint32]
effects.LIB.sa_hardware_transfer_set_samples.argtypes=previous.effects.LIB.sa_hardware_transfer_set_samples.argtypes;effects.LIB.sa_hardware_transfer_set_samples.restype=ctypes.c_int
effects.LIB.sa_display_timing_set_samples.argtypes=previous.effects.LIB.sa_display_timing_set_samples.argtypes;effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;effects.LIB.sa_disc_queue_init.argtypes=previous.effects.LIB.sa_disc_queue_init.argtypes
class ContextReference(previous.HardwareReference):
 def __init__(self,ticks,capacity,c):super().__init__(ticks,capacity,c);self.transfer_registers=[x&0xffffffff for x in c.get('transfer_registers',[0]*48)]
 def __contains__(self,a):return context_address(a) or super().__contains__(a)
 def __getitem__(self,a):
  if not context_address(a):return super().__getitem__(a)
  value=self.transfer_registers[((a&0x1fffffff)-0x005f7800)//4];self.events.append((25,a,value,4));return value
 def __setitem__(self,a,value):
  if not context_address(a):return super().__setitem__(a,value)
  value&=0xffffffff;self.transfer_registers[((a&0x1fffffff)-0x005f7800)//4]=value;self.events.append((26,a,value,4))
raw_reference=namespace['reference_run']
def reference_run(memory,state,f,c,tracked):
 if not c.get('initialize_context'):return raw_reference(memory,state,f,c,tracked)
 original_entry=state.pc;state.pc=INIT;first=raw_reference(memory,state,f,dict(c,entry=INIT),tracked)
 if first[0]:return first
 rc,path,events,ml,gb,mh,hidden,live=first;state.pc=original_entry
 second=raw_reference(memory,state,f,dict(c,macl=ml,gbr=gb,mach=mh,other=hidden),tracked)
 return second[0],path+second[1],events+second[2],*second[3:]
effects.reference_run=reference_run;effects.sequence=False
initialized_fn=effects.LIB.sa_movie_context_initialized_connected;initialized_fn.argtypes=native_fn.argtypes;initialized_fn.restype=ctypes.c_int
def invoke(*args):return (initialized_fn if effects.sequence else native_fn)(*args)
effects.fn=invoke
compare_code=previous.compare_code.replace('HardwareMemory','ContextMemory').replace('HardwareReference(','ContextReference(').replace('sa_hardware_transfer_bus_init','sa_transfer_context_bus_init')
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8])))";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"\n  if target==BOOT:boundaries[-1]['transfer_context_root']=hex(int.from_bytes(bytes((ctypes.c_uint8*4).from_address(ctypes.addressof(mem.contents)+0x5fbb14)),'little'))")
marker="ctypes.byref(memory),view,ctypes.byref(bus),c.get('port_data',0),c.get('control_word',0))";assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,marker+"\n memory.transfer_registers[:]=c.get('transfer_registers',[0]*48)")
marker='memory.fifo_cursor,memory.fifo_count,memory.control_word))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'memory.fifo_cursor,memory.fifo_count,memory.control_word,tuple(memory.transfer_registers)))')
marker="  if c.get('reject_dma')==a:return";assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,"  if context_address(a) and c.get('context_write_status'):return c['context_write_status']\n"+marker)
marker=' return dict(fifo_cursor=';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker," assert list(memory.transfer_registers)==ref.transfer_registers\n return dict(transfer_registers=list(memory.transfer_registers),context_events=[list(e) for e in ref.events if e[0] in (25,26)],fifo_cursor=")
exec(compile(compare_code,'transfer_context_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace,ContextMemory=ContextMemory,ContextReference=ContextReference,context_address=context_address,BOOT=BOOT),local:={});_compare=local['compare']
def compare(m,s,f,c):effects.sequence=c.get('initialize_context',False);return _compare(m,s,f,c)
def prepare(ram,c):
 m,s,f=previous.prepare(ram,c)
 if c['entry'] not in NEW:return m,s,f
 if c['entry']==INIT:
  start=CROOT&0xffffff;m[start-16:start+0x150]=bytes([c.get('pattern',0xa5)])*0x160;store(m,0x8c5fbb14,0)
  if c.get('pool_full'):
   for i in range(1,256):store(m,0x8c892840+i*16+12,0x80000000)
 elif c['entry']==DESC:
  store(m,0x8c5fbb14,CROOT);store(m,CROOT,BASE);s.r[4]=CROOT+4+c.get('channel',0)*52
  for i in range(13):store(m,s.r[4]+i*4,0xa5a5a5a5)
  store(m,s.r[4],c.get('channel',0))
 elif c['entry']==WINDOW:s.r[4]=c['start'];s.r[5]=c['end']
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f
def cases():
 for mode,pattern,full in itertools.product((0,4),(0,0xa5,0xff),(False,True)):yield dict(entry=INIT,mode=mode,pattern=pattern,pool_full=full,transfer_registers=[0x13572468+i for i in range(48)])
 for mode,channel in itertools.product((0,4),range(4)):yield dict(entry=DESC,mode=mode,channel=channel,transfer_registers=[0xfedc0000+i for i in range(48)])
 rng=random.Random(WINDOW);pairs=[(0,0),(0xffffffff,0xffffffff),(0x0c000000,0x0cffffff),(0x80000000,0x7fffffff)]+[(rng.getrandbits(32),rng.getrandbits(32)) for _ in range(16)]
 for mode,pair in itertools.product((0,4),pairs):yield dict(entry=WINDOW,mode=mode,start=pair[0],end=pair[1])
 for entry in NEW:
  base=dict(entry=entry,mode=0,start=0x0c000000,end=0x0cffffff)
  yield dict(base,null_bus=True)
  for status in (-2,-3):yield dict(base,context_write_status=status)
 yield dict(entry=INIT,mode=0,bad_stack=True)
def semantics(m,a,before,fp,s,f,record,c):
 assert bytes(fp)==bytes(f)
 if record['status']:return 1
 assert s.r[15]==before.r[15] and s.pr==before.pr and s.r[0]==0;checks=4
 if c['entry']==INIT:
  assert load(a,0x8c5fbb14)==CROOT and load(a,CROOT)==BASE and load(a,CROOT+0xd4)==1 and load(a,0x8c7ac8c0)==0
  for i in range(4):
   address=CROOT+4+i*52;assert load(a,address)==i and load(a,address+0x18)==3
   assert all(load(a,address+off)==0 for off in (4,8,12,16,20,28,32,40,44,48));assert record['transfer_registers'][(i*32+0x14)//4]==0
  assert record['transfer_registers'][0xbc//4]==0x4659404f and load(a,0x8c5fbb10)==0x4659404f;checks+=14
 elif c['entry']==DESC:
  address=before.r[4];assert [load(a,address+off) for off in range(0,52,4)]==[c['channel'],0,0,0,0,0,3,0,0,0xa5a5a5a5,0,0,0]
  expected=c['transfer_registers'].copy();expected[(c['channel']*32+0x14)//4]=0;assert record['transfer_registers']==expected;checks+=2
 elif c['entry']==WINDOW:
  expected=(((c['start']&0x07f00000)>>20)<<8)|((c['end']&0x07f00000)>>20)|0x46590000
  assert record['transfer_registers'][0xbc//4]==expected and load(a,0x8c5fbb10)==expected;checks+=2
 return checks
def coverage_rows(coverage,edges):
 rows=[]
 for routine in manifest['routines']:
  body=PER_BODY[int(routine['address'],16)];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 return rows
def main():
 proofs=[MP,P/'analysis/movie_context_connection.json',P/'analysis/movie_hardware_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=INIT,mode=0),dict(entry=DESC,mode=4,channel=2,transfer_registers=[0]*48),dict(entry=WINDOW,mode=0,start=0x0c000000,end=0x0cffffff)]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path)
  for j,pc in enumerate(path):
   if pc not in ROWS:continue
   text=ROWS[pc];distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 rows=coverage_rows(coverage,edges)
 if '--smoke' in sys.argv:print('Original context smoke passed',len(records));return
 assert all(not row['missing_rows'] and not row['missing_edges'] for row in rows)
 files=[Path(__file__),Path(previous.__file__)]+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original 193-row context initializer, 99-row descriptor reset and 20-row window setter execute real memory fill and event registration. Complete machine/RAM/FPU/banked/special state, boundaries, raw input streams and channel-register storage match independent instructions. Previous hardware source is adapted only for the newly modeled valid register aperture.',limitations=['Register words are declared host storage; automatic DMA/IRQ/electrical behavior, audio, pixels and natural system entry are not modeled.','Direct event-table/stack/register fixtures are explicit; pool-full cases retain actual registration failures in the original initializer.','The two-stage initialization/startup sequence is a declared host orchestration of original bodies; it is not proof of the original reset-to-main call order. Whole game and all requested ports/settings remain unfinished.'],cases=records)
 (P/'analysis/movie_context_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
