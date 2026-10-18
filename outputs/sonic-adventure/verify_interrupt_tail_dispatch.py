import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/interrupt_tail_dispatch.dll';fn=ctypes.CDLL(str(dll)).sa_interrupt_tail_dispatch
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c01025a,0x8c60434c,0x8c60178c,0x8c6017a4]
 for flag,other,result,bank in itertools.product([0,1,0xffffffff],[0,1],[0,1,6,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x782974,flag);struct.pack_into('<I',ram,0x161cb0,other)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,t,pr):
   trace=(t,tuple(r[4:8]),r[15],pr)
   if t==0x8c01025a:struct.pack_into('<I',mem,0x782974,0)
   for i in range(4):r[i]=0x34560000+i
   if t==0x8c60178c:r[0]=result
   return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c09ce40
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and actual==expected and bytes(nf)==bytes(f)
  assert not errors
  wanted=[0x8c01025a,0x8c60434c] if flag and not other else [0x8c60178c]+([0x8c6017a4] if result in [0,6] else [])
  assert [x[0] for x in actual]==wanted
  if wanted[-1] in [0x8c60434c,0x8c6017a4]:assert actual[-1][3]==0x8c000100 and actual[-1][2]==0x8cf00000
  if wanted[-1]==0x8c6017a4:assert actual[-1][1][0]==result
  count+=1
 report=dict(cases_passed=count,instruction_count=29,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_interrupt_tail_dispatch.c').read_bytes()).hexdigest(),scope='Full retail state and call-trace comparisons; independent flag/result gates, pointer-independent branch mutation, tail-call PR and stack restoration. Four subordinate routines injected.')
 (out/'analysis/interrupt_tail_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'interrupt tail comparisons passed')
if __name__=='__main__':main()
