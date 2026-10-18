import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_mode_initialize.dll')).sa_engine_mode_initialize;fn.argtypes=abi
cases=0
for mode,option,result,bank in itertools.product([0xffffffff,0,1,2,3,7],[0,1,0xffffffff],[0,0x12345678],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def service(target,r,events):
  events.append((target,tuple(r[i] for i in range(4,8))))
  if target==0x8c6510a4:r[0]=0xabcdef01
  elif target==0x8c665a80:r[0]=option
  elif target==0x8c651384:r[0]=result
  else:raise AssertionError(hex(target))
 hooks={}
 for target in [0x8c6510a4,0x8c665a80,0x8c651384]:
  def hook(m,r,sr,target=target):service(target,r,expected);return sr
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,sr,r,pr):
  if target not in hooks:errors.append(target);return -9
  service(target,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=mode;s.r[15]=0x8cf00000;s.pc=0x8c65262c;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 valid=mode in [0,1,2]
 assert [e[0] for e in actual]==([0x8c6510a4,0x8c665a80,0x8c651384] if valid else [0x8c6510a4])
 assert r[0]==(0 if valid else 2)
 assert r[14]==0x2234000e and r[15]==0x8cf00000
 if valid:
  assert actual[1][1][:3]==(0x8c671b2c,0x8c671b34,mode&3)
  assert actual[2][1][0]==0x8c671b2c
  assert struct.unpack_from('<I',native,0x8a2d74)[0]==option
 cases+=1
report={'cases_passed':cases,'instructions_recovered':28,'entry':'0x8c65262c','scope':'Retail CPU/RAM/FPU/SR/PR comparison, mode0/1/2 and invalid signed modes. Independent unconditional engine-init ordering, valid option/device dispatch, stored selection, return0/2 and callee-saved/stack checks. Engine initializer, option selector and device initializer modeled; boot order not established.'}
(root/'analysis/engine_mode_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'engine mode initializer comparisons passed')

