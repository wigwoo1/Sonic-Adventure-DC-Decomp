"""Check exact-dyadic rounding against independently encoded numeric vectors."""
import hashlib,json,random,struct
from sh4_binary32 import add_sub,round_dyadic
from verify_startup import ROOT

def value(bits):return struct.unpack('<f',struct.pack('<I',bits))[0]
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def main():
 vectors=[(1<<24, -24,0,0x3f800000),( (1<<24)+1,-24,0,0x3f800000),((1<<24)+3,-24,0,0x3f800002),((1<<24)+3,-24,1,0x3f800001),(1,-150,0,0),(3,-150,0,2),(3,-150,1,1),(-3,-150,0,0x80000002),(1,128,0,0x7f800000),(1,128,1,0x7f7fffff)]
 for coefficient,power,mode,expected in vectors:assert round_dyadic(coefficient,power,mode)[0]==expected
 rng=random.Random(0x79476);cases=len(vectors)
 for _ in range(10000):
  # Exponent separation <=20 ensures the host binary64 sum is exact.
  e=rng.randrange(40,200);a=(rng.randrange(2)<<31)|(e<<23)|rng.getrandbits(23);b=(rng.randrange(2)<<31)|((e+rng.randrange(-20,21))<<23)|rng.getrandbits(23)
  for subtract in (False,True):
   exact=value(a)+(-value(b) if subtract else value(b));expected=bits(exact)
   result,inexact=add_sub(a,b,subtract,0);assert result==expected,(hex(a),hex(b),subtract)
   assert inexact==(value(expected)!=exact)
   truncated,_=add_sub(a,b,subtract,1)
   assert abs(value(truncated))<=abs(exact)
   assert truncated==expected or truncated==(expected-1)&0xffffffff
   cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'sh4_binary32.py').read_bytes()).hexdigest(),manual='https://www.renesas.com/en/document/mas/sh-4-software-manual',scope='Exact finite dyadic binary32 numerical rounding core: nearest-even and toward-zero, tie parity, gradual subnormal rounding, overflow and signed results. Random additions/subtractions use exactly representable binary64 intermediates as independent oracle. Not yet connected to instruction execution; FPSCR flags, DN flushing, traps, NaNs/infinities and FMAC remain separate pending work.')
 (out/'analysis/sh4_binary32_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} binary32 arithmetic checks passed')
if __name__=='__main__':main()
