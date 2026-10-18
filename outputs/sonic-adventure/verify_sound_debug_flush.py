import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_debug_flush.dll')).sa_sound_debug_flush
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for callback,mode,bank in itertools.product([0,0x8cba0000],[0,1,2,0xffffffff],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c65a368),callback);struct.pack_into('<I',ram,lit(0x8c65a36c),mode)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,tuple(r[i] for i in range(4,8))));r[0]=123;return 0
 def hook(m,r,sr):events.append((0x8cba0000,tuple(r[i] for i in range(4,8))));r[0]=123;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65a33e
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cba0000:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and calls==events
 assert len(calls)==int(bool(callback) and mode==1) and r[0]==0
 cases+=1
(out/'analysis/sound_debug_flush_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='16-instruction native/reference full RAM/CPU/FPU/SR/PR and callback argument traces. Independent callback null/non-null, modes0/1/2/ffffffff and both FPU banks; callback runs exactly once only with non-null pointer and mode1, return0 despite callback return123. Callback body modeled.'),indent=2)+'\n')
print(cases,'debug flush comparisons passed')
