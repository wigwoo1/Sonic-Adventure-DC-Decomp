import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_byte_read.dll')).sa_sound_byte_read;fn.argtypes=abi;cases=0
for address,pattern,bank in itertools.product([0,1,2,3,4,0x1ffffc,0x1fffff,0x200000,0x200001,0x7fffffff,0x80000000,0xffffffff],[bytes([0,127,128,255]),bytes([255,128,127,0])],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa20000:0xa20010]=bytes([0x66])*16
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c66910c;actual.append((r[4],r[5]));
  for i,v in enumerate(pattern):m[(r[5]&0xffffff)+i]=v
  r[0]=123;return 0
 def lower(m,r,sr):expected.append((r[4],r[5]));m[r[5]&0xffffff:(r[5]&0xffffff)+4]=pattern;r[0]=123;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=address;s.r[5]=0x8ca20004;s.r[15]=0x8cf00000;s.pc=0x8c6671c4;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c66910c:lower},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 valid=ctypes.c_int32(address).value<0x200000;output=bytearray([0x66]*16)
 if valid:output[4]=pattern[address&3]
 assert actual==([(address&0xfffffffc,0x8ceffff8)] if valid else [])
 assert native[0xa20000:0xa20010]==output and r[0]==0 and r[15]==0x8cf00000;cases+=1
scope='48 complete retail RAM/CPU/FPU/SR/PR comparisons of byte reader6671c4..6671f8. Twelve addresses cover all byte offsets,2MiB boundary and signed-negative addresses;two four-byte payloads,both banks. Independent signed address gate,aligned lower read,selected byte/16-byte output guards and always-zero result. Lower66910c modeled;native transfer/hardware/game integration unverified.'
(p/'analysis/sound_byte_read_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'byte read comparisons passed')
