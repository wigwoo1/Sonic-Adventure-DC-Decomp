import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 callback=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_compose_implicit.dll')).sa_model_matrix_compose_implicit;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_void_p,callback];cases=0
 for bank,rm,sr0,alias,seed in itertools.product(range(2),range(2),[0,0x700083f3],[False,True],range(32)):
  rng=random.Random(seed);data=bytes(rng.randrange(256) for _ in range(256))
  for m in (old,native):m[0xa00000:0xa00100]=data
  f=Fpu();f.fpscr=(bank<<21)|rm|0x40000;f.fpul=0x12345678
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0;state.r[5]=0x8ca00000;state.r[15]=0x8ca00040 if alias else 0x8ca000e0;state.sr=sr0;state.pc=0x8c63997c;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r);events=[]
  def primitive(word,fp):
   events.append((word,bytes(fp)));active=(fp.fpscr>>21)&1;start=(word>>8)&12
   for i in range(4):fp.banks[active][start+i]=0x3f800000+len(events)*100+i
   fp.fpscr^=0x1000
  reference(old,state,fpu=f,vector_instruction=primitive);expected=events[:];events.clear()
  @callback
  def service(ctx,word,fp):primitive(word,fp.contents);return 0
  sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,None,service)==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and events==expected and native[0xa00000:0xa00100]==old[0xa00000:0xa00100],(bank,rm,sr0,alias,seed)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_compose_implicit_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_matrix_compose_implicit.c').read_bytes()).hexdigest(),scope='Implicit path at8c63997c through return: full integer/FPU/SR/fixture RAM and vector instruction inputs, FR banks, rounding modes and source/stack aliasing. FTRV outputs injected; explicit-matrix path and mathematical fidelity incomplete.'),indent=2)+'\n');print(f'{cases} implicit matrix composition dataflow comparisons passed')
if __name__=='__main__':main()
