import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
clock=lib.sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
elapsed=lib.sa_file_elapsed_get;elapsed.argtypes=abi
@Call
def call(*args):return -9
values=[0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff]
cases=0
for pc,a,b,bank in [(0x8c60260e,a,0,bank) for a,bank in itertools.product(values,[0,0x200000])]+[(0x8c602618,a,b,bank) for a,b,bank in itertools.product(values,values,[0,0x200000])]:
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 reads=[];errors=[]
 @Read
 def read(ctx,address,n):
  reads.append((address,n))
  if address!=0xffd8000c or n!=4:errors.append((address,n))
  return a
 @Write
 def write(*args):errors.append('unexpected write')
 io=IO(None,read,write)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=a;s.r[5]=b;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=pc
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,special=special,mmio={0xffd8000c:a})
 args=[view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl)]
 if pc==0x8c60260e:args.append(ctypes.byref(io))
 assert (clock if pc==0x8c60260e else elapsed)(*args,None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert r[0]==((~a if pc==0x8c60260e else b-a)&0xffffffff)
 assert reads==([(0xffd8000c,4)] if pc==0x8c60260e else [])
 cases+=1
(root/'analysis/file_timer_helpers_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native clock with explicit modeled timer register and native elapsed helper against full retail RAM/CPU/FPU/SR/PR/MACL; independent complement/subtraction modulo32 assertions, signed boundaries and wraparound, both FPU banks. Static timer inputs; dynamic waits not verified.'},indent=2)+'\n')
print(cases,'file timer helper comparisons passed')
