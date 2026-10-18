import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/interrupt_gameplay.dll';fn=ctypes.CDLL(str(dll)).sa_interrupt_gameplay_callback
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for timer,mode,selector,bank in itertools.product([0,1,2,0xffffffff],[0,4,5,9],[0,7,0x100,0x207],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x754ec0,timer);struct.pack_into('<I',ram,0x760970,mode)
  target=0x8c010200+(selector>>8)*16+(selector&255)*2
  struct.pack_into('<I',ram,0x19e3e4+(selector>>8)*32+(selector&255)*4,target)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  targets=[0x8c04f7e0,0x8c643f98,0x8c602cb8,0x8c09ce40,target]
  def effect(mem,r,sr,t,pr):
   trace=(t,tuple(r[4:8]),r[15],pr)
   r[0]=selector if t==0x8c04f7e0 else 0x34560000
   for i in range(1,4):r[i]=0x34560000+i
   return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c051810
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and actual==expected and bytes(nf)==bytes(f),(timer,mode,selector,bank)
  assert not errors,errors
  assert struct.unpack_from('<I',native,0x754ec0)[0]==(timer-1)&0xffffffff
  assert [t[0] for t in actual]==([0x8c04f7e0]+([target] if mode in [4,5,9] else [])+[0x8c643f98] if timer==1 else [])+[0x8c602cb8,0x8c09ce40]
  assert actual[-1][3]==0x8c000100
  count+=1
 report=dict(cases_passed=count,instruction_count=55,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_interrupt_gameplay_callback.c').read_bytes()).hexdigest(),scope='Full retail registers, RAM, FPU, SR, PR and call trace comparisons. Independent countdown wrapping, gated mode/selector dispatch and tail-call PR checks. Called gameplay/input/device routines remain injected; full runtime unfinished.')
 (out/'analysis/interrupt_gameplay_callback_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'interrupt gameplay callback comparisons passed')
if __name__=='__main__':main()
