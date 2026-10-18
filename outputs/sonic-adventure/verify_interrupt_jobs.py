import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/interrupt_jobs.dll';lib=ctypes.CDLL(str(dll));counts={}
 for name,entry,offset in [('query',0x8c60178c,0x54),('dispatch',0x8c6017a4,0x58)]:
  fn=getattr(lib,'sa_interrupt_job_'+name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for present,result,bank,saved in itertools.product([0,1],[0,1,6,0xffffffff],[0,0x200000],[0,0x8ca01000]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
   struct.pack_into('<I',ram,0x673420,0x8ca00000 if present else 0);struct.pack_into('<I',ram,0xa00000+offset,0x8c010200)
   native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
   def effect(mem,r,sr,pr):
    trace=(tuple(r[4:8]),r[14],r[15],pr);struct.pack_into('<I',mem,0x673420,0x8ca02000)
    for i in range(1,4):r[i]=0x34560000+i
    r[0]=result;return trace,sr^0x31
   @Call
   def call(ctx,t,m,f,sr,r,pr):
    if t!=0x8c010200:errors.append(hex(t));return -9
    trace,sr[0]=effect(native,r,sr[0],pr[0]);actual.append(trace);return 0
   def hook(mem,r,sr):trace,sr=effect(mem,r,sr,s.pr);expected.append(trace);return sr
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[14]=saved;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
   reference(ram,s,{0x8c010200:hook},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
   assert not errors and r[0]==(result if present else 0)
   assert len(actual)==present
   if present:assert actual[0]==(tuple(0x22340000+i for i in range(4,8)),saved,0x8cf00000,0x8c000100)
   count+=1
  counts[name]=count
 report=dict(cases_passed=counts,instruction_count_each=12,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={name:hashlib.sha256((out/'source'/('sa_interrupt_job_'+name+'.c')).read_bytes()).hexdigest() for name in ['query','dispatch']},scope='Full retail state comparisons; nullable backend object and indirect tail calls at offsets 0x54/0x58. Independent argument, saved-register, stack and return-address assertions. Backend virtual methods injected.')
 (out/'analysis/interrupt_jobs_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'interrupt job comparisons passed')
if __name__=='__main__':main()
