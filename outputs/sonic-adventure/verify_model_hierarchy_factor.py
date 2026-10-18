import ctypes,itertools,json,hashlib,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;view=(ctypes.c_uint8*len(ram)).from_buffer(ram);P=ctypes.POINTER(ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_factor.dll')).sa_model_hierarchy_factor;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_uint];cases=0
 for bank,rm,sr0,mode,wrapping,pair,phase in itertools.product(range(2),range(2),[0,0x700083f3],[2,3],range(2),[(0,1),(2,7),(0x80000000,0x80000010),(0xffffffe0,0xfffffff0)],[0.0,0.25,2.5,100.0]):
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca02000;state.r[5]=0x8ca02010;state.r[8]=0x8ca01000;state.r[7]=4;state.r[14]=0x8ca00100;state.pc=(0x8c057cf6 if wrapping else 0x8c057d16) if mode==2 else (0x8c057f1a if wrapping else 0x8c057f3a);state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  struct.pack_into('<I',ram,0xa02000,pair[0]);struct.pack_into('<I',ram,0xa02010,pair[1]);struct.pack_into('<I',ram,0xa01004,0x8ca02000);struct.pack_into('<f',ram,0xa00110,phase)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  reference(ram,state,{0x8c057d46 if mode==2 else 0x8c057f6a:stop},f);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),wrapping)==0;nf.fpul=fpul.value
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f),(pair,phase,mode,wrapping);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_factor_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_factor.c').read_bytes()).hexdigest(),scope='Both layouts normal/wrapping interpolation factor verified with native FLOAT/FADD/FSUB/FDIV, no arithmetic services. All integer/FPU/SR/FPUL compared across banks/RM0/1, unsigned high keys and finite phases; nonfinite/trap/zero-duration behavior unverified. Channel value interpolation pending.'),indent=2)+'\n');print(cases,'hierarchy factor comparisons passed')
if __name__=='__main__':main()
