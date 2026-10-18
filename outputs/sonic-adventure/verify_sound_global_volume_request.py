import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_global_volume_request.dll')).sa_sound_global_volume_request
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]
def word(a):return struct.unpack_from('<h',binary,a-0x8c010000)[0]
commit=lit(0x8c6437e4);error=lit(0x8c6437ec);request=lit(0x8c6437d8)&0xffffff
cases=0
for channel,level,version,enabled,bank in itertools.product([-1,0,1,15,16],[0,127,128,255],[38,39],[0,1],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c6437cc)&0xffffff,0x8ca09000 if enabled else 0)
 ram[0xa09000+word(0x8c6437c6)]=version
 ram[request:request+20]=bytes([0x55])*20
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):actual.append((a,tuple(r[i] for i in range(4,8))));r[0]=0;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,tuple(r[i] for i in range(4,8))));r[0]=0;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel&0xffffffff;s.r[5]=0;s.r[6]=level;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c643754
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={commit:hook(commit),error:hook(error)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and actual==events
 valid=enabled and 0<=channel<=15
 assert sum(a==commit for a,args in actual)==int(valid)
 if valid:
  expected=bytearray([0x55]*20);struct.pack_into('<I',expected,0,word(0x8c6437c8)&0xffffffff)
  expected[(lit(0x8c6437dc)&0xffffff)-request]=channel
  expected[6]=word(0x8c6437ca)&255
  signed=ctypes.c_int8(level).value
  expected[(lit(0x8c6437e0)&0xffffff)-request]=((signed>>3 if version<39 else signed)+word(0x8c6437ca))&255
  assert native[request:request+20]==expected
  assert r[0]==0
 else:
  assert native[request:request+20]==bytes([0x55])*20
  assert r[0]==lit(0x8c6437d4 if enabled else 0x8c6437d0)
 cases+=1
(out/'analysis/sound_global_volume_request_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='57-instruction native/reference full RAM/CPU/FPU/SR/PR and lower-service trace comparisons. Independent channel validation, disabled context errors, signed-byte level conversion, version38/39 scaling branch and exact20-byte request bytes. Request commit/error reporting modeled; no actual queue or audio.'),indent=2)+'\n')
print(cases,'global volume request comparisons passed')
