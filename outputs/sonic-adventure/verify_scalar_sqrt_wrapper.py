import ctypes,hashlib,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import P
from verify_tikal_position_native import Vector
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/scalar_sqrt_wrapper.dll')).sa_scalar_signed_sqrt;fn.argtypes=[ctypes.POINTER(Fpu),P,ctypes.c_void_p,Vector];count=0
 for value,bank,rm,sr0,answer in itertools.product([0,0x80000000,0x00800000,0x80800000,0x3f800000,0xbf800000,0x40000000,0xc0000000,0x7f7fffff,0xff7fffff],range(2),range(2),[0,0x700083f3],[0,0x3f000000,0x3f800000]):
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x12345678
  for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
  f.banks[bank][4]=value;nf=Fpu.from_buffer_copy(f);expected=[];actual=[]
  def apply(word,fp,log):
   assert word==0xf06d;log.append((word,bytes(fp)[:132]));fp.banks[(fp.fpscr>>21)&1][0]=answer;fp.fpscr=(fp.fpscr&~0x3f000)|0x1004
  @Vector
  def callback(ctx,word,fp):apply(word,fp.contents,actual);return 0
  s=State();s.r[:]=[0x12340000+i for i in range(16)];s.pc=0x8c63a9c4;s.pr=0x8c000100;s.sr=sr0
  reference(ram,s,fpu=f,vector_instruction=lambda w,fp:apply(w,fp,expected));sr=ctypes.c_uint32(sr0)
  assert fn(ctypes.byref(nf),ctypes.byref(sr),None,callback)==0
  assert sr.value==s.sr and bytes(nf)==bytes(f) and expected==actual,(value,bank,rm,sr0,answer)
  assert list(s.r)==[0x12340000+i for i in range(16)] and s.pr==0x8c000100;count+=1
 report=dict(cases_passed=count,instruction_count=10,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/scalar_sqrt_wrapper.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/scalar_sqrt_wrapper.dll').read_bytes()).hexdigest(),scope='Complete signed-square-root wrapper, negative/positive finite normal and signed zero, both banks/RM0/1, SR preservation and exact FSQRT input snapshots. Retail body unhooked; FSQRT numerical output/FPSCR service injected. PR=SZ=0; NaN/Infinity/subnormal compare paths unsupported. Numerical square-root recovery and caller integration pending.')
 (out/'analysis/scalar_sqrt_wrapper_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'signed-sqrt wrapper comparisons passed')
if __name__=='__main__':main()
