import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_render_single_mesh.dll')).sa_model_render_single_mesh;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];cases=0
 for bank,sr0,count,limit,matrix in itertools.product(range(2),[0,0x700083f3],[0,1,2,127,128,255],[1,4],[0,0x8ca02000]):
  rng=random.Random(count);data=rng.randbytes(8192)
  for m in (old,native):
   m[0xa02000:0xa04000]=data;m[0xf00000:0xf00100]=bytes([0x5a])*256;m[0xa0000c]=count;struct.pack_into('<I',m,0xf00078,matrix);struct.pack_into('<2I',m,0x88f698,limit,1);struct.pack_into('<I',m,0x88f5f8,0x8ca03000)
  f=Fpu();f.fpscr=(bank<<21)|0x40000;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  f.banks[bank][13]=0;f.banks[bank][14]=0x3f800000;nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[14]=0x8ca00000;state.r[15]=0x8cf00000;state.sr=sr0;state.pc=0x8c07a8b0;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  reference(old,state,{0x8c07a93c:stop},f)
  sr=ctypes.c_uint32(sr0);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r)==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and native[0xf00000:0xf00100]==old[0xf00000:0xf00100] and native[0xa02000:0xa04000]==old[0xa02000:0xa04000] and native[0x88f5f8:0x88f6a0]==old[0x88f5f8:0x88f6a0],(bank,count,limit,matrix)
  axis=0 if count==0 else 1 if count==1 else 2
  assert struct.unpack_from('<6I',native,0xf0007c)==tuple(0x3f800000 if i==axis else 0 for i in range(6))
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_render_single_mesh_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_render_single_mesh.c').read_bytes()).hexdigest(),scope='Single-mesh fallback through record dispatch, original matrix push executes without hooks. Full integer/FPU/SR/frame/matrix/global RAM checks and independent axis assertion; counts0/1/2/127/128/255, banks and push success/failure, null/explicit matrices. Full per-record setup and rendering integration incomplete.'),indent=2)+'\n');print(f'{cases} single mesh setup comparisons passed')
if __name__=='__main__':main()
