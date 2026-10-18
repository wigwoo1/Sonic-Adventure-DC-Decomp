import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_format_engine.dll')).sa_sound_format_engine
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
length=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_format_engine.dll')).sa_format_string_length;length.argtypes=fn.argtypes
division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=fn.argtypes
integer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_format_engine.dll')).sa_format_integer_convert;integer.argtypes=fn.argtypes
unsigned=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/unsigned_division_helpers.dll'))
helpers={}
for address,name in [(0x8c10cb10,'sa_unsigned_quotient'),(0x8c10cc80,'sa_unsigned_remainder_8c10cc80')]:
 helper=getattr(unsigned,name);helper.argtypes=fn.argtypes;helpers[address]=helper
emit=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/format_emit.dll')).sa_format_emit;emit.argtypes=fn.argtypes
move=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_request_enqueue.dll')).sa_memory_move;move.argtypes=fn.argtypes
output_helpers=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/format_output_helpers.dll'))
for address,name in [(0x8c10e566,'sa_format_finish'),(0x8c10e576,'sa_format_ready')]:
 helper=getattr(output_helpers,name);helper.argtypes=fn.argtypes;helpers[address]=helper
wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_debug_format.dll')).sa_sound_debug_format;wrapper.argtypes=fn.argtypes
helpers[0x8c10dbc4]=fn
fixtures=[(b'%c',[65],b'A'),(b'%c',[255],bytes([255])),(b'%s',[0x8ca03000],b'Sonic'),(b'%s:%c',[0x8ca03000,65],b'Sonic:A'),(b'%.3s',[0x8ca03000],b'Son'),(b'%8s',[0x8ca03000],b'   Sonic'),(b'%-8s',[0x8ca03000],b'Sonic   '),(b'%8.3s',[0x8ca03000],b'     Son'),(b'%.0s',[0x8ca03000],b''),(b'%*.*s',[8,3,0x8ca03000],b'     Son'),(b'%s:%d:%c',[0x8ca03000,0xffffffff,65],b'Sonic:-1:A'),(b'%% %s %08x',[0x8ca03000,15],b'% Sonic 0000000f'),(b'%3c',[65],b'  A'),(b'%-3c',[65],b'A  '),(b'%c',[0],bytes(1))]
cases=0
for (text,args,expected_text),bank in itertools.product(fixtures,[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa01000:0xa01000+len(text)+1]=text+bytes(1)
 ram[0xa03000:0xa03006]=b'Sonic\0'
 struct.pack_into('<'+'I'*len(args),ram,0xa02000,*args)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];actual_output=[];reference_output=[];errors=[]
 def service(m,r,a,trace,output):
  trace.append((a,tuple(r[i] for i in range(4,8))))
  if a==0x8c10e50e:
   if r[5]>4096:errors.append(("oversized emission",hex(r[4]),hex(r[5]),hex(r[6])));r[0]=0;return
   data=bytes(m[(r[4]&0xffffff)+i] for i in range(r[5]));output.append(data)
   pos=(r[6]&0xffffff)+8;value=sum(m[pos+i]<<(8*i) for i in range(4))+len(data)
   for i in range(4):m[pos+i]=(value>>(8*i))&255
  elif a not in (0x8c10e576,0x8c10e566):errors.append(hex(a))
  r[0]=0
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10e50e:return emit(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10d25c:return move(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10e304:return integer(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10d3f0:return length(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  service(m,r,a,calls,actual_output);return -9 if errors else 0
 def hook(a):
  def run(m,r,sr):service(m,r,a,events,reference_output);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[5]=0x8ca01000;s.r[6]=0x8ca02000;s.r[7]=0x22340007;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10d400
 struct.pack_into('<I',ram,0xf00000,0x8ca02000);struct.pack_into('<I',native,0xf00000,0x8ca02000)
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0}
 reference(ram,s,special=special,fpu=f,instruction_budget=100000,hooks={})
 assert wrapper(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,(text,args,errors)
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and calls==events and actual_output==reference_output and macl.value==special["macl"]
 assert native[0xa00000:0xa00000+len(expected_text)]==expected_text and r[0]==len(expected_text),(text,args,expected_text,r[0])
 assert native[0xa00000+len(expected_text):0xa00000+len(expected_text)+16]==bytes(16)
 cases+=1
(out/'analysis/sound_debug_format_native_engine_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native17-instruction debug formatting wrapper and878-instruction formatting engine, full RAM/CPU/FPU/SR/PR and remaining service traces match retail execution for mixed string/character/integer conversions, string/character width, string precision and dynamic string width/precision, embedded NUL and byte255, with native257-instruction converter and native quotient/remainder helpers in both FPU banks. Independent emitted text/count checked. Native44-instruction emitter and native memory move connected, independent actual output buffer/count and tail guard checked. Native8-instruction finish and11-instruction readiness helpers connected; no modeled services in these buffer-mode cases; additional argument conversions, floating formatting, FILE mode, actual I/O pending.'),indent=2)+'\n')
print(cases,'debug formatting native engine comparisons passed')

