import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_apply.dll')).sa_sound_subordinate_apply;fn.argtypes=abi
reporter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_error_report.dll')).sa_sound_error_report;reporter.argtypes=abi;cases=0
for present,effect,level,bank,answer in itertools.product([0],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,123]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa08000:0xa08090]=bytes([0x55])*144
 struct.pack_into('<I',ram,0xa08040,0x8ca09000);struct.pack_into('<2I',ram,0x67c37c,0,0)
 before=bytes(ram[0xa08000:0xa08090]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[];errors=[]
 @Call
 def call(ctx,a,m,fp,srp,r,prp):
  if a==0x8c668498:actual.append(tuple(r[i] for i in range(4,7)));r[0]=answer;return 0
  assert a==0x8c667b7a;errors.append(r[4]);return reporter(m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 def driver(m,r,sr):expected.append(tuple(r[i] for i in range(4,7)));r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca08000 if present else 0,effect&0xffffffff,level&0xffffffff];s.r[15]=0x8cf00000;s.pc=0x8c65df78;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c668498:driver},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 wanted=bytearray(before)
 if present:
  struct.pack_into('<2I',wanted,0x88,effect,level)
  assert actual==[(0x8ca09000,effect,level)] and errors==[]
 else:assert not actual and errors==[0x8c67217c]
 assert native[0xa08000:0xa08090]==wanted and r[15]==0x8cf00000
 cases+=1
scope='196 null-object full RAM/CPU/FPU/SR/PR comparisons of apply routine65df78..65e062. Seven patterns for two initial registers,both banks,two initial result fixtures. Native callback-unset error reporter; independent144-byte object preservation and restored stack. Non-null driver reads and setter chain unverified; this report does not establish full routine recovery.'
(p/'analysis/sound_subordinate_apply_null_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,'apply null-path comparisons passed')
