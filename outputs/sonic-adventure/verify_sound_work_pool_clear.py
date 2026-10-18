import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_pool_clear.dll')).sa_sound_work_pool_clear;fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=abi
cases=0
for pattern,bank,stack in itertools.product([0,1,127,128,255,0x55],[0,1],[0,0x8ca40000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x8a3908:0x8a3d98]=bytes([pattern])*1168;struct.pack_into('<I',ram,0xf00000,stack)
 ram[0x8d61ac:0x8d670c]=bytes([pattern^0x55])*1376;ram[0x890680:0x890820]=bytes([pattern^0xaa])*416
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];expected=[]
 @Call
 def call(ctx,address,m,f,srp,r,prp):
  assert address==0x8c10d31c and tuple(r[i] for i in range(4,7))==(0x8c8a3910,0,1152) and r[15]==0x8cf00000
  return fill(m,f,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65b5fc;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and calls==expected==[]
 assert native[0x8a3910:0x8a3d90]==bytes(1152) and native[0x8a3908:0x8a3910]==bytes([pattern])*8 and native[0x8a3d90:0x8a3d98]==bytes([pattern])*8
 assert native[0x8d61ac:0x8d670c]==bytes([pattern^0x55])*1376 and native[0x890680:0x890820]==bytes([pattern^0xaa])*416
 assert r[15]==0x8cf00000;cases+=1
(p/'analysis/sound_work_pool_clear_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='24 full retail RAM/CPU/FPU/SR/PR comparisons of work pool clear65b5fc..65b606: six byte patterns,both banks,two stack values. Native byte fill tail-call clears1152 bytes at8c8a3910,8-byte guards preserved; independent entire1376-byte stream and416-byte decoder handle pools unchanged. No modeled calls. This is a raw work-pool clear,not stream/handle destruction. Caller lifecycle/fullgame remain unverified.'),indent=2)+'\n')
print(cases,'audio work pool initialization comparisons passed')
