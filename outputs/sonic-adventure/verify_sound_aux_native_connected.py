import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_aux_set.dll')).sa_sound_subordinate_aux_set;fn.argtypes=abi
driver_fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_aux_noop.dll')).sa_sound_driver_aux_noop;driver_fn.argtypes=abi
getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_aux_get.dll')).sa_sound_subordinate_aux_get;getter.argtypes=abi
reporter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_error_report.dll')).sa_sound_error_report;reporter.argtypes=abi;cases=0
for present,effect,level,bank,answer in itertools.product([0,1],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,123]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa08000:0xa08090]=bytes([0x55])*144
 struct.pack_into('<I',ram,0xa08040,0x8ca09000);struct.pack_into('<2I',ram,0x67c37c,0,0)
 before=bytes(ram[0xa08000:0xa08090]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[];errors=[]
 @Call
 def call(ctx,a,m,fp,srp,r,prp):
  if a==0x8c668498:actual.append(tuple(r[i] for i in range(4,7)));return driver_fn(m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c667b7a;errors.append(r[4]);return reporter(m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 def driver(m,r,sr):expected.append(tuple(r[i] for i in range(4,7)));r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca08000 if present else 0,effect&0xffffffff,level&0xffffffff];s.r[15]=0x8cf00000;s.pc=0x8c65df2a;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 wanted=bytearray(before)
 if present:
  struct.pack_into('<2I',wanted,0x88,effect,level)
  assert actual==[(0x8ca09000,effect,level)] and errors==[]
 else:assert not actual and errors==[0x8c67217c]
 assert native[0xa08000:0xa08090]==wanted and r[15]==0x8cf00000
 for alias in [0,1]:
  native[0xa20000:0xa20010]=bytes([0x66])*16;ram[0xa20000:0xa20010]=bytes([0x66])*16
  first=0x8ca20004;second=first if alias else 0x8ca20008
  s.r[4:7]=[0x8ca08000 if present else 0,first,second];s.pc=0x8c65df5e;s.pr=0x8c000100
  r[4]=s.r[4];r[5]=first;r[6]=second;pr.value=s.pr
  reference(ram,s,{},fpu=f,special={'macl':macl.value})
  assert getter(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  output=bytearray([0x66]*16)
  if present:
   for pointer,value in [(first,effect),(second,level)]:struct.pack_into('<I',output,(pointer&0xffffff)-0xa20000,value)
  assert native[0xa20000:0xa20010]==output and native[0xa08000:0xa08090]==wanted and r[15]==0x8cf00000
 cases+=1
scope='392 persistent setter/query sequences,1176 public operations. Seven full-word patterns for both auxiliary fields,null/present objects,both banks,two initial result fixtures. After setter65df2a,getter65df5e runs on the same RAM/CPU/FPU state with distinct then aliased outputs. Independent144-byte cache and16-byte guarded outputs verify+88/+8c readback and alias order;null paths preserve outputs. Complete retail comparisons after each operation. Native driver668498 verified as RTS with zero result delay slot;no modeled calls. Hardware and game integration unverified.'
(p/'analysis/sound_aux_native_connected_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,'auxiliary setter comparisons passed')
