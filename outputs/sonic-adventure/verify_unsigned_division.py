"""Compare the original unsigned division helper and native source, including SR and zero divisor."""
import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/unsigned_division.dll';fn=ctypes.CDLL(str(dll_path)).sa_unsigned_division_8c10cb10
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_uint32
 boundaries=[0,1,2,3,67,68,69,0x7fff,0xffff,0x10000,0x7fffffff,0x80000000,0xfffffffe,0xffffffff]
 pairs=list(itertools.product(boundaries,boundaries));rng=random.Random(0x10cb10)
 pairs.extend((rng.getrandbits(32),rng.getrandbits(32)) for _ in range(2048))
 pairs.extend((x,68) for x in range(4096))
 cases=0
 for numerator,denominator in pairs:
  for initial_sr in [0,0x700083f3,0x200,0x101]:
   for memory in (ram,native):memory[0x7ac104:0x7ac110]=b'\xa5'*12
   regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[0]=denominator;regs.r[1]=numerator;regs.r[15]=0x8cf00000;regs.pc=0x8c10cb10;regs.pr=0x8c000100;regs.sr=initial_sr
   saved=list(regs.r[2:]);sr=ctypes.c_uint32(initial_sr);r1=ctypes.c_uint32();reference(ram,regs)
   result=fn(view,ctypes.byref(sr),numerator,denominator,ctypes.byref(r1))
   assert result==regs.r[0] and r1.value==regs.r[1] and sr.value==regs.sr,(numerator,denominator,initial_sr)
   if denominator:assert result==numerator//denominator
   else:assert result==0 and r1.value==0x44e
   assert ram[0x7ac104:0x7ac110]==native[0x7ac104:0x7ac110]
   assert list(regs.r[2:])==saved and regs.pr==0x8c000100
   cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c10cb10','routine_end_exclusive':'0x8c10cbae','retail_span_sha256':hashlib.sha256(binary[0xfcb10:0xfcbae]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/unsigned_division.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Boundary operand cross product, deterministic 32-bit pairs, all numerators 0..4095 divided by 68, four initial SR states. Original instructions without hooks; quotient also checked against independent host division. r0/r1, full SR, r2-r15 and PR, zero-divisor error word and neighbors compared.'}
 (out/'analysis/unsigned_division_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} unsigned division comparisons passed')
if __name__=='__main__':main()
