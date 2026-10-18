"""Compare doubly linked allocation removal against original unhooked instructions."""
import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/allocation_list_remove.dll';fn=ctypes.CDLL(str(dll_path)).sa_allocation_list_remove_8c664b52
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*4;fn.restype=ctypes.c_uint32
 cases=0;rng=random.Random(0x664b52)
 for length in range(1,25):
  for shuffle in [False,True]:
   nodes=[0x8ca00100+i*32 for i in range(length)]
   if shuffle:rng.shuffle(nodes)
   for position in range(length):
    for initial_sr in [0,0x700083f3]:
     for memory in (old,native):
      memory[0xa00000:0xa00500]=b'\xa5'*0x500;struct.pack_into('<2I',memory,0xa00000,nodes[0],nodes[-1])
      for i,node in enumerate(nodes):struct.pack_into('<2I',memory,(node&0xffffff)+4,nodes[i-1] if i else 0,nodes[i+1] if i+1<length else 0)
     incoming_r0=0x12345678 if position&1 else 0
     regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[0]=incoming_r0;regs.r[4]=0x8ca00000;regs.r[5]=0x8ca00004;regs.r[6]=nodes[position];regs.r[15]=0x8cf00000;regs.pc=0x8c664b52;regs.pr=0x8c000100;regs.sr=initial_sr
     saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
     assert fn(view,ctypes.byref(sr),0x8ca00000,0x8ca00004,nodes[position],incoming_r0)==regs.r[0] and sr.value==regs.sr
     assert old[0xa00000:0xa00500]==native[0xa00000:0xa00500]
     assert list(regs.r[8:])==saved and regs.pr==0x8c000100
     cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c664b52','routine_end_exclusive':'0x8c664baa','retail_span_sha256':hashlib.sha256(binary[0x654b52:0x654baa]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/allocation_list_remove.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Every position in ordered and shuffled valid chains of lengths 1..24; head/tail/link updates, unchanged removed-node fields, incoming r0, full SR and preserved registers. Whole neighboring RAM compared; no service hooks.'}
 (out/'analysis/allocation_list_remove_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} allocation list removal comparisons passed')
if __name__=='__main__':main()
