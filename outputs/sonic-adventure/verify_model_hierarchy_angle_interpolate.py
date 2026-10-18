import ctypes,itertools,json,hashlib,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;view=(ctypes.c_uint8*len(ram)).from_buffer(ram);P=ctypes.POINTER(ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_angle_interpolate.dll')).sa_model_hierarchy_angle_interpolate;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,ctypes.c_uint];cases=0
 for bank,rm,sr0,mode,pair,phase in itertools.product(range(2),range(2),[0,0x700083f3],[2,3],[(0,1),(0xffff,0),(0x8000,0),(0xffff8000,0x7fff),(0x12340000,0xabcdffff)],[0.0,0.25,0.5,1.0,-1.0]):
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca02000;state.r[5]=0x8ca02010;state.r[8]=0x8ca01000;state.r[7]=4;state.r[14]=0x8ca00100;state.r[15]=0x8cefff80;state.r[12]=0x8c055d4e;state.pc=0x8c057d8c if mode==2 else 0x8c057fe0;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  struct.pack_into('<3I',ram,0xa02004,pair[0],pair[1],pair[0]^0xffff);struct.pack_into('<3I',ram,0xa02014,pair[1],pair[0],pair[1]^0x8000);ram[0xefff80:0xefff98]=bytes([0x55])*24;native=bytearray(ram);native_view=(ctypes.c_uint8*len(native)).from_buffer(native);struct.pack_into('<I',ram,0xa01004,0x8ca02000);f.banks[bank][15]=struct.unpack('<I',struct.pack('<f',phase))[0];nf=Fpu.from_buffer_copy(f)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  reference(ram,state,{0x8c057dfa if mode==2 else 0x8c05804e:stop},f);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678);assert fn(native_view,ctypes.byref(nf),r,ctypes.byref(fpul),mode)==0;nf.fpul=fpul.value
  assert ram[0xefff80:0xefff98]==native[0xefff80:0xefff98]
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f),(pair,phase,mode);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_angle_interpolate_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_angle_interpolate.c').read_bytes()).hexdigest(),scope='Both layouts angle interpolation through rotation-call boundary, native 16-bit difference/FLOAT/FMUL/FTRC no function or arithmetic services. Integer/FPU/SR/FPUL/scratch compared, wraparound/high angle bits, finite rates and banks/RM0/1. Rotation integration/nonfinite/traps pending.'),indent=2)+'\n');print(cases,'angle interpolation comparisons passed')
if __name__=='__main__':main()
