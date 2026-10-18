import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,dllname,name in [(0x8c669470,'sound_driver_setup_wrapper','sa_sound_driver_setup_wrapper'),(0x8c669488,'sound_driver_level_wrapper','sa_sound_driver_level_wrapper')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,level,cached,bank,answer in itertools.product([0,1,3],[-43,-42,-1,0,1],[0,7],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c6682be;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_control_6682be.dll')).sa_sound_driver_control_6682be
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08008,channels);struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,cached)
 setup_target=struct.unpack_from('<I',binary,0x668404-0x10000)[0];level_target=struct.unpack_from('<I',binary,0x66840c-0x10000)[0];error_target=struct.unpack_from('<I',binary,0x668400-0x10000)[0]
 setup_lower=struct.unpack_from('<I',binary,0x669484-0x10000)[0];level_lower=struct.unpack_from('<I',binary,0x669494-0x10000)[0]
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5],r[6]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5],r[6]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=7;s.r[6]=level&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (setup_lower,level_lower,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 converted=-127 if level<-42 else 127 if level>=0 else 2*level-(abs(level)//2)
 wanted=[]
 if cached!=7:wanted.extend((setup_lower,0x8ca09000,7,127) for i in range(channels))
 wanted.extend((level_lower,0x8ca09000,ctypes.c_int8(converted).value&0xffffffff,converted&0xffffffff) for i in range(channels))
 assert calls==events and [x for x in calls if x[0]!=error_target]==wanted
 assert len([x for x in calls if x[0]==error_target])==(len(wanted) if answer else 0)
 assert struct.unpack_from('<I',native,0xa08040)[0]==7 and r[0]==(answer if channels else 0)
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_control_native_wrappers_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native81-instruction driver control6682be. Channels0/1/3,level threshold-43/-42/-1/0/1,cached control equal/different,banks,lower returns. Full retail state and independent setup-on-change/cache update,integer level conversion,all-channel iteration and error call counts; native setup/level byte wrappers connected and independent lower argument conversion checked; canonical/error services modeled, precise game meaning/audio unverified.'),indent=2)+'\n')
print(cases,'driver control native wrapper comparisons passed')
