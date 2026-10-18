import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_render_prepare.dll')).sa_model_render_prepare;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];cases=0
 for bank,sr0,kind,index,seed in itertools.product(range(2),[0,0x700083f3],[0,2,0xffff,0x8000],[0,7,65535],range(8)):
  rng=random.Random(seed);cfg=0x8ca00000;frame=0x8cf00000;entry=0x8ca01000+index*16
  for m in (old,native):
   m[0xf00000:0xf00100]=bytes([seed])*256;struct.pack_into('<H',m,0xa00000,kind);struct.pack_into('<H',m,0xa00004,index);struct.pack_into('<I',m,0xa00010,0x3f000000+seed);struct.pack_into('<I',m,0xa00020,0x8ca01000);struct.pack_into('<I',m,0xa00028,0x8ca08000);struct.pack_into('<I',m,entry&0xffffff,0x8ca06000);m[(entry&0xffffff)+4]=seed*31
  f=Fpu();f.fpscr=(bank<<21)|(seed%2)|0x40000;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[8]=cfg;state.r[15]=frame;state.sr=sr0;state.pc=0x8c07a740;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  reference(old,state,{0x8c07a782:stop},f)
  sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r)==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and native[0xf00000:0xf00100]==old[0xf00000:0xf00100],(bank,kind,index,seed)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_render_prepare_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_render_prepare.c').read_bytes()).hexdigest(),scope='Rendering entry8c07a740..8c07a782 through hierarchy call preparation: full integer/FPU/SR/frame RAM. Ordinary/special configuration, unsigned indices0/7/65535, raw byte modes and FR banks. Hierarchy function and full rendering entry integration remain incomplete.'),indent=2)+'\n');print(f'{cases} rendering entry preparation comparisons passed')
if __name__=='__main__':main()
