import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;view=(ctypes.c_uint8*len(ram)).from_buffer(ram);P=ctypes.POINTER(ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_frame_convert.dll')).sa_model_hierarchy_frame_convert;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_uint];cases=0
 for bank,rm,sr0,mode,raw in itertools.product(range(2),range(2),[0,0x700083f3],[2,3],[0,0x80000000,0x3e800000,0xbe800000,0x3fc00000,0xbfc00000,0x4effffff,0x4f000000,0x4f000001,0x4f7fffff,0x4f800000,0xcf000000,0xcf000001]):
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x12345678;f.banks[bank][14]=0x4f000000;nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=range(16);state.r[14]=0x8ca00100;state.pc=0x8c057c80 if mode==2 else 0x8c057ed0;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r);struct.pack_into('<I',ram,0xa00110,raw)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  reference(ram,state,{0x8c057cc4 if mode==2 else 0x8c057ee8:stop},f);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),mode)==0;nf.fpul=fpul.value
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f),(mode,hex(raw));cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_frame_convert_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_frame_convert.c').read_bytes()).hexdigest(),scope='Both layout frame conversion through keyframe-search entry; native FCMP/FADD/FTRC no services, all integer/FPU/SR/FPUL compared. Signed zeros/fractions, 2^31 threshold and high frame boundaries, banks/RM0/1. Nonfinite/subnormal/exception behavior outside scope; full channel integration pending.'),indent=2)+'\n');print(cases,'frame conversion comparisons passed')
if __name__=='__main__':main()
