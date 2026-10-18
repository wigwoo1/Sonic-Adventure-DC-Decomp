import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_group_control_set.dll')).sa_sound_group_control_set;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
for mode,descriptor,value,bank,sr0 in itertools.product(range(3),[0,1,127,128,255],[0,1,127,128,255,0xffffffff],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 context=0 if mode==0 else 0x8ca08000;inputptr=0 if mode==1 else 0x8ca10000
 struct.pack_into('<I',ram,0x67c1d0,context);ram[0xa10001]=descriptor
 record=0xa080a8+44*ctypes.c_int8(descriptor).value
 ram[record:record+44]=bytes([0x55])*44
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c65acac,a
  return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=inputptr;s.r[5]=value;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c66b1a0
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={'macl':0};reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 code=[0x0d000001,0x07000103,0][mode]
 assert r[0]==code and macl.value==special['macl']
 expected=bytearray([0x55])*44
 if mode==2:expected[6]=0 if ctypes.c_int8(value&255).value==127 else (-1-2*value)&255
 else:assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66b1a0,code)
 assert native[record:record+44]==expected
 cases+=1
(out/'analysis/sound_group_control_set_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native50-instruction group control-byte setter66b1a0 with native error recorder.360 full retail RAM/CPU/FPU/SR/PR/MACL comparisons. Missing context/null input/valid; signed descriptor0/1/127/128/255,values0/1/127/128/255/FFFFFFFF,banks/SR. Independent44-byte record selection, byte6 encoding0 for signed-byte127 else(-1-2*value)mod256, neighboring fields preserved. Meaning of control field and full game unverified.'),indent=2)+'\n')
print(cases,'group control comparisons passed')
