import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_format_engine.dll')).sa_sound_format_engine
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for text,bank in itertools.product([b'',b'hello',b'Sonic Adventure',b'%%',b'a%%b'],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa01000:0xa01000+len(text)+1]=text+bytes(1)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];actual_output=[];reference_output=[];errors=[]
 def service(m,r,a,trace,output):
  trace.append((a,tuple(r[i] for i in range(4,8))))
  if a==0x8c10e50e:
   data=bytes(m[(r[4]&0xffffff)+i] for i in range(r[5]));output.append(data)
   pos=(r[6]&0xffffff)+8;value=sum(m[pos+i]<<(8*i) for i in range(4))+len(data)
   for i in range(4):m[pos+i]=(value>>(8*i))&255
  elif a not in (0x8c10e576,0x8c10e566):errors.append(hex(a))
  r[0]=0
 @Call
 def call(ctx,a,m,f,sr,r,pr):service(m,r,a,calls,actual_output);return -9 if errors else 0
 def hook(a):
  def run(m,r,sr):service(m,r,a,events,reference_output);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=1;s.r[5]=0;s.r[6]=0x8ca00000;s.r[7]=0x8ca01000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10dbc4
 struct.pack_into('<I',ram,0xf00000,0x8ca02000);struct.pack_into('<I',native,0xf00000,0x8ca02000)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={a:hook(a) for a in (0x8c10e50e,0x8c10e576,0x8c10e566)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and calls==events and actual_output==reference_output
 assert b''.join(actual_output)==text.replace(b'%%',b'%') and r[0]==len(text.replace(b'%%',b'%'))
 cases+=1
(out/'analysis/sound_format_engine_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native878-instruction formatting engine, full RAM/CPU/FPU/SR/PR and emission-service traces match retail execution for empty/literal/escaped-percent strings in both FPU banks. Independent emitted text/count checked. Emit/ready/flush services modeled; argument conversions, floating formatting, FILE mode, actual I/O pending.'),indent=2)+'\n')
print(cases,'format engine comparisons passed')
