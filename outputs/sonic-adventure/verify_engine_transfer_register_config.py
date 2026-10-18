import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_register_config.dll')).sa_engine_transfer_register_config;fn.argtypes=abi
cases=0
values=[0,0xfffff,0x100000,0x7f00000,0x8000000,0xffffffff]
for first,second,bank in itertools.product(values,values,[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<3I',ram,0x5fbb0c,0x12345678,0xdeadbeef,0x87654321)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);writes=[];errors=[]
 @Read
 def read(ctx,a,n):errors.append(('read',a,n));return 0
 @Write
 def write(ctx,a,v,n):writes.append((a,v,n))
 @Call
 def call(ctx,a,m,f,sr,r,pr):errors.append(a);return -1
 io=IO(None,read,write);mmio={0xa05f78bc:0xdeadbeef}
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=first;s.r[5]=second;s.r[15]=0x8cf00000;s.pc=0x8c10ee64;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f,mmio=mmio)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 encoded=0x46590000|(((first>>20)&127)<<8)|((second>>20)&127)
 assert writes==[(0xa05f78bc,encoded,4)] and mmio[0xa05f78bc]==encoded and not errors
 assert struct.unpack_from('<3I',native,0x5fbb0c)==(0x12345678,encoded,0x87654321) and r[0]==0 and r[15]==0x8cf00000
 cases+=1
scope='72 full retail RAM/CPU/FPU/SR/PR comparisons of20-instruction transfer register configuration10ee64. Six values per input including mask edges/high bits,both banks. Independent seven-bit fields from input bits20..26 packed into bytes1/0 with46590000 prefix,exact one32-bit MMIO write at a05f78bc,and matching RAM shadow at5fbb10 with neighbor guards. No modeled calls. Controlled MMIO;physical hardware/full game integration unverified.'
(p/'analysis/engine_transfer_register_config_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'transfer register configuration comparisons passed')
