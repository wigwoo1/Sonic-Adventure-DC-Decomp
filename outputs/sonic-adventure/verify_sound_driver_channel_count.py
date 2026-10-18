import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for level,bank,answer in itertools.product([-129,-128,-1,0,1,2,3,127,128,255],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c66846a;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_channel_count.dll')).sa_sound_driver_channel_count
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 division_target=0x8c10ca5c;level_target=0x8c6694b8;error_target=0x8c667b7a
 division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=fn.argtypes
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==division_target:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==level_target:assert struct.unpack_from('<i',native,0xa08008)[0]==ctypes.c_int8(level).value
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=level&0xffffffff;s.r[6]=0x12345678;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (level_target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 converted=ctypes.c_int8(level).value
 assert calls==events and calls[0]==(level_target,0x8ca09000,level&0xffffffff,0x12345678,0x22340007)
 assert struct.unpack_from('<i',native,0xa08008)[0]==converted
 assert len(calls)==(2 if answer else 1) and r[0]==answer
 assert struct.unpack_from('<I',native,0xa08040)[0]==0x55
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_channel_count_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native23-instruction channel-count setter66846a. Signed-byte truncation boundaries,both FPU banks,lower success/error. Full retail RAM/CPU/FPU/SR/PR/MACL and service traces match. Independent count field write before lower call,lower arguments,error count,result and cached-field preservation checked. Lower channel-count and diagnostic services modeled; real audio unverified.'),indent=2)+'\n')
print(cases,'driver channel count comparisons passed')
