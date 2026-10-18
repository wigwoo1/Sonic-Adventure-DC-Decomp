import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/format_emit.dll')).sa_format_emit
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
move=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_request_enqueue.dll')).sa_memory_move;move.argtypes=fn.argtypes
cases=0
for mode,count,bank,fail in itertools.product([0,1],[-1,0,1,7],[0,0x200000],[-1,3]):
 if mode==1 and count<0:continue
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=bytes([65,128,255,68,69,70,71]);ram[0xa00000:0xa00007]=data;ram[0xa01000:0xa01010]=bytes([0x55])*16
 struct.pack_into('<4I',ram,0xa02000,mode,0x8ca01000,7,0x8cba0000)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10d25c:return move(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5]));r[0]=0xffffffff if len(calls)-1==fail else 0;return 0
 def hook(m,r,sr):events.append((0x8cba0000,r[4],r[5]));r[0]=0xffffffff if len(events)-1==fail else 0;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[5]=count&0xffffffff;s.r[6]=0x8ca02000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10e50e
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cba0000:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and calls==events
 processed=max(0,count) if mode==1 or fail<0 else min(max(0,count),fail)
 assert struct.unpack_from('<2I',native,0xa02004)==(0x8ca01000+(count if mode==1 else 0),7+processed)
 if mode==1:assert native[0xa01000:0xa01010]==data[:count]+bytes([0x55])*(16-count)
 else:
  attempts=processed+int(0<=fail<count)
  assert calls==[(0x8cba0000,ctypes.c_int8(data[i]).value&0xffffffff,0x8ca01000) for i in range(attempts)]
 cases+=1
(out/'analysis/format_emit_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='44-instruction native/reference full RAM/CPU/FPU/SR/PR and callback trace. Native memory move in buffer mode; lengths0/1/7 and callback mode signed-1, both banks, callback failure at3 or never. Independent copied bytes/guards,destination/count, signed callback bytes and early failure checked. Buffer-mode negative length not executed; callback body modeled.'),indent=2)+'\n')
print(cases,'format emitter comparisons passed')
