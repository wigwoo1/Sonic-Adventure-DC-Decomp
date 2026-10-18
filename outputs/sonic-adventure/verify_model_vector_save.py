import ctypes,hashlib,itertools,json,struct,random
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);rng=random.Random(0xae16)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_save.dll')).sa_model_vector_save;fn.restype=ctypes.c_uint32;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*4;cases=0
 for count,offset,sr0 in itertools.product([1,2,8,32],[0,1,7],[0,0x700083f3]):
  data=rng.randbytes(0x4000)
  for m in [old,native]:
   m[0xa00000:0xa04000]=data;m[0xa0000c]=count;struct.pack_into('<2I',m,0xa00010,0x8ca01000,0x8ca00100)
   struct.pack_into('<2I',m,0xa00200,0x8ca02000,0x8ca02400);struct.pack_into('<2I',m,0xa00300,0x8ca02800,0x8ca02c00)
   for i in range(count):struct.pack_into('<2H',m,0xa00100+i*4,(i+offset)%32,(31-i+offset)%32)
  state=State();state.r[14]=0x8ca00000;state.r[6]=0x8ca00200;state.r[7]=0x8ca00300;state.r[4]=0;state.pc=0x8c07a60a;state.pr=0x8c000100;state.sr=sr0
  def stop(memory,r,s):state.pr=0x8c000100;return s
  reference(old,state,{0x8c07a6dc:stop});sr=ctypes.c_uint32(sr0);r0=fn(view,ctypes.byref(sr),0x8ca00000,0x8ca00200,0x8ca00300,0)
  assert r0==state.r[0] and sr.value==state.sr and old[0xa00000:0xa04000]==native[0xa00000:0xa04000],(count,offset,sr0);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_save_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_save.c').read_bytes()).hexdigest(),scope='Internal mapped vector save loop with counts1/2/8/32, mapping permutations and random raw vectors. RAM/r0/SR compared; following lookup stop-hooked. Nonoverlapping valid arrays; full helper incomplete.'),indent=2)+'\n');print(f'{cases} vector save comparisons passed')
if __name__=='__main__':main()
