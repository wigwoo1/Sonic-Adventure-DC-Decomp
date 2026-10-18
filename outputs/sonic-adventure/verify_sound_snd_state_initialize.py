import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_snd_state_initialize.dll'))
fn=dll.sa_sound_snd_state_initialize;fn.argtypes=abi
lower={0x8c669e6a:dll.sa_sound_snd_metadata,0x8c669c14:dll.sa_sound_snd_parse,0x8c10cb10:dll.sa_unsigned_quotient,0x8c10d31c:dll.sa_memory_fill}
for sub in lower.values():sub.argtypes=abi
cases=0
for tag,size,offset,encoding,channels,bank in itertools.product([b'.snd',b'.sd\0',b'BAD!'],[7,8,32,65560,0xffffffff],[0,24,32768,65560],[1,2,3,4],[1,2,128],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=tag+struct.pack('>5I',offset,19,encoding,22050,channels)+bytes(range(16))
 ram[0xa10000:0xa10000+len(data)]=data;ram[0xa20000:0xa20100]=bytes([0x55])*256
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca20000,0x8ca10000,size];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c669f2e
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a in lower,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'],(tag,size,offset,encoding,channels,bank)
 short_offset=ctypes.c_int16(offset&65535).value
 accepted=8<=size<0x80000000 and tag in (b'.snd',b'.sd\0') and offset<=size and encoding in (1,2,3) and short_offset>0
 target=bytearray([0x55])*256;struct.pack_into('<H',target,2,1)
 if accepted:
  bits=16 if encoding==3 else 8;signed_channels=ctypes.c_int8(channels).value;stride=int(signed_channels*bits/8)&255
  target[12]=255;target[13]=bits;target[14]=channels;target[15]=stride
  struct.pack_into('<3I',target,16,1,22050,(19 if encoding!=3 else 9)//channels)
  for x in (0x1c,0x48,0x4a,0x26,0x24,0x7c,0x7e):struct.pack_into('<H',target,x,0)
  for x in (0x34,0x30,0x2c,0x28,0x20,0x4c,0x98,0x9c):struct.pack_into('<I',target,x,0)
  struct.pack_into('<3I',target,0x58,signed_channels&0xffffffff,ctypes.c_int8(stride).value&0xffffffff,1)
  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)
  struct.pack_into('<I',target,0x80,0x8ca2004c);struct.pack_into('<H',target,0xa8,4);struct.pack_into('<H',target,0xaa,{1:2,2:1,3:0}[encoding])
 assert r[0]==(short_offset if accepted else 0)
 assert native[0xa20000:0xa20100]==target,(tag,size,offset,encoding,channels,[(hex(i),native[0xa20000+i],target[i]) for i in range(256) if native[0xa20000+i]!=target[i]])
 assert native[0xa10000:0xa10000+len(data)]==data
 cases+=1
(out/'analysis/sound_snd_state_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native116-instruction SND stream-state initializer669f2e with native metadata/parser/unsigned quotient/memory fill; no modeled calls.1440 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Both valid tags/invalid tag,signed size thresholds,offset0/24/32768/65560,encodings1..4,channels1/2/128,banks. Independent complete256-byte state including preserved rejection bytes,metadata,reset fields,signed channel/stride extension,copied defaults,internal pointer,format4,encoding selector and returned signed-short offset. Constructed fixtures only; loader/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'SND state comparisons passed')
