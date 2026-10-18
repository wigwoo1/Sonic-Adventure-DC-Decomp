"""Check normal/zero/infinity FCMP behavior against host numeric comparisons."""
import hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 ram=bytearray(16*1024*1024);cases=0
 bits=[0,0x80000000,0x3f800000,0xbf800000,0x3f000000,0xbf000000,0x00800000,0x80800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000]
 for bank,n,a,b,form,sr0,cause in itertools.product(range(2),[0,3,14,15],bits,bits,[4,5],[0,0x700083f3],[0,0x3f000]):
  m=(n+1)&15;fpu=Fpu();fpu.fpscr=0x40001|(bank<<21)|cause;fpu.banks[bank][n]=a;fpu.banks[bank][m]=b
  before=[list(x) for x in fpu.banks]
  struct.pack_into('<3H',ram,0x100,0xf000|(n<<8)|(m<<4)|form,11,9)
  state=State();state.pc=0x8c000100;state.pr=0x8c000080;state.sr=sr0
  x=struct.unpack('<f',struct.pack('<I',a))[0];y=struct.unpack('<f',struct.pack('<I',b))[0]
  reference(ram,state,fpu=fpu);expect=int(x==y if form==4 else x>y)
  assert state.sr==(sr0&~1)|expect,(bank,n,hex(a),hex(b),form)
  assert [list(v) for v in fpu.banks]==before and fpu.fpscr==0x40001|(bank<<21)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),scope='FCMP/eq and FCMP/gt operand order, signs, positive/negative zero, finite extrema and infinities, both FPU banks, SR.T with unrelated SR preserved. Comparison does not alter operand bits. Guarded scope: PR=0, cause clearing supported, no NaNs or subnormals; exception and arithmetic behavior still unmodeled.')
 (out/'analysis/fpu_helper_comparison_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} FPU comparison checks passed')
if __name__=='__main__':main()
