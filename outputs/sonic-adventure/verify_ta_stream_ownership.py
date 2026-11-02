"""Exercise native owned pages, immutable copies and failure/retirement contracts."""
import ctypes,hashlib,json
from pathlib import Path
import ta_stream_native as n
U=n.U;V=n.V;Q=n.Q
READ=ctypes.CFUNCTYPE(ctypes.c_int,V,U,ctypes.c_uint,ctypes.POINTER(U))
WRITE=ctypes.CFUNCTYPE(ctypes.c_int,V,U,U,ctypes.c_uint)
CACHE=ctypes.CFUNCTYPE(ctypes.c_int,V,U,ctypes.c_uint)
class Bus(ctypes.Structure):_fields_=[('context',V),('read',READ),('write',WRITE),('cache',CACHE)]
class Event(ctypes.Structure):_fields_=[(x,U) for x in ('kind','address','value','width')]
class Vertex(ctypes.Structure):_fields_=[('ram',V),('qacr',U*2),('queue',U*16)]+[(x,U) for x in ('invalidates','prefetches','flushes','trace_count','trace_dropped')]+[('trace',Event*4096)]
n.declare(Bus)
ALLOC=ctypes.CFUNCTYPE(V,V,ctypes.c_size_t);FREE=ctypes.CFUNCTYPE(None,V,V)
n.LIB.sa_ta_stream_create.argtypes=[ALLOC,FREE,V];n.LIB.sa_ta_stream_create.restype=V
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def all_packets(handle,which=0):
 out=[]
 for i in range(n.LIB.sa_ta_stream_count(handle,which)):
  p=n.Packet();assert n.LIB.sa_ta_stream_packet(handle,which,i,ctypes.byref(p))==0;out.append((p.destination,p.queue,tuple(p.words)))
 return out
class Fixture:
 def __init__(self,custom=False):
  self.owned={};self.budget=None;self.reject=0;self.parent_writes=[];self.parent_caches=[]
  @ALLOC
  def allocate(ctx,size):
   if self.budget is not None:
    if self.budget==0:return None
    self.budget-=1
   raw=ctypes.create_string_buffer(size);address=ctypes.addressof(raw);self.owned[address]=raw;return address
  @FREE
  def release(ctx,address):assert address in self.owned;del self.owned[address]
  @READ
  def read(ctx,address,width,out):out[0]=0x12345678;return 0
  @WRITE
  def write(ctx,address,value,width):self.parent_writes.append((address,value,width));return self.reject
  @CACHE
  def cache(ctx,address,op):self.parent_caches.append((address,op));return self.reject
  self.callbacks=(allocate,release,read,write,cache)
  self.handle=n.LIB.sa_ta_stream_create(allocate,release,None) if custom else n.LIB.sa_ta_stream_create_win32();assert self.handle
  self.vertex=Vertex();self.vertex.qacr[:]=[16,16];self.parent=Bus(None,read,write,cache);self.bus=Bus();self.pending=U()
  assert n.LIB.sa_ta_stream_bind(self.handle,ctypes.byref(self.vertex),ctypes.byref(self.parent),ctypes.byref(self.bus),ctypes.byref(self.pending),None,None)==0
 def count(self,which=0):return n.LIB.sa_ta_stream_count(self.handle,which)
 def pages(self,which=0):return n.LIB.sa_ta_stream_pages(self.handle,which)
 def state(self):
  e=(U*9)();l=(Q*8)();assert n.LIB.sa_ta_stream_events(self.handle,e)==n.LIB.sa_ta_stream_lifecycle(self.handle,l)==0;return tuple(e),tuple(l)
 def sq(self,words,queue=0):
  self.vertex.queue[queue*8:queue*8+8]=words;return self.bus.cache(self.bus.context,0xe0000000+queue*32,1)
 def append(self,index):
  p=n.Packet(0x10000000,0,(U*8)(0x20000000,index,*([0]*6)));return n.LIB.sa_ta_stream_append(self.handle,ctypes.byref(p))
 def close(self):n.LIB.sa_ta_stream_destroy(self.handle);self.handle=None;assert not self.owned

