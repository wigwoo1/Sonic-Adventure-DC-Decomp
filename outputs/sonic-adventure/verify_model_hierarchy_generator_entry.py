import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_generator_entry.dll')).sa_model_hierarchy_generator_entry;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32)];cases=0
 for bank,sr0,count,limit,mask,seed in itertools.product(range(2),[0,0x700083f3],[0,1,3,0xffffffff],[0,1,4],[0,3,4,0x80000000],range(2)):
  rng=random.Random(seed)
  for m in (old,native):
   m[0xefff80:0xf00000]=bytes([0x55])*128;m[0xa03000:0xa03100]=bytes([0x5a])*256;struct.pack_into('<I',m,0xa00108,mask);struct.pack_into('<2I',m,0x78b448,count,limit);struct.pack_into('<2I',m,0x88f698,1 if seed==0 else 4,1);struct.pack_into('<I',m,0x88f5f8,0x8ca03000)
  f=Fpu();f.fpscr=(bank<<21)|0x40000|seed;f.fpul=0x12345678
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca04000;state.r[5]=0x8ca00100;state.r[15]=0x8cf00000;state.sr=sr0;state.pc=0x8c057bc0;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r);dest=[]
  def stop(m,regs,s):dest.append(0x8c057c14);state.pr=0x8c000100;return s
  reference(old,state,{0x8c057c14:stop},f)
  sr=ctypes.c_uint32(sr0);nextpc=ctypes.c_uint32();assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,0x8c000100,ctypes.byref(nextpc))==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and nextpc.value==(dest[0] if dest else 0) and native[0xefff80:0xf00000]==old[0xefff80:0xf00000] and native[0xa03000:0xa03100]==old[0xa03000:0xa03100] and native[0x78b448:0x78b450]==old[0x78b448:0x78b450] and native[0x88f5f8:0x88f6a0]==old[0x88f5f8:0x88f6a0],(count,limit,mask,seed)
  assert struct.unpack_from('<I',native,0x78b448)[0]==min((count+1)&0xffffffff,limit)
  if dest:assert r[9]==min(mask.bit_length(),5)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_generator_entry_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_generator_entry.c').read_bytes()).hexdigest(),scope='Generator entry through native matrix push and count-limit rejection through return. Full integer/FPU/SR/frame/stack/global RAM and independent count/mask assertions. Unsigned counter overflow, caps0/1/4, masks0/3/4/high-bit, FR banks/RM0/1/capacity. Animation channels and recursive traversal remain incomplete.'),indent=2)+'\n');print(f'{cases} hierarchy generator entry comparisons passed')
if __name__=='__main__':main()
