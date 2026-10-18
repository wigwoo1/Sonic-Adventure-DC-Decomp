"""Compare allocation-list insertion with original instructions without hooks."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/allocation_list_insert.dll';fn=ctypes.CDLL(str(dll_path)).sa_allocation_list_insert_8c664b12
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*4;fn.restype=ctypes.c_uint32
 cases=0
 for length,head_prev,initial_sr,incoming_r0 in itertools.product(range(17),[0,0x8ca00400],[0,0x700083f3],[0,0x89abcdef]):
  for memory in (old,native):
   memory[0xa00000:0xa00500]=b'\xa5'*0x500
   struct.pack_into('<2I',memory,0xa00000,0x8ca00100 if length else 0,0x8ca00100+(length-1)*32 if length else 0)
   for i in range(length):struct.pack_into('<2I',memory,0xa00100+i*32+4,0x8ca00100+(i-1)*32 if i else head_prev,0x8ca00100+(i+1)*32 if i+1<length else 0)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[0]=incoming_r0;regs.r[4]=0x8ca00000;regs.r[5]=0x8ca00004;regs.r[6]=0x8ca00040;regs.r[15]=0x8cf00000;regs.pc=0x8c664b12;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
  assert fn(view,ctypes.byref(sr),0x8ca00000,0x8ca00004,0x8ca00040,incoming_r0)==regs.r[0] and sr.value==regs.sr
  assert old[0xa00000:0xa00500]==native[0xa00000:0xa00500]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c664b12','routine_end_exclusive':'0x8c664b36','retail_span_sha256':hashlib.sha256(binary[0x654b12:0x654b36]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/allocation_list_insert.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Empty and lengths 1..16, prior head-link values, incoming r0 and SR boundaries, head/tail/node/full list and neighbor RAM, preserved registers; original instructions with no hooks.'}
 (out/'analysis/allocation_list_insert_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} allocation list insertion comparisons passed')
if __name__=='__main__':main()
