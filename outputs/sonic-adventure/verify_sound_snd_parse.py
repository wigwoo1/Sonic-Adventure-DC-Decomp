import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_snd_parse.dll'))
fn=dll.sa_sound_snd_parse;fn.argtypes=abi
divide=dll.sa_unsigned_quotient;divide.argtypes=abi
cases=0
for tag,offset,size,encoding,rate,channels,bank in itertools.product([b'.snd',b'.sd\0',b'BAD!'],[24,25],[24,32],[0,1,2,3,4],[22050,0x80001234],[0,1,2],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=tag+struct.pack('>5I',offset,19,encoding,rate,channels)+bytes(range(16))
 ram[0xa20000:0xa20000+len(data)]=data
 ram[0xa30000:0xa30040]=bytes([0x55])*64
 struct.pack_into('<3I',ram,0xf00000,0x8ca30008,0x8ca3000c,0x8ca30010)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)]
 s.r[4:8]=[0x8ca20000,size,0x8ca30000,0x8ca30004];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c669c14
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10cb10,a
  calls.append(a);return divide(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'],(tag,offset,size,encoding,rate,channels,bank)
 valid=tag in (b'.snd',b'.sd\0') and offset<=size and encoding in (1,2,3)
 target=bytearray([0x55])*64
 if valid:
  frame_bytes=19 if encoding!=3 else 19//2
  frames=frame_bytes//channels if channels else 0
  struct.pack_into('<5I',target,0,rate,channels,16 if encoding==3 else 8,frames,{1:2,2:1,3:0}[encoding])
 assert native[0xa30000:0xa30040]==target
 assert r[0]==(0x8ca20000+offset if valid else 0)
 assert len(calls)==int(valid)
 assert native[0xa20000:0xa20000+len(data)]==data
 cases+=1
report=dict(cases_passed=cases,scope='Native274-instruction SND parser669c14 with native unsigned quotient10cb10, no modeled calls. Constructed headers .snd/.sd-NUL/BAD, offsets24/25, provided sizes24/32, encodings0..4, rates22050/high-bit80001234, channels0/1/2, both FPU banks. Full retail RAM/CPU/FPU/SR/PR/MACL equality; independent return pointer, five metadata words, unsigned division including zero denominator, rejection/output guards and input preservation. Arbitrary/truncated headers, asset integration, playback/fullgame unverified.')
(out/'analysis/sound_snd_parse_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'SND parser comparisons passed')
