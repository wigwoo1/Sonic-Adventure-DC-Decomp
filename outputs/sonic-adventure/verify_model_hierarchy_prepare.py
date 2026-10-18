import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 callback=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_prepare.dll')).sa_model_hierarchy_prepare;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,callback];cases=0
 for bank,sr0,index,matrix,dest,seed in itertools.product(range(2),[0,0x700083f3],[0,1,127,128,0xffffffff],[0,0x8ca00000],[0,0x8ca04000],range(2)):
  # Out-of-range requested indices are still copied when a destination exists;
  # index overflow wraps source address per the original32-bit shifts.
  rng=random.Random(seed);data=rng.randbytes(8192)
  source=((index<<6)+0x8c789448)&0xffffffff
  for m in (old,native):
   m[0xa00000:0xa02000]=data;m[0xa03000:0xa04000]=bytes([0x55])*4096;m[0xa04000:0xa04040]=bytes([0x5a])*64;m[0xefff80:0xf00000]=bytes([0x55])*128;struct.pack_into('<I',m,0xa00100,0x8ca00600);struct.pack_into('<I',m,0xa00104,0x8ca00200);struct.pack_into('<2I',m,0xa00200,0x8ca00700,0x8ca00800);struct.pack_into('<H',m,0xa00208,65535 if seed else 3);struct.pack_into('<2I',m,0x88f698,1 if seed==0 else 4,1);struct.pack_into('<I',m,0x88f5f8,0x8ca03000);struct.pack_into('<I',m,0x78b448,99);struct.pack_into('<I',m,0x78b44c,99);m[source&0xffffff:(source&0xffffff)+64]=data[:64]
  f=Fpu();f.fpscr=(bank<<21)|0x40000|seed;f.fpul=0x12345678
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=matrix;state.r[5]=0x8ca00100;state.r[6]=index;state.r[7]=dest;state.r[15]=0x8cf00000;state.sr=sr0;state.pc=0x8c0583a0;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r);events=[]
  def invoke(m,fp,regs,s,pr):
   events.append(([regs[i] for i in range(16)],bytes(fp),s,pr,bytes(m[0xefffd4:0xeffff8])));struct.pack_into('<I',m,0x78b448,3);regs[0]=0x12340000;regs[2]=0x8ca00900;fp.banks[(fp.fpscr>>21)&1][3]=0x40000000;return s^0x01000031
  def hook(m,regs,s):return invoke(m,f,regs,s,state.pr)
  reference(old,state,{0x8c057bc0:hook},f);expected=events[:];events.clear()
  @callback
  def service(ctx,mem,fp,s,regs,pr):s[0]=invoke(native,fp.contents,regs,s[0],pr);return 0
  sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,0x8c000100,None,service)==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and events==expected and native[0xa03000:0xa04040]==old[0xa03000:0xa04040] and native[0xefff80:0xf00000]==old[0xefff80:0xf00000] and native[0x78b448:0x78b450]==old[0x78b448:0x78b450],(bank,index,matrix,dest,seed)
  assert struct.unpack_from('<I',native,0x78b44c)[0]==min((index+1)&0xffffffff,128)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_prepare_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_prepare.c').read_bytes()).hexdigest(),scope='Hierarchy preparation8c0583a0 through return with native matrix push/pop/copy; recursive57bc0 generator is sole function service. All integer/FPU/SR/frame/stack/global/output RAM and generator inputs compared. Index0/1/127/128/ffffffff, cap128 wrap, optional initial/output matrices, FR banks/RM0/1/capacity. Independent limit assertion. Recursive generator/full rendering entry remains incomplete.'),indent=2)+'\n');print(f'{cases} hierarchy preparation wrapper comparisons passed')
if __name__=='__main__':main()

