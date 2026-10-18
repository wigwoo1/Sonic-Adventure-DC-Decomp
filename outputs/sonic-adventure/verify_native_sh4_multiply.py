import ctypes,hashlib,itertools,json,random,struct
from sh4_binary32 import multiply,execute_multiply
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def value(b):return struct.unpack('<f',struct.pack('<I',b))[0]
def main():
 dll=ROOT/'work/sonic-adventure/build/sh4_multiply.dll';fn=ctypes.CDLL(str(dll)).sa_sh4_multiply;fn.argtypes=[ctypes.POINTER(ctypes.c_uint32)]*4;fn.restype=ctypes.c_int
 rng=random.Random(0xf3f2);values=[0,0x80000000,1,0x800000,0x3f800000,0xbf800000,0x40400000,0x7f7fffff,0xff7fffff];pairs=list(itertools.product(values,repeat=2))+[(rng.randrange(0x800000,0x7f800000)|(rng.randrange(2)<<31),rng.randrange(0x800000,0x7f800000)|(rng.randrange(2)<<31)) for _ in range(4000)];cases=0
 ram=bytearray(16*1024*1024);struct.pack_into('<3H',ram,0x100,0xf3f2,11,9)
 for (av,bv),mode,alias in itertools.product(pairs,range(2),range(3)):
  fpu=Fpu();fpu.fpscr=0x27f07c|mode;fpu.banks[1][3]=av;fpu.banks[1][15]=bv;state=State();state.pc=0x8c000100;state.pr=0x8c000080;state.sr=0x700083f3
  reference(ram,state,fpu=fpu);assert state.sr==0x700083f3
  a=ctypes.c_uint32(av);b=ctypes.c_uint32(bv);fpscr=ctypes.c_uint32(0x27f07c|mode);out=ctypes.c_uint32();dest=[out,a,b][alias]
  assert fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(fpscr),ctypes.byref(dest))==0
  assert dest.value==fpu.banks[1][3] and fpscr.value==fpu.fpscr,(hex(av),hex(bv),mode,alias)
  if alias!=1:assert a.value==(av if av&0x7fffffff>=0x800000 else av&0x80000000)
  if alias!=2:assert b.value==fpu.banks[1][15]
  cases+=1
 # Binary64 holds the exact 48-bit finite significand product.
 for a,b in pairs:
  exact=value(a)*value(b)
  try:expected=struct.unpack('<I',struct.pack('<f',exact))[0]
  except OverflowError:expected=((a^b)&0x80000000)|0x7f800000
  assert multiply(a,b,0)[0]==expected;cases+=1
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/sh4_multiply.c').read_bytes()).hexdigest(),scope='Finite FMUL model and native integer core: signed zero, boundary/random exponents, nearest/toward-zero, DN flush, FPSCR sticky/cause, destination aliases, instruction SR unchanged. Exact binary64 product independently checks numerical rounding. Other exceptions and hardware unverified.')
 (outdir/'analysis/native_sh4_multiply_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} finite multiply checks passed')
if __name__=='__main__':main()
