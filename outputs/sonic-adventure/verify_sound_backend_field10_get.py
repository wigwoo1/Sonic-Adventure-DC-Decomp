import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field10_get.dll')).sa_sound_backend_field10_get;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
for mode,selector,descriptor,value,bank,sr0,fmt in itertools.product(range(4),[0,1,128,255],[0,127,128,255],[0,127,128,65535,0x80000000,0xffffffff],[0,0x200000],[0x70000063],range(4)):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 context=0 if mode==0 else 0x8ca08000;inputptr=0 if mode==1 else 0x8ca10000;output=0 if mode==2 else 0x8ca20000
 struct.pack_into('<I',ram,0x67c1d0,context);struct.pack_into('<I',ram,0xa081b8,0x8ca30000)
 ram[0xa10001]=descriptor;struct.pack_into('<2I',ram,0xa20000,0xdeadbeef,0x12345678)
 voice=0x8ca080a8+ctypes.c_int8(descriptor).value*44
 struct.pack_into('<I',ram,voice&0xffffff,fmt);ram[(voice+7+ctypes.c_int8(selector).value)&0xffffff]=0x81
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[];lookups=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c65a900:
   lookups.append(tuple(r[i] for i in [4,5,6,7]));native[r[6]&0xffffff:(r[6]&0xffffff)+4]=bytes([0x31])*4;native[r[7]&0xffffff:(r[7]&0xffffff)+4]=bytes([0x42])*4;r[0]=123;return 0
  assert a==0x8c667150,a
  actual.append(r[4]);struct.pack_into('<I',native,r[5]&0xffffff,value);r[0]=123;sr[0]^=1;return 0
 def read_hook(mem,r,sr):
  expected.append(r[4]);struct.pack_into('<I',mem,r[5]&0xffffff,value);r[0]=123;return sr^1
 def lookup(mem,r,sr):
  mem[r[6]&0xffffff:(r[6]&0xffffff)+4]=bytes([0x31])*4;mem[r[7]&0xffffff:(r[7]&0xffffff)+4]=bytes([0x42])*4;r[0]=123;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=inputptr;s.r[5]=selector;s.r[6]=output;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66accc
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c667150:read_hook,0x8c65a900:lookup},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 code=[0x0d000001,0x07000103,0x07000203,0][mode]
 assert r[0]==code and actual==expected
 if mode==3:
  address=(0x8ca30400+ctypes.c_int8(descriptor).value*64+ctypes.c_int8(selector).value*32+24)&0xffffffff
  assert actual==[address]
  signed=ctypes.c_int32(value).value
  result=(value*2)&0xffffffff if fmt==1 else (int(abs(signed)//2)*(-1 if signed<0 else 1))&0xffffffff if fmt==2 else value
  assert struct.unpack_from('<2I',native,0xa20000)==(result,0x12345678)
  assert lookups==[(0x52535053,0xffffff81,0x8cefffe4,0x8cefffe8)]
  assert macl.value==(ctypes.c_int8(descriptor).value*44)&0xffffffff
 else:
  assert not actual and not lookups
  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66accc,code)
  assert struct.unpack_from('<2I',native,0xa20000)==(0xdeadbeef,0x12345678)
 cases+=1
(out/'analysis/sound_backend_field10_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='3072 full retail RAM/CPU/FPU/SR/PR comparisons of103-instruction field reader66accc. Disabled context,null handle,null output,success;four signed selectors/descriptors,six32-bit payloads,both banks,four voice formats. Native diagnostic;lower667150 and resource lookup65a900 modeled. Independent read address+24,full32-bit output:format1 doubles with wrap,format2 signed truncation toward zero,formats0/3 unchanged. Independent lookup tag52535053,signed voice byte and stack output pointers;lookup outputs ignored. Neighbor words preserved,ignored lower returns,error globals and descriptor MACL checked. Physical hardware/full game integration and semantic field name unverified.'),indent=2)+'\n')
print(cases,'descriptor byte query comparisons passed')
