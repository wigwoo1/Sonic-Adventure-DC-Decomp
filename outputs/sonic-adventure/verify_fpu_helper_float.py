"""Instruction-level FPUL raw transfers and signed conversion checks."""
import itertools,random,struct,json,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 rng=random.Random(0x79476);values=[0,1,0xffffffff,0x7fffffff,0x80000000,0x1000000,0x1000001,0x1000003,0xfeffffff,0xfefffffd]+[rng.getrandbits(32) for _ in range(1000)]
 ram=bytearray(16*1024*1024);cases=0
 for raw,bank,n,mode,sticky in itertools.product(values,range(2),[0,3,15],range(2),[0,0x7c]):
  signed=raw if raw<0x80000000 else raw-0x100000000
  expected=struct.unpack('<I',struct.pack('<f',signed))[0]
  rounded=struct.unpack('<f',struct.pack('<I',expected))[0]
  if mode and abs(rounded)>abs(signed):expected-=1
  exact=struct.unpack('<f',struct.pack('<I',expected))[0]==signed
  fpu=Fpu();fpu.fpscr=(bank<<21)|mode|sticky|0x3f000
  state=State();state.r[4]=raw;state.pc=0x8c000100;state.pr=0x8c000080;state.sr=0x700083f3
  struct.pack_into('<5H',ram,0x100,0x445a,0xf80d,0xf02d|(n<<8),11,9)
  reference(ram,state,fpu=fpu)
  assert fpu.fpul==raw and fpu.banks[bank][8]==raw
  assert fpu.banks[bank][n]==expected,(hex(raw),mode)
  assert fpu.fpscr==(bank<<21)|mode|sticky|(0 if exact else 0x1004)
  assert state.sr==0x700083f3;cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'sh4_binary32.py').read_bytes()).hexdigest(),verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),scope='LDS Rn,FPUL, FSTS FPUL,FRn bit transfer and signed int32 FLOAT conversion; banks, both rounding modes, cause clearing and sticky inexact flags. Boundary and random integers checked with host numeric oracle. PR=1, enabled inexact trap and reserved rounding modes rejected. No hardware or parent helper verification claim.')
 (out/'analysis/fpu_helper_float_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} FPUL/FLOAT checks passed')
if __name__=='__main__':main()
