import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
Read=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint)
Write=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint)
Writeback=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32)
class IO(ctypes.Structure):_fields_=[('user',ctypes.c_void_p),('read',Read),('write',Write)]
class Cache(ctypes.Structure):_fields_=[('registers',IO),('writeback',Writeback)]
def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();cases=0
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_cache_invalidate_io.dll')).sa_file_cache_invalidate_io
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(Cache),ctypes.c_void_p,Call]
 for length,alignment,bank,ccr,pattern in itertools.product([196608, 196609, 393216],[0,31],[0,0x200000],[0,0x20,0x80,0xa0],['outside','mixed']):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  trace=[];expected_trace=[];errors=[];reads=[];reference_reads=[]
  registers={0xff00001c:ccr}
  for offset in range(0,0x4000,32):registers[0xf4000000+offset]=0x0ca00000 if pattern=='mixed' and (offset//32)%3==0 else 0x01000000
  class Registers(dict):
   def __getitem__(self,a):reference_reads.append((a,4));return super().__getitem__(a)
  ref_registers=Registers(registers)
  @Read
  def read(ctx,a,n):
   reads.append((a,n))
   if a not in registers:errors.append(('read',a,n));return 0
   return registers[a]
  @Write
  def write(ctx,a,v,n):errors.append(('write',a,v,n))
  @Writeback
  def writeback(ctx,a):trace.append(a);return 0
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(('call',t));return -9
  cache=Cache(IO(None,read,write),writeback);f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.r[4]=0x8ca00000+alignment;s.r[5]=length;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5fc0d8
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,cache_invalidate=expected_trace.append,mmio=ref_registers)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache),None,call)==0 and not errors
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert trace==expected_trace
  assert reads==reference_reads
  assert len(reads)==1+(256 if ccr&0x20 else 512)
  assert reads[0]==(0xff00001c,4)
  if pattern=='outside':assert not trace
  if pattern=='mixed':assert trace
  if not ccr&0x80:
   half=0x1000 if ccr&0x20 else 0x2000
   offsets=list(range(0,half,32))+list(range(0x2000,0x2000+half,32))
   independent=[0x8ca00000+o for o in offsets if pattern=='mixed' and (o//32)%3==0]
   assert trace==independent
  cases+=1
 (ROOT/'outputs/sonic-adventure/analysis/file_cache_invalidate_large_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Full RAM/CPU/FPU, CCR/cache-tag read order and OCBI trace comparisons for large ranges across CCR mode bits 5/7, mixed/outside tags, offsets and FPU banks. Independent read-count, initial CCR read, empty/nonempty trace checks and complete invalidate address list in direct-tag mode. Synthetic tags model hardware reads; callbacks record events and do not implement physical cache or transfer completion.'),indent=2)+'\n')
 print(cases,'cache invalidate whole-cache comparisons passed')
if __name__=='__main__':main()
