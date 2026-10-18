import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_pan_get.dll')).sa_sound_backend_pan_get;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
for mode,selector,descriptor,value,bank,sr0,channels in itertools.product([3],[0,1,128,255],[0,127,128,255],[0,1,63,64,127,128,255],[0,0x200000],[0x70000063],[0,1,2,3]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 context=0 if mode==0 else 0x8ca08000;inputptr=0 if mode==1 else 0x8ca10000;output=0 if mode==2 else 0x8ca20000
 struct.pack_into('<I',ram,0x67c1d0,context);struct.pack_into('<I',ram,0xa081b8,0x8ca30000)
 ram[0xa10001]=descriptor;struct.pack_into('<2I',ram,0xa20000,0xdeadbeef,0x12345678)
 voice=0xa08000+0xa8+ctypes.c_int8(descriptor).value*44;ram[voice+5]=channels
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c667188,a
  actual.append(r[4]);native[r[5]&0xffffff]=value;r[0]=123;sr[0]^=1;return 0
 def read_hook(mem,r,sr):
  expected.append(r[4]);mem[r[5]&0xffffff]=value;r[0]=123;return sr^1
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=inputptr;s.r[5]=selector;s.r[6]=output;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66bb4c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,{0x8c667188:read_hook},fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 code=[0x0d000001,0x07000103,0x07000203,0][mode]
 assert macl.value==special['macl']
 assert r[0]==code and actual==expected
 if mode==3:
  address=(0x8ca30400+ctypes.c_int8(descriptor).value*64+ctypes.c_int8(selector).value*32+2)&0xffffffff
  assert actual==[address]
  result=ctypes.c_int8(value).value
  if channels==2:
   if selector==0:result+=64
   elif selector==1:result=(result if result else -1)-64
   result*=2
  assert struct.unpack_from('<2I',native,0xa20000)==((0xdeadbeef&0xffffff00)|(result&255),0x12345678)
 else:
  assert not actual
  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66bb4c,code)
  assert struct.unpack_from('<2I',native,0xa20000)==(0xdeadbeef,0x12345678)
 cases+=1
(out/'analysis/sound_backend_pan_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='896 complete retail RAM/CPU/FPU/SR/PR/MACL comparisons of91-instruction backend pan reader66bb4c. Four signed selectors/descriptors,seven byte boundaries,channels0/1/2/3,both FPU banks. Native error recorder;lower667188 modeled. Independent address plus2,mono/nonstereo unchanged byte,stereo selector0 bias+64 and selector1 zero-to-minus1/bias-64 followed by doubling with final byte wrapping. One-byte output/neighbor guards,ignored lower status,restored stack. Native lower transfer/hardware/game integration pending. Separate56-case native diagnostics report covers errors.'),indent=2)+'\n')
print(cases,'descriptor byte query comparisons passed')
