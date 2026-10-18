import ctypes,itertools,json,math,random,struct
from fractions import Fraction
from verify_startup import ROOT
out=ROOT/'outputs/sonic-adventure'
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sh4_sqrt.dll')).sa_sh4_sqrt;fn.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)]
def value(raw):
 exponent=(raw>>23)-150;significand=(raw&0x7fffff)|0x800000
 return Fraction(significand*(1<<exponent),1) if exponent>=0 else Fraction(significand,1<<-exponent)
def oracle(raw,mode):
 if raw in (0,0x80000000,0x7f800000):return raw,False
 x=value(raw);candidate=struct.unpack('<I',struct.pack('<f',math.sqrt(float(x))))[0]
 # Exact squared rational comparisons find the representable lower bracket.
 while value(candidate)**2>x:candidate-=1
 while value(candidate+1)**2<=x:candidate+=1
 exact=value(candidate)**2==x
 if mode==0 and not exact:
  midpoint=(value(candidate)+value(candidate+1))/2
  if x>midpoint**2 or (x==midpoint**2 and candidate&1):candidate+=1
 return candidate,not exact
rng=random.Random(0x534134)
values=[0,0x80000000,0x7f800000,0x00800000,0x00800001,0x3f800000,0x40000000,0x40800000,0x7f7fffff]
values += [rng.randrange(0x800000,0x7f800000) for _ in range(2048)]
cases=0
for raw,mode,enabled,sticky in itertools.product(values,[0,1],[0,0x80],[0,0x7c]):
 expected,inexact=oracle(raw,mode);initial=0x200000|0x3f000|mode|enabled|sticky
 fpscr=ctypes.c_uint32(initial);result=ctypes.c_uint32(0xabcdef01)
 status=fn(raw,ctypes.byref(fpscr),ctypes.byref(result));normal=raw not in (0,0x80000000,0x7f800000)
 trap=bool(normal and enabled)
 assert status==int(trap) and result.value==(0xabcdef01 if trap else expected),(hex(raw),mode,status,hex(result.value),hex(expected))
 assert fpscr.value==((initial&~0x3f000)|(0x1004 if inexact else 0)),hex(raw)
 cases+=1
for raw,status in [(0xbf800000,0),(0x7fc00000,0),(1,0),(0x3f800000,0x80000),(0x3f800000,2)]:
 f=ctypes.c_uint32(status);r=ctypes.c_uint32(0xabcdef01)
 assert fn(raw,ctypes.byref(f),ctypes.byref(r))==-1 and f.value==status and r.value==0xabcdef01
report=dict(cases_passed=cases,unsupported_guard_cases_passed=5,scope='Positive normal binary32 FSQRT plus signed zero/+infinity. Seeded2048 random normal inputs and boundaries, RM0/1, enabled inexact and sticky flags. Independent exact rational squared-bracket/midpoint oracle; cause clear, sticky I, unconditional normal EN.I trap/destination preservation. Negative/NaN/denormal/PR and reserved RM paths explicitly unsupported.',primary_source='https://www.renesas.com/en/document/mas/sh-4-software-manual',section='9.43 FSQRT')
(out/'analysis/native_sh4_sqrt_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'native binary32 square-root comparisons passed')
