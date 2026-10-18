"""Independent SH-4 manual-derived NaN FMUL exception vectors."""
import ctypes, hashlib, itertools, json
from verify_startup import ROOT

def main():
 out=ROOT/'outputs/sonic-adventure';dll=ROOT/'work/sonic-adventure/build/sh4_multiply.dll'
 fn=ctypes.CDLL(str(dll)).sa_sh4_multiply
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint32)]*4
 values=[0,0x80000000,1,0x3f800000,0x7f800000,0xff800000,0x7f800001,0xffbfffff,0x7fc00000,0xffffffff]
 count=0
 for av,bv,sub,enable,dn,rm,alias in itertools.product(values,values,range(1),[0,0x800,0x80,0x100,0x200,0xb80],range(2),range(2),range(3)):
  na=(av&0x7fffffff)>0x7f800000;nb=(bv&0x7fffffff)>0x7f800000
  if not (na or nb):continue
  invalid=bool((na and av&0x400000) or (nb and bv&0x400000))
  initial=0x3f024|enable|(dn<<18)|rm
  expected=(initial&~0x3f000)|(0x10040 if invalid else 0)
  a=ctypes.c_uint32(av);b=ctypes.c_uint32(bv);status=ctypes.c_uint32(initial);other=ctypes.c_uint32(0xdeadbeef)
  dest=[other,a,b][alias];before=dest.value;trap=bool(enable&0x380) or (invalid and bool(enable&0x800))
  assert fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(status),ctypes.byref(dest))==int(trap)
  assert status.value==expected and dest.value==(before if trap else 0x7fbfffff)
  if alias!=1:assert a.value==av
  if alias!=2:assert b.value==bv
  count+=1
 report=dict(cases_passed=count,manual_source='https://www.renesas.com/us/en/node/735531',source_sha256=hashlib.sha256((out/'source/sh4_multiply.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Manual-derived quiet/signaling NaN priority over finite, infinity and denormal operands; FMUL, both rounding modes, DN settings, V enable, sticky/cause bits and destination aliases. SH-4 bit22 signaling convention. Hardware execution unverified; infinity-only arithmetic remains unsupported.')
 (out/'analysis/native_sh4_multiply_nan_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(count,'native SH-4 NaN multiply checks passed')
if __name__=='__main__':main()


