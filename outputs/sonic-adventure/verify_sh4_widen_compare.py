"""Check conversion/comparison status, untouched state and register-bank selection."""
import ctypes,hashlib,itertools,json,random
from pathlib import Path
import sh4_widen_reference as model
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
class Fpu(ctypes.Structure):_fields_=[('banks',(ctypes.c_uint32*16)*2),('fpscr',ctypes.c_uint32)]
def main():
 dll=ROOT/'work/sonic-adventure/build/movie_diagnostics.dll';lib=ctypes.CDLL(str(dll));conv=lib.sa_sh4_fcnvsd;cmp=lib.sa_sh4_compare_pair;conv.argtypes=[ctypes.POINTER(Fpu),ctypes.c_uint32,ctypes.c_uint];cmp.argtypes=[ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint,ctypes.c_uint,ctypes.c_int]
 f=Fpu();rng=random.Random(7091);f.banks[0][:]=[rng.getrandbits(32) for _ in range(16)];f.banks[1][:]=[rng.getrandbits(32) for _ in range(16)];initial=bytes(f);counts={'conversion':0,'double_comparison':0,'single_regression':0,'invalid_contract':0}
 raw_values=[0,0x80000000,1,0x80000001,0x007fffff,0x807fffff,0x00800000,0x80800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7f800001,0x7fbfffff,0x7fc00000,0x7fffffff]+[rng.getrandbits(32) for _ in range(128)]
 for raw,fr,sz,dn,enable,rm,n in itertools.product(raw_values,(0,0x200000),(0,0x100000),(0,0x40000),(0,0x800),(0,1,2,3),(0,2,14)):
  f=Fpu.from_buffer_copy(initial);f.fpscr=0x80000|0x3f07c|fr|sz|dn|enable|rm;expected=Fpu.from_buffer_copy(f);rc=model.widen(expected,raw,n)
  assert conv(ctypes.byref(f),raw,n)==rc and bytes(f)==bytes(expected),(hex(raw),hex(f.fpscr),n);counts['conversion']+=1
 values64=[0,1<<63,1,(1<<63)|1,0xfffffffffffff,0x10000000000000,0x3ff0000000000000,0xbff0000000000000,0x7fefffffffffffff,0x7ff0000000000000,0xfff0000000000000,0x7ff0000000000001,0x7ff7ffffffffffff,0x7ff8000000000000,0xfff8000000000000]
 for a,b,fr,sz,dn,enable,t,greater in itertools.product(values64,values64,(0,0x200000),(0,0x100000),(0,0x40000),(0,0x800),(0,1),(0,1)):
  f=Fpu.from_buffer_copy(initial);f.fpscr=0x80000|0x3f07c|fr|sz|dn|enable;bank=fr>>21;f.banks[bank][2]=a>>32;f.banks[bank][3]=a&0xffffffff;f.banks[bank][4]=b>>32;f.banks[bank][5]=b&0xffffffff;sr=ctypes.c_uint32(0x700083f2|t);expected=Fpu.from_buffer_copy(f);rc,tgt=model.compare(expected,sr.value,2,4,greater)
  assert cmp(ctypes.byref(f),ctypes.byref(sr),2,4,greater)==rc and sr.value==tgt and bytes(f)==bytes(expected);counts['double_comparison']+=1
 for a,b,fr,dn,enable,t,greater in itertools.product(raw_values[:18],raw_values[:18],(0,0x200000),(0,0x40000),(0,0x800),(0,1),(0,1)):
  f=Fpu.from_buffer_copy(initial);f.fpscr=0x3f07c|fr|dn|enable;bank=fr>>21;f.banks[bank][3]=a;f.banks[bank][5]=b;sr=ctypes.c_uint32(0x700083f2|t);expected=Fpu.from_buffer_copy(f);rc,tgt=model.compare(expected,sr.value,3,5,greater)
  assert cmp(ctypes.byref(f),ctypes.byref(sr),3,5,greater)==rc and sr.value==tgt and bytes(f)==bytes(expected);counts['single_regression']+=1
 for n,status in itertools.product((0,1,15,16,0xffffffff),(0,0x80000)):
  if n<=14 and not n&1 and status&0x80000:continue
  f=Fpu.from_buffer_copy(initial);f.fpscr=status;before=bytes(f);assert conv(ctypes.byref(f),0,n)==-1 and bytes(f)==before;counts['invalid_contract']+=1
 for n,m,status in ((1,2,0x80000),(2,3,0x80000),(16,0,0),(0,16,0)):
  f=Fpu.from_buffer_copy(initial);f.fpscr=status;before=bytes(f);sr=ctypes.c_uint32(0x12345679);assert cmp(ctypes.byref(f),ctypes.byref(sr),n,m,0)==-1 and bytes(f)==before and sr.value==0x12345679;counts['invalid_contract']+=1
 assert conv(None,0,0)==-1 and cmp(None,None,0,0,0)==-1;counts['invalid_contract']+=2
 files=[Path(__file__),P/'sh4_widen_reference.py',P/'source/sh4_widen_compare.c',P/'source/sh4_widen_compare.h',P/'source/sh4_compare_bits.h']
 report=dict(cases_passed=sum(counts.values()),counts=counts,manual_source=model.MANUAL,manual_sections=['9.27','9.29'],dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Independent Python host-struct finite conversions and comparisons plus manual exception/NaN/denormal rules versus native integer-bit model. Every FPU byte and SR checked. Includes FR/SZ, signed zeros, finite edges, infinities, both SH-4 NaN classes, DN, invalid enables, sticky flags/cause reset and destination/T preservation on traps. No physical processor or trap delivery claim; no game routine credit.')
 (P/'analysis/sh4_widen_compare_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['cases_passed'],counts)
if __name__=='__main__':main()
