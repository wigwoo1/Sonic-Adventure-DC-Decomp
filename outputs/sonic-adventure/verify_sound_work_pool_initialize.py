import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_pool_initialize.dll')).sa_sound_work_pool_initialize;fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/byte_fill.dll')).sa_byte_fill;fill.argtypes=abi
cases=0
for pattern,bank,stack in itertools.product([0,1,127,128,255,0x55],[0,1],[0,0x8ca40000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x8a3908:0x8a3d98]=bytes([pattern])*1168;struct.pack_into('<I',ram,0xf00000,stack)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];expected=[]
 def hook(mem,r,sr):
  assert mem[0x8a3910:0x8a3d90]==bytes([pattern])*1152;expected.append(0x8c667370);return sr
 @Call
 def call(ctx,address,m,f,srp,r,prp):
  if address==0x8c667370:
   assert bytes(m[0x8a3910:0x8a3d90])==bytes([pattern])*1152;calls.append(address);return 0
  assert address==0x8c10d31c and tuple(r[i] for i in range(4,7))==(0x8c8a3910,0,1152) and r[15]==0x8cf00000
  return fill(m,f,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c65b5e8;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c667370:hook},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and calls==expected==[0x8c667370]
 assert native[0x8a3910:0x8a3d90]==bytes(1152) and native[0x8a3908:0x8a3910]==bytes([pattern])*8 and native[0x8a3d90:0x8a3d98]==bytes([pattern])*8
 assert r[15]==0x8cf00000;cases+=1
(p/'analysis/sound_work_pool_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='24 retail RAM/CPU/FPU/SR/PR comparisons of work pool initializer65b5e8..65b5fc:six fill patterns,both banks,two stack values. Stream-pool initializer667370 modeled and observed before clearing. Native byte fill10d31c tail-called with work pool8c8a3910,length1152,value0,original stack restored. Independent all1152 work bytes zero and8-byte guards unchanged. Native subordinate initialization/allocation integration and fullgame unverified.'),indent=2)+'\n')
print(cases,'audio work pool initialization comparisons passed')
