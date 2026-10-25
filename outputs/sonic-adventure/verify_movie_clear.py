"""Full original clear: every native call and independently decoded instruction."""
import collections,csv,ctypes,hashlib,json,random,sys,types
from pathlib import Path
import verify_movie_driver as old
import verify_movie_hardware as hardware
P=old.P;ROOT=old.ROOT;LOOP=0x8c66598c;END=0x8c66599c;CLEAR=0x8c6656e2;COPY=0x8c65253e;WORDS=0x200000
class Stats(ctypes.Structure):
 _fields_=[(n,ctypes.c_uint64) for n in ('calls','writes','attempts','cpu_hash','write_hash','steps','coverage','fpu_errors','code_errors')]+[('first',ctypes.c_uint32*16),('last',ctypes.c_uint32*16)]+[(n,ctypes.c_uint32) for n in ('first_sr','last_sr','first_pr','last_pr')]
class Events:
 """Exact logical trace, compactly storing the constant sequential clear span."""
 def __init__(self):self.parts=[];self.count=0
 def append(self,event):self.parts.append(event);self.count+=1
 def add_clear(self,start,count,value):
  if count:self.parts.append(('clear',start,count,value));self.count+=count
 def __len__(self):return self.count
 def __iter__(self):
  for p in self.parts:
   if p[0]=='clear':
    for i in range(p[2]):yield (14,p[1]+i*4,p[3],4)
   else:yield p
 def __getitem__(self,index):
  assert isinstance(index,slice) and index.start is None and index.step is None
  import itertools
  return list(itertools.islice(iter(self),index.stop))
 def retained(self):
  for p in self.parts:
   if p[0]=='clear':
    for i in range(min(p[2],8)):yield (14,p[1]+i*4,p[3],4)
   else:yield p
def stats_dict(s):return {n:list(getattr(s,n)) if n in ('first','last') else getattr(s,n) for n,_ in s._fields_}
effects=types.SimpleNamespace(**old.effects.__dict__);effects.DLL=ROOT/'work/sonic-adventure/build/movie_clear.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
effects.LIB.sa_clear_reference.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.State),ctypes.POINTER(Stats),ctypes.c_uint32];effects.LIB.sa_clear_reference.restype=ctypes.c_int
observed=effects.LIB.sa_movie_clear_observed;observed.argtypes=old.native_fn.argtypes+[ctypes.POINTER(Stats),ctypes.c_uint32];observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=Stats();failure=0xffffffff
def invoke(*args):return observed(*args,ctypes.byref(native_stats),failure)
effects.fn=invoke
class Reference(old.DriverReference):
 def __init__(self,ticks,capacity,c):
  super().__init__(ticks,capacity,c);self.events=Events();self.clear_stats=Stats();self.clear_stats.cpu_hash=self.clear_stats.write_hash=14695981039346656037
  if c.get('seed_vram',True):self.vram[:]=bytes(range(256))*(len(self.vram)//256)
def run_segment(memory,regs,sr,state,c,ref):
 local=effects.State();local.r[:]=regs;local.sr=sr;local.pr=state.pr;local.pc=LOOP
 ram=(ctypes.c_uint8*len(memory)).from_buffer(memory);vram=(ctypes.c_uint8*len(ref.vram)).from_buffer(ref.vram)
 before=hashlib.sha256(memory).digest();start=local.r[14];value=old.load(memory,local.r[15]);stats=Stats()
 rc=effects.LIB.sa_clear_reference(ram,vram,ctypes.byref(local),ctypes.byref(stats),c.get('fail_clear_after',0xffffffff))
 assert hashlib.sha256(memory).digest()==before and not stats.code_errors and stats.steps==stats.calls*16-(0 if not rc else 8)
 ref.events.add_clear(start,stats.writes,value);ref.clear_stats=stats
 for i,value in enumerate(local.r):regs[i]=value
 return rc,local
# Only the verification executor's hook contract is extended. All old files and
# default hook behavior stay intact; the decoder actually runs every iteration.
assert old.namespace['reference'] is hardware.cpu_ns['reference']
cpu_code=hardware.cpu_code
marker='            sr = hooks[pc](ram, regs, sr); pc = state.pr';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,"            hook_result=hooks[pc](ram,regs,sr)\n            if isinstance(hook_result,tuple):sr,pc,state.pr=hook_result\n            else:sr=hook_result;pc=state.pr")
cpu_ns=dict(hardware.cpu_ns);exec(compile(cpu_code,'full_clear_reference_executor','exec'),cpu_ns)
namespace=dict(old.namespace,reference=cpu_ns['reference'],GUARDS={k:v for k,v in old.GUARDS.items() if k!=0x8c665998},run_segment=run_segment)
code=old.code;marker=' hooked={BIOS:firmware_hook}';assert code.count(marker)==1
code=code.replace(marker,marker+"\n def clear_hook(ram,regs,sr):\n  rc,result=run_segment(memory,regs,sr,state,c,queue_reference)\n  if rc:\n   state.pr=result.pr;stop(result.pc,regs,result.sr,rc)\n  return result.sr,result.pc,result.pr\n hooked[0x8c66598c]=clear_hook")
exec(compile(code,'full_clear_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects.reference_run=reference_run;effects.namespace=namespace
compare_code=old.compare_code.replace('DriverReference','Reference')
marker=' bus.write=write';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," if any(k in c for k in ('reject_bus','ccr_write_status','context_write_status','reject_dma')):bus.write=write")
marker=" memory.normal_status=c.get";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," if c.get('seed_vram',True):ctypes.memmove(memory.vram,bytes(range(256))*(ctypes.sizeof(memory.vram)//256),ctypes.sizeof(memory.vram))\n"+marker)
compare_code=compare_code.replace('for e in ref.events if','for e in ref.events.retained() if').replace('for x in ref.events if','for x in ref.events.retained() if')
marker=' return dict(dma_operation=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert native_stats.calls==ref.clear_stats.calls and native_stats.writes==ref.clear_stats.writes and native_stats.attempts==ref.clear_stats.attempts\n for field in ('cpu_hash','write_hash','first','last','first_sr','last_sr','first_pr','last_pr'):\n  left=getattr(native_stats,field);right=getattr(ref.clear_stats,field)\n  assert (list(left)==list(right) if field in ('first','last') else left==right),(field,stats_dict(native_stats),stats_dict(ref.clear_stats))\n assert native_stats.code_errors==native_stats.fpu_errors==0\n return dict(clear_native=stats_dict(native_stats),clear_reference=stats_dict(ref.clear_stats),full_vram_bytes_compared=ctypes.sizeof(memory.vram),vram_write_sample_is_truncated=True,dma_operation=")
exec(compile(compare_code,'full_clear_comparison','exec'),dict(old._compare.__globals__,effects=effects,namespace=namespace,Reference=Reference,native_stats=native_stats,stats_dict=stats_dict),local:={});_compare=local['compare']
def compare(m,s,f,c):
 global failure
 failure=c.get('fail_clear_after',0xffffffff);return _compare(m,s,f,c)
