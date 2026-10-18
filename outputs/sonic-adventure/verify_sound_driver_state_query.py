import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_state_query.dll')).sa_sound_driver_state_query;fn.argtypes=abi
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'));helpers={}
for a,name in [(0x8c65a568,'sa_engine_object_state_get'),(0x8c65acac,'sa_engine_error_record')]:
 f=getattr(lib,name);f.argtypes=abi;helpers[a]=f
def word(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]
error=word(0x8c6686c0);getter=word(0x8c6686f0);errorcode=word(0x8c65a59c);cases=0
for selector,state_value,missing,bank,sr0 in itertools.product([-1,0,1,2,255,256],[0,1,2,0xffffffff],[False,True],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 handle=0x8ca08000;output=0x8ca20000
 for channel in [-1,0,1,2]:struct.pack_into('<I',ram,0xa08018+4*channel,0x8ca10000+64*(channel+1))
 target=0x8ca10000+64*(ctypes.c_int8(selector).value+1)
 struct.pack_into('<I',ram,(target&0xffffff)+4,state_value)
 if missing:struct.pack_into('<I',ram,0xa08018+4*ctypes.c_int8(selector).value,0);target=0
 struct.pack_into('<I',ram,0xf00000-24,0xdeadbeef);struct.pack_into('<I',ram,output&0xffffff,0xa5a5a5a5)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];getargs=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==getter:getargs.append((r[4],r[5]))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==error;calls.append((a,r[4],r[5],r[6],r[7]));r[0]=777;return 0
 def hook(m,r,sr):events.append((error,r[4],r[5],r[6],r[7]));r[0]=777;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=selector&0xffffffff;s.r[6]=output;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c6685bc
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,fpu=f,special=special,hooks={error:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert calls==events and getargs==[(target,0x8cf00000-24)]
 assert struct.unpack_from('<I',native,output&0xffffff)[0]==(0xdeadbeef if missing else state_value)
 assert r[0]==(errorcode if missing else 0)
 assert len(calls)==int(missing)
 if missing:
  assert calls[0][1]==word(0x8c6686f4)
  assert struct.unpack_from('<2I',native,0x67c1c8)==(word(0x8c65a5a4),errorcode)
 else:assert struct.unpack_from('<I',native,(target&0xffffff)+4)[0]==state_value
 cases+=1
(out/'analysis/sound_driver_state_query_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native32-instruction driver state query6685bc with native state getter65a568/error recorder65acac. Signed-byte selector/truncation,state0/1/2/FFFFFFFF,null selected object,banks/SR. Full retail RAM/CPU/FPU/SR/PR/MACL and diagnostics match. Independent selected getter arguments,output,state preservation,error/message/result verified. Null-object error copies seeded unwritten stack wordDEADBEEF to output; value depends on prior stack contents. Outer diagnostics modeled; full game unverified.'),indent=2)+'\n')
print(cases,'native driver state query comparisons passed')
