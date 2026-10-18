import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,level,bank,answer in itertools.product([0,1,3],[-32769,-960,-10,0,32767,65535,65536],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c6681e8;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_volume.dll')).sa_sound_driver_volume
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08080]=bytes([0x55])*128;struct.pack_into('<I',ram,0xa08040,0x8ca09000)
 struct.pack_into('<I',ram,0xa08008,channels);struct.pack_into('<I',ram,0xa08028,0x8ca09000)
 target=struct.unpack_from('<I',binary,0x6682f8-0x10000)[0];error_target=struct.unpack_from('<I',binary,0x6682f4-0x10000)[0]
 division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=fn.argtypes
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=level&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 def truncdiv(a,b):return (abs(a)//abs(b))*(-1 if (a<0)!=(b<0) else 1)
 converted=-127 if level<=-999 else 127 if level>=0 else max(-127,truncdiv(truncdiv(128-255*level,-999)*10,18))
 driver_calls=[x for x in calls if x[0]==target]
 assert driver_calls==[(target,0x8ca09000,i,converted&0xffffffff,0) for i in range(1 if answer and channels else channels)]
 assert len([x for x in calls if x[0]==error_target])==int(bool(answer and channels))
 cases+=1
(out/'analysis/sound_driver_volume_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native69-instruction handle volume driver volume6681e8, signed/boundary levels,both banks and lower returns. Full retail state; independent integer level conversion,channel0/1/3 iteration,early error handling; signed division native. Lower service modeled; audio unverified.'),indent=2)+'\n')
print(cases,'driver volume comparisons passed')
