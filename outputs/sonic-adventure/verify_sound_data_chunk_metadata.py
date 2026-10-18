import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_data_chunk_metadata.dll'))
fn=dll.sa_sound_data_chunk_metadata;fn.argtypes=abi
lower={0x8c10ca5c:dll.sa_signed_division,0x8c10cb10:dll.sa_unsigned_quotient}
for sub in lower.values():sub.argtypes=abi
cases=0
def signed_div(n,d):
 if not d:return 0
 return ((abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1))&0xffffffff
for words,position,bits,stride,channels,length,bank in itertools.product([0,1,12,16],[None,0,10],[4,8,16,7],[0,2,128],[1,2,128],[19,0x80000001],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=bytearray([0x77])*96;struct.pack_into('<H',data,22,channels);struct.pack_into('<I',data,24,22050);struct.pack_into('<H',data,32,stride);struct.pack_into('<H',data,34,bits)
 if position is not None:data[position*4:position*4+4]=b'data';struct.pack_into('<I',data,position*4+4,length)
 ram[0xa10000:0xa10060]=data;ram[0xa20000:0xa20040]=bytes([0x55])*64
 struct.pack_into('<7I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24,28,32)])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca10000,words*4+3,0x8ca20000,0x8ca20004];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c66a63c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a in lower,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 found=position is not None and position<words;target=bytearray([0x55])*64
 if found:
  struct.pack_into('<H',target,0,(position+2)*4);target[4]=255;target[8]=bits;target[12]=stride;target[16]=channels
  frames=signed_div(ctypes.c_int32(length).value,ctypes.c_int8(stride).value);kind=1
  if bits==4:
   target[8]=16;target[12]=(channels*2)&255;kind=4
   denom=ctypes.c_int8(channels).value&0xffffffff;frames=(length>>1)//denom if denom else 0
  struct.pack_into('<3I',target,20,22050,frames,kind)
  if bits in (4,8,16):struct.pack_into('<H',target,32,{4:2,8:1,16:0}[bits])
 assert r[0]==(0 if found else 0xffffffff) and native[0xa20000:0xa20040]==target,(words,position,bits,stride,channels,length)
 assert native[0xa10000:0xa10060]==data
 cases+=1
(out/'analysis/sound_data_chunk_metadata_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native108-instruction data-chunk audio metadata66a63c with native signed division10ca5c and unsigned quotient10cb10; no modeled calls.1728 full retail RAM/CPU/FPU/SR/PR/MACL comparisons: word-limit0/1/12/16 plus3 ignored trailing bytes,marker absent/index0/index10,width4/8/16/7,stride0/2/128,channels1/2/128,data length19/80000001,banks. Independent marker bound,offset,rate/channel/width/stride, signed frame division and4-bit normalization/unsigned division,selector/unknown preservation,guards and source preservation. Marker at final allowed word and actual asset integration not yet tested; playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'data-chunk metadata comparisons passed')
