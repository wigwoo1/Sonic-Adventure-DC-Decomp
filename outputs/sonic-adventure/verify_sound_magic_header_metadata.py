import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_magic_header_metadata.dll'));fn=dll.sa_sound_magic_header_metadata;fn.argtypes=abi;divide=dll.sa_signed_division;divide.argtypes=abi
cases=0
for magic,size,stride,bits,offset,rate,bank in itertools.product([0x8000,0x8001],[15,16,0xffffffff],[2,18,128],[0,4,128],[24,65535],[22050,0x80000001],[0,0x200000]):
 data=struct.pack('>HH4B2I',magic,offset,3,stride,bits,2,rate,0x81234567)
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10010]=data;ram[0xa20000:0xa20040]=bytes([0x55])*64
 struct.pack_into('<6I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24,28)])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca10000,size,0x8ca20000,0x8ca20004];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65ae44
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10ca5c,a
  return divide(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 valid=size==16 and magic==0x8000;target=bytearray([0x55])*64
 if valid:
  struct.pack_into('<H',target,0,(offset+4)&65535);target[4]=3;target[8]=bits;target[12]=stride;target[16]=2
  n=(ctypes.c_int8(stride).value-2)*8;d=ctypes.c_int8(bits).value;q=(abs(n)//abs(d))*(-1 if (n<0)!=(d<0) else 1) if d else 0
  struct.pack_into('<3I',target,20,rate,0x81234567,q&0xffffffff)
 assert native[0xa20000:0xa20040]==target and r[0]==(0 if valid else 0xffffffff if size!=16 else 0xfffffffe)
 assert native[0xa10000:0xa10010]==data
 cases+=1
(out/'analysis/sound_magic_header_metadata_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native127-instruction magic8000 audio metadata65ae44 with native signed division10ca5c,no modeled calls.432 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Magic8000/8001,signed size15/16/ffffffff,stride2/18/128,width0/4/128,offset24/65535,rate22050/80000001,banks. Independent distinct size/signature rejection,preserved outputs,BE16 offset+4 wrap,raw bytes4..7,BE32 rate/length and signed((stride-2)*8)/width including zero denominator. Constructed16-byte headers; actual encoded files/decoder/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'magic audio metadata comparisons passed')
