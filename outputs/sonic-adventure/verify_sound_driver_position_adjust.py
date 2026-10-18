import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_position_adjust.dll')).sa_sound_driver_position_adjust;fn.argtypes=abi
helpers={}
for a,dll,name in [(0x8c10ca5c,'sound_volume_dispatch','sa_signed_division'),(0x8c10cbb8,'integer_remainder','sa_integer_remainder')]:
 f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(dll+'.dll'))),name);f.argtypes=abi;helpers[a]=f
def signed(v):return ctypes.c_int32(v).value
def quotient(a,b):return (abs(a)//abs(b))*(-1 if (a<0)!=(b<0) else 1) if b else 0
cases=0
for remaining,width,length,position,bank,sr0 in itertools.product([0,128,255,256],[0,1,2,4],[256,512],[0,128,0xffffffff],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 handle=0x8ca08000;output=0x8ca20000
 struct.pack_into('<I',ram,0xa0800c,width);struct.pack_into('<2I',ram,0xa08038,length,remaining)
 struct.pack_into('<3I',ram,0xa20000,0xa5a5a5a5,0xdeadbeef,position)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a in helpers,a
  return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=output;s.r[6]=output+4;s.r[7]=output+8;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c668a32
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 adjusted=(position-8*quotient(remaining,width))&0xffffffff if remaining<256 else position
 divisor=signed(8*quotient(length,width));n=signed(adjusted)
 residue=(n-quotient(n,divisor)*divisor)&0xffffffff if divisor else 0
 assert struct.unpack_from('<3I',native,0xa20000)==(residue,0,adjusted)
 assert r[0]==residue
 if not width:assert struct.unpack_from('<I',native,0x7ac108)[0]==0x44e
 cases+=1
(out/'analysis/sound_driver_position_adjust_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native28-instruction position adjustment668a32 with native signed division/remainder. Remaining0/128/255/256,width0/1/2/4,length256/512,position0/128/FFFFFFFF,banks/SR. Full retail RAM/CPU/FPU/SR/PR matches. Independent conditional position subtraction,signed residue using8*trunc(length/width),second outputzero,returned residue and zero-width error44E checked. Field meanings inferred; physical playback/full game unverified.'),indent=2)+'\n')
print(cases,'position adjustment comparisons passed')
