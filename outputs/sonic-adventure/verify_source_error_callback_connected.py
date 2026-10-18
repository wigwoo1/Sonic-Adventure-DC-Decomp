import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def load(stem):
 f=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);f.argtypes=abi;return f
fn=load('sound_source_contiguous_check');helpers={0x8c65e368:load('sound_source_lock_enter'),0x8c65e380:load('sound_source_lock_exit'),0x8c65e44a:load('sound_source_identity_get'),0x8c65e44e:load('sound_source_error_callback_set')}
def signed(v):return ctypes.c_int32(v&0xffffffff).value
cases=0
for mode,fixture,request,mirror,callback,bank in itertools.product([0,1,2,0xffffffff,0x80000000],[(0,0),(32,1),(63,31),(64,64),(65,0xffffffff)],[0,1,31,64,127,0xffffffff,0x80000000],[0,16,0xffffffff],[0,0x8c65e44a],[0,1]):
 position,capacity=fixture
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa20000:0xa20040]=bytes([0x55])*64
 for off,value in [(0xc,capacity),(0x10,capacity),(0x14,position),(0x18,position),(0x20,64),(0x24,mirror),(0x28,0),(0x2c,0)]:struct.pack_into('<I',ram,0xa20000+off,value)
 ram[0xa50001]=7;ram[0xa30000:0xa3000c]=bytes([0xa5])*12
 original=bytes(ram[0xa20000:0xa20040]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca20000,mode,request,0x8ca30004];s.r[15]=0x8cf00000;s.pc=0x8c65e712;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 callback_calls=[]
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  assert a in helpers,hex(a)
  if a==0x8c65e44a:
   assert tuple(regs[i] for i in range(4,6))==(0x8ca20000,0xfffffffd)
   callback_calls.append(a)
  return helpers[a](m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s.r[4:7]=[0x8ca20000,callback,0x8ca20000]
 for index in range(4,7):r[index]=s.r[index]
 s.pc=0x8c65e44e
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert helpers[0x8c65e44e](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 installed=bytearray(original);struct.pack_into('<2I',installed,0x28,callback,0x8ca20000)
 assert native[0xa20000:0xa20040]==installed
 original=bytes(installed)
 s.r[4:8]=[0x8ca20000,mode,request,0x8ca30004]
 for index in range(4,8):r[index]=s.r[index]
 s.pc=0x8c65e712
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 allowed=min(signed(capacity),signed(64-position+mirror),signed(request))&0xffffffff if mode in [0,1] else 0
 assert struct.unpack_from('<I',native,0xa30004)[0]==allowed and r[0]==int(allowed==request)
 assert native[0xa30000:0xa30004]==bytes([0xa5])*4 and native[0xa30008:0xa3000c]==bytes([0xa5])*4
 assert native[0xa20000:0xa20040]==original and native[0xa50001]==7
 assert (sr.value&~1)==0x70000062 and r[15]==0x8cf00000
 assert callback_calls==([0x8c65e44a] if mode not in [0,1] and callback else [])
 cases+=1
(p/'analysis/source_error_callback_connected_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='2100 persistent native source callback-installation-to-contiguous-check sequences. Setter65e44e installs callback/userdata,then checker65e712 uses the same RAM/CPU/FPU state. Full retail RAM/CPU/FPU/SR/PR comparisons after setter and checker;native source locks and controlled callback identity getter65e44a, no modeled calls. Independent whole64-byte source object after installation and checking,exact output count/R0,output guards,interrupt mask and stack. Valid modes0/1 never call callback;three invalid modes call installed callback exactly once when non-null with configured source pointer R4 and error-3 R5. Seven request patterns,five position/capacity fixtures,three mirror values,both banks,callback absent/present. Callback is a controlled native test routine,not the retail default handler. Full source construction/error-handler integration and fullgame unverified.'),indent=2)+'\n')
print(cases,'source contiguous capacity comparisons passed')
