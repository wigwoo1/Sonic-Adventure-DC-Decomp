import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_wait.dll')).sa_engine_transfer_wait;fn.argtypes=abi
ready=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_ready_get.dll')).sa_engine_transfer_ready_get;ready.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
cases=0
for sequence,counter,bank in itertools.product([(0,),(32,0),(0xffffffff,33,31),(0x80000020,63,32,0x80000000)],[0,1,0x7fffffff,0xffffffff],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x5fbb14,0x8ca08000);struct.pack_into('<3I',ram,0xa08108,0x12345678,counter,0x87654321)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 reads=[];errors=[]
 @Read
 def read(ctx,a,n):
  assert a==0xa05f688c and n==4 and len(reads)<len(sequence)
  value=sequence[len(reads)];reads.append((a,n));return value
 @Write
 def write(ctx,a,v,n):errors.append((a,v,n))
 io=IO(None,read,write)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10fb48
  actual.append(r[4]);return ready(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
 class Registers(dict):
  def __init__(self):super().__init__({0xa05f688c:0});self.reads=[]
  def __getitem__(self,a):
   assert a==0xa05f688c and len(self.reads)<len(sequence)
   value=sequence[len(self.reads)];self.reads.append(a);return value
 mmio=Registers()
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c10eee0;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert actual==[0x8ceffff0]*len(sequence) and reads==[(0xa05f688c,4)]*len(sequence) and mmio.reads==[0xa05f688c]*len(sequence) and not errors
 assert struct.unpack_from('<3I',native,0xa08108)==(0x12345678,(counter+1)&0xffffffff,0x87654321) and r[0]==0 and r[15]==0x8cf00000
 cases+=1
scope='32 full retail RAM/CPU/FPU/SR/PR comparisons of native transfer wait10eee0 connected with native readiness getter10fb48. Four controlled MMIO register sequences,immediate and delayed readiness,unrelated high bits,four counters including overflow,both banks. Independent exact read order/count at a05f688c,width4,stack status pointer,inverted hardware bit5,one context counter increment+10c,neighbor guards,status0 and stack restoration. No modeled calls. Nonterminating devices and physical hardware/full game integration unverified.'
(p/'analysis/engine_transfer_wait_native_ready_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'transfer wait comparisons passed')
