import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/input_frame_manager.dll';fn=ctypes.CDLL(str(dll)).sa_input_frame_manager
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c110954,0x8c1108e0,0x8c668cca]
 for jobs,style,tick,bank,mutation in itertools.product([0,1,3],range(5),[0,5,0xffffffff],[0,0x200000],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;obj=0x8ca00000
  struct.pack_into('<2I',ram,0x8e9dd8,0x8cab0000,0xdeadbeef);struct.pack_into('<2I',ram,0xa00204,tick,jobs)
  states=[];due=[]
  for i in range(jobs):
   state=(style+i)%5;when=(tick+[0,1,-1][i])&0xffffffff;states.append(state);due.append(when)
   struct.pack_into('<I',ram,0xa00210+i*0x124+0xc,state);struct.pack_into('<I',ram,0xa00210+i*0x124+0x14,when)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,fp,t,pr):
   trace=(t,tuple(r[4:8]),r[15],pr,fp.fpscr)
   if mutation and t==0x8c110954:
    struct.pack_into('<I',mem,0x8e9ddc,0x8cab1234);fp.fpscr^=0x200000
   fp.banks[(fp.fpscr>>21)&1][1]^=0x1234
   for i in range(4):r[i]=0x34560000+i
   return trace,sr^0x31
  @Call
  def call(ctx,t,m,fp,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],fp.contents,t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=obj;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c668e60
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
  assert not errors and nf.fpscr==bank
  wanted=[(0x8c110954,obj+4)]
  for i,state in enumerate(states):
   job=obj+0x210+i*0x124
   if state==3:wanted.append((0x8c668cca,obj))
   if state==1 and due[i]<=tick:wanted.append((0x8c1108e0,job+0x18))
   if state==2:assert struct.unpack_from('<I',native,(job+0xc)&0xffffff)[0]==3 and struct.unpack_from('<I',native,(job+0x20)&0xffffff)[0]==0x8c668d56
  assert [(x[0],x[1][0]) for x in actual]==wanted
  assert actual[0][4]==0x40001
  assert struct.unpack_from('<I',native,0xa00204)[0]==(tick+1)&0xffffffff
  assert struct.unpack_from('<I',native,0x8e9dd8)[0]==(0x8cab1234 if mutation else 0x8cab0000)
  if jobs:assert struct.unpack_from('<I',native,0xa02690)[0]==obj+0x210+(jobs-1)*0x124
  count+=1
 report=dict(cases_passed=count,instruction_count=85,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_frame_manager.c').read_bytes()).hexdigest(),scope='Full retail registers/RAM/FPU/call comparisons; independent job-state dispatch, unsigned due-time comparison, frame-counter wrapping, record updates and FPSCR/context restoration checks. Three subordinate routines injected; device system unfinished.')
 (out/'analysis/input_frame_manager_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'input frame manager comparisons passed')
if __name__=='__main__':main()
