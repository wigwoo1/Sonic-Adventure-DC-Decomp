import ctypes,hashlib,itertools,json,struct,random
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);rng=random.Random(0xadf6)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_initialize.dll')).sa_model_vector_initialize;fn.restype=ctypes.c_uint32;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*3+[ctypes.POINTER(ctypes.c_uint32)];cleanup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_cleanup.dll')).sa_model_vector_cleanup;cleanup.restype=ctypes.c_uint32;cleanup.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*2+[ctypes.POINTER(ctypes.c_uint32)];cases=0
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
  state=State();state.r[14]=0x8ca00000;state.r[11]=0;state.r[8]=0x8ca00900;state.r[15]=0x8cf00000;state.pc=0x8c07a5a0;state.pr=0x8c000100;state.sr=sr0;state.r[0]=0x12345678
  def stop(memory,r,s):state.pr=0x8c000100;return s
  reference(old,state,{0x8c07aeda:stop});sr=ctypes.c_uint32(sr0);last=ctypes.c_uint32();r0=fn(view,ctypes.byref(sr),0x8ca00900,0x8ca00000,0x8cf00000,ctypes.byref(last));case=(nodes,count,kind,sr0)
  assert r0==state.r[0] and sr.value==state.sr and last.value==state.r[14],case
  assert old[0xa00000:0xa06000]==native[0xa00000:0xa06000] and old[0x880000:0x884000]==native[0x880000:0x884000] and old[0x79f03c:0x79f040]==native[0x79f03c:0x79f040],case
  saved_vectors=bytes(native[0xa02000:0xa04000])
  for mem in [old,native]:mem[0xa02000:0xa04000]=bytes([0x5a])*8192
  state.pc=0x8c07adf6;state.r[14]=0x8ca00000;state.r[11]=0;reference(old,state,{0x8c07aeda:stop})
  r0=cleanup(view,ctypes.byref(sr),0x8ca00000,r0,ctypes.byref(last))
  assert r0==state.r[0] and sr.value==state.sr and last.value==state.r[14],case
  assert old[0xa00000:0xa06000]==native[0xa00000:0xa06000] and old[0x880000:0x884000]==native[0x880000:0x884000] and old[0x79f03c:0x79f040]==native[0x79f03c:0x79f040],case
  for i in range(nodes):assert struct.unpack_from('<I',native,0xa00010+i*24)[0]==0,case
  if nodes and count and kind not in [0,4,9]:
   for i in range(count):
    for base,mapped in [(0xa02000,i),(0xa02800,i),(0xa03000,7-i),(0xa03800,7-i)]:
     start=base+mapped*12;offset=start-0xa02000
     assert native[start:start+12]==saved_vectors[offset:offset+12],case
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_lifecycle_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_initialize.c').read_bytes()).hexdigest(),scope='Consecutive initialization, mutation of live vector arrays, restoration and native buffer release. Connected initialization branch with native allocation, mapped vector save and recursive descriptor lookup. Empty/single/multiple record arrays, record types0/1/4/9/10/255, zero/nonzero vector counts. RAM/r0/SR/r14 at epilogue compared; only epilogue stop hooked. Rendering state and full entry integration unrecovered.'),indent=2)+'\n');print(f'{cases} vector save/restore/release lifecycles passed')
if __name__=='__main__':main()

