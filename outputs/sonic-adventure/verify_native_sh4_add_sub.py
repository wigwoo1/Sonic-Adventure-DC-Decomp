"""Native wide-integer add/sub versus exact dyadic verifier support."""
import ctypes,itertools,hashlib,json,random
from verify_startup import ROOT
from verify_fpu_cleanup_transfer import Fpu
from sh4_binary32 import execute_add_sub

def main():
 dll=ROOT/'work/sonic-adventure/build/sh4_add_sub.dll';fn=ctypes.CDLL(str(dll)).sa_sh4_add_sub
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint32)]*4+[ctypes.c_int];fn.restype=ctypes.c_int
 rng=random.Random(0x79476);pairs=[(0,0),(0x80000000,0x80000000),(0x3f800000,0x33800000),(0x7f7fffff,0x7f7fffff),(0x00800001,0x00800000),(1,0x3f800000)]
 for _ in range(3000):
  pairs.append(tuple((rng.randrange(2)<<31)|(rng.randrange(1,255)<<23)|rng.getrandbits(23) for _ in range(2)))
 cases=0
 for pair,sub,mode,dn in itertools.product(pairs,range(2),range(2),range(2)):
  av,bv=pair;initial=(dn<<18)|mode|0x3f000|0x7c
  a=ctypes.c_uint32(av);b=ctypes.c_uint32(bv);status=ctypes.c_uint32(initial);out=ctypes.c_uint32(0xdeadbeef)
  result=fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(status),ctypes.byref(out),sub)
  if not dn and any(0<(v&0x7fffffff)<0x800000 for v in pair):
   assert result==1 and status.value==(initial&~0x3f000)|0x20000 and out.value==0xdeadbeef
  else:
   fpu=Fpu();fpu.fpscr=initial;fpu.banks[0][0]=av;fpu.banks[0][1]=bv;execute_add_sub(fpu,0,1,bool(sub))
   assert result==0 and out.value==fpu.banks[0][0] and status.value==fpu.fpscr,(pair,sub,mode,dn,hex(out.value),hex(fpu.banks[0][0]))
   assert b.value==fpu.banks[0][1]
  cases+=1
 for av,bv in pairs[:6]:
  for enabled in [0x80,0x100,0x200,0x380]:
   initial=0x40001|enabled|0x3f000
   a=ctypes.c_uint32(av);b=ctypes.c_uint32(bv);status=ctypes.c_uint32(initial);out=ctypes.c_uint32(0xdeadbeef)
   assert fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(status),ctypes.byref(out),0)==1
   assert out.value==0xdeadbeef
   fpu=Fpu();fpu.fpscr=initial&~0x380;fpu.banks[0][0]=av;fpu.banks[0][1]=bv;execute_add_sub(fpu,0,1)
   assert status.value==fpu.fpscr|enabled;cases+=1
 for initial,av in [(0x80000,0x3f800000),(2,0x3f800000),(3,0x3f800000),(0,0x7f800000)]:
  a=ctypes.c_uint32(av);b=ctypes.c_uint32(0x3f800000);status=ctypes.c_uint32(initial);out=ctypes.c_uint32(0xdeadbeef)
  assert fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(status),ctypes.byref(out),0)==-1
  assert a.value==av and b.value==0x3f800000 and status.value==initial and out.value==0xdeadbeef;cases+=1
 root=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((root/'source/sh4_add_sub.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Native ten-limb integer finite binary32 addition/subtraction versus exact dyadic implementation: signs, both rounding modes, all normal exponent ranges, cancellation, large exponent separation, overflow, subnormal result and DN behavior, FPSCR cause/sticky bits. Denormal-input error tested. Infinity-only/PR=1 unsupported; NaN behavior separately verified; enabled O/U/I traps preserve destination; unsupported modes/operands preserve outputs; runtime support, not recovered parent game routine.')
 (root/'analysis/native_sh4_add_sub_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} native finite add/sub checks passed')
if __name__=='__main__':main()