def fold(h,value):return ((h^value)*1099511628211)&0xffffffffffffffff
def calibrate(ram):
 records=[];rng=random.Random(66598)
 for count in (1,2,3,17,257,4096):
  m=bytearray(ram);state=effects.State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[11]=COPY;state.r[12]=count;state.r[13]=0;state.r[14]=0xa5000000;state.r[15]=0x8ce80000;state.sr=rng.getrandbits(32);state.pr=0x8cfed000;state.pc=LOOP;old.store(m,state.r[15],rng.getrandbits(32));other=effects.State.from_buffer_copy(state)
  class Memory:
   def __init__(self):self.data=bytearray(0x800000);self.writes=[]
   def __contains__(self,a):return 0xa5000000<=a<0xa5800000
   def __setitem__(self,a,value):self.data[a-0xa5000000:a-0xa5000000+4]=value.to_bytes(4,'little');self.writes.append((a,value))
  mmio=Memory();seen=[]
  def trace(pc,word,r,sr,pr,fp):
   if pc==COPY:seen.append((tuple(r),sr,pr))
  steps=old.namespace['reference'](m,other,mmio=mmio,trace=trace,stop_pc=END,instruction_budget=100000,special=dict(other=[rng.getrandbits(32) for _ in range(8)],mach=0,macl=0,gbr=0,vbr=0x8ce50000))
  expected_ram=bytes(m);vram=bytearray(0x800000);stats=Stats()
  rc=effects.LIB.sa_clear_reference((ctypes.c_uint8*len(m)).from_buffer(m),(ctypes.c_uint8*len(vram)).from_buffer(vram),ctypes.byref(state),ctypes.byref(stats),0xffffffff)
  h=w=14695981039346656037
  for r,sr,pr in seen:
   for value in (*r,sr,pr):h=fold(h,value)
  for a,value in mmio.writes:
   for value in (a,value,4):w=fold(w,value)
  assert rc==0 and bytes(state)==bytes(other) and bytes(m)==expected_ram and vram==mmio.data
  assert stats.cpu_hash==h and stats.write_hash==w and stats.calls==stats.writes==stats.attempts==count and stats.steps==steps==16*count and stats.coverage==0xffff
  records.append(dict(iterations=count,instructions=steps,cpu_and_memory_match=True,ordered_cpu_and_write_hashes_match=True))
 return records
