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
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_cache_purge_io.dll')).sa_file_cache_purge_io
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(Cache),ctypes.c_void_p,Call]
 for length,alignment,bank in itertools.product([0,1,31,32,33,65535], [0,1,31],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  trace=[];expected_trace=[];errors=[]
  @Read
  def read(ctx,a,n):errors.append(('read',a,n));return 0
  @Write
  def write(ctx,a,v,n):errors.append(('write',a,v,n))
  @Writeback
  def writeback(ctx,a):trace.append(a);return 0
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(('call',t));return -9
  cache=Cache(IO(None,read,write),writeback);f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.r[4]=0x8ca00000+alignment;s.r[5]=length;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5fc336
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,cache_purge=expected_trace.append)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache),None,call)==0 and not errors
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert trace==expected_trace
  independent=[] if not length else [0x8ca00000+i*32 for i in range((alignment+length+31)//32)]
  assert trace==independent
  cases+=1
 (ROOT/'outputs/sonic-adventure/analysis/file_cache_purge_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Full RAM/CPU/FPU and explicit OCBP trace comparisons for zero/small-range paths below 65536 bytes. Independent 32-byte aligned range trace check across offsets and both FPU banks. Large cache-tag/CCR path not tested; callbacks record events, do not implement a physical cache.'),indent=2)+'\n')
 print(cases,'cache purge comparisons passed')
if __name__=='__main__':main()
