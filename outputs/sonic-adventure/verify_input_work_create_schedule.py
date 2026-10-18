import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_work_create_native
 schedule=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_frame_context_native
 schedule.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call]
 scheduled=0
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for busy,bank,size,tick in itertools.product(range(4),[0,0x200000],[4,257],[0,0xffffffff]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<3I',ram,0xa00204,tick,3,busy)
  for i in range(3):struct.pack_into('<I',ram,0xa00210+i*0x124+0xc,3 if i<busy else 0)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,t,pr):
   trace=(t,tuple(r[i] for i in range(4,8)),pr)
   if t==0x8c10d31c:mem[r[4]&0xffffff:(r[4]&0xffffff)+r[6]]=bytes([r[5]&255])*r[6]
   elif t==0x8c10d25c:mem[r[4]&0xffffff:(r[4]&0xffffff)+r[6]]=mem[r[5]&0xffffff:(r[5]&0xffffff)+r[6]]
   else:
    mem[r[4]&0xffffff:(r[4]&0xffffff)+152]=bytes((i*7+3)&255 for i in range(152))
   r[0]=0;return trace,sr
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in [0x8c10d31c,0x8c10d25c,0x8c110954]:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[5]=0x8c010600;s.r[6]=0x8ca08000;s.r[7]=size;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c668bee
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666)
  reference(ram,s,fpu=f,special=dict(mach=mach.value,macl=0,gbr=gbr.value))
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  assert r[0]==(busy if busy<3 else 0xffffffff) and not actual and r[15]==0x8cf00000
  if busy<3:
   job=0xa00210+busy*0x124
   assert struct.unpack_from('<6I',native,job)==(0x8ca08000,size>>2,0,1,0,tick)
   assert struct.unpack_from('<I',native,job+0x18)[0]==0x8ca08000+((size>>2)-1)*4
   assert struct.unpack_from('<I',native,job+0x20)[0]==0x8c010600
   assert struct.unpack_from('<2I',native,job+0x118)==(0,0)
  if busy<3:
   struct.pack_into('<2I',ram,0x8e9dd8,0x8cbabe00,0xdeadbeef)
   struct.pack_into('<2I',native,0x8e9dd8,0x8cbabe00,0xdeadbeef)
   s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.pr=0x8c000100;s.pc=0x8c668e60
   r[4]=0x8ca00000;r[15]=0x8cf00000;pr.value=0x8c000100
   reference(ram,s,fpu=f,special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value),stop_pc=0x8c010600)
   assert schedule(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==1
   nf.fpul=fpul.value
   assert not errors and not actual and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   assert pr.value==0x8c010600 and r[15]==0x8ca08000+((size>>2)-1)*4
   assert struct.unpack_from('<I',native,0xa00204)[0]==tick
   assert struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8ca00000
   scheduled+=1
  count+=1
 report=dict(creation_cases_passed=count,scheduling_cases_passed=scheduled,instruction_count=87,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_work_create.c').read_bytes()).hexdigest(),scope='Full retail state comparison; free-record selection, full capacity, record initialization, due tick, stack pointer and entry address checked independently. Memory fill/move and context save native; no injected services. Natively created contexts then scheduled through native manager/context restore to their entry address. No injected services; work entry execution and concrete callbacks unfinished.')
 (out/'analysis/input_work_create_schedule_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'creation and',scheduled,'native scheduling comparisons passed')
if __name__=='__main__':main()
