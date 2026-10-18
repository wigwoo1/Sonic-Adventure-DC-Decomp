"""Compare exact resource type classification with original instructions, without hooks."""
import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 dll_path=ROOT/'work/sonic-adventure/build/resource_type_classifier.dll'
 fn=ctypes.CDLL(str(dll_path)).sa_resource_type_classifier_8c64f206
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];fn.restype=ctypes.c_uint32
 rng=random.Random(0x64f206)
 values=list(range(65536))+[0x10000|x for x in [0x500,0x600,0x700,0x800]]+[0xffffffff,0x80000000]+[rng.getrandbits(32) for _ in range(1024)]
 cases=0
 for initial_sr in [0,0x700083f3]:
  for value in values:
   regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=value;regs.r[15]=0x8cf00000;regs.pc=0x8c64f206;regs.pr=0x8c000100;regs.sr=initial_sr
   saved=list(regs.r[2:]);sr=ctypes.c_uint32(initial_sr);reference(ram,regs)
   assert fn(ctypes.byref(sr),value)==regs.r[0] and sr.value==regs.sr,(value,initial_sr)
   assert list(regs.r[2:])==saved and regs.pr==0x8c000100
   cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report={'routine_start':'0x8c64f206','routine_end_exclusive':'0x8c64f22e','retail_span_sha256':hashlib.sha256(binary[0x63f206:0x63f22e]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/resource_type_classifier.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Exhaustive 16-bit input space, high-bit aliases and deterministic 32-bit inputs, both initial T states; return, full SR, untouched r2-r15 and PR compared with original instructions; no service hooks.'}
 (out/'analysis/resource_type_classifier_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f'{cases} resource type classifier comparisons passed')
if __name__=='__main__':main()
