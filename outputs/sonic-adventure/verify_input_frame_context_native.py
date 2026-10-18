import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_frame_context_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for jobs,style,tick,bank,callback in itertools.product([0,1,3],range(4),[0,0xffffffff],[0,0x200000],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;obj=0x8ca00000
  struct.pack_into('<2I',ram,0x8e9dd8,0x8cab0000,0xdeadbeef);struct.pack_into('<3I',ram,0xa00204,tick,jobs,jobs)
  states=[[0,2,3,4][(style+i)%4] for i in range(jobs)]
  for i,state in enumerate(states):
   struct.pack_into('<I',ram,0xa00210+i*0x124+0xc,state);struct.pack_into('<I',ram,0xa00210+i*0x124+0x118,0x8c010600 if callback else 0)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,fp,pr):
   trace=(tuple(r[i] for i in range(16)),sr,pr,fp.fpscr)
   struct.pack_into('<I',mem,0x8e9ddc,0x8cab1234);fp.banks[(fp.fpscr>>21)&1][1]^=0x1234
   r[0]=0x12345678;r[1]=0x87654321;return trace,sr^0x31
  @Call
  def call(ctx,t,m,fp,sr,r,pr):
   if t!=0x8c010600:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],fp.contents,pr[0]);actual.append(trace);return 0
  def hook(mem,r,sr):
   trace,sr=effect(mem,r,sr,f,s.pr);expected.append(trace);return sr
  f=Fpu();f.fpscr=bank;f.fpul=0xabcdef01;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=obj;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c668e60
  special=dict(mach=0x11112222,macl=0x33334444,gbr=0x55556666);r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(special['macl']);mach=ctypes.c_uint32(special['mach']);gbr=ctypes.c_uint32(special['gbr'])
  reference(ram,s,{0x8c010600:hook},f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  nf.fpul=fpul.value
  assert not errors and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and native==ram and actual==expected and bytes(nf)==bytes(f)
  assert len(actual)==(states.count(3) if callback else 0) and nf.fpscr==bank
  assert struct.unpack_from('<I',native,0xa00204)[0]==(tick+1)&0xffffffff
  assert struct.unpack_from('<I',native,0xa0020c)[0]==jobs-states.count(3)
  assert struct.unpack_from('<3I',native,0xa00048)==tuple(special[x] for x in ['mach','macl','gbr'])
  assert struct.unpack_from('<2I',native,0xa00094)==(0xabcdef01,0x40001)
  for i,state in enumerate(states):assert struct.unpack_from('<I',native,0xa00210+i*0x124+0xc)[0]==(0 if state==3 else 3 if state==2 else state)
  count+=1
 report=dict(cases_passed=count,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_frame_manager','sa_input_context_save','sa_input_work_dispatch','input_frame_context_native']},scope='Full retail state comparison of connected frame manager, native context save and work dispatch. Explicit MACH/MACL/GBR, optional work callbacks, state transitions, saved context, global mutation and counter wrapping checked. States 0/2/3/4 tested; state 1 restore and nonlocal continuation transfer unfinished. Callback bodies injected.')
 (out/'analysis/input_frame_context_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native frame context comparisons passed')
if __name__=='__main__':main()
