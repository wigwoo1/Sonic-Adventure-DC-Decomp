import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_ready_get.dll')).sa_engine_transfer_ready_get;fn.argtypes=abi
cases=0
for value,bank,dest in itertools.product([0,1,31,32,33,63,0x80000000,0xffffffff],[0,0x200000],[0x8ca20004,0x8ca20008,0x8ca2000c]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa20000:0xa20020]=bytes([0x77])*32;before=bytes(ram)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);reads=[];errors=[]
 @Read
 def read(ctx,a,n):reads.append((a,n));return value
 @Write
 def write(ctx,a,v,n):errors.append((a,v,n))
 @Call
 def call(ctx,a,m,f,sr,r,pr):errors.append(a);return -1
 io=IO(None,read,write)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=dest;s.r[15]=0x8cf00000;s.pc=0x8c10fb48;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f,mmio={0xa05f688c:value})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 expected=bytearray(before[0xa20000:0xa20020]);struct.pack_into('<I',expected,dest-0x8ca20000,int(not(value&32)))
 assert native[0xa20000:0xa20020]==expected and reads==[(0xa05f688c,4)] and not errors and r[0]==0
 cases+=1
scope='48 full retail RAM/CPU/FPU/SR/PR comparisons of seven-instruction transfer readiness getter10fb48 with controlled MMIO. Eight register values,both banks,three guarded output positions. Independent one32-bit read at a05f688c,inverted bit5 ready output,zero status,and32-byte output guard. No modeled calls;physical hardware/full game integration unverified.'
(p/'analysis/engine_transfer_ready_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'readiness comparisons passed')
