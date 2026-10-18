import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 callback=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_normalize.dll')).sa_model_vector_normalize;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.c_void_p,callback];cases=0
 for bank,rm,sr0,seed in itertools.product(range(2),range(2),[0,0x700083f3],range(64)):
  rng=random.Random(seed);words=[(rng.getrandbits(1)<<31)|(rng.randrange(110,145)<<23)|rng.getrandbits(23) for _ in range(3)]
  if seed<8:words=[0x80000000 if seed&1 else 0,1 if seed&2 else 0x3f800000,0x80000001 if seed&4 else 0xbf800000]
  for m in (old,native):struct.pack_into('<3I',m,0xa00000,*words)
  f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca00000;state.sr=sr0;state.pc=0x8c63a94c;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r);events=[]
  def primitive(word,fp):
   events.append((word,bytes(fp)));active=(fp.fpscr>>21)&1
   assert word in [0xf0ed,0xf37d];fp.banks[active][3]=0x40000001 if word==0xf0ed else 0x3eaaaaab;fp.fpscr^=0x1000
  reference(old,state,fpu=f,vector_instruction=primitive);expected=events[:];events.clear()
  @callback
  def service(ctx,word,fp):primitive(word,fp.contents);return 0
  assert fn(view,ctypes.byref(nf),r,None,service)==0
  assert list(r)==list(state.r) and bytes(nf)==bytes(f) and events==expected and native[0xa00000:0xa0000c]==old[0xa00000:0xa0000c],(bank,rm,seed)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_normalize_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_normalize.c').read_bytes()).hexdigest(),scope='Normalization dataflow8c63a94c through return, full integer/FPU/fixture RAM, FR banks/RM0/1, finite values/signed zero/DN1 subnormals. Four scalar FMULs execute native arithmetic. FIPR and FSRRA outputs injected; numerical vector fidelity incomplete.'),indent=2)+'\n');print(f'{cases} normalization dataflow comparisons passed')
if __name__=='__main__':main()
