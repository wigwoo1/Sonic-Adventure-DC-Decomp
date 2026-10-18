import hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();assert struct.unpack_from('<H',binary,0x69e58)[0]==0xf433
 vectors=[(0x3f800000,0x40400000,0,0x3eaaaaab,0x1004),(0x3f800000,0x40400000,1,0x3eaaaaaa,0x1004),(0xbf800000,0x40400000,0,0xbeaaaaab,0x1004),(0xbf800000,0x40400000,1,0xbeaaaaaa,0x1004),(0x80000000,0xbf800000,0,0,0),(0x00800000,0x40000000,0,0x00400000,0),(0x00800000,0x40000000,0x40000,0,0x300c),(0x7f7fffff,0x3f000000,0,0x7f800000,0x5014),(0x7f7fffff,0x3f000000,1,0x7f7fffff,0x5014),(1,0x3f800000,0x40000,0,0),(0x3f800000,0,0,0x7f800000,0x8020),(0,0,0,0x7fbfffff,0x10040),(0xbf800000,0,1,0xff800000,0x8020),(0x3f800000,1,0x40000,0x7f800000,0x8020)]
 cases=0;ram=bytearray(16*1024*1024);struct.pack_into('<3H',ram,0x100,0xf433,11,9)
 for vector,bank,sticky,sr0 in itertools.product(vectors,range(2),[0,0x7c],[0,0x700083f3]):
  a,b,mode,expected,flags=vector;fpu=Fpu();fpu.fpscr=(bank<<21)|mode|sticky|0x3f000;fpu.banks[bank][4]=a;fpu.banks[bank][3]=b;fpu.fpul=0xabcdef12;other=list(fpu.banks[1-bank])
  state=State();state.pc=0x8c000100;state.pr=0x8c000080;state.sr=sr0;reference(ram,state,fpu=fpu)
  assert fpu.banks[bank][4]==expected and fpu.fpscr==((bank<<21)|mode|sticky|flags),vector
  assert state.sr==sr0 and fpu.fpul==0xabcdef12 and list(fpu.banks[1-bank])==other;cases+=1
 for fpscr,a,b in [(0,1,0x3f800000),(0,0x7f800000,0x3f800000),(0x80000,0x3f800000,0x3f800000),(2,0x3f800000,0x3f800000),(0x80,0x3f800000,0x3f800000)]:
  fpu=Fpu();fpu.fpscr=fpscr;fpu.banks[0][4]=a;fpu.banks[0][3]=b;before=bytes(fpu);state=State();state.pc=0x8c000100;state.pr=0x8c000080
  try:reference(ram,state,fpu=fpu)
  except AssertionError:pass
  else:raise AssertionError('Unsupported execution accepted')
  assert bytes(fpu)==before;cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'sh4_binary32.py').read_bytes()).hexdigest(),verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),manual='https://www.renesas.com/en/document/mas/sh-4-software-manual',scope='Finite FDIV instruction model using verified retail opcode. Rounding, signed zero, DN input/output flushing, overflow, exact/inexact tiny result flags, banks and sticky/cause flags, unchanged SR/FPUL. Zero-divisor exception classification and DN input flushing are covered. Unsupported precision, nonfinite inputs, denormal traps without DN, reserved modes and enabled traps are rejected without mutation. Native C and hardware unverified.')
 (out/'analysis/fpu_helper_divide_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} FDIV instruction checks passed')
if __name__=='__main__':main()
