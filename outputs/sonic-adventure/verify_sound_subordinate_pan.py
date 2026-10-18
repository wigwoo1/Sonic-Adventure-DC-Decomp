import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
p=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for a,stem in [(0x8c65dcca,'sound_subordinate_pan'),(0x8c66837c,'sound_driver_pan'),(0x8c669498,'sound_driver_pan_wrapper'),(0x8c667b7a,'sound_error_report'),(0x8c10ca5c,'signed_division')]:
 helpers[a]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);helpers[a].argtypes=abi
cases=0
for present,level,selector,bank,answer in itertools.product([0,1],[-32768,-16,-15,-1,0,1,15,16,32767],[0,1],[0,1],[0,123]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa08000:0xa08080]=bytes([0x55])*128;ram[0xa09000:0xa09080]=bytes([0x66])*128
 struct.pack_into('<I',ram,0xa08040,0x8ca09000);struct.pack_into('<I',ram,0xa09028,0x8ca0a000)
 struct.pack_into('<2I',ram,0x67c37c,0,0)
 before=bytes(ram[0xa08000:0xa08080]);driver_before=bytes(ram[0xa09000:0xa09080]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);actual=[];expected=[];errors=[]
 @Call
 def call(ctx,a,m,fp,srp,regs,prp):
  if a==0x8c66b374:
   actual.append(tuple(regs[i] for i in range(4,8)));regs[0]=answer;return 0
  assert a in helpers,hex(a)
  if a==0x8c667b7a:errors.append(regs[4])
  return helpers[a](m,fp,srp,regs,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 def service(m,r,sr):expected.append(tuple(r[i] for i in range(4,8)));r[0]=answer;return sr
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:7]=[0x8ca08000 if present else 0,selector,level&0xffffffff];s.r[15]=0x8cf00000;s.pc=0x8c65dcca;s.pr=0x8c000100;s.sr=0x70000063
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank<<21;nf=Fpu.from_buffer_copy(f);special={'macl':0}
 reference(ram,s,{0x8c66b374:service},fpu=f,special=special)
 assert helpers[0x8c65dcca](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and actual==expected
 wanted=bytearray(before)
 if present:
  clamped=max(-15,min(15,level));struct.pack_into('<i',wanted,0x70+4*selector,clamped)
  converted=(abs(127*clamped)//15)*(-1 if clamped<0 else 1)
  assert actual==[(0x8ca0a000,selector,converted&0xffffffff,0)]
  assert len(errors)==(2 if answer else 0)
  if answer:assert errors[-1]==0x8c672330
 else:assert not actual and errors==[0x8c67217c]
 assert native[0xa08000:0xa08080]==wanted and native[0xa09000:0xa09080]==driver_before and r[15]==0x8cf00000
 cases+=1
scope='144 full retail RAM/CPU/FPU/SR/PR/MACL comparisons of subordinate pan65dcca..65dd26 with native driver pan66837c,wrapper669498,signed division10ca5c,and error reporter667b7a (callback unset). Null/present objects,nine signed pan values around clamp -15..15,two channels,both banks,canonical success/error returns. Independent entire128-byte sound-object window/cache70+4*channel,driver128-byte preservation,clamping,127*clamped/15 truncation,backend/channel/aux forwarding,error count/message and stack. Canonical pan command66b374 modeled;native command/enqueue/application callbacks/hardware/game integration unverified.'
(p/'analysis/sound_subordinate_pan_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope=scope),indent=2)+'\n')
print(cases,'subordinate pan comparisons passed')
