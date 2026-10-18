import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 P=ctypes.POINTER(ctypes.c_uint32);callback=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,ctypes.c_uint32)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_generator_tail.dll')).sa_model_hierarchy_generator_tail;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,ctypes.c_void_p,callback];cases=0
 for bank,rm,sr0,children,depth in itertools.product(range(2),range(2),[0,0x700083f3],range(4),[1,2]):
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[13]=0x8ca04000;state.r[14]=0x8ca00100;state.r[15]=0x8cefffc0;state.pc=0x8c0580a0;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  chain=[0x8ca05000+i*64 for i in range(children)];returns=dict(zip(chain,chain[1:]+[0]));saved=[rng.getrandbits(32) for _ in range(9)]
  for m in (old,native):
   m[0xefff80:0xf00000]=bytes([0x55])*128;struct.pack_into('<I',m,0xefffd8,0x8c000100);struct.pack_into('<9I',m,0xefffdc,*saved);struct.pack_into('<I',m,0xa00104,0x8ca02000);struct.pack_into('<2I',m,0xa0402c,chain[0] if chain else 0,0x8ca06000);struct.pack_into('<I',m,0x88f69c,depth);struct.pack_into('<I',m,0x88f5f8,0x8ca03040)
   m[0xa02000:0xa02040]=bytes([0x5a])*64;struct.pack_into('<16I',m,0xa03000,*[0x3f800000+i for i in range(16)])
  expected=[];actual=[]
  def hook(m,regs,s):
   expected.append(([regs[i] for i in range(16)],s,bytes(f),state.pr));regs[0]=returns[regs[4]];return s
  reference(old,state,{0x8c057bc0:hook},f)
  @callback
  def child(ctx,ram,fp,srp,regs,pr):
   actual.append(([regs[i] for i in range(16)],srp[0],bytes(fp.contents),pr));regs[0]=returns[regs[4]];return 0
  sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,None,child)==0
  assert actual==expected and len(actual)==children and list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f),cases
  for lo,hi in [(0xefff80,0xf00000),(0xa00100,0xa00110),(0xa02000,0xa02040),(0x88f5f8,0x88f6a0)]:assert old[lo:hi]==native[lo:hi],cases
  assert struct.unpack_from('<I',native,0xa00104)[0]==0x8ca02040 and r[0]==0x8ca06000 and r[15]==0x8cf00000
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_generator_tail_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_generator_tail.c').read_bytes()).hexdigest(),scope='Original traversal tail through return with native matrix save/pop. Recursive child generator service; all integer/FPU/SR/scratch/global/output RAM and child input ABI compared, zero to three child visits and successful/failed pop. Channel transforms and recursive generator implementation pending.'),indent=2)+'\n');print(f'{cases} hierarchy generator tail comparisons passed')
if __name__=='__main__':main()
