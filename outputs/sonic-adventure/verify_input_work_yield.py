import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_work_yield.dll')).sa_input_work_yield
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for delay,tick,bank,resumed in itertools.product([0,1,0xffffffff,0x80000000],[0,5,0xffffffff],[0,0x200000],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8e9dd8,0x8ca00000);struct.pack_into('<I',ram,0xa00204,tick);struct.pack_into('<I',ram,0xa02690,0x8ca03000)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,t,pr):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr)
   r[0]=resumed if t==0x8c110954 else 1;r[1]=0xabcdef01;r[2]=0xabcdef02;r[3]=0xabcdef03;return trace,sr^0x30
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in [0x8c110954,0x8c1108e0]:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=delay;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c668e1e
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in [0x8c110954,0x8c1108e0]},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(f)==bytes(nf)
  assert struct.unpack_from('<I',native,0xa03014)[0]==((tick+delay)&0xffffffff if delay<0x80000000 else 0x7fffffff)
  assert actual[0][0:2]==(0x8c110954,(0x8ca03018,0x22340005,0x22340006,0x22340007)) and len(actual)==(1 if resumed else 2)
  if not resumed:assert actual[1][1][0:2]==(0x8ca00004,1)
  count+=1
 report=dict(cases_passed=count,instruction_count=31,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_work_yield.c').read_bytes()).hexdigest(),scope='Full retail state comparison; independent signed delay/sentinel, wrapping due-time, work context pointer, initial/resumed branch and manager restore arguments. Context services injected in this standalone test; connected suspension unfinished.')
 (out/'analysis/input_work_yield_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'work yield comparisons passed')
if __name__=='__main__':main()
