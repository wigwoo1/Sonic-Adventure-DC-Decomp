import ctypes,hashlib,itertools,json,math,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 dll=ROOT/'work/sonic-adventure/build/sh4_truncate_i32.dll';fn=ctypes.CDLL(str(dll)).sa_sh4_truncate_i32;fn.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int
 rng=random.Random(0xf33d);values=[0,0x80000000,1,0x807fffff,0x3fc00000,0xbfc00000,0x4effffff,0x4f000000,0xcf000000,0xcf000001,0x7f800000,0xff800000,0x7fc00001,0xff800001]+[rng.getrandbits(32) for _ in range(4000)];ram=bytearray(16*1024*1024);struct.pack_into('<3H',ram,0x100,0xf33d,11,9);cases=0
 for raw,mode,bank,dn in itertools.product(values,range(4),range(2),[0,0x40000]):
  value=struct.unpack('<f',struct.pack('<I',raw))[0];invalid=not math.isfinite(value) or value>=2147483648 or value< -2147483648
  result=(0x80000000 if math.isnan(value) or value<0 else 0x7fffffff) if invalid else int(value)&0xffffffff
  status=(bank<<21)|dn|mode|0x3f03c;expected=(status&~0x3f000)|(0x10040 if invalid else 0)
  fpu=Fpu();fpu.fpscr=status;fpu.banks[bank][3]=raw;fpu.fpul=0xabcdef12;state=State();state.pc=0x8c000100;state.pr=0x8c000080;state.sr=0x700083f3;reference(ram,state,fpu=fpu)
  assert fpu.fpul==result and fpu.fpscr==expected and fpu.banks[bank][3]==raw and state.sr==0x700083f3
  fpscr=ctypes.c_uint32(status);out=ctypes.c_uint32(0xabcdef12);assert fn(raw,ctypes.byref(fpscr),ctypes.byref(out))==0 and (out.value,fpscr.value)==(result,expected)
  fpscr.value=status|0x800;out.value=0xabcdef12;code=fn(raw,ctypes.byref(fpscr),ctypes.byref(out));assert code==int(invalid) and out.value==(0xabcdef12 if invalid else result);cases+=2
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/sh4_truncate_i32.c').read_bytes()).hexdigest(),manual='https://www.renesas.com/en/document/mas/sh-4-software-manual',scope='Single FTRC instruction and native integer conversion checked against host exact integer truncation with explicit SH-4 range/NaN saturation rules. All RM values, banks, DN, signed zero, subnormals, fractional values, boundaries, infinities/NaNs, V flags and enabled-V destination preservation. Double precision/hardware unverified.')
 (outdir/'analysis/native_sh4_truncate_i32_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} integer truncation checks passed')
if __name__=='__main__':main()
