"""Finite combined multiply-add, including a separate-rounding counterexample."""
import itertools,random,struct,json,hashlib
from sh4_binary32 import multiply_add
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def value(b):return struct.unpack('<f',struct.pack('<I',b))[0]
def bits(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def main():
 rng=random.Random(0x79476);cases=0
 for _ in range(10000):
  # All 48 product bits and the addend fit exactly in host binary64.
  a=(rng.randrange(2)<<31)|(127<<23)|rng.getrandbits(23);b=(rng.randrange(2)<<31)|(127<<23)|rng.getrandbits(23);c=(rng.randrange(2)<<31)|(127<<23)|rng.getrandbits(23)
  exact=value(a)*value(b)+value(c)
  result,inexact=multiply_add(a,b,c,0);assert result==bits(exact)
  assert inexact==(value(result)!=exact)
  truncated,_=multiply_add(a,b,c,1);assert abs(value(truncated))<=abs(exact)
  cases+=1
 vectors=[(0x3f800001,0x3f7ffffe,0xbf800000,0xa8800000,0),(0x3f800000,0x40000000,0x40400000,0x40a00000,0),(0,0x3f800000,0x80000000,0x80000000,0),(0x7f7fffff,0x40000000,0,0x7f800000,0x5014)]
 ram=bytearray(16*1024*1024)
 for vector,bank,sticky in itertools.product(vectors,range(2),[0,0x7c]):
  a,b,c,expected,status=vector;fpu=Fpu();fpu.fpscr=(bank<<21)|sticky|0x3f000;fpu.banks[bank][0]=a;fpu.banks[bank][3]=b;fpu.banks[bank][4]=c
  struct.pack_into('<3H',ram,0x100,0xf43e,11,9);state=State();state.pc=0x8c000100;state.pr=0x8c000080;state.sr=0x700083f3
  reference(ram,state,fpu=fpu)
  assert fpu.banks[bank][4]==expected and fpu.fpscr==(bank<<21)|sticky|status,vector
  assert state.sr==0x700083f3;cases+=1
 assert bits(value(0x3f800001)*value(0x3f7ffffe))==0x3f800000
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'sh4_binary32.py').read_bytes()).hexdigest(),verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),manual='https://www.renesas.com/en/document/mas/sh-4-software-manual',scope='Finite single-precision exact product plus sum with one final rounding. Random exact binary64 oracle, both rounding numerical cores, bank/cause/sticky instruction cases, signed zero and overflow. Explicit cancellation distinguishes separate multiply/add rounding. Nonfinite operands, enabled traps, PR=1 rejected; physical hardware and parent game helper unverified.')
 (out/'analysis/fpu_helper_fmac_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} finite FMAC checks passed')
if __name__=='__main__':main()
