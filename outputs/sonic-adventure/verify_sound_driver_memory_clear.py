import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_memory_clear.dll'))
fn=lib.sa_sound_driver_memory_clear;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
cases=0
for result,bank,mask in itertools.product([0,3,0x0d000001,0xffffffff],[0,0x200000],[0,7,15]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(t,m,r,events):
  stack=tuple(sum(m[(r[15]&0xffffff)+j+i]<<(8*i) for i in range(4)) for j in (0,4)) if t==0x8c10d770 else ()
  events.append((t,tuple(r[i] for i in range(4,8)),stack));r[0]=result
 hooks={}
 for t in [0x8c65a200,0x8c10d770]:
  def hook(m,r,sr,t=t):service(t,m,r,expected);return sr
  hooks[t]=hook
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t not in hooks:errors.append(t);return -9
  service(t,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65a8c8;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,hooks=hooks,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 assert r[0]==0 and r[14]==0x2234000e and r[15]==0x8cf00000
 assert actual[0][0]==0x8c65a200
 assert actual[1]==(0x8c10d770,(0x8c67c1c4,0xa0800000,4,0x80000),(0,1))
 cases+=1
report={'cases_passed':cases,'instructions_recovered':18,'scope':'Full original RAM/CPU/SR/PR/FPU/events equality, four subordinate returns/both FPU banks/three interrupt masks; independent stop then strided clear(source67c1c4,destinationa0800000,bytes4,stride80000,stack0/1), return0 and stack/R14 preserved. Stop and transfer modeled; actual sound RAM clearing unverified.'}
(out/'analysis/sound_driver_memory_clear_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'sound memory clear comparisons passed')
