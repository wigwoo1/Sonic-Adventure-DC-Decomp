import ctypes,hashlib,itertools,json,random
from sh4_binary32 import divide
from verify_startup import ROOT

def expected(a,b,status):
 aa=a&0x7fffffff;bb=b&0x7fffffff
 if status&0x80000 or status&3>1 or aa>=0x7f800000 or bb>=0x7f800000:return -1,a,b,status,None
 status&=~0x3f000
 if not status&0x40000 and ((0<aa<0x800000) or (0<bb<0x800000)):return 1,a,b,status|0x20000,None
 if status&0x40000:
  if aa<0x800000:a&=0x80000000
  if bb<0x800000:b&=0x80000000
 if not b&0x7fffffff:
  invalid=not a&0x7fffffff;status|=0x10040 if invalid else 0x8020
  result=0x7fbfffff if invalid else ((a^b)&0x80000000)|0x7f800000
  return (1 if status&(0x800 if invalid else 0x400) else 0),a,b,status,result
 result,inexact=divide(a,b,status&1);nearest,_=divide(a,b,0)
 if nearest&0x7fffffff==0x7f800000:status|=0x4010;inexact=True
 if result&0x7fffffff<0x800000:
  if status&0x40000 and result&0x7fffffff:result&=0x80000000;inexact=True
  if inexact:status|=0x2008
 if inexact:status|=0x1004
 return (1 if status&0x380 else 0),a,b,status,result

def main():
 dll=ROOT/'work/sonic-adventure/build/sh4_divide.dll';fn=ctypes.CDLL(str(dll)).sa_sh4_divide;fn.argtypes=[ctypes.POINTER(ctypes.c_uint32)]*4;fn.restype=ctypes.c_int
 rng=random.Random(0xf433);values=[0,0x80000000,1,0x7fffff,0x800000,0x00800001,0x3f800000,0x40400000,0x7f7fffff,0xff7fffff,0x7f800000,0x7fc00000]
 pairs=list(itertools.product(values,repeat=2))+[(rng.randrange(0x7f800000)|(rng.randrange(2)<<31),rng.randrange(1,0x7f800000)|(rng.randrange(2)<<31)) for _ in range(4000)];cases=0
 for (av,bv),mode,dn in itertools.product(pairs,range(2),[0,0x40000]):
  status=0x3f07c|mode|dn;a=ctypes.c_uint32(av);b=ctypes.c_uint32(bv);fpscr=ctypes.c_uint32(status);out=ctypes.c_uint32(0xabcdef12)
  code,ea,eb,ef,result=expected(av,bv,status);actual=fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(fpscr),ctypes.byref(out))
  assert (actual,a.value,b.value,fpscr.value,out.value)==(code,ea,eb,ef,result if code==0 else 0xabcdef12),(hex(av),hex(bv),mode,dn)
  cases+=1
 for alias,av,bv,status in itertools.product(range(3),[0x3f800000,0xbf800000,0x7f7fffff],[0x40400000,0x3f000000],[0,1,0x80,0x100,0x200,0x80000,2]):
  a=ctypes.c_uint32(av);b=ctypes.c_uint32(bv);fpscr=ctypes.c_uint32(status);out=ctypes.c_uint32(0xabcdef12);dest=[out,a,b][alias];code,ea,eb,ef,result=expected(av,bv,status)
  fn(ctypes.byref(a),ctypes.byref(b),ctypes.byref(fpscr),ctypes.byref(dest))
  if code==0:
   if alias==1:ea=result
   if alias==2:eb=result
  assert (a.value,b.value,fpscr.value)==(ea,eb,ef);assert dest.value==(result if code==0 else [0xabcdef12,av,bv][alias]);cases+=1
 report=dict(cases_passed=cases,source_sha256=hashlib.sha256((ROOT/'outputs/sonic-adventure/source/sh4_divide.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Portable integer-only finite division versus verified rational core and explicit FPSCR model. Exponents, signed zero, under/overflow, rounding, DN flushing, unsupported classes, denormal/enabled traps, destination aliasing. Zero-divisor special results implemented; nonfinite operands unsupported; no hardware verification.')
 (ROOT/'outputs/sonic-adventure/analysis/native_sh4_divide_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} native division comparisons passed')
if __name__=='__main__':main()
