"""Instruction-level finite FADD/FSUB result and FPSCR cases."""
import itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 # a, b, subtraction, rounding mode, DN, expected result, cause+new flags
 vectors=[(0x3f800000,0x3f800000,0,0,0,0x40000000,0),(0x3f800000,0x33800000,0,0,0,0x3f800000,0x1004),(0x3f800001,0x33800000,0,0,0,0x3f800002,0x1004),(0x3f800001,0x33800000,0,1,0,0x3f800001,0x1004),(0x3f800000,0x3f800000,1,0,0,0,0),(0x80000000,0x80000000,0,0,0,0x80000000,0),(0x00800001,0x00800000,1,0,0,1,0),(0x00800001,0x00800000,1,0,1,0,0x300c),(1,0x3f800000,0,0,1,0x3f800000,0),(0x7f7fffff,0x7f7fffff,0,0,0,0x7f800000,0x5014),(0x7f7fffff,0x7f7fffff,0,1,0,0x7f7fffff,0x5014)]
 ram=bytearray(16*1024*1024);cases=0
 for vector,bank,n,sticky in itertools.product(vectors,range(2),[0,3,14,15],[0,0x7c]):
  a,b,sub,rm,dn,expected,status=vector;m=(n+1)&15
  fpu=Fpu();fpu.fpscr=(bank<<21)|(dn<<18)|rm|sticky|0x3f000;fpu.banks[bank][n]=a;fpu.banks[bank][m]=b
  before=[list(v) for v in fpu.banks];before[bank][n]=expected
  if dn and (b&0x7fffffff)<0x800000:before[bank][m]=b&0x80000000
  struct.pack_into('<3H',ram,0x100,0xf000|(n<<8)|(m<<4)|sub,11,9)
  state=State();state.pc=0x8c000100;state.pr=0x8c000080;state.sr=0x700083f3
  reference(ram,state,fpu=fpu)
  assert [list(v) for v in fpu.banks]==before,(vector,bank,n)
  assert fpu.fpscr==(bank<<21)|(dn<<18)|rm|sticky|status,(vector,hex(fpu.fpscr))
  assert state.sr==0x700083f3;cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report=dict(cases_passed=cases,verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),arithmetic_source_sha256=hashlib.sha256((out/'sh4_binary32.py').read_bytes()).hexdigest(),scope='Instruction-level finite FADD/FSUB, both rounding modes/banks, sticky flags and cause clearing, signed zero, halfway ties, denormal input flush, exact subnormal output, DN output flush and overflow. Enabled traps, PR=1 and nonfinite input rejected. FMAC and complete helper translation pending.')
 (out/'analysis/fpu_helper_add_sub_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} FADD/FSUB instruction checks passed')
if __name__=='__main__':main()
