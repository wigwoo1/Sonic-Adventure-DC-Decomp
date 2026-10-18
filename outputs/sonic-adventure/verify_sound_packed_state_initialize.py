import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_packed_state_initialize.dll'))
fn=dll.sa_sound_packed_state_initialize;fn.argtypes=abi
lower={0x8c66a378:dll.sa_sound_packed_header_metadata,0x8c10d31c:dll.sa_memory_fill}
for sub in lower.values():sub.argtypes=abi
cases=0
for encoding,offset_unit,channel_byte,length,rate,bank in itertools.product([0,1,2,3,4,128,255],[0,1,255],[0,127,255],[0,1,0x7fffffff,0x80000000,0xffffffff],[22050,65535],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=bytearray([0x77])*64;data[7]=offset_unit;data[8]=encoding;data[9]=channel_byte;struct.pack_into('<I',data,12,length);struct.pack_into('<H',data,42,rate)
 ram[0xa10000:0xa10040]=data;ram[0xa20000:0xa20100]=bytes([0x55])*256
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca20000,0x8ca10000,3];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c66a440
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a in lower,a
  return lower[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 target=bytearray([0x55])*256;struct.pack_into('<H',target,2,1)
 channels=(channel_byte+1)&255;target[12:16]=bytes([255,16,channels,2])
 signed_length=ctypes.c_int32(length).value;half=(abs(signed_length)//2)*(-1 if signed_length<0 else 1)
 struct.pack_into('<3I',target,16,1,rate,half&0xffffffff)
 for x in (0x1c,0x48,0x4a,0x26,0x24,0x7c,0x7e):struct.pack_into('<H',target,x,0)
 for x in (0x34,0x30,0x2c,0x28,0x20,0x4c,0x98,0x9c):struct.pack_into('<I',target,x,0)
 struct.pack_into('<3I',target,0x58,ctypes.c_int8(channels).value&0xffffffff,2,1)
 for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)
 struct.pack_into('<I',target,0x80,0x8ca2004c);struct.pack_into('<H',target,0xa8,2)
 if encoding<=3:struct.pack_into('<H',target,0xaa,{0:0,1:1,2:2,3:2}[encoding])
 assert r[0]==offset_unit*16 and native[0xa20000:0xa20100]==target,(encoding,offset_unit,channel_byte,length,rate)
 assert native[0xa10000:0xa10040]==data
 cases+=1
(out/'analysis/sound_packed_state_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native packed audio-state initializer66a440 with native metadata66a378 and memory fill10d31c; no modeled calls.1260 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Encoding0/1/2/3/4/128/255,offset-unit0/1/255,channel-byte0/127/255,signed length extremes,rate22050/65535,banks. Independent complete256-byte state: normalized metadata,resets,signed channel extension,copied defaults,internal pointer,format2,encoding selector or preserved unknown and returned offset. Provided size3 demonstrates retail metadata has no size validation; truncated data not tested. Source format identity/loader/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'packed audio state comparisons passed')
