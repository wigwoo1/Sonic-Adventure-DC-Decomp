"""Special FDIV vectors derived from SH-4 manual9.30 and FPSCR field layout."""
import ctypes,hashlib,itertools,json
from verify_startup import ROOT
def main():
 out=ROOT/'outputs/sonic-adventure';fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sh4_divide_zero.dll')).sa_sh4_divide;P=ctypes.POINTER(ctypes.c_uint32);fn.argtypes=[P,P,P,P];count=0
 for a0,b0,rm,dn,sticky,enable,alias in itertools.product([0,0x80000000,0x3f800000,0xbf800000,0x00800000,0x80800000,0x7f7fffff,0xff7fffff],[0,0x80000000],range(2),[0,0x40000],[0,0x7c,0x3f07c],[0,0x400,0x800,0xc00],range(2)):
  a=ctypes.c_uint32(a0);b=ctypes.c_uint32(b0);status0=rm|dn|sticky|enable;status=ctypes.c_uint32(status0);dest=ctypes.c_uint32(0x12345678);target=a if alias else dest
  invalid=not a0&0x7fffffff;trap=bool(enable&(0x800 if invalid else 0x400));expected_status=(status0&~0x3f000)|(0x10040 if invalid else 0x8020)
  expected_result=(a0 if alias else 0x12345678) if trap else (0x7fbfffff if invalid else ((a0^b0)&0x80000000)|0x7f800000)
  result=fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(status),ctypes.byref(target))
  assert result==int(trap) and status.value==expected_status and target.value==expected_result and b.value==b0
  if not alias:assert a.value==a0
  count+=1
 names=['sh4_divide.c']
 report=dict(cases_passed=count,source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/sh4_divide_zero.dll').read_bytes()).hexdigest(),manual_source='https://www.renesas.com/us/en/node/735531',manual_sections=['9.30 FDIV','6.3.2 FPSCR','6.3 NaN'],scope='Independent manual-derived finite/signed-zero FDIV vectors, signed infinity/default quiet NaN, cause reset and sticky flags, enabled Z/V exception return and destination preservation, destination aliases numerator. PR=0, RM0/1, DN0/1. Nonfinite operands and denormal flush/exception cases remain outside this verification.')
 (out/'analysis/native_sh4_divide_zero_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'manual-derived zero-divisor comparisons passed')
if __name__=='__main__':main()
