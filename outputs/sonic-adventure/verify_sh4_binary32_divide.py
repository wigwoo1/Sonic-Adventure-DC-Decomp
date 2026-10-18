"""Exact quotient core against an independent Fraction/binary-search oracle."""
import hashlib,itertools,json,random
from fractions import Fraction
from sh4_binary32 import divide
from verify_startup import ROOT

def value(bits):
 e=(bits>>23)&255;m=bits&0x7fffff
 if e:m|=0x800000
 p=-149 if not e else e-150
 return Fraction(m*(1<<p),1) if p>=0 else Fraction(m,1<<-p)
def oracle(a,b,mode):
 target=value(a&0x7fffffff)/value(b&0x7fffffff);sign=(a^b)&0x80000000
 low,high=0,0x7f7fffff
 while low<high:
  middle=(low+high+1)//2
  if value(middle)<=target:low=middle
  else:high=middle-1
 exact=value(low)==target
 if mode==1 or exact:return low|sign,not exact
 upper=low+1
 # Infinity rounds at the finite conceptual next value 2^128.
 next_value=Fraction(1<<128) if upper==0x7f800000 else value(upper)
 dl=target-value(low);du=next_value-target
 result=upper if du<dl or (du==dl and low&1) else low
 return result|sign,not exact

def main():
 rng=random.Random(0xf003);values=[0,0x80000000,1,0x7fffff,0x800000,0x3f800000,0x40000000,0x40400000,0x7f7fffff,0xff7fffff]
 pairs=list(itertools.product(values,[v for v in values if v&0x7fffffff]))+[(rng.randrange(0x7f800000)|(rng.randrange(2)<<31),rng.randrange(1,0x7f800000)|(rng.randrange(2)<<31)) for _ in range(2000)]
 for a,b in pairs:
  for mode in [0,1]:assert divide(a,b,mode)==oracle(a,b,mode),(hex(a),hex(b),mode,divide(a,b,mode),oracle(a,b,mode))
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=len(pairs)*2,source_sha256=hashlib.sha256((out/'sh4_binary32.py').read_bytes()).hexdigest(),scope='Exact finite nonzero-divisor binary32 numerical core against independent Fraction binary-search oracle, nearest-even and toward-zero, signed zero, subnormal and overflow boundaries. FPSCR, instruction execution, native C and exceptional classification not yet implemented.')
 (out/'analysis/sh4_binary32_divide_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(pairs)*2} exact quotient comparisons passed')
if __name__=='__main__':main()
