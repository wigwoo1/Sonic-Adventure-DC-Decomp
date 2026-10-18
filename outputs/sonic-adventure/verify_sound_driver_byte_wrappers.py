import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for kind,level,auxiliary,bank,answer in itertools.product(['setup','level'],[-129,-128,-1,0,127,128,255],[0,127,255],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c669470 if kind=='setup' else 0x8c669488;handle=0x8ca08000
 fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/sound_driver_{kind}_wrapper.dll')),f'sa_sound_driver_{kind}_wrapper')
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 target=struct.unpack_from('<I',binary,(0x669484 if kind=='setup' else 0x669494)-0x10000)[0]
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5],r[6]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5],r[6]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=17;s.r[6]=level&0xffffffff;s.r[7]=auxiliary;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (target,)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 expected_level=ctypes.c_int8(level).value&0xffffffff
 expected_aux=ctypes.c_int8(auxiliary).value&0xffffffff if kind=='setup' else level&0xffffffff
 assert calls==events==[(target,handle,expected_level,expected_aux)] and macl.value==special['macl']
 assert r[0]==answer and r[15]==0x8cf00000 and pr.value==0x8c000100
 cases+=1
(out/'analysis/sound_driver_byte_wrappers_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native10-instruction setup669470 and6-instruction level669488 wrappers. Signed byte truncation,auxiliary byte conversion,tails preserve caller PR/stack,full retail state across banks/lower returns. Lower canonical services modeled; audio unverified.'),indent=2)+'\n')
print(cases,'driver byte wrapper comparisons passed')
