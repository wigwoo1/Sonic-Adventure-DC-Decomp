"""Compare coalescing with all three original helper bodies executing, without hooks."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/allocation_release.dll';fn=ctypes.CDLL(str(dll_path)).sa_allocation_release_8c66497c
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
 patterns=[[],[(2000,100)],[(1100,100)],[(900,100)],[(900,100),(1100,100)],[(800,100),(900,100),(1100,100),(1200,100)],[(800,100),(900,100),(1100,100),(1200,100),(4000,100)]]
 cases=0
 for bank_index,position,pattern,reverse,flags,total,initial_sr in itertools.product([0,1,0x10000,0x10001],range(3),patterns,[False,True],[0,5,0xffff],[0,0xfffffff0],[0,0x700083f3]):
  base=0x8c8a83e4;allocated=[base+i*24 for i in range(3)];free=[base+(i+3)*24 for i in range(len(pattern))]
  if reverse:free.reverse()
  bank=0x67c2e0+(bank_index&0xffff)*20;node=allocated[position]
  for memory in (old,native):
   memory[0x8a83e0:0x8c03e8]=b'\xa5'*0x18008;memory[0x67c2e0:0x67c308]=bytes(40)
   struct.pack_into('<5I',memory,bank,allocated[0],allocated[-1],free[0] if free else 0,free[-1] if free else 0,total)
   for chain in [allocated,free]:
    for i,record in enumerate(chain):struct.pack_into('<2I',memory,(record&0xffffff)+4,chain[i-1] if i else 0,chain[i+1] if i+1<len(chain) else 0)
   for i,record in enumerate(allocated):
    struct.pack_into('<H',memory,record&0xffffff,flags)
    struct.pack_into('<2I',memory,(record&0xffffff)+12,1000 if i==position else 6000+i*100,100)
   for i,(address,size) in enumerate(pattern):
    record=base+(i+3)*24;struct.pack_into('<H',memory,record&0xffffff,flags|1);struct.pack_into('<2I',memory,(record&0xffffff)+12,address,size)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=bank_index;regs.r[5]=node;regs.r[15]=0x8cf00000;regs.pc=0x8c66497c;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
  assert fn(view,ctypes.byref(sr),bank_index,node)==regs.r[0]==0 and sr.value==regs.sr,(bank_index,position,pattern,reverse)
  assert old[0x8a83e0:0x8c03e8]==native[0x8a83e0:0x8c03e8]
  assert old[0x67c2e0:0x67c308]==native[0x67c2e0:0x67c308]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c66497c','routine_end_exclusive':'0x8c664ac0','retail_span_sha256':hashlib.sha256(binary[0x65497c:0x654ac0]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/allocation_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'original_bodies_connected':4,'scope':'Both banks and low-halfword aliases, every allocated-node position, empty/nonadjacent/successor/predecessor/multiple-neighbor free lists in both traversal orders, flags and total wrap boundaries. Full pool/banks, return/SR, preserved registers compared against original release, removal, insertion and pool scan without hooks.'}
 (out/'analysis/allocation_release_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} allocation coalescing comparisons passed')
if __name__=='__main__':main()
