import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for kind,first,second,bank,answer in itertools.product(['mute','restore'],[0,0x8ca08000],[0,0x8ca08100],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c107bf2 if kind=='mute' else 0x8c107c22
 fn=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/sound_{kind}_service.dll')),f'sa_sound_{kind}_service')
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c107c78),first);struct.pack_into('<I',ram,lit(0x8c107c68),second)
 target=struct.unpack_from('<I',binary,0x107c7c-0x10000)[0]
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (target,)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 expected=[]
 if first:expected.append((target,first,0xfffffc40 if kind=='mute' else 0xfffffff6))
 if second:expected.append((target,second,0xfffffc40 if kind=='mute' else 0))
 assert calls==events==expected and macl.value==special['macl']
 cases+=1
(out/'analysis/sound_mute_restore_services_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native24-instruction mute and19-instruction restore final services. All zero/nonzero combinations of two handles, both FPU banks and lower-service returns0/123. Full original RAM/CPU/FPU/SR/PR/MACL comparison; independent ordered calls with mute-960,restore first-10/second0. Lower volume service modeled; actual audio unverified.'),indent=2)+'\n')
print(cases,'mute/restore final service comparisons passed')
