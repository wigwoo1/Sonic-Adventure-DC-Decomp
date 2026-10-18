import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_magic_stream_parameters.dll')).sa_sound_magic_stream_parameters;fn.argtypes=abi
cases=0
for magic,size,offset,flag,high,bank in itertools.product([0x8000,0x8001],[47,48,0xffffffff],[43,44,65535],[0,1,65535],[False,True],[0,0x200000]):
 words=[0x8001 if high else 0x1234,0xffff if high else 0x2345];values=[0x80000001,0xffffffff,0x12345678,0x87654321] if high else [1,2,3,4]
 data=bytearray([0x77])*48;struct.pack_into('>HH',data,0,magic,offset);struct.pack_into('>HH',data,20,words[0],flag);struct.pack_into('>H4I',data,26,words[1],*values)
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa10000:0xa10030]=data;ram[0xa20000:0xa20040]=bytes([0x55])*64
 struct.pack_into('<5I',ram,0xf00000,*[0x8ca20000+x for x in (8,12,16,20,24)])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=[0x8ca10000,size,0x8ca20000,0x8ca20004];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65afca
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 @Call
 def call(*args):raise AssertionError('Unexpected service')
 special={'macl':0};reference(ram,s,{},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 target=bytearray([0x55])*64;struct.pack_into('<H',target,4,0)
 parsed=size==48 and magic==0x8000 and offset>=44
 if parsed:
  struct.pack_into('<I',target,0,words[0]);struct.pack_into('<H',target,4,flag)
  if flag==1:struct.pack_into('<H',target,8,words[1]);struct.pack_into('<4I',target,12,*values)
 result=0xffffffff if size!=48 or (magic==0x8000 and offset<44) else 0 if parsed and flag==1 else 0xfffffffe
 assert native[0xa20000:0xa20040]==target and r[0]==result,(magic,size,offset,flag,high)
 assert native[0xa10000:0xa10030]==data;cases+=1
(out/'analysis/sound_magic_stream_parameters_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native magic8000 stream-parameter helper65afca,no calls.216 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Magic8000/8001,signed size47/48/ffffffff,offset43/44/ffff,flag0/1/ffff,low/high-bit parameter values,banks. Independent unconditional flag reset,distinct size/signature/offset/flag rejection,partial output preservation,BE16 fields at20/22/26 andBE32 at28/32/36/40. Actual encoded files/initializer integration/decoding/playback/fullgame unverified.'),indent=2)+'\n')
print(cases,'magic stream parameter comparisons passed')
