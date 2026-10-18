import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_work_dispatch.dll')).sa_input_work_dispatch
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for counter,callback,bank,mutation in itertools.product([0,1,0xffffffff],[0,0x8c010600],[0,0x200000],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  obj=0x8ca00000;job=0x8ca03000
  struct.pack_into('<I',ram,0xa0020c,counter);struct.pack_into('<I',ram,0xa0300c,3);struct.pack_into('<I',ram,0xa03118,callback)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,pr):
   trace=(tuple(r[i] for i in range(16)),sr,pr)
   if mutation:struct.pack_into('<I',mem,0xa0300c,7)
   r[0]=0x98765432;r[1]=0x87654321
   return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t!=callback or not callback:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],pr[0]);actual.append(trace);return 0
  def hook(mem,r,sr):
   trace,sr=effect(mem,r,sr,s.pr);expected.append(trace);return sr
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=obj;s.r[5]=job;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c668cca
  saved14=s.r[14];r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{callback:hook} if callback else {},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and native==ram and actual==expected and bytes(f)==bytes(nf)
  assert struct.unpack_from('<I',native,0xa0020c)[0]==(counter-1)&0xffffffff
  assert struct.unpack_from('<I',native,0xa0300c)[0]==(7 if callback and mutation else 0)
  assert len(actual)==bool(callback) and r[14]==saved14 and r[15]==0x8cf00000
  if callback:assert actual[0][0][4:6]==(obj,job) and actual[0][0][14]==saved14 and actual[0][0][15]==0x8cf00000 and actual[0][2]==0x8c000100
  count+=1
 report=dict(cases_passed=count,instruction_count=17,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_work_dispatch.c').read_bytes()).hexdigest(),scope='Full retail register/RAM/FPU/call comparison; independent wrapping counter decrement, work state clearing, optional callback tail dispatch and restored stack/register checks. Callback bodies injected; CPU context switching remains unrecovered.')
 (out/'analysis/input_work_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'work dispatch comparisons passed')
if __name__=='__main__':main()



