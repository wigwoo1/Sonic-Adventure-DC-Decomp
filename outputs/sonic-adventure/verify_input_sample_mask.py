import ctypes,hashlib,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/input_sample_mask.dll';fn=ctypes.CDLL(str(dll)).sa_input_sample_mask
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 for mask,nonzero,bank in itertools.product(range(256),[1,0xffffffff],[0,0x200000]):
  expected=[];actual=[];errors=[]
  def effect(r,sr,pr):
   channel=r[4];trace=(channel,r[15],pr);value=nonzero if mask&(1<<channel) else 0
   for i in range(4):r[i]=0x34560000+i
   r[0]=value;return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t!=0x8c660dac:errors.append(hex(t));return -9
   trace,sr[0]=effect(r,sr[0],pr[0]);actual.append(trace);return 0
  def hook(mem,r,sr):trace,sr=effect(r,sr,s.pr);expected.append(trace);return sr
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c648234
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{0x8c660dac:hook},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
  assert not errors and r[0]==mask
  assert [x[0] for x in actual]==list(range(8))
  assert all(x[1:]==(0x8cefffe8,0x8c648250) for x in actual)
  count+=1
 report=dict(cases_passed=count,instruction_count=29,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_sample_mask.c').read_bytes()).hexdigest(),scope='Full retail state comparisons; all 256 masks, two nonzero query values, both FPU banks, independent result/order/call-frame assertions. Query 0x8c660dac injected; device implementation incomplete.')
 (out/'analysis/input_sample_mask_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'input sample mask comparisons passed')
if __name__=='__main__':main()
