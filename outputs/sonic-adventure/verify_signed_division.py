import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/signed_division.dll';fn=ctypes.CDLL(str(dll)).sa_signed_division;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 @Call
 def call(*args):raise AssertionError('Unexpected service')
 values=[0,1,2,99,100,0x7fffffff,0x80000000,0x80000001,0xfffffffe,0xffffffff];rng=random.Random(0x10ca5c);pairs=list(itertools.product(values,repeat=2))+[(rng.getrandbits(32),rng.getrandbits(32)) for _ in range(1000)];count=0
 for (numerator,denominator),sr0,mode in itertools.product(pairs,[0,0x301,0x700083f3,0xffffffff],[0,0x380003]):
  for mem in [ram,native]:mem[0xefffc0:0xf00000]=bytes(64);struct.pack_into('<I',mem,0x7ac108,0x12345678)
  f=Fpu();f.fpscr=mode;f.fpul=0x12345678
  for bank in range(2):f.banks[bank][:]=[0x12340000+i for i in range(16)]
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[0]=denominator;s.r[1]=numerator;s.r[15]=0x8cf00000;s.pc=0x8c10ca5c;s.pr=0x8c000100;s.sr=sr0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(sr0);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  if denominator:
   n=numerator if numerator<0x80000000 else numerator-0x100000000;d=denominator if denominator<0x80000000 else denominator-0x100000000;q=abs(n)//abs(d)
   assert r[0]==(-q if (n<0)!=(d<0) else q)&0xffffffff,(numerator,denominator,hex(r[0]))
  else:assert r[0]==0 and struct.unpack_from('<I',native,0x7ac108)[0]==0x44e
  nf.fpul=fpul.value
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and native==ram,(numerator,denominator,sr0,mode)
  count+=1
 report=dict(cases_passed=count,instruction_count=85,source_sha256=hashlib.sha256((out/'source/sa_signed_division.c').read_bytes()).hexdigest(),binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Whole signed32 quotient helper, DIV0S/SUBC/ROTCL/DIV1/ADDC chain, initial SR modes, sign/extrema/random operands, zero divisor error global and stack/register restoration. Independent integer quotient formula including wrapped INT_MIN/-1 plus full regs/SR/PR/FPU/FPUL/stack/RAM against original instruction model. No services; hardware unverified; caller connection pending.')
 (out/'analysis/signed_division_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'signed division comparisons passed')
if __name__=='__main__':main()
