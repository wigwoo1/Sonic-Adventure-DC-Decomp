"""Portable native conversion versus exact integer rounding and trap vectors."""
import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT
from sh4_binary32 import round_dyadic

def main():
 dll=ROOT/'work/sonic-adventure/build/sh4_float_i32.dll';fn=ctypes.CDLL(str(dll)).sa_sh4_float_i32
 fn.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int
 rng=random.Random(0x79476);values=[0,1,0xffffffff,0x7fffffff,0x80000000,0x1000000,0x1000001,0x1000003,0xfeffffff,0xfefffffd]+[rng.getrandbits(32) for _ in range(10000)]
 cases=0
 for raw,mode,sticky,enabled in itertools.product(values,range(2),[0,0x7c],[0,0x80]):
  integer=raw if raw<0x80000000 else raw-0x100000000
  expected,inexact=round_dyadic(integer,0,mode)
  initial=0x600000|0x40000|0x3f000|mode|sticky|enabled
  status=ctypes.c_uint32(initial);out=ctypes.c_uint32(0xdeadbeef)
  result=fn(raw,ctypes.byref(status),ctypes.byref(out))
  assert result==bool(enabled) and out.value==(0xdeadbeef if enabled else expected)
  assert status.value==(initial&~0x3f000)|(0x1004 if inexact else 0)
  cases+=1
 for initial in [0x80000,2,3,0x80003]:
  status=ctypes.c_uint32(initial);out=ctypes.c_uint32(0xdeadbeef)
  assert fn(0x7fffffff,ctypes.byref(status),ctypes.byref(out))==-1
  assert status.value==initial and out.value==0xdeadbeef;cases+=1
 root=ROOT/'outputs/sonic-adventure'
 report=dict(cases_passed=cases,source_sha256=hashlib.sha256((root/'source/sh4_float_i32.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),manual='https://www.renesas.com/en/document/mas/sh-4-software-manual',scope='Portable C signed int32 to binary32 using integer operations; both rounding modes, halfway parity, signed extrema, status cause clearing/sticky flags, unconditional enable.I trap preserving destination, unsupported PR/RM preservation. Numerical oracle is independently tested exact dyadic arithmetic. This is runtime support, not an additional recovered game routine; hardware unverified.')
 (root/'analysis/native_sh4_float_i32_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} native int32 conversion checks passed')
if __name__=='__main__':main()