def main():
 target=P/'analysis/ta_stream_ownership_verification.json';assert not target.exists();cases=[]
 f=Fixture(custom=True)
 try:
  expected=[]
  for i in range(195):
   words=(0x80000000 if i==0 else 0xe0000000,i,0,0,0,0,0,0);q=i%2
   assert f.sq(words,q)==0;expected.append((0x10000000+q*32,q,words))
   assert f.count()==i+1 and f.pages()==(i+64)//64
  assert all_packets(f.handle)==expected and f.state()[0]==(195,0,0,1,1,194,0,0,1)
  assert n.LIB.sa_ta_stream_snapshot(f.handle)==0 and all_packets(f.handle,2)==expected
  # External queue writes and later accepted packets cannot mutate the copy.
  for i in range(2):assert f.sq((0x20000000,i,0,0,0,0,0,0))==0
  before=all_packets(f.handle);assert len(before)==197 and all_packets(f.handle,2)==expected
  assert f.bus.write(f.bus.context,0x5f8144,0x80000000,4)==0
  assert not f.count() and not f.pages() and all_packets(f.handle,1)==before and all_packets(f.handle,2)==expected
  assert f.state()[1][:3]==(1,197,2)
  assert f.sq((0x80000000,0,0,0,0,0,0,0))==0
  assert f.bus.write(f.bus.context,0x5f8144,0x80000000,4)==0
  assert f.count(1)==1 and f.pages(1)==1 and all_packets(f.handle,2)==expected
  cases.append(dict(name='paged_SQ_order_snapshot_and_two_retirements',packets=197,pages=4,immutable_snapshot_packets=195,discarded_after_submission=2))
 finally:f.close()
 f=Fixture()
 try:
  header=(0x80000000,1,2,3,4,5,6,7)
  for lane,value in enumerate(header):
   assert f.bus.write(f.bus.context,0x10000100+lane*4,value,4)==0
   assert f.count()==int(lane==7)
   if lane<7:assert n.LIB.sa_ta_stream_snapshot(f.handle)==-6
  assert all_packets(f.handle)==[(0x10000100,2,header)]
  end=(0,9,8,7,6,5,4,3)
  for lane,value in enumerate(end):assert f.bus.write(f.bus.context,0x10000120+lane*4,value,4)==0
  assert all_packets(f.handle)[1]==(0x10000120,2,end) and f.pending.value==128
  out=U();assert f.bus.read(f.bus.context,0x5f6900,4,ctypes.byref(out))==0 and out.value==(0x12345678|128)
  assert f.bus.write(f.bus.context,0x5f6900,128,4)==0 and f.pending.value==0 and f.state()[0][1]==0
  cases.append(dict(name='partial_FIFO_packets_EOL_trailing_words_and_ack',packets=2,partial_snapshots_rejected=7))
 finally:f.close()
 f=Fixture(custom=True)
 try:
  for i in range(64):assert f.append(i)==0
  assert n.LIB.sa_ta_stream_snapshot(f.handle)==0;old=all_packets(f.handle,2)
  for i in range(64,128):assert f.append(i)==0
  before=all_packets(f.handle);allocations=len(f.owned);f.budget=0
  assert f.append(128)==-4 and all_packets(f.handle)==before and f.pages()==2
  f.budget=1
  assert n.LIB.sa_ta_stream_snapshot(f.handle)==-4 and all_packets(f.handle,2)==old and len(f.owned)==allocations
  f.budget=None;assert n.LIB.sa_ta_stream_snapshot(f.handle)==0 and all_packets(f.handle,2)==before
  cases.append(dict(name='allocation_failure_preserves_active_and_old_snapshot',active_packets=128,old_snapshot_packets=64,temporary_allocations_released=True))
 finally:f.close()
 f=Fixture()
 try:
  assert f.bus.write(f.bus.context,0x10000001,0,4)==-3 and not f.count()
  assert f.bus.write(f.bus.context,0x10000000,0,2)==-3 and not f.count()
  assert f.bus.write(f.bus.context,0x10000004,0,4)==-3 and not f.count()
  assert f.sq((0x80000000,0,0,0,0,0,0,0))==0
  before=(all_packets(f.handle),f.state());f.reject=-7
  assert f.bus.write(f.bus.context,0x5f8144,0x80000000,4)==-7 and (all_packets(f.handle),f.state())==before
  assert f.bus.write(f.bus.context,0x5f8144,0x80000000,2)==-3 and (all_packets(f.handle),f.state())==before
  f.reject=0;assert f.bus.cache(f.bus.context,0x8c000000,2)==0 and f.parent_caches==[(0x8c000000,2)]
  assert f.sq((0x87000000,0,0,0,0,0,0,0))==-2 and f.count()==2 and f.state()[0][7]==1
  cases.append(dict(name='invalid_FIFO_parent_failure_width_and_format_guards',accepted_unsupported_packet_retained=True))
 finally:f.close()
 files=[Path(__file__),P/'ta_stream_native.py',P/'build_model_ta_stream.ps1',*[P/'source'/n for n in ('ta_stream.c','ta_stream.h','ta_stream_win32.c')],n.DLL]
 report=dict(status='verified_native_paged_storage_snapshot_lifetime_and_failure_contracts',cases=cases,case_groups=len(cases),page_packets=64,legacy_fixed_collector_unchanged=True,synthetic_API_inputs=True,original_game_frame_qualified=False,allocator_callbacks_produce_no_game_results=True,all_custom_allocator_allocations_released=True,overall_completion_percent=10,goal_paused=False,sha256={str(x):sha(x) for x in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='sha256'},indent=2),flush=True)
if __name__=='__main__':main()
