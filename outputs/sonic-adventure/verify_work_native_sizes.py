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
pool=0x8a3910;cases=0
services=[0x8c65e798,0x8c65e79c,0x8c65e7a0,0x8c6673b4,0x8c66768c]
for slot,channels,created,bank in itertools.product(range(9),[0,1,2],[0x8ca70000],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[pool:pool+1152]=bytes([0x77])*1152
 for index in range(8):ram[pool+index*144]=0 if index>=slot else [1,2,128,255][index%4]
 struct.pack_into('<2I',ram,0xa10000,0x8ca30000,0x8ca30100)
 struct.pack_into('<3I',ram,0xa3001c,0x8ca40000,1025,257)
 source_before=bytes(ram[0xa30000:0xa30030])
 ram[0xa70000:0xa70100]=bytes([0x55])*256;stream_before=bytes(ram[0xa70000:0xa70100])
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
 reference(ram,s,{a:hook(a) for a in services if a not in helpers},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 wanted=bytearray(before)
 if slot<8:
  offset=slot*144;struct.pack_into('<I',wanted,offset+4,created)
  if created:
   wanted[offset:offset+4]=bytes([1,0,channels,0]);struct.pack_into('<I',wanted,offset+8,0x8ca80000)
   for index in range(channels):struct.pack_into('<I',wanted,offset+12+index*4,0x8ca30000+index*256)
   for off,value in [(0x2c,0),(0x30,0),(0x34,0),(0x38,struct.unpack_from('<I',binary,0x64b710)[0]),(0x3c,0xffffffff),(0x40,0),(0x44,0),(0x48,0),(0x4c,0)]:struct.pack_into('<I',wanted,offset+off,value)
 assert native[pool:pool+1152]==wanted and r[0]==(selected if slot<8 and created else 0) and r[15]==0x8cf00000
 assert len(actual)==(1 if slot<8 else 0)
 assert native[0xa30000:0xa30030]==source_before
 wanted_stream=bytearray(stream_before)
 if slot<8:struct.pack_into('<2I',wanted_stream,0x88,0x8c65b88c,selected)
 assert native[0xa70000:0xa70100]==wanted_stream
 cases+=1
(out/'analysis/work_native_sizes_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='54 original RAM/CPU/FPU/SR/PR comparisons of constructor65b624..65b700: all8 selected slots and full pool,channels0/1/2,creation success,both banks. Independent entire1152-byte pool, return pointer, exact service arguments and ordered trace; reset65b606 native. Buffer getter65e798 and input callback setter66768c native,independent complete stream callback fields88/8c. Ring size65e79c and mirror size65e7a0 getters native,independent full48-byte source object unchanged. Only stream creation modeled; null-creation callback path excluded from native setter integration; successful selection initializes fields and preserves other slots. Native subordinate constructor integration and fullgame unverified.'),indent=2)+'\n')
print(cases,'sound work constructor comparisons passed')
