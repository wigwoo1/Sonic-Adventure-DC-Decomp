import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_pan.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for horizontal,slot,bank in itertools.product([-1.,-.5,0.,.5,1.],[0,17,36],[0,0x200000]):
 entry=0x8c0955c0;name='sa_sound_voice_pan'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 record=lit(0x8c0956e0)+slot*52
 struct.pack_into('<3f',ram,record+36,3.,4.,5.)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 def service(m,r,a,trace):
  address=r[4]&0xffffff;words=tuple(sum(m[address+4*j+i]<<(8*i) for i in range(4)) for j in range(3));trace.append((a,words))
  if a==0x8c63a94c:
   raw=struct.pack('<f',horizontal)
   for i in range(4):m[address+i]=raw[i]
  elif a!=0x8c02d528:errors.append(hex(a))
 @Call
 def call(ctx,a,m,f,sr,r,pr):service(m,r,a,calls);return -9 if errors else 0
 def hook(a):
  def run(m,r,sr):service(m,r,a,events);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=slot;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (0x8c02d528,0x8c63a94c)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 nf.fpul=fpul.value
 assert calls==events==[(0x8c02d528,tuple(struct.unpack('<3I',struct.pack('<3f',3.,4.,5.)))),(0x8c63a94c,tuple(struct.unpack('<3I',struct.pack('<3f',3.,0.,5.))))]
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert r[0]==int((horizontal+1)*15.5)*4 and macl.value==slot*52
 cases+=1
(out/'analysis/sound_voice_pan_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='41-instruction panning helper; slots0/17/36,both FPU banks and horizontal values-1/-.5/0/.5/1. Full retail RAM/CPU/FPU/SR/PR/FPUL and independent pan return/MACL/copy-and-zero-Y service argument checks. Coordinate transform02d528 and normalization63a94c modeled; other FPU modes untested.'),indent=2)+'\n')
print(cases,'voice pan comparisons passed')
