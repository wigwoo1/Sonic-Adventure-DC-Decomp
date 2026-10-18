import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for present,level,bank,answer in itertools.product([0,1],[-32769,-960,-10,0,32767,65535,65536],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c65dc64;handle=0x8ca08000 if present else 0
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_volume.dll')).sa_sound_subordinate_volume
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08080]=bytes([0x55])*128;struct.pack_into('<I',ram,0xa08040,0x8ca09000)
 target=0x8c6681e8;error_target=0x8c667b7a;minimum=struct.unpack_from('<h',binary,0x65dd66-0x10000)[0]
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=level&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 if present:
  stored=0 if level>=0 else max(level,minimum)
  assert struct.unpack_from('<i',native,0xa0806c)[0]==stored
  assert calls[0]==(target,0x8ca09000,stored&0xffffffff)
  assert len(calls)==(2 if answer else 1)
 else:assert len(calls)==1 and calls[0][0]==error_target
 cases+=1
(out/'analysis/sound_subordinate_volume_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native41-instruction handle volume subordinate volume65dc64, signed/boundary levels,both banks and lower returns. Full retail state; independent signed level clamp stored at object+108,object+64 forwarded,optional null object and error-return paths. Lower service modeled; audio unverified.'),indent=2)+'\n')
print(cases,'subordinate volume comparisons passed')
