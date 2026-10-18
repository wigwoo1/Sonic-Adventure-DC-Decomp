import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_effect.dll')).sa_sound_subordinate_effect;fn.argtypes=abi
reporter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_error_report.dll')).sa_sound_error_report;reporter.argtypes=abi;cases=0
for present,effect,level,bank,answer in itertools.product([0,1],[-1,0,7,16,17,0x80000000],[-32768,-46,-45,-43,-42,-1,0,1,32767],[0,1],[0,123]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa08000:0xa08080]=bytes([0x55])*128
 struct.pack_into('<I',ram,0xa08040,0x8ca09000);struct.pack_into('<2I',ram,0x67c37c,0,0)
 before=bytes(ram[0xa08000:0xa08080]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[];errors=[]
 @Call
 def call(ctx,a,m,fp,srp,r,prp):
  if a==0x8c6682be:actual.append(tuple(r[i] for i in range(4,7)));r[0]=answer;return 0
  assert a==0x8c667b7a;errors.append(r[4]);return reporter(m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 def driver(m,r,sr):expected.append(tuple(r[i] for i in range(4,7)));r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca08000 if present else 0,effect&0xffffffff,level&0xffffffff];s.r[15]=0x8cf00000;s.pc=0x8c65dd26;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{0x8c6682be:driver},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 wanted=bytearray(before)
 if present and 0<=effect<=16:
  clamped=max(-45,min(0,level));struct.pack_into('<2I',wanted,0x78,effect,clamped&0xffffffff)
  assert actual==[(0x8ca09000,effect,clamped&0xffffffff)] and errors==([0x8c67237c] if answer else [])
 else:assert not actual and errors==[0x8c67217c if not present else 0x8c672350]
 assert native[0xa08000:0xa08080]==wanted and r[15]==0x8cf00000
 cases+=1
scope='432 full retail RAM/CPU/FPU/SR/PR comparisons of effect setter65dd26..65ddb2,including skipped literal gap. Null/present objects,six IDs including signed-invalid and valid0/7/16,nine levels around -45..0,both banks,driver success/error returns. Independent complete128-byte object/guard window: invalid inputs preserve object and report exact error string;valid inputs forward bound driver/effect/clamped level,then cache+78/+7c even after downstream error,with exact error-message selection. Native error reporter667b7a with callback unset. Driver6682be modeled;native driver/queue/hardware/game caller integration unverified.'
(p/'analysis/sound_subordinate_effect_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,'subordinate effect comparisons passed')
