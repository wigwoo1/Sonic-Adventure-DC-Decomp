import ctypes,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/unsigned_division_helpers.dll'))
cases=0
for target,name,operation in [(0x8c10cb10,'sa_unsigned_quotient',lambda n,d:n//d),(0x8c10cc80,'sa_unsigned_remainder_8c10cc80',lambda n,d:n%d)]:
 fn=getattr(lib,name);fn.argtypes=abi
 for numerator,denominator,bank,initial_sr in itertools.product([0,1,7,0x80000000,0xffffffff],[0,1,3,0xffffffff],[0,0x200000],[0,0x70000363]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[0]=denominator;s.r[1]=numerator;s.r[15]=0x8cf00000;s.pc=target;s.pr=0x8c000100;s.sr=initial_sr
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);special={'macl':0}
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  if native!=ram or list(r)!=list(s.r) or sr.value!=s.sr or pr.value!=s.pr or bytes(nf)!=bytes(f) or macl.value!=special['macl']:
   mismatch=next((i for i,(a,b) in enumerate(zip(native,ram)) if a!=b),None)
   reg_diffs={i:(hex(s.r[i]),hex(r[i])) for i in range(16) if s.r[i]!=r[i]}
   raise AssertionError(f'{name} numerator={numerator:#x} denominator={denominator:#x} bank={bank:#x} sr={initial_sr:#x}: RAM byte={mismatch}, registers={reg_diffs}, SR={(hex(s.sr),hex(sr.value))}, PR={(hex(s.pr),hex(pr.value))}, FPU={bytes(nf)==bytes(f)}, MACL={(hex(special["macl"]),hex(macl.value))}')
  if denominator:assert r[0]==operation(numerator,denominator),(name,numerator,denominator,hex(r[0]))
  else:assert r[0]==0
  cases+=1
report={'cases_passed':cases,'instructions_recovered':166,'scope':'Native unsigned quotient10cb10 and remainder10cc80, full retail RAM/CPU/FPU/SR/PR/MACL equality; five numerators, four denominators including zero, both FPU banks and two SR masks. Independent Python quotient/remainder formulas and zero return checked. No interpreted/external services.'}
(root/'analysis/unsigned_division_helpers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'unsigned division helper comparisons passed')
