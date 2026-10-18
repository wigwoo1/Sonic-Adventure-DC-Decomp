import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def bits(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def exact_determinant(values):
 total=0
 for order in itertools.permutations(range(4)):
  value=1
  for row in range(4):value*=values[row*4+order[row]]
  total+=(-value if sum(order[i]>order[j] for i in range(4) for j in range(i+1,4))%2 else value)
 return total

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_determinant.dll')).sa_model_matrix_determinant;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];cases=0;independent=0
 for bank,rm,sr0,explicit,seed in itertools.product(range(2),range(2),[0,0x700083f3],[False,True],range(128)):
  rng=random.Random(seed);values=[rng.randrange(-2,3) for _ in range(16)]
  if seed==0:values=[int(i%5==0) for i in range(16)]
  elif seed==1:values=[0]*16
  elif seed==2:values[4:8]=values[:4]
  words=[bits(v) for v in values] if seed<64 else [(rng.getrandbits(1)<<31)|(rng.randrange(110,134)<<23)|rng.getrandbits(23) for _ in range(16)]
  for m in (old,native):struct.pack_into('<16I',m,0xa00000,*words)
  f=Fpu();f.fpscr=(bank<<21)|0x40000|rm;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  f.banks[bank^1][:]=words;nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca00000 if explicit else 0;state.sr=sr0;state.pc=0x8c64f3ec;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r)
  reference(old,state,fpu=f)
  sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r)==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and native[0xa00000:0xa00040]==old[0xa00000:0xa00040],(bank,rm,explicit,seed)
  if seed<64:
   actual=struct.unpack('<f',struct.pack('<I',nf.banks[bank][0]))[0];assert actual==exact_determinant(values),(seed,actual,values);independent+=1
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_determinant_verification.json').write_text(json.dumps(dict(cases_passed=cases,independent_integer_determinants_passed=independent,source_sha256=hashlib.sha256((out/'source/model_matrix_determinant.c').read_bytes()).hexdigest(),scope='Complete scalar determinant8c64f3ec through return, explicit and implicit matrix source. Full integer/FPU/SR/RAM comparison without instruction services, both FR banks/RM0/1, bounded finite matrices. Independent exact permutation determinant for small integer matrices including identity/zero/repeated rows. Nonfinite/overflow/trap cases unverified; inverse integration pending.'),indent=2)+'\n');print(f'{cases} determinant comparisons and {independent} independent exact determinants passed')
if __name__=='__main__':main()
