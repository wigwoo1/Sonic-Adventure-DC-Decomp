import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);rng=random.Random(0x638f94)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_stack_pop.dll')).sa_model_matrix_stack_pop_8c639b98;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];cases=0
 for bank,rm,sr0,depth,count in itertools.product(range(2),range(4),[0,0x700083f3],[0,1,2,8],[0,1,2,8,16,0xffffffff]):
  matrix_data=rng.randbytes(8192)
  for mem in [old,native]:
   mem[0xa02000:0xa04000]=matrix_data
   import struct
   struct.pack_into("<I",mem,0x88f69c,depth);struct.pack_into("<I",mem,0x88f5f8,0x8ca03000)
  fpu=Fpu();fpu.fpscr=0x7f07c|(bank<<21)|rm
  for k in range(2):fpu.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  fpu.fpul=0xabcdef12;expected=Fpu.from_buffer_copy(fpu);state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=count;state.pc=0x8c639b98;state.pr=0x8c000100;state.sr=sr0;initial=(ctypes.c_uint32*16)(*state.r);sr=ctypes.c_uint32(sr0);reference(old,state,{},expected);result=fn(view,ctypes.byref(fpu),ctypes.byref(sr),initial)
  assert result==0 and old[0x88f5f8:0x88f6a0]==native[0x88f5f8:0x88f6a0] and bytes(fpu)==bytes(expected) and list(state.r)==list(initial) and state.sr==sr.value;cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_stack_pop_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_matrix_stack_pop.c').read_bytes()).hexdigest(),scope='Original matrix stack pop through return without hooks. Both FR banks, RM bit patterns, random raw registers and SR; RAM, all integer registers and FPU state compared. PR0/SZ0 only; hardware unverified.'),indent=2)+'\n');print(f'{cases} matrix stack pop comparisons passed')
if __name__=='__main__':main()
