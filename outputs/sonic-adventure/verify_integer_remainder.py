import ctypes,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/integer_remainder.dll')).sa_integer_remainder
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
@Call
def call(*args):return -9
bounds=[-2147483648,-129,-1,0,1,2,127,2147483647]
pairs=list(itertools.product(bounds,bounds));rng=random.Random(0x10cbb8)
pairs.extend((ctypes.c_int32(rng.getrandbits(32)).value,ctypes.c_int32(rng.getrandbits(32)).value) for _ in range(32))
cases=0
for (numerator,denominator),bank,sr0 in itertools.product(pairs,[0,0x200000],[0,0x70000363]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x7ac104:0x7ac110]=bytes([0xa5])*12
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[0]=denominator&0xffffffff;s.r[1]=numerator&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c10cbb8
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
 if denominator:
  q=abs(numerator)//abs(denominator)
  if (numerator<0)!=(denominator<0):q=-q
  expected=(numerator-q*denominator)&0xffffffff
  assert r[0]==expected,(numerator,denominator,hex(r[0]),hex(expected))
  assert native[0x7ac104:0x7ac110]==bytes([0xa5])*12
 else:
  assert r[0]==0 and struct.unpack_from('<I',native,0x7ac108)[0]==0x44e
  assert native[0x7ac104:0x7ac108]==native[0x7ac10c:0x7ac110]==bytes([0xa5])*4
 cases+=1
(out/'analysis/integer_remainder_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native95-instruction signed integer remainder10cbb8. Signed boundaries including INT_MIN/-1,zero divisors,32 deterministic random pairs,banks/SR. Full retail RAM/CPU/FPU/SR/PR matches; independent truncation-toward-zero remainder and zero-divisor error44E/resultzero/neighbor preservation checked. No modeled services. Hardware timing unverified.'),indent=2)+'\n')
print(cases,'signed integer remainder comparisons passed')
