import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_snd_metadata.dll'))
fn=dll.sa_sound_snd_metadata;fn.argtypes=abi
lower={0x8c669c14:dll.sa_sound_snd_parse,0x8c10cb10:dll.sa_unsigned_quotient}
for sub in lower.values():sub.argtypes=abi
cases=0
for tag,size,offset,encoding,channels,bank in itertools.product([b'.snd',b'.sd\0',b'BAD!'],[7,8,32,65560,0xffffffff],[0,24,32768,65560],[1,2,3,4],[1,2,128],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=tag+struct.pack('>5I',offset,19,encoding,22050,channels)+bytes(range(16))
 ram[0xa10000:0xa10000+len(data)]=data;ram[0xa20000:0xa20040]=bytes([0x55])*64
 struct.pack_into('<7I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24,28,32)])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca10000,size,0x8ca20000,0x8ca20004];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c669e6a
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a in lower,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'],(tag,size,offset,encoding,channels,bank)
 eligible=8<=size<0x80000000
 parsed=eligible and tag in (b'.snd',b'.sd\0') and offset<=size and encoding in (1,2,3)
 short_offset=ctypes.c_int16(offset&65535).value
 accepted=parsed and short_offset>0
 target=bytearray([0x55])*64
 if not eligible:struct.pack_into('<H',target,0,0)
 if parsed:
  struct.pack_into('<H',target,0,offset&65535)
  struct.pack_into('<I',target,32,{1:2,2:1,3:0}[encoding])
 if accepted:
  bits=16 if encoding==3 else 8
  signed_channels=ctypes.c_int8(channels).value
  stride=int(signed_channels*bits/8)&255
  target[4]=255;target[8]=bits;target[12]=stride;target[16]=channels&255
  struct.pack_into('<3I',target,20,22050,(19 if encoding!=3 else 9)//channels,1)
 assert r[0]==(0 if accepted else 0xffffffff)
 assert native[0xa20000:0xa20040]==target,(tag,size,offset,encoding,channels,bytes(native[0xa20000:0xa20040]),target)
 assert native[0xa10000:0xa10000+len(data)]==data
 cases+=1
(out/'analysis/sound_snd_metadata_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native79-instruction SND metadata wrapper669e6a with native parser669c14 and quotient10cb10; no modeled calls.1440 full retail RAM/CPU/FPU/SR/PR/MACL comparisons across both tags/invalid tag, signed size7/8/32/65560/ffffffff, offsets0/24/32768/65560, encodings1..4,channels1/2/128,banks. Independent rejection/preserved outputs,signed short offset and signed byte stride,metadata/format output and input guards. Offsets32768 and65560 tested with sufficient provided size: signed-short rejection and wrapped-positive24 acceptance independently checked. Constructed fixtures only; playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'SND metadata comparisons passed')

