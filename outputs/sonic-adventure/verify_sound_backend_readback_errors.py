import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_readback.dll')).sa_sound_backend_readback;fn.argtypes=abi
diagnostic=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_error_record.dll')).sa_engine_error_record;diagnostic.argtypes=abi
cases=0
for (enabled,present),value,bank in itertools.product([(0,0),(0,1),(1,0)],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08090]=bytes([0x55])*144;ram[0xa20000:0xa20028]=bytes([0x66])*40
 struct.pack_into('<I',ram,0x67c1d0,0x8ca09000 if enabled else 0);struct.pack_into('<2I',ram,0x67c1c8,0x12345678,0x9abcdef0)
 before=bytes(ram);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c65acac;calls.append((r[4],r[5]));return diagnostic(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000 if present else 0;s.r[5]=value;s.r[6]=0x8ca20004;s.r[15]=0x8cf00000;s.pc=0x8c66adc4;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 error=0x07000103 if enabled else 0x0d000001
 assert calls==[(0x8c66b8c4,error)] and r[0]==error and struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66b8c4,error)
 assert native[0xa20000:0xa20028]==before[0xa20000:0xa20028] and native[0xa08000:0xa08090]==before[0xa08000:0xa08090] and r[15]==0x8cf00000
 cases+=1
scope='42 complete retail RAM/CPU/FPU/SR/PR comparisons of backend readback66adc4 error paths. Disabled backend with null/present handle and enabled backend with null handle;seven selector words,both banks. Native diagnostic65acac with independent exact label/error/global values,40-byte output preservation,144-byte handle preservation,stack restoration. No modeled calls. Successful backend field-read path unverified;not full routine recovery.'
(p/'analysis/sound_backend_readback_errors_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'backend error comparisons passed')
