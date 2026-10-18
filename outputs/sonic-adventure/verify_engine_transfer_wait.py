import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_wait.dll')).sa_engine_transfer_wait;fn.argtypes=abi
cases=0
for sequence,counter,bank in itertools.product([(1,),(0,1),(2,0,1),(0xffffffff,2,0,1)],[0,1,0x7fffffff,0xffffffff],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x5fbb14,0x8ca08000);struct.pack_into('<3I',ram,0xa08108,0x12345678,counter,0x87654321)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10fb48 and len(actual)<len(sequence)
  value=sequence[len(actual)];actual.append(r[4]);struct.pack_into('<I',native,r[4]&0xffffff,value);r[0]=0xdeadbeef;return 0
 def status(mem,r,sr):
  assert len(expected)<len(sequence)
  value=sequence[len(expected)];expected.append(r[4]);struct.pack_into('<I',mem,r[4]&0xffffff,value);r[0]=0xdeadbeef;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c10eee0;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c10fb48:status},fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert actual==expected==[0x8ceffff0]*len(sequence)
 assert struct.unpack_from('<3I',native,0xa08108)==(0x12345678,(counter+1)&0xffffffff,0x87654321) and r[0]==0 and r[15]==0x8cf00000
 cases+=1
scope='32 full retail RAM/CPU/FPU/SR/PR comparisons of transfer wait10eee0. Four completion sequences including immediate,zero,non-one positive and negative status;four counters including wrap,both banks. Only status getter10fb48 modeled. Independent exact polling count,stack status output,one context counter increment at+10c,neighbor guards,ignored service return,status0 and stack restoration. Nonterminating device status and physical hardware/game integration unverified.'
(p/'analysis/engine_transfer_wait_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'transfer wait comparisons passed')
