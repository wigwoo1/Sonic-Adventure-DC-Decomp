import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_mode_set.dll'))
fn=lib.sa_sound_driver_mode_set;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
cases=0
for result,bank,mask,context in itertools.product([0,1,0x80000000,0xffffffff],[0,0x200000],[0,7,15],[0,0x8cb08000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,context)
 ram[0xb08000:0xb08200]=bytes([0x99])*512
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 def hook(m,r,sr):expected.append(r[4]);r[0]=result;return sr
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c65acc0:actual.append(r[4]);r[0]=result;return 0
  if t==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c6436d0;s.r[4]=result;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected==[]
 assert r[0]==0 and r[14]==0x2234000e and r[15]==0x8cf00000
 offset=struct.unpack_from('<h',binary,0x6436e0-0x10000)[0]
 expected_context=bytearray([0x99])*512
 if context:struct.pack_into('<I',expected_context,offset,result)
 assert native[0xb08000:0xb08200]==expected_context
 cases+=1
report={'cases_passed':cases,'instructions_recovered':8,'scope':'Complete original-instruction RAM/CPU/SR/PR/FPU equality across absent/present context, four field values, both FPU banks and three interrupt masks; independent exact context field and preserved surrounding bytes; no modeled services. Semantic field meaning provisional.'}
(out/'analysis/sound_driver_mode_set_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'sound driver context setter comparisons passed')
