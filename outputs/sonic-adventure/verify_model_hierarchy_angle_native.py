import ctypes,itertools,json,hashlib,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;view=(ctypes.c_uint8*len(ram)).from_buffer(ram);P=ctypes.POINTER(ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_angle_native.dll')).sa_model_hierarchy_angle_native;CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_uint,ctypes.c_void_p,CB];cases=0
 for bank,rm,sr0,mode,pair,phase in itertools.product(range(2),range(2),[0,0x700083f3],[2,3],[(0,1),(0xffff,0),(0x8000,0),(0xffff8000,0x7fff),(0x12340000,0xabcdffff)],[0.0,0.25,0.5,1.0,-1.0]):
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca02000;state.r[5]=0x8ca02010;state.r[8]=0x8ca01000;state.r[7]=4;state.r[14]=0x8ca00100;state.r[15]=0x8cefff80;state.r[12]=0x8c055d4e;state.r[11]=0x8c639cf4;state.pc=0x8c057d8c if mode==2 else 0x8c057fe0;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  struct.pack_into('<3I',ram,0xa02004,pair[0],pair[1],pair[0]^0xffff);struct.pack_into('<3I',ram,0xa02014,pair[1],pair[0],pair[1]^0x8000);ram[0xefff80:0xefff98]=bytes([0x55])*24;native=bytearray(ram);native_view=(ctypes.c_uint8*len(native)).from_buffer(native);struct.pack_into('<I',ram,0xa01004,0x8ca02000);f.banks[bank][15]=struct.unpack('<I',struct.pack('<f',phase))[0];nf=Fpu.from_buffer_copy(f)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  ev=[];av=[]
  def apply(word,fp,fpul,log):
   log.append((word,fpul,bytes(fp)));active=(fp.fpscr>>21)&1
   if word==0xf0fd:fp.banks[active][0:2]=[0x3f000001,0x3f800002]
   else:
    start=(word>>8)&12;fp.banks[active][start:start+4]=[0x3f800000+i+len(log)*4 for i in range(4)]
  def vector(word,fp):apply(word,fp,fp.fpul,ev)
  @CB
  def instruction(ctx,word,fp,fpul):fp.contents.fpul=fpul;apply(word,fp.contents,fpul,av);return 0
  reference(ram,state,{0x8c057dfe if mode==2 else 0x8c05808a:stop},f,vector_instruction=vector);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678);assert fn(native_view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),mode,None,instruction)==0;nf.fpul=fpul.value
  assert ram[0xefff80:0xefff98]==native[0xefff80:0xefff98]
  assert ev==av and list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f),(pair,phase,mode);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_angle_native_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_angle_native.c').read_bytes()).hexdigest(),scope='Both layouts angle interpolation and recovered rotation through common increment boundary, no original function hooks. Native difference/FLOAT/FMUL/FTRC; FSCA/FTRV results injected. Integer/FPU/SR/FPUL/scratch and vector input snapshots compared, wraparound/high bits, finite rates and banks/RM0/1. Numerical fidelity/full generator pending.'),indent=2)+'\n');print(cases,'angle interpolation comparisons passed')
if __name__=='__main__':main()
