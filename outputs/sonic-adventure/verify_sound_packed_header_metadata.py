import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_packed_header_metadata.dll')).sa_sound_packed_header_metadata;fn.argtypes=abi
cases=0
for encoding,offset_unit,channel_byte,length,rate,bank in itertools.product([0,1,2,3,4,128,255],[0,1,255],[0,127,255],[0,1,0x7fffffff,0x80000000,0xffffffff],[22050,65535],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 data=bytearray([0x77])*64;data[7]=offset_unit;data[8]=encoding;data[9]=channel_byte;struct.pack_into('<I',data,12,length);struct.pack_into('<H',data,42,rate)
 ram[0xa10000:0xa10040]=data;ram[0xa20000:0xa20040]=bytes([0x55])*64
 struct.pack_into('<7I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24,28,32)])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca10000,3,0x8ca20000,0x8ca20004];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c66a378
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(*args):raise AssertionError('Unexpected external service')
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 target=bytearray([0x55])*64;struct.pack_into('<H',target,0,offset_unit*16)
 target[4]=255;target[8]=16;target[12]=2;target[16]=(channel_byte+1)&255
 signed_length=ctypes.c_int32(length).value
 half=(abs(signed_length)//2)*(-1 if signed_length<0 else 1)
 struct.pack_into('<3I',target,20,rate,half&0xffffffff,1)
 if encoding<=3:struct.pack_into('<H',target,32,{0:0,1:1,2:2,3:2}[encoding])
 assert r[0]==0 and native[0xa20000:0xa20040]==target,(encoding,offset_unit,channel_byte,length,rate)
 assert native[0xa10000:0xa10040]==data
 cases+=1
(out/'analysis/sound_packed_header_metadata_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native100-instruction packed audio metadata66a378; no external calls.1260 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Encoding0/1/2/3/4/128/255,offset-unit0/1/255,channel-byte0/127/255,signed length0/1/7fffffff/80000000/ffffffff,rate22050/65535,banks. Independent offset byte7*16,channel byte9+1 wrap,rateLE16 at42,final width16/stride2/type1,frames signed lengthLE32 at12 divided2 towardzero,encoding selector0/1/2 or preserved unknown,all output guards and source preservation. Source format identity/header validation/loader/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'packed audio metadata comparisons passed')
