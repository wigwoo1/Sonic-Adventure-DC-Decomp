import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/task_delete_native.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={};allocations=[]
 targets=[0x8c098a80]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
  allocations.append((r[4],r[5]))
  r[0]=0 if len(allocations)==variant else 0x8ca00800+(len(allocations)-1)*128
  for i in range(1,4):r[i]=0x12340000+i
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);actual.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);expected.append(trace);return sr
  return body
 for name,entry in [('sa_task_create_native',0x8c09852e)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product(range(5),range(16),range(8),[0,0x200000]):
   for mem in [ram,native]:
    mem[0xa00000:0xa02000]=bytes(8192);mem[0xefffc0:0xf00000]=bytes(64)
    mem[0xa00100:0xa00130]=bytes([0xa5])*48
    struct.pack_into('<I',mem,0x79f048,0x8ca00100 if alias&1 else 0)
    struct.pack_into('<I',mem,0xa00104,0)
    for i in range(4):mem[0xa00800+i*128:0xa00840+i*128]=bytes([0xa5])*64
    for i in range(4):struct.pack_into('<4I',mem,0xa007f0+i*128,0,0,0x8ca01800,0)
    struct.pack_into('<I',mem,0x79f03c,0)
    struct.pack_into('<I',mem,0xa00100,0x8ca00200 if alias&2 else 0)
    struct.pack_into('<I',mem,0xa00204,0xdeadbeef)
    for i in range(8):struct.pack_into('<I',mem,0x79f04c+i*4,0x8ca00300 if alias&4 else 0)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=stage;s.r[5]=0;s.r[6]=0x8c07d560;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   allocations.clear();reference(ram,s,{t:hook(t) for t in targets},f);ref_allocations=list(allocations);allocations.clear()
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   requests=[pair for bit,pair in [(2,(1,64)),(4,(2,28)),(1,(1,64)),(8,(1,16))] if stage&bit]
   completed=bool(alias&1) and not (variant and variant<=len(requests))
   assert r[0]==(0x8ca00100 if completed else 0)
   expected_requests=requests[:variant] if variant else requests
   assert allocations==ref_allocations==([(n*size,size) for n,size in expected_requests] if alias&1 else [])
   successful=max(0,len(allocations)-(1 if variant and variant<=len(requests) else 0))
   for i in range(successful):
    size=requests[i][0]*requests[i][1]
    assert native[0xa00800+i*128:0xa00800+i*128+size]==bytes(size)
   if completed:
    assert struct.unpack_from('<I',native,0xa00110)[0]==0x8c07d560
    for i,off in enumerate([off for bit,off in [(2,32),(4,40),(1,36),(8,44)] if stage&bit]):
     assert struct.unpack_from('<I',native,0xa00100+off)[0]==0x8ca00800+i*128
   elif alias&1:
    assert struct.unpack_from('<I',native,0x79f048)[0]==0x8ca00100
    assert struct.unpack_from('<I',native,0x79f04c)[0]==(0x8ca00300 if alias&4 else 0)
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=162,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in ['sa_task_create','sa_task_create_base','sa_heap_allocate_zeroed','task_delete_native']},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Higher-level constructor with native base and native deletion/heap release on allocation failure. All16 allocation flag combinations, failures at each1..4th allocation, exhausted/single/multiple free list, active list empty/nonempty and both banks. Full-state retail comparisons with independent allocation order/sizes, return, installed pointers and failure list restoration assertions. Allocation service remains injected; no playable engine yet.')
 (out/'analysis/task_create_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'native task creation comparisons passed')
if __name__=='__main__':main()
