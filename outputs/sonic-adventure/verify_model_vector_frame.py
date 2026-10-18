import ctypes,itertools,json,hashlib,struct,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);P=ctypes.POINTER(ctypes.c_uint32);dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_frame.dll'));enter=dll.sa_model_vector_frame_enter;enter.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,ctypes.c_uint32,P];leave=dll.sa_model_vector_frame_leave;leave.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P];cases=0
 for bank,rm,sr0,mode,seed in itertools.product(range(2),range(4),[0,0x700083f3],[0,1,2,3,0x7fff,0x8000,0xffff],range(8)):
  rng=random.Random(seed);f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[6]=0x8ca01000;state.r[15]=0x8cf00000;state.pc=0x8c07a55c;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  for m in (old,native):m[0xefff00:0xf00000]=bytes([0x55])*256;struct.pack_into('<H',m,0xa01122,mode);struct.pack_into('<I',m,0xa01144,0x8ca02000)
  targets=[]
  def stop(pc):
   def apply(m,regs,s):targets.append(pc);state.pr=0x8c000100;return s
   return apply
  reference(old,state,{pc:stop(pc) for pc in [0x8c07a5a0,0x8c07a740,0x8c07adf6]},f);sr=ctypes.c_uint32(sr0);nextpc=ctypes.c_uint32();assert enter(view,ctypes.byref(nf),ctypes.byref(sr),r,0x8c000100,ctypes.byref(nextpc))==0
  assert list(r)==list(state.r) and bytes(nf)==bytes(f) and sr.value==state.sr and nextpc.value==targets[0] and old[0xefff00:0xf00000]==native[0xefff00:0xf00000]
  state.pc=0x8c07aeda;reference(old,state,{},f);assert leave(view,ctypes.byref(nf),r)==0;assert list(r)==list(state.r) and bytes(nf)==bytes(f) and sr.value==state.sr;cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_frame_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_frame.c').read_bytes()).hexdigest(),scope='Original helper prologue/state dispatch and epilogue. Full integer/FPU/SR/stack RAM compared, raw FR banks/RM patterns and signed mode boundaries. Helper bodies/full ABI integration pending.'),indent=2)+'\n');print(cases,'model vector frame comparisons passed')
if __name__=='__main__':main()
