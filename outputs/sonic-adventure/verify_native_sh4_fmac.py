"""Native exact finite product-plus-sum versus integer dyadic oracle."""
import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT
from verify_fpu_cleanup_transfer import Fpu
from sh4_binary32 import execute_fmac

def main():
 dll=ROOT/'work/sonic-adventure/build/sh4_fmac.dll';fn=ctypes.CDLL(str(dll)).sa_sh4_fmac
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint32)]*5;fn.restype=ctypes.c_int
 rng=random.Random(0x79476);triples=[(0x3f800001,0x3f7ffffe,0xbf800000),(0,0x3f800000,0x80000000),(0x7f7fffff,0x40000000,0),(0x00800000,0x3f000000,0),(1,0x3f800000,0)]
 for _ in range(3000):triples.append(tuple((rng.randrange(2)<<31)|(rng.randrange(1,255)<<23)|rng.getrandbits(23) for _ in range(3)))
 cases=0
 for triple,mode,dn in itertools.product(triples,range(2),range(2)):
  initial=(dn<<18)|mode|0x3f000|0x7c
  operands=[ctypes.c_uint32(v) for v in triple];status=ctypes.c_uint32(initial);out=ctypes.c_uint32(0xdeadbeef)
  result=fn(*[ctypes.byref(v) for v in operands],ctypes.byref(status),ctypes.byref(out))
  if not dn and any(0<(v&0x7fffffff)<0x800000 for v in triple):
   assert result==1 and out.value==0xdeadbeef and status.value==(initial&~0x3f000)|0x20000
  else:
   fpu=Fpu();fpu.fpscr=initial
   for i,v in enumerate(triple):fpu.banks[0][i]=v
   execute_fmac(fpu,2,1)
   assert result==0 and out.value==fpu.banks[0][2] and status.value==fpu.fpscr,(triple,mode,dn,hex(out.value),hex(fpu.banks[0][2]))
  cases+=1
 for triple,mode,m,n in itertools.product(triples[:5],range(2),range(3),range(3)):
  words=[ctypes.c_uint32(v) for v in triple];initial=0x40000|mode
  status=ctypes.c_uint32(initial);fpu=Fpu();fpu.fpscr=initial
  for i,v in enumerate(triple):fpu.banks[0][i]=v
  execute_fmac(fpu,n,m)
  assert fn(ctypes.byref(words[0]),ctypes.byref(words[m]),ctypes.byref(words[n]),ctypes.byref(status),ctypes.byref(words[n]))==0
  assert [v.value for v in words]==list(fpu.banks[0][:3]) and status.value==fpu.fpscr;cases+=1
 for enabled in [0x80,0x100,0x200,0x380]:
  words=[ctypes.c_uint32(v) for v in (0x3f800000,0x3f800000,0x3f800000)];status=ctypes.c_uint32(enabled|0x3f000);out=ctypes.c_uint32(0xdeadbeef)
  assert fn(*[ctypes.byref(v) for v in words],ctypes.byref(status),ctypes.byref(out))==1
  assert out.value==0xdeadbeef and status.value==enabled;cases+=1
 for initial,raw in [(0x80000,0x3f800000),(2,0x3f800000),(3,0x3f800000),(0,0x7f800000),(0,0x7fc00000)]:
  words=[ctypes.c_uint32(v) for v in (raw,0x3f800000,0x3f800000)];status=ctypes.c_uint32(initial);out=ctypes.c_uint32(0xdeadbeef)
  assert fn(*[ctypes.byref(v) for v in words],ctypes.byref(status),ctypes.byref(out))==-1
  assert status.value==initial and out.value==0xdeadbeef and words[0].value==raw;cases+=1
 root=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((root/'source/sh4_fmac.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Portable finite single-precision combined multiply/add with exact wide-integer product/sum and final rounding. Random full normal exponent range, both modes/DN, cancellation counterexample, overflow/underflow/status, denormal input error. Runtime support; no parent helper or hardware equivalence claim. Operand/destination aliases, enabled-trap destination preservation, unsupported modes/operands also tested.')
 (root/'analysis/native_sh4_fmac_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} native finite FMAC checks passed')
if __name__=='__main__':main()
