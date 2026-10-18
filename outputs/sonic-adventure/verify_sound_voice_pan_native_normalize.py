import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_tikal_position_native import Vector
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_pan.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
transform=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_position_transform.dll')).sa_sound_position_transform;transform.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,ctypes.c_void_p,Vector]
normalize=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_normalize.dll')).sa_model_vector_normalize;normalize.argtypes=transform.argtypes
cases=0
for (x,z),slot,bank in itertools.product([(-2.,0.),(-1.,0.),(0.,1.),(0.,2.),(1.,0.),(2.,0.)],[0,17,36],[0,0x200000]):
 horizontal=0. if x==0 else (1. if x>0 else -1.)
 entry=0x8c0955c0;name='sa_sound_voice_pan'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 record=lit(0x8c0956e0)+slot*52
 struct.pack_into('<3f',ram,record+36,x,4.,z)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 def service(m,r,a,trace):
  address=r[4]&0xffffff;words=tuple(sum(m[address+4*j+i]<<(8*i) for i in range(4)) for j in range(3));trace.append((a,words))
  if a==0x8c63a94c:
   raw=struct.pack('<f',horizontal)
   for i in range(4):m[address+i]=raw[i]
  elif a!=0x8c02d528:errors.append(hex(a))
 def instruction(word,fp):
  b=(fp.fpscr>>21)&1
  def raw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
  if word==0xf1fd:
   assert list(fp.banks[b][:4])==[raw(x),raw(4.),raw(z),raw(1.)]
  elif word==0xf0ed:
   assert list(fp.banks[b][:4])==[raw(x),raw(0.),raw(z),raw(0.)]
   fp.banks[b][3]=raw(x*x+z*z)
  elif word==0xf37d:
   assert fp.banks[b][3]==raw(x*x+z*z)
   fp.banks[b][3]=raw(1./max(abs(x),abs(z)))
  else:raise AssertionError(hex(word))
 @Vector
 def vector(ctx,word,fp):instruction(word,fp.contents);return 0
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c02d528:return transform(m,f,r,None,vector)
  if a==0x8c63a94c:return normalize(m,f,r,None,vector)
  service(m,r,a,calls);return -9 if errors else 0
 def hook(a):
  def run(m,r,sr):service(m,r,a,events);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=slot;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={},vector_instruction=instruction)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 nf.fpul=fpul.value
 assert calls==events==[]
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert r[0]==int((horizontal+1)*15.5)*4 and macl.value==slot*52
 cases+=1
(out/'analysis/sound_voice_pan_native_normalize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='41-instruction panning helper; slots0/17/36,both FPU banks and axis vectors(-2,0)/(-1,0)/(0,1)/(0,2)/(1,0)/(2,0). Full retail RAM/CPU/FPU/SR/PR/FPUL and independent pan return/MACL/copy-and-zero-Y service argument checks. Native coordinate transform02d528 wrapper connected with identity FTRV instruction service; native normalization63a94c connected with exact axis FIPR/FSRRA instruction services; no modeled function calls; other FPU modes untested.'),indent=2)+'\n')
print(cases,'voice pan native normalization comparisons passed')
