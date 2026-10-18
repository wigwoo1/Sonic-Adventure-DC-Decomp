import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_ready_io.dll'))
 fn=lib.sa_engine_transfer_ready_wait
 getter=lib.sa_engine_transfer_ready_get_io
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 getter.argtypes=fn.argtypes[:7]+[ctypes.POINTER(IO)]+fn.argtypes[7:]
 cases=0
 for delay,counter,background,bank in itertools.product([0,1,7],[0,0xffffffff],[0,0xffffffdf],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200)
  struct.pack_into('<I',ram,0xa0030c,counter)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  reads=[];reference_reads=[]
  @Read
  def read(ctx,a,n):
   reads.append((a,n))
   if a!=0xa05f688c or n!=4:errors.append((a,n));return 0
   return background|(0x20 if len(reads)<=delay else 0)
  @Write
  def write(ctx,a,v,n):errors.append(('write',a,v,n))
  io=IO(None,read,write)
  class Registers(dict):
   def __getitem__(self,a):
    reference_reads.append((a,4))
    return background|(0x20 if len(reference_reads)<=delay else 0)
  mmio=Registers({0xa05f688c:0})
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c10fb48:return getter(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10eee0;s.r[4]=0x8ca00000;s.r[5]=0x8ca00100
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,mmio=mmio)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==0 and struct.unpack_from('<I',native,0xa0030c)[0]==((counter+1)&0xffffffff)
  assert reads==reference_reads==[(0xa05f688c,4)]*(delay+1)
  cases+=1
 (out/'analysis/engine_transfer_ready_wait_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=32,scope='Full retail RAM/CPU/FPU comparisons for ready wait with native IO getter. Independent read count/address, cleared-bit return and counter wrap checks across finite delays/background flags/FPU banks. Hardware state transitions are synthetic fixtures. Permanently busy wait intentionally not run; actual transfer completion delivery remains absent.'),indent=2)+'\n')
 print(cases,'ready wait comparisons passed')
if __name__=='__main__':main()
