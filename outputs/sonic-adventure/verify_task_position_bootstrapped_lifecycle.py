import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram)
 view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll'
 lib=ctypes.CDLL(str(dll));fn=lib.sa_task_update_list_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 create=lib.sa_position_task_create_native;create.argtypes=fn.argtypes
 pool_initialize=lib.sa_task_pool_initialize_native;pool_initialize.argtypes=fn.argtypes
 initialize=lib.sa_heap_initialize;initialize.argtypes=fn.argtypes
 errors=[]
 @Call
 def external(ctx,target,m,f,sr,r,pr):errors.append(hex(target));return -9
 count=0;create_count=0
 for level,length,timer,bank in itertools.product([0],[1,4],[120],range(2)):
  pool=[0x8ca00100+i*128 for i in range(length)]
  for mem in (ram,native):
   mem[0xa00000:0xa05000]=bytes([0xa5])*20480;mem[0xeffe00:0xf00000]=bytes(512)
   for i in range(8):struct.pack_into('<I',mem,0x79f04c+i*4,0)
   struct.pack_into('<I',mem,0x79f048,pool[0] if pool else 0)
   for i,task in enumerate(pool):
    struct.pack_into('<I',mem,0x78c668+i*4,0x8ca01400+i*32)
    struct.pack_into('<H',mem,0xa01406+i*32,0x1234)
  f=Fpu();f.fpscr=0x4007c|(bank<<21);f.fpul=0x12345678
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+j for j in range(16)]
  s.r[15]=0x8cf00000;s.pc=0x8c098a10;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
  reference(ram,s,{},f)
  assert initialize(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,external)==0
  nf.fpul=fpul.value
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native
  assert struct.unpack_from('<I',native,0x79f03c)[0]==0x8c7ee8e0
  f=Fpu();f.fpscr=0x4007c|(bank<<21);f.fpul=0x12345678
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+j for j in range(16)]
  s.r[15]=0x8cf00000;s.pc=0x8c09844a;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
  reference(ram,s,{},f)
  assert pool_initialize(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,external)==0
  nf.fpul=fpul.value
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native
  assert struct.unpack_from('<I',native,0x79f044)[0]==0x8c7ee8f0
  for i in range(4096):
   address=0x8c7ee8f0+i*48
   assert struct.unpack_from('<12I',native,address&0xffffff)==(address+48 if i<4095 else 0,address-48 if i else 0,0,0,0,0,0,0,0,0,0,0)
  pool=[0x8c7ee8f0+i*48 for i in range(length)]
  # Retail position-task creator fixes priority0. Scheduler tests other priority inputs on the earlier fixture suite.
  for i,task in enumerate(pool):
   f=Fpu();f.fpscr=0x4007c|(bank<<21);f.fpul=0x12345678
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+j for j in range(16)]
   s.r[4]=i;s.r[15]=0x8cf00000;s.pc=0x8c07d5aa;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
   reference(ram,s,{},f)
   assert create(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,external)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native
   pointer=struct.unpack_from('<I',native,(task&0xffffff)+32)[0]
   assert pointer==0x8c81e900+i*80
   assert native[(pointer&0xffffff)+8]==i
   for mem in (ram,native):struct.pack_into('<H',mem,(pointer&0xffffff)+6,timer)
   create_count+=1
  tasks=list(reversed(pool));payloads=[0x8c81e900+i*80 for i in range(length)]
  for tick in range(5):
   f=Fpu();f.fpscr=0x4007c|(bank<<21);f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x3f800000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)]
   s.r[4]=0;s.r[15]=0x8cf00000;s.pc=0x8c09878c;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
   reference(ram,s,{},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,external)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(level,length,timer,bank,tick)
   # A callback exceeding120 schedules deletion; deletion runs on the following update.
   pending_tick=next((k for k in range(5) if ((timer+k+1)&65535)>120),None)
   removed=pending_tick is not None and tick>pending_tick
   head=struct.unpack_from('<I',native,0x79f04c)[0]
   assert head==(0 if removed or not tasks else tasks[0])
   if removed:
    assert struct.unpack_from('<I',native,0x79f048)[0]==(tasks[-1] if tasks else 0)
    assert struct.unpack_from('<I',native,0x79f03c)[0]==0x8c81e8f0
    assert struct.unpack_from('<4I',native,0x81e8f0)==(0x8c87e8e0,0,0x8c87e8e0,0x8c7ee8e0)
    for j in range(4096):
     address=0x8c7ee8f0+j*48
     assert struct.unpack_from('<12I',native,address&0xffffff)==(address+48 if j<4095 else 0,address-48 if j else 0,0,0,0,0,0,0,0,0,0,0)
    for task in tasks:assert struct.unpack_from('<I',native,(task&0xffffff)+32)[0]==0
   for i in range(length):
    flag=struct.unpack_from('<H',native,0xa01406+i*32)[0]
    assert flag==(0x1234 if pending_tick is not None and tick>=pending_tick else 0x9234)
   count+=1
 assert not errors,errors
 sources=json.loads((out/'analysis/task_delete_native_verification.json').read_text())['source_sha256']
 names=[n+'.c' for n in sources]+['sa_task_update_list.c','sa_position_task_callback.c','sa_task_remove_request.c','sa_task_create.c','sa_task_create_base.c','sa_heap_allocate.c','sa_heap_allocate_zeroed.c','sa_position_task_create.c','sa_heap_initialize.c','sa_task_pool_initialize.c']
 report=dict(cases_passed=count,creation_comparisons=create_count,fixture_count=count//5,updates_per_fixture=5,binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},scope='Original heap and4096-record task pool initialization then position task creation through native task pool/core heap allocation/zeroing, then5 scheduler updates through callback and deferred deletion/release/coalescing.1/4 tasks,timer120,both banks, fixed priority0. No service hooks. Full-state retail comparisons plus independent payload IDs/layout, threshold/deletion, task/heap heads and flag assertions. Other engine tasks and playable port remain incomplete.')
 (out/'analysis/task_position_bootstrapped_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native position task lifecycle comparisons passed')
if __name__=='__main__':main()
