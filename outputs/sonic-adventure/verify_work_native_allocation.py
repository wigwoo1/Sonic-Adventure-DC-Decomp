import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure'
binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_create.dll')).sa_sound_work_create
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_reset.dll')).sa_sound_work_reset;reset.argtypes=fn.argtypes
helpers={0x8c65b606:reset}
for address,stem,name in [(0x8c65e798,'sound_source_buffer_get','sa_sound_source_buffer_get'),(0x8c66768c,'sound_stream_input_callback_set','sa_sound_stream_input_callback_set')]:
 helpers[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);helpers[address].argtypes=fn.argtypes
for address,stem,name in [(0x8c65e79c,'sound_source_ring_size_get','sa_sound_source_ring_size_get'),(0x8c65e7a0,'sound_source_mirror_size_get','sa_sound_source_mirror_size_get')]:
 helpers[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);helpers[address].argtypes=fn.argtypes
abi=fn.argtypes
def load(dll,name):
 f=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);f.argtypes=abi;return f
b=binary;lower={}
import math
from fractions import Fraction
from sh4_binary32 import multiply,multiply_add,divide,add_sub
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def val(word):
    if word==0:return Fraction(0)
    exponent=(word>>23)-150; significand=(word&0x7fffff)|0x800000
    return Fraction(significand)*Fraction(2)**exponent
def sqrt_service(word,fp):
    assert word&0xf0ff==0xf06d
    bank=(fp.fpscr>>21)&1; n=(word>>8)&15; x=val(fp.banks[bank][n])
    candidate=raw(math.sqrt(float(x)))
    while val(candidate)**2>x:candidate-=1
    while val(candidate+1)**2<=x:candidate+=1
    inexact=val(candidate)**2!=x
    if inexact:
        midpoint=(val(candidate)+val(candidate+1))/2
        if x>midpoint**2 or (x==midpoint**2 and candidate&1):candidate+=1
    fp.banks[bank][n]=candidate
    fp.fpscr=(fp.fpscr&~0x3f000)|(0x1004 if inexact else 0)
def trig_oracle(angle):
    x=angle&0x7fffffff;index=int(struct.unpack('<f',struct.pack('<I',divide(x,0x3f490fdb)[0]))[0])
    remainder=add_sub(x,multiply(raw(float(index)),0x3f490fdb)[0],True)[0];octant=index&7
    argument=add_sub(remainder,0xbf490fdb)[0] if octant&1 else remainder
    cosine=octant in [0,3,4,7];table=0x8c160444 if cosine else 0x8c160460
    coefficients=struct.unpack_from('<7I',b,table-0x8c010000);square=multiply(argument,argument)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if not cosine:answer=multiply(argument,answer)[0]
    if octant in [1,2,3,4]:answer^=0x80000000
    return struct.unpack('<f',struct.pack('<I',answer))[0]
