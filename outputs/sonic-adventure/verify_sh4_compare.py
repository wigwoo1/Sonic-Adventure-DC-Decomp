import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT
def main():
 out=ROOT/'outputs/sonic-adventure';fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sh4_compare.dll')).sa_sh4_compare;P=ctypes.POINTER(ctypes.c_uint32);fn.argtypes=[ctypes.c_uint32,ctypes.c_uint32,P,P,ctypes.c_int];count=0
 values=[0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7f800001,0xffbfffff,0x7fc00001,0xffc00001]
 for a,b,greater,enable,t0 in itertools.product(values,values,range(2),[0,0x800],range(2)):
  original=0x3f07c|enable;fpscr=ctypes.c_uint32(original);sr=ctypes.c_uint32(0x700083f2|t0)
  nan=any((v&0x7fffffff)>0x7f800000 for v in [a,b]);signaling=any((v&0x7fffffff)>0x7f800000 and v&0x400000 for v in [a,b]);invalid=signaling or (greater and nan)
  status=(original&~0x3f000)|(0x10040 if invalid else 0);trap=bool(invalid and enable)
  fa=struct.unpack('<f',struct.pack('<I',a))[0];fb=struct.unpack('<f',struct.pack('<I',b))[0]
  condition=False if nan else (fa>fb if greater else fa==fb);expected_sr=(0x700083f2|t0) if trap else 0x700083f2|int(condition)
  assert fn(a,b,ctypes.byref(fpscr),ctypes.byref(sr),greater)==int(trap) and fpscr.value==status and sr.value==expected_sr,(a,b,greater,enable,t0)
  count+=1
 report=dict(cases_passed=count,manual_source='https://www.renesas.com/us/en/node/735531',manual_section='9.27 FCMP',source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in ['sh4_compare.c','sh4_compare_bits.h']},scope='Manual-derived EQ/GT NaN invalid/unordered rules, infinity ordering, signed-zero equivalence, sticky flags, cause reset and T preservation on enabled invalid exception. PR=0; denormal DN0 unsupported, DN1 support not covered here.')
 (out/'analysis/sh4_compare_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'SH-4 comparison checks passed')
if __name__=='__main__':main()
