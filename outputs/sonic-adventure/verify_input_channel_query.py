import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/input_channel_query.dll';fn=ctypes.CDLL(str(dll)).sa_input_channel_query
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,status,flags,mapping,bank in itertools.product(range(8),[0,1,128,255],[0,1,2,3,0xffffffff],[0,0x80000000],[0,0x200000]):
  mapped=mapping+channel*5+1;ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x6724f4+channel*4,mapped);ram[0xa00000]=status;struct.pack_into('<I',ram,0xa00100,flags)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(r,sr,t,pr):
   trace=(t,r[4],r[15],pr)
   for i in range(1,4):r[i]=0x34560000+i
   r[0]=0x8ca00000 if t==0x8c646856 else 0x8ca00100
   return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in [0x8c646856,0x8c646868]:errors.append(hex(t));return -9
   trace,sr[0]=effect(r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c660dac
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in [0x8c646856,0x8c646868]},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
  assert not errors and r[0]==int(bool(status) and bool(flags&2))
  assert actual==[(0x8c646856,mapped,0x8ceffff4,0x8c660dc0),(0x8c646868,mapped,0x8ceffff4,0x8c660dc8)]
  count+=1
 report=dict(cases_passed=count,instruction_count=32,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_channel_query.c').read_bytes()).hexdigest(),scope='Full retail state comparisons; independent mapped-channel arguments, both query calls, byte-status gate and flag bit 1 checks. Two lower pointer-returning services injected; device backends incomplete.')
 (out/'analysis/input_channel_query_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'input channel query comparisons passed')
if __name__=='__main__':main()
