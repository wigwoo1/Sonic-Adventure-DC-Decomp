"""Compare metadata construction and recovered division with original unhooked instructions."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/payload_metadata.dll';fn=ctypes.CDLL(str(dll_path)).sa_payload_metadata_8c6652c0
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
 cases=0
 for size,flags,alias,initial_sr in itertools.product(list(range(256))+[0x10008,0x80000000,0xffffffff],[0,1,2,0xffffffff],[False,True],[0,0x700083f3]):
  output=0x8ca00000 if alias else 0x8ca00100
  for memory in (old,native):
   memory[0xa00000:0xa00200]=b'\xa5'*0x200;struct.pack_into('<I',memory,0xa0000c,size);struct.pack_into('<I',memory,0xa00018,flags)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=output;regs.r[5]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c6652c0;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
  assert fn(view,ctypes.byref(sr),output,0x8ca00000)==regs.r[0] and sr.value==regs.sr,(size,flags,alias,initial_sr)
  assert old[0xa00000:0xa00200]==native[0xa00000:0xa00200]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c6652c0','routine_end_exclusive':'0x8c6653e8','retail_span_sha256':hashlib.sha256(binary[0x6552c0:0x6553e8]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/payload_metadata.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Sizes 0..255 and high-bit invalid values, flag boundaries, output/config aliasing, return/SR, whole neighboring RAM and preserved registers. Original metadata and division execute without hooks.'}
 (out/'analysis/payload_metadata_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} payload metadata comparisons passed')
if __name__=='__main__':main()
