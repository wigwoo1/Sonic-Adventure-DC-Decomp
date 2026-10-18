import ctypes,hashlib,itertools,json,struct,random
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);rng=random.Random(0xadf6)
 from verify_fpu_cleanup_transfer import Fpu
 P=ctypes.POINTER(ctypes.c_uint32);CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),P);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_call.dll')).sa_model_vector_call;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_uint32,ctypes.c_void_p,CB];cases=0
 @CB
 def unexpected(ctx,word,fp,fpul):raise AssertionError('Lifecycle should not execute vector arithmetic')
 for nodes,count,kind,sr0 in itertools.product([0,1,3],[0,1,8],[0,1,4,9,10,255],[0,0x700083f3]):
  data=rng.randbytes(0x6000)
  for m in [old,native]:
   m[0xa00000:0xa06000]=data;m[0xa00000:0xa00100]=bytes(256);m[0x880000:0x884000]=bytes([0xa5])*0x4000
   struct.pack_into('<2I',m,0xa00200,0x8ca00300,0x8ca00400);struct.pack_into('<2I',m,0xa00304,0x8ca00500,0);struct.pack_into('<I',m,0xa00404,0x8ca00600)
   struct.pack_into('<2I',m,0xa00500,0x8ca02000,0x8ca02800);struct.pack_into('<2I',m,0xa00600,0x8ca03000,0x8ca03800)
   for i in range(8):
    block=0x8c880000+i*512;struct.pack_into('<4I',m,0x880000+i*512,0,0,block+512,block-512 if i else 0)
    if i<nodes:struct.pack_into('<3I',m,0xa00000+i*24,0x8ca01000,0x8ca00300,0x8ca00400);m[0xa0000c+i*24]=count;m[0xa0000d+i*24]=kind;struct.pack_into('<2I',m,0xa00010+i*24,0,0x8ca00800)
   struct.pack_into('<4I',m,0x881000,0,0,0,0x8c880e00);struct.pack_into('<4I',m,0x880000,0x8c881000,0,0x8c881000,0);struct.pack_into('<I',m,0x79f03c,0x8c880000);struct.pack_into('<2I',m,0xa0102c,0x8ca00300,0);struct.pack_into('<2I',m,0xa0032c,0,0x8ca00400);struct.pack_into('<2I',m,0xa0042c,0,0)
   for i in range(8):struct.pack_into('<2H',m,0xa00800+i*4,i,7-i)
  for m in [old,native]:struct.pack_into('<H',m,0xa00902,0);struct.pack_into('<I',m,0xa00924,0x8ca00000)
  f=Fpu();f.fpscr=0x40000;f.fpul=0xabcdef12
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f)
  state=State();state.r[14]=0x8ca00000;state.r[11]=0;state.r[8]=0x8ca00900;state.r[15]=0x8cf000c0;state.r[6]=0x8ca007e0;state.pc=0x8c07a55c;state.pr=0x8c000100;state.sr=sr0;state.r[0]=0x12345678
  def stop(memory,r,s):state.pr=0x8c000100;return s
  regs=(ctypes.c_uint32*16)(*state.r);reference(old,state,{},f);sr=ctypes.c_uint32(sr0);last=ctypes.c_uint32();fpul=ctypes.c_uint32(nf.fpul);assert fn(view,ctypes.byref(nf),ctypes.byref(fpul),ctypes.byref(sr),regs,0x8c000100,None,unexpected)==0;r0=regs[0];nf.fpul=fpul.value;case=(nodes,count,kind,sr0)
  assert r0==state.r[0] and sr.value==state.sr and list(regs)[8:16]==list(state.r)[8:16] and bytes(nf)==bytes(f),case
  assert old[0xa00000:0xa06000]==native[0xa00000:0xa06000] and old[0x880000:0x884000]==native[0x880000:0x884000] and old[0x79f03c:0x79f040]==native[0x79f03c:0x79f040],case
  saved_vectors=bytes(native[0xa02000:0xa04000])
  for mem in [old,native]:mem[0xa02000:0xa04000]=bytes([0x5a])*8192
  for m in [old,native]:struct.pack_into('<H',m,0xa00902,2)
  state.pc=0x8c07a55c;state.pr=0x8c000100;state.r[6]=0x8ca007e0;regs[6]=0x8ca007e0
  reference(old,state,{},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(fpul),ctypes.byref(sr),regs,0x8c000100,None,unexpected)==0;r0=regs[0];nf.fpul=fpul.value
  assert r0==state.r[0] and sr.value==state.sr and list(regs)[8:16]==list(state.r)[8:16] and bytes(nf)==bytes(f),case
  assert old[0xa00000:0xa06000]==native[0xa00000:0xa06000] and old[0x880000:0x884000]==native[0x880000:0x884000] and old[0x79f03c:0x79f040]==native[0x79f03c:0x79f040],case
  for i in range(nodes):assert struct.unpack_from('<I',native,0xa00010+i*24)[0]==0,case
  if nodes and count and kind not in [0,4,9]:
   for i in range(count):
    for base,mapped in [(0xa02000,i),(0xa02800,i),(0xa03000,7-i),(0xa03800,7-i)]:
     start=base+mapped*12;offset=start-0xa02000
     assert native[start:start+12]==saved_vectors[offset:offset+12],case
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_call_lifecycle_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_call.c').read_bytes()).hexdigest(),scope='Consecutive initialization, mutation of live vector arrays, restoration and native buffer release. Connected initialization branch with native allocation, mapped vector save and recursive descriptor lookup. Empty/single/multiple record arrays, record types0/1/4/9/10/255, zero/nonzero vector counts. RAM/r0/SR/r14 at epilogue compared; only epilogue stop hooked. Complete helper entry/return integration now compared for initialization and cleanup, including FPU and callee-preserved registers; volatile scratch excluded. Vector numerical fidelity remains unfinished.'),indent=2)+'\n');print(f'{cases} vector save/restore/release lifecycles passed')
if __name__=='__main__':main()

