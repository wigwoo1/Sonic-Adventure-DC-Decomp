import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_initialize_wrapper.dll'))
fn=lib.sa_sound_initialize_wrapper;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
assert struct.unpack_from('<I',binary,0x643f8c-0x10000)[0]==0x8c65acc0
assert struct.unpack_from('<I',binary,0x643f94-0x10000)[0]==0x8c65acac
cases=0
for result,bank,mask in itertools.product([0,3,0x0d000001,0xffffffff],[0,0x200000],[0,7,15]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def hook(m,r,sr):expected.append(r[4]);r[0]=result;return sr
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c65acc0:actual.append(r[4]);r[0]=result;return 0
  if t==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c643f6c;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,hooks={0x8c65acc0:hook},fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected==[0]
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<2I',native,0x67c1c8)==((0x8c643f6c,result) if result else (0xabcdef01,0x12345678))
 cases+=1
report={'cases_passed':cases,'instructions_recovered':16,'scope':'Full original-instruction RAM/register/SR/PR/FPU equality; initializer argument0, four return values, both FPU banks/three interrupt masks, native error recorder, preserved result/R14/stack and independent error globals. Sound initializer65acc0 modeled; startup ordering/playback unverified.'}
(out/'analysis/sound_initialize_wrapper_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'sound initialization wrapper comparisons passed')
