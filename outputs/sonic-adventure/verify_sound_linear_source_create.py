import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_linear_source_create.dll')).sa_sound_linear_source_create;fn.argtypes=abi
reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_linear_source_reset.dll')).sa_sound_linear_source_reset;reset.argtypes=abi
cases=0;pool=0x8a6138
for slot,size,bank in itertools.product(range(33),[0,1,2,127,128,255,0xffffffff],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[pool-8:pool+1160]=bytes([0x55])*1168
 for i in range(32):struct.pack_into('<I',ram,pool+i*36+4,0 if i>=slot else [1,2,128,0xffffffff][i%4])
 original=bytes(ram[pool:pool+1152]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca40000,size,16];s.r[15]=0x8cf00000;s.pc=0x8c65e0ea;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  assert a==0x8c65e178,hex(a)
  return reset(m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 wanted=bytearray(original);selected=0x8c000000|pool+slot*36
 if slot<32:
  offset=slot*36
  for off,value in [(0,0x8c67c1fc),(4,1),(8,0x8c672424),(0xc,size),(0x10,0),(0x14,0x8ca40000),(0x18,size),(0x1c,0x8c65e0bc),(0x20,selected)]:struct.pack_into('<I',wanted,offset+off,value)
 assert native[pool:pool+1152]==wanted and r[0]==(selected if slot<32 else 0)
 assert native[pool-8:pool]==bytes([0x55])*8 and native[pool+1152:pool+1160]==bytes([0x55])*8 and r[15]==0x8cf00000
 cases+=1
(p/'analysis/sound_linear_source_create_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='462 full retail RAM/CPU/FPU/SR/PR comparisons of linear-source constructor65e0ea..65e150 with native reset65e178,no modeled calls. All32 candidate slots plus full pool,nonzero occupied flag patterns1/2/128/ffffffff,seven length patterns,both banks. Independent entire1152-byte pool and8-byte guards,first-free selection,returned pointer or null,all9 constructed words: method table8c67c1fc,active1,identity8c672424,remaining=length,offset0,buffer,length,default callback65e0bc,self userdata. Other slots preserved. Retail default callback not executed. Connected pool lifecycle/reservations and game callers remain unverified.'),indent=2)+'\n')
print(cases,'native linear source constructor comparisons passed')
