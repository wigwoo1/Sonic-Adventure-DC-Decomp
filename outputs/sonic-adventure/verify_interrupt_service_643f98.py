import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/interrupt_service_643f98.dll';fn=ctypes.CDLL(str(dll)).sa_interrupt_service_643f98
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c65a378,0x8c10d428,0x8c10d444,0x8c65acac]
 for present,mode,timer,flag,values,swap,bank in itertools.product([0,1],[0,1,2],[0,1,0xffffffff],[0,1],[(0,1),(1,1),(0xffffffff,0)],[0,1],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<2I',ram,0x67c1d0,0x8ca00000 if present else 0,mode)
  for offset,f in [(0xa00000,flag),(0xa01000,flag^1)]:struct.pack_into('<I',ram,offset+0x16c,timer);struct.pack_into('<I',ram,offset+0x1ac,f)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,t,pr):
   trace=(t,tuple(r[4:8]),r[15],pr)
   if t==0x8c65a378 and swap:struct.pack_into('<I',mem,0x67c1d0,0x8ca01000)
   if t in [0x8c10d428,0x8c10d444]:struct.pack_into('<I',mem,r[5]&0xffffff,values[1] if t==0x8c10d428 else values[0])
   for i in range(4):r[i]=0x34560000+i
   return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c643f98
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and actual==expected and bytes(nf)==bytes(f)
  assert not errors and r[0]==0
  wanted=[]
  if present and mode==1:
   if timer==1:wanted.append(0x8c65a378)
   if flag^(swap if timer==1 else 0):
    wanted.extend([0x8c10d428,0x8c10d444])
    if values[0]<values[1]:wanted.append(0x8c65acac)
  assert [x[0] for x in actual]==wanted
  if wanted and wanted[-1]==0x8c65acac:assert actual[-1][1][:2]==(0x8c643f98,0x7fffffff)
  count+=1
 report=dict(cases_passed=count,instruction_count=44,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_interrupt_service_643f98.c').read_bytes()).hexdigest(),scope='Retail full-state comparisons and independent gating/call ordering/unsigned comparison checks, including global pointer replacement by first service. Four subordinate routines injected; exact subsystem naming unresolved.')
 (out/'analysis/interrupt_service_643f98_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'interrupt service comparisons passed')
if __name__=='__main__':main()
