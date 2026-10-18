import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_subordinate_apply.dll')).sa_sound_subordinate_apply;fn.argtypes=abi
reporter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_error_report.dll')).sa_sound_error_report;reporter.argtypes=abi
targets=[0x8c668042,0x8c6680e2,0x8c6680da,0x8c6680ea,0x8c668112,0x8c66869a,0x8c6686a4,0x8c65dc1c,0x8c65dc54,0x8c65dc64,0x8c65dcca,0x8c65dd26,0x8c65df2a]
helpers={}
for address,name in [(0x8c668042,'sa_sound_driver_apply_noop'),(0x8c6680e2,'sa_sound_driver_gate_get')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{name[3:]}.dll')),name);helper.argtypes=abi;helpers[address]=helper
cases=0
for present,gate,value,bank,answer in itertools.product([0,1],[0,1,0xffffffff],[0,1,127,128,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,123]):
 gate=0
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa08000:0xa08090]=bytes([0x55])*144
 struct.pack_into('<I',ram,0xa08040,0x8ca09000);struct.pack_into('<2I',ram,0x67c37c,0,0);before=bytes(ram[0xa08000:0xa08090])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[]
 vals=[(value+i)&0xffffffff for i in range(4)]+[0]*4
 def service(a,m,r):
  def put(pointer,v):
   for i in range(4):m[(pointer&0xffffff)+i]=(v>>(8*i))&255
  if a==0x8c6680e2:put(r[5],gate)
  elif a==0x8c6680da:put(r[5],vals[0])
  elif a==0x8c6680ea:put(r[5],vals[1])
  elif a==0x8c668112:put(r[6],vals[2+r[5]])
  elif a in [0x8c66869a,0x8c6686a4]:
   i=4 if a==0x8c66869a else 6;put(r[5],vals[i]);put(r[6],vals[i+1])
  r[0]=answer
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c667b7a:return reporter(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a in targets;actual.append((a,r[4],r[5],r[6]))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  raise AssertionError(hex(a))
 def hook(a):
  def run(m,r,sr):expected.append((a,r[4],r[5],r[6]));service(a,m,r);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000 if present else 0;s.r[15]=0x8cf00000;s.pc=0x8c65df78;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,{a:hook(a) for a in targets if a not in helpers},fpu=f,special={'macl':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and [x for x in actual if x[0] not in helpers]==expected
 wanted=[] if not present else [0x8c668042,0x8c6680e2]
 if present and gate:wanted+= [0x8c6680da,0x8c6680ea,0x8c668112,0x8c668112,0x8c66869a,0x8c6686a4,0x8c65dc1c,0x8c65dc54,0x8c65dc64,0x8c65dcca,0x8c65dcca,0x8c65dd26,0x8c65df2a]
 assert [x[0] for x in actual]==wanted
 if present and gate:
  tail=actual[-7:];assert [x[1] for x in tail]==[0x8ca08000]*7
  assert [x[2] for x in tail]==[gate,vals[0],vals[1],0,1,vals[4],vals[6]]
  assert [tail[i][3] for i in [3,4,5,6]]==[vals[2],vals[3],vals[5],vals[7]]
 assert native[0xa08000:0xa08090]==before and r[15]==0x8cf00000;cases+=1
scope='168 complete retail RAM/CPU/FPU/SR/PR comparisons of control apply65df78 with native no-op668042 and zero gate getter6680e2;native reporter callback unset for null objects. Null/present,seven word fixtures,both banks,initial argument/result fixtures. No modeled services. Exact two-call native non-null path,zero gate stack output,entire144-byte object preservation and restored stack. Retail gate always zero so downstream seven-setter chain bypassed. Hardware and game integration remain unverified.'
(p/'analysis/sound_apply_native_gate_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n');print(cases,'control apply comparisons passed')
