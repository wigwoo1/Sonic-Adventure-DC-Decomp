"""Bit-preserving FPU operations needed by owner cleanup helper execution."""
import hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 ram=bytearray(16*1024*1024);rng=random.Random(0x79476)
 patterns=[0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc00001,0x7fa00001,1,0x007fffff,0x00800000,0x7f7fffff]+[rng.getrandbits(32) for _ in range(52)]
 cases=0
 for bank,n,bits,sr0 in itertools.product(range(2),range(16),patterns,[0,0x700083f3]):
  # Save/load via architectural stack; indexed and direct RAM loads;
  # register copy, sign toggle and exact 1.0 constant load.
  m=(n+1)&15;k=(n+2)&15
  ops=[0xff0b|(n<<4),0xf000|(n<<8)|0x9d,0xf000|(n<<8)|(15<<4)|9,0xf000|(m<<8)|(n<<4)|12,0xf000|(m<<8)|0x4d,0xf000|(k<<8)|(4<<4)|8,0xf000|(n<<8)|(4<<4)|6,0xf000|(k<<8)|0x9d,11,9]
  struct.pack_into('<'+str(len(ops))+'H',ram,0x100,*ops);struct.pack_into('<2I',ram,0x1000,bits,bits^0xa5a5a5a5)
  fpu=Fpu();fpu.fpscr=0x40001|(bank<<21)
  for b in range(2):
   for i in range(16):fpu.banks[b][i]=rng.getrandbits(32)
  fpu.banks[bank][n]=bits;expected=[list(x) for x in fpu.banks]
  expected[bank][n]=bits^0xa5a5a5a5;expected[bank][m]=bits^0x80000000;expected[bank][k]=0x3f800000
  state=State();state.r[:]=[0]*16;state.r[0]=4;state.r[4]=0x8c001000;state.r[15]=0x8c002000;state.pc=0x8c000100;state.pr=0x8c000080;state.sr=sr0
  reference(ram,state,fpu=fpu)
  assert [list(x) for x in fpu.banks]==expected,(bank,n,hex(bits))
  assert state.r[15]==0x8c002000 and struct.unpack_from('<I',ram,0x1ffc)[0]==bits
  assert state.sr==sr0 and fpu.fpscr==0x40001|(bank<<21)
  cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report=dict(cases_passed=cases,verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),scope='Synthetic instruction sequences check exact bits for both register banks, all register indices, signed zeros, infinities, NaN encodings, subnormals and random payloads. FMOV stack/predecrement/postincrement/direct/indexed/register forms, FLDI1 and FNEG; SR and FPSCR unchanged. New FMOV forms reject SZ=1; FLDI1/FNEG reject PR=1. Arithmetic and exception behavior remain unmodeled; this is verifier infrastructure, not recovered game code.')
 (out/'analysis/fpu_helper_transfer_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} FPU helper transfer checks passed')
if __name__=='__main__':main()
