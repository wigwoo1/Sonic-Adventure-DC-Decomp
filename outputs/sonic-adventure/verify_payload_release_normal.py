"""Original paired release and all allocation helpers execute without hooks."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/payload_release_runtime.dll';fn=ctypes.CDLL(str(dll_path)).sa_payload_release_connected
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
 cases=0
 for bank,first_flags,partner_flags,presence,merge,initial_sr in itertools.product([0,1],[0,2,4,6],[0,4],['both','first_missing','partner_missing'],[False,True],[0,0x700083f3]):
  address=1000+bank*0x400000;base=0x8c8a83e4
  for memory in (old,native):
   memory[0x8a83e0:0x8c03e8]=b'\xa5'*0x18008;memory[0x67c2d0:0x67c308]=bytes(56);memory[0xefffe0:0xf00000]=b'\xa5'*32
   struct.pack_into('<2I',memory,0x67c2d4,0x400000,0x400000)
   for b in [0,1]:
    node=base+b*24;free=base+(b+2)*24
    missing=(presence=='first_missing' and b==bank) or (presence=='partner_missing' and b!=bank)
    struct.pack_into('<5I',memory,0x67c2e0+b*20,0 if missing else node,0 if missing else node,free,free,0x1000)
    struct.pack_into('<H',memory,node&0xffffff,first_flags if b==bank else partner_flags)
    struct.pack_into('<4I',memory,(node&0xffffff)+4,0,0,1000+b*0x400000,100)
    struct.pack_into('<H',memory,free&0xffffff,1)
    struct.pack_into('<4I',memory,(free&0xffffff)+4,0,0,(1100 if merge else 4000)+b*0x400000,100)
  for memory in (old,native):
   memory[0xa00000:0xa00040]=bytes(64);struct.pack_into('<I',memory,0xa0001c,address)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c653d68;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
  assert fn(view,ctypes.byref(sr),0x8ca00000,0x8cf00000)==regs.r[0] and sr.value==regs.sr,(bank,first_flags,partner_flags,presence,merge)
  assert old[0x8a83e0:0x8c03e8]==native[0x8a83e0:0x8c03e8]
  assert old[0x67c2d0:0x67c308]==native[0x67c2d0:0x67c308]
  for offset in [0xefffe4,0xefffe8]:assert old[offset:offset+2]==native[offset:offset+2]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c66385a','routine_end_exclusive':'0x8c6638ea','retail_span_sha256':hashlib.sha256(binary[0x65385a:0x6538ea]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/payload_release_runtime.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'original_bodies_connected':7,'scope':'Both banks, pair and partner flags, first/partner lookup failure, adjacent and nonadjacent free range, SR states; full pool/bank bytes and meaningful stack bank outputs, return/SR and preserved registers. Dispatcher, paired release, lookup, coalescing, list removal/insertion and pool release execute without hooks.'}
 (out/'analysis/payload_release_normal_connected_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} paired allocation release comparisons passed')
if __name__=='__main__':main()
