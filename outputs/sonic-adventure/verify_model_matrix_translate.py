import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_matrix_translate.dll')).sa_model_matrix_translate;P=ctypes.POINTER(ctypes.c_uint32);CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu));fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,ctypes.c_void_p,CB];cases=0
 for bank,rm,sr0,source,seed in itertools.product(range(2),range(2),[0,0x700083f3],[0,0x8ca02000,0x8ca01000,0x8ca00ff8],range(32)):
  rng=random.Random(seed);bits=lambda:struct.unpack('<I',struct.pack('<f',rng.randrange(-16,17)/4))[0];f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0xabcdef12
  for k in range(2):f.banks[k][:]=[bits() for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=source;state.r[5]=0x8ca01000;state.pc=0x8c63a804;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r);data=struct.pack('<16I',*[bits() for _ in range(16)]);scale=struct.pack('<3I',*[bits() for _ in range(3)])
  for m in (old,native):m[0xa00f80:0xa02100]=bytes([0x55])*0x1180;m[0xa02000:0xa02040]=data;m[0xa01000:0xa0100c]=scale
  expected=[];actual=[]
  def apply(word,fp,log):
   log.append((word,bytes(fp)));active=(fp.fpscr>>21)&1
   if word==0xf5fd:fp.banks[active][4:8]=[0x3f800001,0x40000002,0x40400003,0x40800004]
   else:fp.banks[active][3]=0x3f000000+len(log)
  def vector(word,fp):apply(word,fp,expected)
  @CB
  def native_vector(ctx,word,fp):apply(word,fp.contents,actual);return 0
  reference(old,state,{},f,vector_instruction=vector);sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,None,native_vector)==0
  assert actual==expected
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and native[0xa00f80:0xa02100]==old[0xa00f80:0xa02100],(bank,rm,source,seed);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_matrix_translate_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_matrix_translate.c').read_bytes()).hexdigest(),scope='Complete translation dataflow through return. FIPR/FTRV results injected; every instruction input snapshot and all integer/FPU/SR/RAM compared, implicit XF and explicit/overlapping matrices, FR banks/RM0/1. Vector numerical fidelity remains incomplete.'),indent=2)+'\n');print(f'{cases} matrix translation dataflow comparisons passed')
if __name__=='__main__':main()
