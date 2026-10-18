import ctypes,hashlib,itertools,json,random,struct
from fractions import Fraction
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def bits(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def value(v):return Fraction.from_float(struct.unpack('<f',struct.pack('<I',v))[0])

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_inverse.dll')).sa_model_matrix_inverse;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];cases=0;independent=0
 for bank,rm,sr0,explicit,seed in itertools.product(range(2),range(2),[0,0x700083f3],[False,True],range(64)):
  rng=random.Random(seed);values=[rng.randrange(-2,3) for _ in range(16)]
  if seed<32:
   values=[rng.choice([-2,-1,1,2]) if i//4==i%4 else rng.randrange(-2,3) if i//4<i%4 else 0 for i in range(16)]
  if seed==0:values=[int(i%5==0) for i in range(16)]
  elif seed==1:values=[0]*16
  elif seed==2:values[4:8]=values[:4]
  words=[bits(v) for v in values]
  for m in (old,native):struct.pack_into('<16I',m,0xa00000,*words);m[0xefff00:0xf00000]=bytes([0x55])*256
  f=Fpu();f.fpscr=(bank<<21)|0x40000|rm;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  f.banks[bank^1][:]=words;nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca00000 if explicit else 0;state.r[15]=0x8cf00000;state.sr=sr0;state.pc=0x8c6390b0;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r)
  reference(old,state,fpu=f)
  sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(nf.fpul);status=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),0x8c000100);nf.fpul=fpul.value
  assert status==0,(bank,rm,explicit,seed,status)
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and native[0xa00000:0xa00040]==old[0xa00000:0xa00040] and native[0xefff00:0xf00000]==old[0xefff00:0xf00000],(bank,rm,explicit,seed)
  if seed<32 and seed not in [1,2]:
   inverse=[value(v) for v in (struct.unpack_from('<16I',native,0xa00000) if explicit else nf.banks[bank^1])]
   for i in range(4):
    for j in range(4):assert sum(values[i*4+k]*inverse[k*4+j] for k in range(4))==int(i==j),(seed,i,j)
   independent+=1
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_inverse_verification.json').write_text(json.dumps(dict(cases_passed=cases,independent_exact_identity_products_passed=independent,source_sha256=hashlib.sha256((out/'source/model_matrix_inverse.c').read_bytes()).hexdigest(),scope='Complete inverse8c6390b0 through return with native determinant/scalar arithmetic, no function/instruction services. All integer/FPU/SR/FPUL/matrix/callee stack saves compared. Explicit/implicit matrices, FR banks/RM0/1, finite integer inputs including singulars and triangular matrices. Independent exact A*inverse(A)=I for power-of-two diagonal triangular cases. Nonfinite/overflow/traps unverified; rendering integration pending.'),indent=2)+'\n');print(f'{cases} full inverse comparisons and {independent} exact identity products passed')
if __name__=='__main__':main()
