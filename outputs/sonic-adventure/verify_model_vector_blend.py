import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_blend.dll')).sa_model_vector_blend;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.c_int];cases=0
 for midpoint,bank,rm,sr0,seed in itertools.product([False,True],range(2),range(2),[0,0x700083f3],range(64)):
  rng=random.Random(seed);words=[rng.getrandbits(32) for _ in range(64)]
  choices=[0,0x80000000,1,0x80000001,0x3f800000,0xbf800000,0x3f000000,0x3eaaaaab,0x4b000001,0xcb000001]
  for off in [124,128,132,136,140,144]:words[off//4]=rng.choice(choices) if seed<32 else ((rng.getrandbits(1)<<31)|(rng.randrange(110,145)<<23)|rng.getrandbits(23))
  for m in (old,native):struct.pack_into('<64I',m,0xf00000,*words)
  f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  f.banks[bank][15]=0x3f000000 if seed%2==0 else 0x3eaaaaab;nf=Fpu.from_buffer_copy(f)
  state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[15]=0x8cf00000;state.sr=sr0;state.pc=0x8c07a9ee if midpoint else 0x8c07aa94;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  reference(old,state,{(0x8c07aa2e if midpoint else 0x8c07aad4):stop},f)
  assert fn(view,ctypes.byref(nf),r,int(midpoint))==0
  assert list(r)==list(state.r) and bytes(nf)==bytes(f) and native[0xf00000:0xf00100]==old[0xf00000:0xf00100],(midpoint,bank,rm,seed)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_blend_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_blend.c').read_bytes()).hexdigest(),scope='Both type1 scalar blending blocks against original FADD/FMUL instructions: all integer/FPU registers and fixture RAM, FR banks/RM0/1, finite values, signed zero and DN1 subnormals. No arithmetic instruction injection. Surrounding type1 branch, normalization/vector transforms remain incomplete.'),indent=2)+'\n');print(f'{cases} vector blending arithmetic comparisons passed')
if __name__=='__main__':main()
