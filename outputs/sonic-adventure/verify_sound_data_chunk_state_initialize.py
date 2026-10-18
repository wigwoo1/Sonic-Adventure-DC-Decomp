import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_data_chunk_state_initialize.dll'))
fn=dll.sa_sound_data_chunk_state_initialize;fn.argtypes=abi
lower={0x8c66a63c:dll.sa_sound_data_chunk_metadata,0x8c10ca5c:dll.sa_signed_division,0x8c10cb10:dll.sa_unsigned_quotient,0x8c10d31c:dll.sa_memory_fill}
for sub in lower.values():sub.argtypes=abi
cases=0
def signed_div(n,d):
 if not d:return 0
 return ((abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1))&0xffffffff
for words,position,bits,stride,channels,length,bank in itertools.product([0,1,12,16],[None,0,10],[4,8,16,7],[0,2,128],[1,2,128],[19,0x80000001],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=bytearray([0x77])*96;struct.pack_into('<H',data,22,channels);struct.pack_into('<I',data,24,22050);struct.pack_into('<H',data,32,stride);struct.pack_into('<H',data,34,bits)
 if position is not None:data[position*4:position*4+4]=b'data';struct.pack_into('<I',data,position*4+4,length)
 ram[0xa10000:0xa10060]=data;ram[0xa20000:0xa20100]=bytes([0x55])*256
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca20000,0x8ca10000,words*4+3];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c66a714
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a in lower,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 found=position is not None and position<words;target=bytearray([0x55])*256;struct.pack_into('<H',target,2,1)
 if found:
  width=bits;step=stride;frames=signed_div(ctypes.c_int32(length).value,ctypes.c_int8(stride).value);kind=1
  if bits==4:
   width=16;step=(channels*2)&255;kind=4
   denom=ctypes.c_int8(channels).value&0xffffffff;frames=(length>>1)//denom if denom else 0
  target[12:16]=bytes([255,width,channels,step]);struct.pack_into('<3I',target,16,kind,22050,frames)
  for x in (0x1c,0x48,0x4a,0x26,0x24,0x7c,0x7e):struct.pack_into('<H',target,x,0)
  for x in (0x34,0x30,0x2c,0x28,0x20,0x4c,0x98,0x9c):struct.pack_into('<I',target,x,0)
  struct.pack_into('<3I',target,0x58,ctypes.c_int8(channels).value&0xffffffff,ctypes.c_int8(step).value&0xffffffff,kind)
  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)
  struct.pack_into('<I',target,0x80,0x8ca2004c);struct.pack_into('<H',target,0xa8,1)
  if bits in (4,8,16):struct.pack_into('<H',target,0xaa,{4:2,8:1,16:0}[bits])
 assert r[0]==((position+2)*4 if found else 0) and native[0xa20000:0xa20100]==target,(words,position,bits,stride,channels,length)
 assert native[0xa10000:0xa10060]==data
 cases+=1
(out/'analysis/sound_data_chunk_state_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native113-instruction audio data-chunk state initializer66a714 with native metadata/signed and unsigned division/memory fill; no modeled calls.1728 full retail RAM/CPU/FPU/SR/PR/MACL comparisons: marker missing/index0/index10 with word-limit0/1/12/16,width4/8/16/7,stride0/2/128,channels1/2/128,length19/80000001,banks. Independent entire256-byte state,partial rejection preservation,4-bit normalization, signed extensions,resets,copied defaults,internal pointer,format1,selector/unknown preservation and returned offset. Constructed fixtures only; actual assets/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'data-chunk state comparisons passed')
