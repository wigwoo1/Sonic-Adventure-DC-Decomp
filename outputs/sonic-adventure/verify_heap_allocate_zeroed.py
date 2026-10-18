import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/heap_allocate_zeroed.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[0x8c098a80]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
  r[0]=0x8ca00300 if alias else 0
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
 for name,entry in [('sa_heap_allocate_zeroed',0x8c098b0c)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product([0,1,2,4,0x40000000,0x80000000,0xffffffff],[0,1,2,3,4,7,8,15,16,63,64,0xfffffffd,0xfffffffe,0xffffffff],range(2),[0,0x200000]):
   size=(variant*((stage+3)&0xfffffffc))&0xffffffff
   # Successful very large allocations would intentionally clear huge memory; failures still verify all arithmetic.
   if alias and size>512:continue
   for mem in [ram,native]:
    mem[0xa00000:0xa01000]=bytes([0xa5])*4096;mem[0xefffc0:0xf00000]=bytes(64)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=variant;s.r[5]=stage;s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   assert len(actual)==1 and actual[0][1][0]==size
   assert macl.value==size
   assert r[0]==(0x8ca00300 if alias else 0)
   assert native[0xa00300:0xa00300+size]==bytes(size) if alias else native[0xa00300:0xa00310]==bytes([0xa5])*16
   if alias:assert native[0xa002ff]==0xa5 and native[0xa00300+size]==0xa5
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=27,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in counts},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Whole zeroed allocation wrapper: round element size up to4 bytes,32-bit product wraparound, allocation null/success, zero-length and word clearing. Full-state retail comparisons with independent size/MACL/zeroing/neighbor assertions. Core allocator injected. Large successful memory clears excluded; failed allocation arithmetic still covered.')
 (out/'analysis/heap_allocate_zeroed_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'zeroed heap allocation comparisons passed')
if __name__=='__main__':main()
