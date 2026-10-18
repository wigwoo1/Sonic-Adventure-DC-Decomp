import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_pan_get.dll')).sa_sound_driver_pan_get;fn.argtypes=abi
division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/signed_division.dll')).sa_signed_division;division.argtypes=abi
cases=0
for value,bank,answer in itertools.product(range(256),[0,1],[0,0xffffffff]):
 dest=0x8ca20004
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08090]=bytes([0x55])*144;ram[0xa20000:0xa20010]=bytes([0x66])*16
 struct.pack_into('<I',ram,dest&0xffffff,value);wanted=bytearray(ram);struct.pack_into('<I',wanted,dest&0xffffff,(int((value if value<128 else value-256)*15/127))&0xffffffff)
 struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',wanted,0xa08028,0x8ca09000)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c669278;actual.append(tuple(r[i] for i in range(4,7)));m[(r[6]&0xffffff)+3]=value;r[0]=answer;return 0
 def backend(m,r,sr):expected.append(tuple(r[i] for i in range(4,7)));m[(r[6]&0xffffff)+3]=value;r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=0;s.r[6]=dest;s.r[15]=0x8cf00000;s.pc=0x8c668112;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c669278:backend},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 assert actual==expected==[(0x8ca09000,0,0x8cefffe0)]
 assert native[0xa08000:0xa08090]==wanted[0xa08000:0xa08090] and native[0xa20000:0xa20010]==wanted[0xa20000:0xa20010]
 assert r[0]==r[4]==answer and r[15]==0x8cf00000;cases+=1
scope='1024 full retail RAM/CPU/FPU/SR/PR comparisons of26-instruction pan getter668112..668146. All256 backend byte values,both banks,success/ffffffff status,selector0. Native signed division10ca5c;independent signed-byte*15/127 truncation,16-byte output guards,144-byte driver preservation,backend handle/selector/stack packet pointer and status propagation. Backend669278 modeled;other selectors and native backend/hardware/game integration unverified.'
(p/'analysis/sound_driver_pan_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'zero gate getter comparisons passed')
