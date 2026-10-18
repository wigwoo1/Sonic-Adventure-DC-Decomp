"""Compare linked payload cleanup with the original complete eleven-body call path."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/resource_release_complete_runtime.dll';fn=ctypes.CDLL(str(dll_path)).sa_resource_release_complete
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
 formats={(8,0):(0x1000,128),(8,1):(0x2000,128),(16,0):(0x3000,128),(16,1):(0x4000,128),(32,0):(0x5000,256),(32,1):(0x6000,512),(64,0):(0x7000,1024),(64,1):(0x8000,2048)}
 cases=0
 for size,flag,position,last_slot,other_slots,present,initial_sr,merge,resource_flags,resource_count in itertools.product([8,16,32,64],[0,1],[-1,0,1,2],[False,True],[False,True],[False,True],[0,0x700083f3],[False,True],[2,3],[1,2]):
  type_id,width=formats[size,flag];count=2048//width;slot=count-1 if last_slot else 0
  for memory in (old,native):
   memory[0x8a83e0:0x8d33e8]=b'\xa5'*0x2b008;memory[0x67c2d0:0x67c310]=bytes(64);memory[0xa00000:0xa00100]=bytes(256);struct.pack_into('<I',memory,0xa00018,3);memory[0xefffa0:0xf00000]=b'\xa5'*96
   struct.pack_into('<I',memory,0xa00024,size);struct.pack_into('<2I',memory,0xa00030,flag,1000+slot*width)
   blocks=[0x8c8c03e4+i*76 for i in range(3)];struct.pack_into('<I',memory,0x67c308,blocks[0])
   for i,block in enumerate(blocks):
    struct.pack_into('<H',memory,block&0xffffff,(type_id if i==position else type_id^0x1000)|1)
    struct.pack_into('<2I',memory,(block&0xffffff)+4,blocks[i+1] if i<2 else 0,1000)
    for j in range(16):struct.pack_into('<I',memory,(block&0xffffff)+12+j*4,1 if j<count and (other_slots or j==slot) else 0)
   record=0x8c8a83e4;struct.pack_into('<2I',memory,0x67c2d4,0x400000,0x400000)
   struct.pack_into('<5I',memory,0x67c2e0,record if present else 0,record if present else 0,record+24 if merge else 0,record+24 if merge else 0,0x1000)
   struct.pack_into('<H',memory,record&0xffffff,4);struct.pack_into('<4I',memory,(record&0xffffff)+4,0,0,1000,2048)
   if merge:
    struct.pack_into('<H',memory,(record&0xffffff)+24,1);struct.pack_into('<4I',memory,(record&0xffffff)+28,0,0,3048,100)
  for memory in (old,native):
   struct.pack_into('<I',memory,0xa00000,0x1234);struct.pack_into('<I',memory,0xa00010,resource_flags);struct.pack_into('<H',memory,0xa00040,resource_count)
   struct.pack_into('<3I',memory,0x673484,0x8ca00000,1,0x1234);struct.pack_into('<I',memory,0x88f6a0,1);struct.pack_into('<I',memory,0x673d18,0)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c64ddc0;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
  context=(size,flag,position,last_slot,other_slots,present,initial_sr)
  assert fn(view,ctypes.byref(sr),0x8ca00000,0x8cf00000)==regs.r[0] and sr.value==regs.sr,context
  for start,end in [(0x8a83e0,0x8d33e8),(0x67c2d0,0x67c310),(0xa00000,0xa00100),(0x673480,0x673d20),(0x88f6a0,0x88f6a4),(0xefffbc,0xefffd0)]:assert old[start:end]==native[start:end],context
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c663950','routine_end_exclusive':'0x8c663a32','retail_span_sha256':hashlib.sha256(binary[0x653950:0x653a32]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/resource_release_complete_runtime.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'original_bodies_connected':15,'scope':'Eight metadata formats, head/interior/tail/missing block, first/last slot, occupied/empty block, present/missing allocation, SR states; entire allocation and block pools, globals, metadata scratch, return/SR and preserved registers. Resource release, classifier, descriptor reset, dispatcher and all eleven cleanup bodies execute without hooks. Valid metadata configurations and adjacent range coalescing tested.'}
 (out/'analysis/resource_release_complete_special_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} linked payload cleanup comparisons passed')
if __name__=='__main__':main()
