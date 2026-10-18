import ctypes,itertools,json,hashlib,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;P=ctypes.POINTER(ctypes.c_uint32);CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_rotate.dll')).sa_model_matrix_rotate;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_void_p,CB];cases=0
 for source,bank,rm,sr0,x,y,z in itertools.product([0,0x8ca02000],range(2),range(2),[0,0x700083f3],[0,1,0xffff,0x80000000],[0,1,0xffff,0x80000000],[0,1,0xffff,0x80000000]):
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);native_ram=bytearray(ram);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=source;state.r[15]=0x8cf00000;state.r[5:8]=[x,y,z];state.pc=0x8c639cf4;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r);ev=[];av=[]
  def apply(word,fp,fpul,log):
   log.append((word,fpul,bytes(fp)));active=(fp.fpscr>>21)&1
   if word==0xf0fd:fp.banks[active][0:2]=[0x3f000001,0x3f800002]
   else:
    start=(word>>8)&12;fp.banks[active][start:start+4]=[0x3f800000+len(log)*4+i for i in range(4)]
  def vector(word,fp):apply(word,fp,fp.fpul,ev)
  @CB
  def native(ctx,word,fp,fpul):fp.contents.fpul=fpul;apply(word,fp.contents,fpul,av);return 0
  for m in (ram,native_ram):m[0xa02000:0xa02040]=bytes(range(64));m[0xeffff0:0xf00000]=bytes([0x55])*16
  reference(ram,state,{},f,vector_instruction=vector);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678);view=(ctypes.c_uint8*len(native_ram)).from_buffer(native_ram);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),None,native)==0;nf.fpul=fpul.value
  assert ram[0xa02000:0xa02040]==native_ram[0xa02000:0xa02040] and ram[0xeffff0:0xf00000]==native_ram[0xeffff0:0xf00000]
  assert ev==av and bytes(nf)==bytes(f) and list(r)==list(state.r) and sr.value==state.sr,(x,y,z);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_rotate_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_matrix_rotate.c').read_bytes()).hexdigest(),scope='Complete implicit/explicit rotation dataflow through return, including matrix and stack RAM. FSCA/FTRV outputs injected; all instruction input snapshots and integer/FPU/SR/FPUL compared, zero/nonzero/large angles, both banks/RM0/1. Vector numerical fidelity and hierarchy integration pending.'),indent=2)+'\n');print(cases,'complete rotation dataflow comparisons passed')
if __name__=='__main__':main()
