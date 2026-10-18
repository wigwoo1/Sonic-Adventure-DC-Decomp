import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_channel_group_setup.dll')).sa_sound_channel_group_setup;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
for present,count,channel,validation,bank,sr0 in itertools.product([False,True],[0,1,2,3],[0,15,16,255],[0,123],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x67c1d0,0x8ca08000 if present else 0)
 struct.pack_into('<I',ram,0xa10000,0x8ca11000);ram[0xa11001]=1
 ram[0xa080a8+44:0xa080a8+88]=bytes([0x55])*44
 struct.pack_into('<3I',ram,0xf00000,count,channel,7)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 def effect(t,r):
  trace=(t,r[4],r[5]);r[0]=validation if t==0x8c65aa30 else 456;return trace
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in (0x8c65aa30,0x8c66b5ac,0x8c66b1a0),a
  actual.append(effect(a,r));return 0
 def hook(t):
  def body(mem,r,sr):expected.append(effect(t,r));return sr
  return body
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c669284
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{t:hook(t) for t in (0x8c65aa30,0x8c66b5ac,0x8c66b1a0)},fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 code=0x0d000001 if not present else 0x03000001 if count not in (1,2) or channel>15 else validation
 assert r[0]==code and actual==expected
 record=0xa080a8+44
 if not code:
  assert actual==[(0x8c65aa30,0x8ca10000,3),(0x8c66b5ac,0x8ca11000,count),(0x8c66b1a0,0x8ca11000,0)]
  assert struct.unpack_from('<I',native,record)[0]==1 and native[record+4]==count
  assert native[record+7:record+7+count]==bytes([channel,7][:count])
  for i in range(count):
   assert struct.unpack_from('<I',native,record+12+4*i)[0]==1
   assert struct.unpack_from('<I',native,record+20+4*i)[0]==0
   assert struct.unpack_from('<I',native,record+36+4*i)[0]==0
 else:
  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66aec8,code)
  assert native[record:record+44]==bytes([0x55])*44
 cases+=1
(out/'analysis/sound_channel_group_setup_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native162-instruction variadic channel-group setup669284. Full retail RAM/CPU/FPU/SR/PR comparison. Native error recorder; validation65aa30 and setup66b5ac/66b1a0 controlled test substitutes. Missing context, count0/1/2/3,channel0/15/16/255,validation0/123,both FPU banks/SR. Independent exact call arguments, descriptor flags/channel list/active and zero arrays, error source66aec8 and record preservation. Lower services and full game unverified.'),indent=2)+'\n')
print(cases,'channel group comparisons passed')
