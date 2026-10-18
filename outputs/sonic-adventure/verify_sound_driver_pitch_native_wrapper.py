import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,coarse,fine,bank,answer in itertools.product([0,1,3],[-101,-100,-99,0,99,100,101],[-101,-100,-99,0,99,100,101],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c6683be;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_pitch.dll')).sa_sound_driver_pitch
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08008,channels);struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 division_target=0x8c10ca5c;wrapper_target=0x8c6694a4;level_target=0x8c66b504;error_target=0x8c667b7a
 division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=fn.argtypes
 wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_pitch_wrapper.dll')).sa_sound_driver_pitch_wrapper;wrapper.argtypes=fn.argtypes
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==wrapper_target:return wrapper(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==division_target:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=coarse&0xffffffff;s.r[6]=fine&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (level_target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 coarse_value=max(-100,min(100,coarse))*256
 fine_value=-256 if fine<=-100 else 256 if fine>=100 else (abs(fine*256)//100)*(-1 if fine<0 else 1)
 converted=(coarse_value+fine_value)&0xffffffff
 wanted=[(level_target,0x8ca09000,ctypes.c_int16(converted).value&0xffffffff,0,0) for i in range(channels)]
 assert calls==events and [x for x in calls if x[0]!=error_target]==wanted
 assert len([x for x in calls if x[0]==error_target])==(channels if answer else 0)
 assert r[0]==(answer if channels else 0x2234000d)
 assert struct.unpack_from('<I',native,0xa08040)[0]==0x55
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_pitch_native_wrapper_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native68-instruction driver pitch6683be with native signed division10ca5c. Channels0/1/3, coarse/fine signed short thresholds-101/-100/-99/0/99/100/101, FPU banks and lower returns. Complete retail RAM/CPU/FPU/SR/PR/MACL and service traces matched; independent coarse clamp*256 plus fine clamp/truncating256*value/100 conversion, all channel arguments, non-short-circuit errors, unchanged cached field and zero-channel inherited R13 return verified. Native8-instruction pitch wrapper6694a4 connected; signed16-bit conversion, auxiliary argument forwarding and caller PR/stack preserved. Canonical pitch66b504/error services modeled; real audio unverified.'),indent=2)+'\n')
print(cases,'driver pitch native wrapper comparisons passed')