for address,stem,name in [(0x8c644be6,'file_audio_configuration_forward','sa_file_audio_configuration_forward'),(0x8c65adc0,'audio_filter_coefficients','sa_audio_filter_coefficients'),(0x8c10f90c,'math_result_error_wrapper','sa_math_result_error_wrapper'),(0x8c10fcbc,'math_trig_octant_dispatch','sa_math_trig_octant_dispatch'),(0x8c10fbb8,'math_range_reduce','sa_math_range_reduce'),(0x8c110218,'math_trig_polynomials','sa_math_cosine_polynomial'),(0x8c110238,'math_trig_polynomials','sa_math_sine_polynomial')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi

handle_pool=struct.unpack_from('<I',binary,0x8c644c9c-0x8c010000)[0]
lower[0x8c644b6e]=load('file_decode_handle_allocate.dll','sa_file_decode_handle_allocate');lower[0x8c644bfe]=load('file_decode_handle_release.dll','sa_file_decode_handle_release')
helpers.update(lower)
for address,dll,name in [(0x8c6673b4,'sound_stream_create.dll','sa_sound_stream_create'),(0x8c667440,'sound_stream_destroy.dll','sa_sound_stream_destroy'),(0x8c10d31c,'byte_fill.dll','sa_byte_fill')]:helpers[address]=load(dll,name)
stream_pool=0x8d61ac
pool=0x8a3910;cases=0
services=[0x8c65e798,0x8c65e79c,0x8c65e7a0,0x8c6673b4,0x8c66768c]
for slot,channels,created,bank in itertools.product(range(9),[0,1,2],[0x8c8d61ac],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[pool:pool+1152]=bytes([0x77])*1152
 for index in range(8):ram[pool+index*144]=0 if index>=slot else [1,2,128,255][index%4]
 struct.pack_into('<2I',ram,0xa10000,0x8ca30000,0x8ca30100)
 struct.pack_into('<3I',ram,0xa3001c,0x8ca40000,1025,257)
 source_before=bytes(ram[0xa30000:0xa30030])
 ram[stream_pool:stream_pool+172*8]=bytes([0x55])*(172*8)
 for index in range(8):struct.pack_into('<H',ram,stream_pool+index*172,0 if index==0 else 1)
 stream_before=bytes(ram[stream_pool:stream_pool+172*8])
 hs=handle_pool&0xffffff;ram[hs:hs+52*8]=bytes([0x77])*(52*8)
 for index in range(8):struct.pack_into('<I',ram,hs+index*52,0 if index==0 else 1)
 handles_before=bytes(ram[hs:hs+52*8])
 before=bytes(ram[pool:pool+1152]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 selected=0x8c000000|pool+slot*144
 def service(address,mem,r,trace):
  args=tuple(r[i] for i in range(4,8));trace.append((address,args))
  if address in services[:3]:
   assert r[4]==0x8ca30000;r[0]={services[0]:0x8ca40000,services[1]:1025,services[2]:257}[address]
  elif address==0x8c6673b4:
   assert args==(channels,0x8ca40000,512,640);r[0]=created
  else:assert address==0x8c66768c and args[:3]==(created,0x8c65b88c,selected)
 def hook(address):
  def run(mem,r,sr):service(address,mem,r,expected);return sr
  return run
 @Call
 def call(ctx,address,m,f,srp,r,prp):
  if address in helpers:return helpers[address](m,f,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  service(address,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca80000,channels,0x8ca10000];s.r[15]=0x8cf00000;s.pc=0x8c65b624;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 special={'macl':0}
 reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 nf.fpul=fpul.value
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 wanted=bytearray(before)
 if slot<8:
  offset=slot*144;struct.pack_into('<I',wanted,offset+4,created)
  if created:
   wanted[offset:offset+4]=bytes([1,0,channels,0]);struct.pack_into('<I',wanted,offset+8,0x8ca80000)
   for index in range(channels):struct.pack_into('<I',wanted,offset+12+index*4,0x8ca30000+index*256)
   for off,value in [(0x2c,0),(0x30,0),(0x34,0),(0x38,struct.unpack_from('<I',binary,0x64b710)[0]),(0x3c,0xffffffff),(0x40,0),(0x44,0),(0x48,0),(0x4c,0)]:struct.pack_into('<I',wanted,offset+off,value)
 assert native[pool:pool+1152]==wanted and r[0]==(selected if slot<8 and created else 0) and r[15]==0x8cf00000
 assert actual==expected==[]
 assert native[0xa30000:0xa30030]==source_before
 wanted_stream=bytearray(stream_before);wanted_handles=bytearray(handles_before)
 if slot<8:
  obj=bytearray(172);struct.pack_into('<H',obj,0,1);struct.pack_into('<I',obj,8,handle_pool)
  struct.pack_into('<4I',obj,0x38,channels,0x8ca40000,512,640)
  struct.pack_into('<4I',obj,0x88,0x8c65b88c,selected,0x8c6673a2,created)
  wanted_stream[:172]=obj
  obj=bytearray(52);struct.pack_into('<2I',obj,0,1,0)
  obj[48:52]=native[hs+48:hs+52]
  wanted_handles[:52]=obj
 assert native[stream_pool:stream_pool+172*8]==wanted_stream
 assert native[hs:hs+52*8]==wanted_handles
 cases+=1
(out/'analysis/work_native_allocation_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='54 full retail RAM/CPU/FPU/SR/PR comparisons of native work constructor65b624 with stream creation6673b4,decode-handle allocation,byte fill,coefficient math,source accessors,callback registration and work reset connected; no modeled calls. Work slots0..7 and exhausted work pool,channels0/1/2,both banks,first stream/handle slots available. Independent whole1152-byte work pool,1376-byte stream pool and decode-handle pool initialization except coefficients independently covered by dedicated coefficient verifier;callback/user fields,output arguments,source object unchanged,return/stack. Other stream/handle free-slot combinations and exhaustion/null-callback path not covered here. Playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'sound work constructor comparisons passed')
