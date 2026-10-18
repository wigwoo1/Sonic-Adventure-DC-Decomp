import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,selector,level,bank,answer in itertools.product([1,2],[0,1,2,3],[-128,-1,0,127,255],[0,0x200000],[0,123]):
 if channels==1 and selector==1:continue
 sr0=0x70000063;entry=0x8c66b374
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_pan_request.dll')).sa_sound_pan_request
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c66b46c),0x8ca09000);ram[0xa08001]=0x42
 voice=0xa09000+struct.unpack_from('<h',binary,0x66b466-0x10000)[0]+0x42*44
 ram[voice+5]=channels
 target=0x8c65a464;error_target=0x8c65acac
 packet=lit(0x8c66b478);expected=bytearray(ram[packet:packet+20]);struct.pack_into('<I',expected,0,0x26)
 expected[4]=ram[lit(0x8c66b480)+(selector&3)]|0x42
 signed_level=ctypes.c_int8(level).value;bias=struct.unpack_from('<h',binary,0x66b468-0x10000)[0]
 auxiliary=0x89abcdef;levels=[signed_level]*channels
 if channels==2:
  if selector==0:levels[0]+=bias
  if selector==1:levels[1]=min(levels[1]-bias,-2)
  levels=[abs(v)//2*(-1 if v<0 else 1) for v in levels]
 levels=[v+bias for v in levels]
 if selector in (0,1):
  for i in range(6):expected[5+i+(i//3)]=0
  off=5+4*selector
  expected[off:off+3]=bytes([levels[selector]&255,auxiliary&255,(auxiliary>>8)&255])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=selector&0xffffffff;s.r[6]=level&0xffffffff;s.r[7]=auxiliary;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 assert native[packet:packet+20]==expected
 assert struct.unpack_from('<i',native,voice+28+4*selector)[0]==signed_level
 assert calls[0][0:2]==(target,0x8c000000+packet) and len(calls)==(2 if answer else 1)
 cases+=1
(out/'analysis/sound_pan_request_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native165-instruction pan command26 builder. One/two-channel state,selectors0/1/2/3,signed byte boundaries,banks,commit returns; single-channel selector1 excluded because it reads an uninitialized second stack word. Full RAM/CPU/FPU/SR/PR/MACL and traces match retail execution. Independent voice state update and entire20-byte packet including stereo conversion,lookup identifier,auxiliary bytes and untouched bytes checked. Commit and error services modeled; real audio unverified.'),indent=2)+'\n')
print(cases,'pan request comparisons passed')
