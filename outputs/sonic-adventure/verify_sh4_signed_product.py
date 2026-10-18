"""Qualify the isolated future DMULS kernel with unbounded integer products."""
import ctypes,hashlib,itertools,json,random
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
DLL=ROOT/'work/sonic-adventure/build/sh4_signed_product.dll'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 library=ctypes.CDLL(str(DLL));fn=library.sa_sh4_dmuls
 pointer=ctypes.POINTER(ctypes.c_uint32);fn.argtypes=[ctypes.c_uint32,ctypes.c_uint32,pointer,pointer];fn.restype=None
 edges=(0,1,2,255,256,65535,65536,0x7ffffffe,0x7fffffff,0x80000000,0x80000001,0xfffffffe,0xffffffff)
 pairs=list(itertools.product(edges,repeat=2));rng=random.Random(0x374ff4)
 pairs.extend((rng.getrandbits(32),rng.getrandbits(32)) for _ in range(2048))
 high_nonzero=0
 for a,b in pairs:
  # Python arbitrary-precision multiplication is the independent oracle; no
  # fixed-width overflow or signed C-cast assumptions are shared with the DLL.
  x=a if a<2**31 else a-2**32;y=b if b<2**31 else b-2**32
  wanted=(x*y)%(2**64)
  lo,hi=ctypes.c_uint32(0xdeadbeef),ctypes.c_uint32(0xcafebabe)
  fn(a,b,ctypes.byref(lo),ctypes.byref(hi))
  assert lo.value+(hi.value<<32)==wanted,(hex(a),hex(b),hex(wanted),hex(lo.value),hex(hi.value))
  high_nonzero+=hi.value!=0
 files=[Path(__file__),PROJECT/'source/sh4_signed_product.c',PROJECT/'source/sh4_signed_product.h',PROJECT/'source/fpu_cleanup_transfer.h',PROJECT/'build_sh4_signed_product.ps1',DLL]
 report=dict(cases_passed=len(pairs),boundary_pairs=len(edges)**2,deterministic_random_pairs=2048,nonzero_high_word_cases=high_nonzero,dll_sha256=digest(DLL),source_sha256=digest(PROJECT/'source/sh4_signed_product.c'),newly_recovered_game_entries=0,connected_to_game=False,scope='Isolated signed 32x32 to complete low/high 64-bit product kernel versus independent Python arbitrary-precision integers. Needed for the next clippers; no game body, native caller connection, bank-state, hardware exception or pixel credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/sh4_signed_product_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Signed wide-product kernel:',len(pairs),'independent comparisons pass; not yet connected to game helpers.')
if __name__=='__main__':main()
