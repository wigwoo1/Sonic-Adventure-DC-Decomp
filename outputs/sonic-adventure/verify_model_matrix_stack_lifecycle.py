import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 functions=[]
 for name,addr in [('push','8c639c70'),('pop','8c639b98')]:
  f=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/model_matrix_stack_{name}.dll')),f'sa_model_matrix_stack_{name}_{addr}');f.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];functions.append(f)
 sequences=steps=0
 for bank,rm,limit,seed in itertools.product(range(2),range(4),[2,4,8],range(16)):
  rng=random.Random(0x639c70+seed);data=rng.randbytes(8192)
  for m in [old,native]:
   m[0xa02000:0xa04000]=data;m[0xa01000:0xa01040]=data[:64]
   struct.pack_into('<I',m,0x88f698,limit);struct.pack_into('<I',m,0x88f69c,1);struct.pack_into('<I',m,0x88f5f8,0x8ca03000)
  fpu=Fpu();fpu.fpscr=0x7f07c|(bank<<21)|rm;fpu.fpul=0x12345678
  for k in range(2):fpu.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  expected=Fpu.from_buffer_copy(fpu);state=State();state.r[:]=[0x12340000+i for i in range(16)];state.pr=0x8c000100;state.sr=0x700083f3;r=(ctypes.c_uint32*16)(*state.r);sr=ctypes.c_uint32(state.sr);snapshots=[]
  for step in range(32+limit):
   depth=struct.unpack_from('<I',native,0x88f69c)[0]
   push=rng.choice([True,True,False]) if step<32 else False
   if not push and depth==1:push=True if step<32 else False
   if not push and depth==1:continue
   argument=rng.choice([0,0x8ca01000]) if push else 1
   saved=list(fpu.banks[bank^1]);state.r[4]=argument;r[4]=argument;state.pc=0x8c639c70 if push else 0x8c639b98
   reference(old,state,{},expected);result=functions[0 if push else 1](view,ctypes.byref(fpu),ctypes.byref(sr),r)
   case=(bank,rm,limit,seed,step,push)
   assert result==0 and list(state.r)==list(r) and state.sr==sr.value and bytes(expected)==bytes(fpu),case
   assert old[0xa02000:0xa04000]==native[0xa02000:0xa04000] and old[0x88f5f8:0x88f6a0]==native[0x88f5f8:0x88f6a0],case
   if push and r[0]:snapshots.append(saved)
   if not push:assert list(fpu.banks[bank^1])==snapshots.pop(),case
   steps+=1
  assert not snapshots and struct.unpack_from('<I',native,0x88f69c)[0]==1 and struct.unpack_from('<I',native,0x88f5f8)[0]==0x8ca03000
  sequences+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(sequences_passed=sequences,calls_passed=steps,dependency_sha256={name:hashlib.sha256((out/f'source/{name}.c').read_bytes()).hexdigest() for name in ['model_matrix_stack_push','model_matrix_stack_pop']},scope='Random mixed push/pop sequences and final drains, depths/capacity failures/null and replacement matrices. Every call compared original instructions without hooks: complete integer/FPU state, SR and matrix/global RAM. Independent saved-matrix restoration, depth and pointer assertions. PR0/SZ0; cache timing and hardware unverified.')
 (out/'analysis/model_matrix_stack_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{sequences} matrix lifecycles passed across {steps} calls')
if __name__=='__main__':main()
