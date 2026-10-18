import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_inverse_singular.dll')).sa_model_matrix_inverse_singular;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu)]+[ctypes.POINTER(ctypes.c_uint32)]*3;cases=0
 for bank,sr0,determinant,dst,seed in itertools.product(range(2),[0,0x700083f3],[0,0x80000000,0x3f800000,0xbf800000],[0,0x8ca00000],range(16)):
  rng=random.Random(seed)
  for m in (old,native):m[0xa00000:0xa00080]=bytes([seed])*128
  f=Fpu();f.fpscr=(bank<<21)|0x7f07c|(seed%2);f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  f.banks[bank][0]=determinant;nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[14]=dst;state.sr=sr0;state.pc=0x8c6390ba;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r);dest=[]
  def hook(addr):
   def stop(m,regs,s):dest.append(addr);state.pr=0x8c000100;return s
   return stop
  reference(old,state,{a:hook(a) for a in [0x8c639112,0x8c6398a8]},f)
  sr=ctypes.c_uint32(sr0);nextpc=ctypes.c_uint32();assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(nextpc))==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and nextpc.value==dest[0] and native[0xa00000:0xa00080]==old[0xa00000:0xa00080],(bank,determinant,dst,seed)
  if determinant&0x7fffffff==0:
   if dst:assert native[0xa00000:0xa00040]==bytes.fromhex('9ec97f7f')*16
   else:assert list(nf.banks[bank^1])==[0x7f7fc99e]*16
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_inverse_singular_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_matrix_inverse_singular.c').read_bytes()).hexdigest(),scope='Post-determinant singular branch to epilogue and nonzero routing. Full integer/FPU/SR/RAM/branch and independent sentinel assertions. Signed-zero/normal determinants, explicit/null destination, FR banks. Determinant calculation, nonzero inversion and nonfinite/subnormal comparisons remain incomplete.'),indent=2)+'\n');print(f'{cases} singular inverse path comparisons passed')
if __name__=='__main__':main()
