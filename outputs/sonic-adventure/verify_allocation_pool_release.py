"""Compare exact fixed-pool membership and flag clearing against original instructions."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/allocation_pool_release.dll';fn=ctypes.CDLL(str(dll_path)).sa_allocation_pool_release_8c664aec
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
 addresses=[0x8c8a83e4+i*24 for i in [0,1,2,31,127,255,511,4094,4095]]+[0x8c8a83e3,0x8c8a83e5,0x8c8c03e4,0x0c8a83e4,0,0xffffffff]
 cases=0
 for address,flags,initial_sr,incoming_r0 in itertools.product(addresses,[0,1,2,0xffff],[0,0x700083f3],[0,0x89abcdef]):
  for memory in (old,native):
   memory[0x8a83e0:0x8c03e8]=b'\xa5'*0x18008
   for i in range(4096):struct.pack_into('<H',memory,0x8a83e4+i*24,flags)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[0]=incoming_r0;regs.r[4]=address;regs.r[15]=0x8cf00000;regs.pc=0x8c664aec;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
  assert fn(view,ctypes.byref(sr),address,incoming_r0)==regs.r[0]==incoming_r0 and sr.value==regs.sr
  assert old[0x8a83e0:0x8c03e8]==native[0x8a83e0:0x8c03e8]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c664aec','routine_end_exclusive':'0x8c664b12','retail_span_sha256':hashlib.sha256(binary[0x654aec:0x654b12]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/allocation_pool_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Pool start/interior/end records, off-grid/outside/alias addresses, header flag and incoming r0 boundaries, entire 4096-record pool and neighbors, return/SR and preserved registers; original instructions with no hooks.'}
 (out/'analysis/allocation_pool_release_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} allocation pool release comparisons passed')
if __name__=='__main__':main()
