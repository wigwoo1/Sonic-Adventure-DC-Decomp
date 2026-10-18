import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_device_initialize.dll')).sa_engine_device_initialize;fn.argtypes=abi
targets=[0x8c653440,0x8c6656e2,0x8c6666c0,0x8c663140,0x8c651520,0x8c665a00,0x8c666706]
cases=0
for mode,result,bank in itertools.product([0,1,2,0xffffffff],[0,0x12345678],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(target,r,events):
  events.append((target,tuple(r[i] for i in range(4,8))))
  assert target in targets
  r[0]=result
 hooks={}
 for target in targets:
  def hook(m,r,sr,target=target):service(target,r,expected);return sr
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target not in hooks:errors.append(target);return -9
  service(target,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=mode;s.r[15]=0x8cf00000;s.pc=0x8c651384;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 assert [e[0] for e in actual]==targets
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<4I',native,0x8a31a4)==(0x100000,0,0,0)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':53,'entry':'0x8c651384','scope':'Exact retail CPU/RAM/FPU/SR/PR comparison; independent seven-service order, global initialization, return value and stack/callee-saved checks. Seven subordinate services modeled; no device hardware or startup order proof. Binary-decoded TSV verified through execution comparison.'}
(root/'analysis/engine_device_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'engine device initializer comparisons passed')
