import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_rate_table_set.dll')).sa_sound_rate_table_set;fn.argtypes=abi
error=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_error_record;error.argtypes=abi
cases=0
table={}
for i in range(13):
 rate,pitch=struct.unpack_from('<Ih',binary,0x67c384-0x10000+i*8)
 if ctypes.c_int32(rate).value<=0:break
 table[rate]=pitch&0xffffffff
for rate,answer,bank,sr0 in itertools.product(list(table)+[0,1,65535,48000+65536],[0,123],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 actual=[];expected=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c66b030,a
  actual.append(tuple(r[i] for i in range(4,8)));r[0]=answer;return 0
 def hook(mem,r,sr):expected.append(tuple(r[i] for i in range(4,8)));r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca10000;s.r[5]=1;s.r[6]=rate;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c6693ec
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c66b030:hook},fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 selected=table.get(rate&65535)
 code=0x03000001 if selected is None else answer
 assert r[0]==code and actual==expected
 if selected is not None:assert actual==[(0x8ca10000,1,selected,0x22340007)]
 else:assert not actual
 if code:assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c6693ec,code)
 cases+=1
(out/'analysis/sound_rate_table_set_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native34-instruction sample-rate table wrapper6693ec.128 full retail RAM/CPU/FPU/SR/PR comparisons. All12 retail rates,invalid0/1/65535 and48000+65536 truncation, lower0/123,banks/SR. Native error recorder; pitch helper66b030 modeled. Independent table-selected pitch, call arguments, error propagation and no-match behavior. Audio/full game unverified.'),indent=2)+'\n')
print(cases,'rate table comparisons passed')