def cases():
 base=dict(entry=CLEAR,mode=0,render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},irq_samples=[0,8],fifo_samples=[0]*32,scan_samples=[0,1]*32)
 for mode,chip in ((0,0),(1,1),(0,2),(0,3),(0,4),(0,5),(0,6),(0,8),(0,9)):yield base|dict(mode=mode,startup_overrides={0x5f689c:chip})
 yield base|dict(ram_words={0x8c8a2d74:1})
 for limit in (0,1,WORDS//2,WORDS-1):yield base|dict(fail_clear_after=limit)
 for rev in (1,2):yield base|dict(render_seed={0x5f8000:0x17fd11db,0x5f8004:rev})
 yield base|dict(render_seed={0x5f8000:0,0x5f8004:0x11})
 yield base|dict(entry=old.MODE,cold_library=True)
 yield base|dict(entry=old.old.ENTRY,cold_library=True)
def main():
 ram=old.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();calibration=calibrate(ram);records=[];coverage=set();edges=set()
 checked=[]
 for path in (P/'analysis/movie-driver-services/8c6656e2_instructions.tsv',P/'analysis/boot-device/8c65253e_instructions.tsv'):
  for row in csv.DictReader(path.open(),delimiter='\t'):
   a=int(row['address'],16)
   if LOOP<=a<END or COPY<=a<0x8c65254e:
    assert ram[a&0xffffff:(a&0xffffff)+2]==bytes.fromhex(row['bytes']);checked.append(hex(a))
 assert len(checked)==16 and old.load(ram,0x8c6659f8)==WORDS and old.load(ram,0x8c6659fc)==0xa5000000 and old.load(ram,0x8c6659f4)==COPY
 for c in cases():
  m,s,f=old.prepare(ram,c);initial_sp=s.r[15];r,path,a,s,f=compare(m,s,f,c)
  if c['entry']==CLEAR:
   if c.get('render_seed')!={0x5f8000:0x17fd11db,0x5f8004:0x11}:assert r['status']==-4 and r['stop_pc']=='0x8c665968' and r['clear_native']['calls']==0
   elif 'fail_clear_after' in c:
    limit=c['fail_clear_after'];assert r['status']==-3 and r['stop_pc']=='0x8c652544' and r['clear_native']['writes']==limit and r['clear_native']['calls']==limit+1
   else:
    assert r['status']==0 and s.r[0]==1 and r['clear_native']['calls']==r['clear_native']['writes']==WORDS and s.r[15]==initial_sp
    assert r['vram_sha256']==hashlib.sha256(bytes(WORDS*4)).hexdigest() and r['clear_reference']['steps']==16*WORDS
  if r['clear_reference']['calls']:
   coverage.update(pc for bit,pc in enumerate(list(range(LOOP,END,2))+list(range(COPY,0x8c65254e,2))) if r['clear_reference']['coverage']&(1<<bit));edges.add((0x8c665998,LOOP))
   if r['status']!=-3:edges.add((0x8c665998,END))
  coverage.update(path)
  for j,pc in enumerate(path):
   text=old.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(r);print(hex(c['entry']),r['status'],r['stop_pc'],'clear calls',r['clear_native']['calls'],'writes',r['clear_native']['writes'],flush=True)
 rows=[]
 for routine in old.manifest['routines']:
  body=set(old.PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),Path(hardware.__file__),P/'analysis/movie_clear_translation.json',P/'analysis/movie_clear_connection.json',P/'source/movie_clear_probe.c',P/'source/movie_clear_probe.h']
 report=dict(cases_passed=len(records),calibration=calibration,retail_loop_and_helper_rows_checked=checked,retail_literal_words_checked=3,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,full_clear_word_count=WORDS,full_clear_bytes=WORDS*4,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Every native helper call and write runs. Separate verification-only C decodes every original loop/helper instruction, calibrated against the independent Python executor. Prefix/suffix full-state and final 16 MiB RAM/8 MiB VRAM comparisons remain. Ordered inner calls/writes use streaming fingerprints plus exact counts, endpoints and invariants; fingerprints alone are not collision-free proofs. No physical DMA, IRQ timing or pixels are established.')
 (P/'analysis/movie_clear_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Full-clear comparisons',len(records),report['status_counts']);print(next(r for r in rows if r['address']==hex(CLEAR)))
if __name__=='__main__':main()
