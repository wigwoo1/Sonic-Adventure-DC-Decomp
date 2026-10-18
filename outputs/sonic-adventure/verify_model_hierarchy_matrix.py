import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_matrix.dll')).sa_model_hierarchy_matrix;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];cases=0
 for bank,sr0,count,index,dst in itertools.product(range(2),[0,0x700083f3],[0,1,4],[0,1,3,4,65535],[0,0x8ca00000,0x8c789448,0x8c789458]):
  rng=random.Random(index+count*100);data=rng.randbytes(512)
  for m in (old,native):
   m[0x789448:0x789648]=data;m[0xa00000:0xa00080]=bytes([0x5a])*128;m[0xeffff4:0xf00000]=bytes([0x55])*12;struct.pack_into('<I',m,0x78b448,count)
  f=Fpu();f.fpscr=(bank<<21)|0x40000;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=index;state.r[5]=dst;state.r[15]=0x8cf00000;state.sr=sr0;state.pc=0x8c058434;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r)
  reference(old,state,fpu=f)
  sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r)==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and native[0x789448:0x789648]==old[0x789448:0x789648] and native[0xa00000:0xa00080]==old[0xa00000:0xa00080] and native[0xeffff4:0xeffffc]==old[0xeffff4:0xeffffc],(bank,count,index,dst)
  assert r[0]==int(index<count)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_matrix_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_matrix.c').read_bytes()).hexdigest(),scope='Indexed hierarchy matrix retrieval and native copy through return without hooks. All integer/FPU/SR, matrix/destination/canonical scratch RAM and independent status assertion; count/index limits, FR banks, null/ordinary/overlapping destinations. Saved PR bytes excluded; matrix generation and rendering integration incomplete.'),indent=2)+'\n');print(f'{cases} hierarchy matrix retrieval comparisons passed')
if __name__=='__main__':main()
