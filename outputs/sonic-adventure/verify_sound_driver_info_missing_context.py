import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_info.dll'))
fn=lib.sa_sound_driver_info;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
logger=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_frame_debug_log;logger.argtypes=abi
cases=0
for bank,mask in itertools.product([0,0x200000],[0,7,15]):
 result=0x0d000001
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 struct.pack_into('<I',ram,0x67c1d0,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t==0x8c65a2fc:return logger(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  errors.append(t);return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65aaf4;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<2I',native,0x67c1c8)==(0xabcdef01,0x12345678)
 cases+=1
report={'cases_passed':cases,'instructions_recovered':167,'scope':'Full original RAM/CPU/SR/PR/FPU equality for missing sound context, both FPU banks/three interrupt masks; native disabled debug logger, return0d000001, unchanged error globals, stack/R14 preserved. Context-present driver-info reads not verified.'}
(out/'analysis/sound_driver_info_missing_context_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'driver info missing-context comparisons passed')
