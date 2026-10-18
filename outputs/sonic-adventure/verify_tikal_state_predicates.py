import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_state_predicates.dll'));flags=dll.sa_tikal_flags_transition;flags.argtypes=[ctypes.POINTER(ctypes.c_uint8),P,P]
 idle=dll.sa_tikal_idle_transition;idle.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_void_p,Call]
 count=0;events=[];actual=[]
 for raw in range(65536):
  for m in (ram,native):
   m[0xa00100]=11;struct.pack_into('<H',m,0xa00104,raw);struct.pack_into('<H',m,0xa00324,0x1234)
  s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[6]=0x8ca00200;s.r[15]=0x8cf00000;s.pc=0xcb00e74;s.pr=0x8c000100;s.sr=0x700083f3 if raw&1 else 0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);reference(ram,s);assert flags(view,ctypes.byref(sr),r)==0
  assert list(r)==list(s.r) and sr.value==s.sr and ram[0xa00000:0xa00400]==native[0xa00000:0xa00400],raw;count+=1
 def effect(mem,regs,status,fp,pr):
  trace=(tuple(regs[4:8]),regs[15],pr)
  if mutation:regs[4]=0x8ca00300;regs[6]=0x8ca00380;fp.fpscr^=0x200000
  regs[0]=result;regs[1]=0x11223344;regs[2]=0x22334455;regs[3]=0x33445566
  return trace,status^0x31
 @Call
 def call(ctx,target,m,fp,sr,regs,pr):
  assert target==0x8c06cd22;trace,sr[0]=effect(native,regs,sr[0],fp.contents,pr[0]);events.append(trace);return 0
 def hook(m,regs,status):
  trace,status=effect(m,regs,status,f,s.pr);actual.append(trace);return status
 idle_count=0
 for bank,rm,sr0,result,mutation,raw in itertools.product(range(2),range(2),[0,0x700083f3],range(2),range(2),[0,0x80000000,0x3e800000,0xbe800000,0x3f800000,0xbf800000,0x7f7fffff]):
  for m in (ram,native):
   m[0xa00000:0xa00400]=bytes(1024);m[0xf00000-32:0xf00000]=bytes(32);m[0xa00100]=11;struct.pack_into('<H',m,0xa00324,0x1234);struct.pack_into('<I',m,0xa00238,raw)
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x12345678
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*37
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[6]=0x8ca00200;s.r[15]=0x8cf00000;s.pc=0xcb00e90;s.pr=0x8c000100;s.sr=sr0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);events.clear();actual.clear();reference(ram,s,{0x8c06cd22:hook},f)
  assert idle(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),None,call)==0
  assert events==actual and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(bank,rm,sr0,result,mutation,hex(raw));idle_count+=1
 report=dict(flag_cases_passed=count,idle_cases_passed=idle_count,module_sha256=hashlib.sha256(module).hexdigest(),source_sha256=hashlib.sha256((out/'source/tikal_state_predicates.c').read_bytes()).hexdigest(),scope='Full flags leaf routine all 65536 flag words, all registers/SR/work+entity RAM. Full idle predicate all registers/SR/PR/FPU/FPUL/RAM/call trace, external eligibility injected with volatile R4/R6 clobber and FPSCR bank change; normal/zero finite PR=SZ=0 scope. Both signed zeros take idle transition; nonzero values do not. Pre-update integration pending.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_state_predicates.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_state_predicates.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_state_predicates_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'flag and',idle_count,'idle predicate comparisons passed')
if __name__=='__main__':main()
