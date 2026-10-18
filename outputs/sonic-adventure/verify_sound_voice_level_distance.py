import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_tikal_position_native import Vector
import math
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_level.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
sqrt=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/scalar_sqrt_wrapper.dll')).sa_scalar_signed_sqrt;sqrt.argtypes=[ctypes.POINTER(Fpu),P,ctypes.c_void_p,Vector]
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
cases=0
for distance,slot,level,bank in itertools.product([0,10,20,30,100,600,1000],[0,36],[-50,0,90],[0,0x200000]):
 enabled=1;cap=127
 entry=0x8c0947ba;name='sa_sound_voice_level'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c094870),0x8ca01000 if enabled else 0)
 record=lit(0x8c09486c)+slot*52
 struct.pack_into('<2i',ram,record+24,level,cap)
 struct.pack_into('<3f',ram,record+36,float(distance),0.,0.);struct.pack_into('<3f',ram,0xa01020,0.,0.,0.)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 def instruction(word,fp):
  assert word==0xf06d
  b=(fp.fpscr>>21)&1
  assert fp.banks[b][0]==struct.unpack('<I',struct.pack('<f',float(distance*distance)))[0]
  fp.banks[b][0]=struct.unpack('<I',struct.pack('<f',float(distance)))[0]
  fp.fpscr&=~0x3f000
 @Vector
 def square_root(ctx,word,fp):instruction(word,fp.contents);return 0
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c63a9c4:return sqrt(f,sr,None,square_root)
  errors.append(hex(a));return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=slot;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,vector_instruction=instruction)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 nf.fpul=fpul.value
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 attenuation=0 if distance<20 else int(-f32(f32(max(0.,f32(f32(distance-20.)*f32(1/580))))*127.))
 assert r[0]==(min(max(level+attenuation,-127),cap)&0xffffffff) and macl.value==slot*52,(distance,level,r[0],attenuation)
 cases+=1
(out/'analysis/sound_voice_level_distance_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='83 reachable instruction native voice level routine; listener enabled, distances0/10/20/30/100/600/1000, slots0/36, signed base levels, both FPU banks. Full retail RAM/CPU/FPU/SR/PR and independent clamped level/MACL checks. Native signed-square-root wrapper connected; FSQRT instruction output supplied as independently exact roots of perfect integer squares, FPSCR clear; numerical FSQRT implementation and other FPU modes untested.'),indent=2)+'\n')
print(cases,'voice distance level comparisons passed')
