import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_transition.dll')).sa_tikal_transition
 P=ctypes.POINTER(ctypes.c_uint32);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P]
 cases=0
 for bank,rm,sr0,mode,height,velocity,threshold in itertools.product(range(2),range(2),[0,0x700083f3],[0,1,2,8,0xffff],[0,0x80000000,0x3f800000,0xbf800000],[0,0x3e800000,0xbe800000],[0,0x3f800000,0xbf800000]):
  for m in (ram,native):
   m[0xa00000:0xa00400]=bytes(1024)
   struct.pack_into('<I',m,0xa00138,velocity);struct.pack_into('<I',m,0xa00238,height)
   struct.pack_into('<I',m,0xa002c0,threshold);struct.pack_into('<H',m,0xa00324,mode)
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x12345678
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*37
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)]
  s.r[5]=0x8ca00100;s.r[6]=0x8ca00200;s.r[15]=0x8cf00000;s.pc=0x0cb00994;s.pr=0x8c000100;s.sr=sr0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(sr0)
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r)==0
  assert list(r)==list(s.r) and sr.value==s.sr and bytes(nf)==bytes(f) and ram==native,(bank,rm,sr0,mode,hex(height),hex(velocity),hex(threshold))
  cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report=dict(cases_passed=cases,module_sha256=hashlib.sha256(module).hexdigest(),routine_start='0x0cb00994',routine_end_exclusive='0x0cb009de',source_sha256=hashlib.sha256((out/'source/tikal_transition.c').read_bytes()).hexdigest(),retail_span_sha256=hashlib.sha256(module[0x994:0x9de]).hexdigest(),scope='Complete leaf routine versus original instructions, no hooks; all registers, SR, FPU/FPUL and RAM compared. Mode 8 delay-slot FMAC included, state 1/2 threshold, positive/negative velocities and signed zero, both banks/RM0/1. PR/SZ1 and nonfinite/subnormal FCMP excluded. Tikal controller integration pending.')
 (out/'analysis/tikal_transition_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(cases,'Tikal transition comparisons passed')
if __name__=='__main__':main()
