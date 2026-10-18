"""Compare block unlink against original instructions for all positions in valid chains."""
import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/block_unlink.dll';fn=ctypes.CDLL(str(dll_path)).sa_block_unlink_8c665478
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
 cases=0;rng=random.Random(0x665478)
 for length in range(1,25):
  for shuffled in [False,True]:
   nodes=[0x8ca00100+i*32 for i in range(length)]
   if shuffled:rng.shuffle(nodes)
   for position in range(length):
    for initial_sr in [0,0x700083f3]:
     for memory in (old,native):
      memory[0xa00000:0xa00600]=b'\xa5'*0x600
      struct.pack_into('<I',memory,0xa00000,nodes[0])
      for i,node in enumerate(nodes):struct.pack_into('<I',memory,(node&0xffffff)+4,nodes[i+1] if i+1<length else 0)
     regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=0x8ca00000;regs.r[5]=nodes[position];regs.r[15]=0x8cf00000;regs.pc=0x8c665478;regs.pr=0x8c000100;regs.sr=initial_sr
     saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs)
     assert fn(view,ctypes.byref(sr),0x8ca00000,nodes[position])==regs.r[0] and sr.value==regs.sr
     assert old[0xa00000:0xa00600]==native[0xa00000:0xa00600]
     assert list(regs.r[8:])==saved and regs.pr==0x8c000100
     cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c665478','routine_end_exclusive':'0x8c6654a8','retail_span_sha256':hashlib.sha256(binary[0x655478:0x6554a8]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/block_unlink.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'All removal positions in valid chains of lengths 1..24, ordered and shuffled RAM nodes, two initial SR states, predecessor return, whole chain and neighbor RAM, preserved registers. No hooks. Original node-membership precondition retained; invalid membership is not treated as success.'}
 (out/'analysis/block_unlink_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} block unlink comparisons passed')
if __name__=='__main__':main()
