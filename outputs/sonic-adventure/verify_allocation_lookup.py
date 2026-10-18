"""Compare signed bank selection and linked allocation lookup with original instructions."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/allocation_lookup.dll';fn=ctypes.CDLL(str(dll_path)).sa_allocation_lookup_8c664940
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
 cases=0
 for threshold,address,length,match,initial_sr in itertools.product([0,0x400000,0x80000000,0xffffffff],[0,0x3fffff,0x400000,0x7fffffff,0x80000000,0xffffffff],[0,1,4,8],range(-1,8),[0,0x700083f3]):
  for memory in (old,native):
   memory[0xa00000:0xa00500]=b'\xa5'*0x500;memory[0x67c2d0:0x67c310]=b'\xa5'*0x40
   struct.pack_into('<I',memory,0x67c2d8,threshold)
   for bank in [0,1]:
    base=0xa00100+bank*0x200;struct.pack_into('<I',memory,0x67c2e0+bank*20,0x8c000000+base if length else 0)
    for i in range(length):struct.pack_into('<2I',memory,base+i*32+8,0x8c000000+base+(i+1)*32 if i+1<length else 0,address if i==match else address^0x12345678)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=address;regs.r[5]=0x8ca00020;regs.r[15]=0x8cf00000;regs.pc=0x8c664940;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
  assert fn(view,ctypes.byref(sr),address,0x8ca00020)==regs.r[0] and sr.value==regs.sr,(threshold,address,length,match)
  assert old[0xa00000:0xa00500]==native[0xa00000:0xa00500] and old[0x67c2d0:0x67c310]==native[0x67c2d0:0x67c310]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c664940','routine_end_exclusive':'0x8c66497c','retail_span_sha256':hashlib.sha256(binary[0x654940:0x65497c]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/allocation_lookup.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Signed threshold/address boundaries, both bank lists, empty and 1/4/8-node chains, match positions and misses, bank output and neighboring RAM, return/SR, preserved registers. No service hooks.'}
 (out/'analysis/allocation_lookup_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} allocation lookup comparisons passed')
if __name__=='__main__':main()
