import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/interrupt_input_events.dll';fn=ctypes.CDLL(str(dll)).sa_interrupt_input_events
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c647908,0x8c648234,0x8c6033e4,0x8c602df8,0x8c602e34,0x8c647bb8]
 patterns=[(0,0),(0,255),(255,0),(0x55,0xaa),(0xaa,0x55),(1,1),(0x80000000,0x800000ff),(255,255)]
 for enabled,alternate,values,available,pending,bank in itertools.product([0,1],[0,1],patterns,[0,255,0x55,0xaa],[0,255,0xa5a50055],[0,0x200000]):
  previous,current=values;ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67342c,enabled);struct.pack_into('<5I',ram,0x88f55c,0xdeadbeef,previous,pending,pending,alternate)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,sr,t,pr):
   trace=(t,tuple(r[4:8]),r[15],pr);value=current if t==0x8c648234 else (0 if available&(1<<r[4]) else 1) if t==0x8c6033e4 else 0x12340000
   for i in range(4):r[i]=0x34560000+i
   r[0]=value;return trace,sr^0x31
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
  def hook(t):
   def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c602cb8
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
  assert not errors
  wanted=[];press=pending;release=pending
  if enabled:
   wanted=[(0x8c647908,None),(0x8c648234,None)]+[(0x8c6033e4,i) for i in range(8)]
   for rising,which,target in [(True,'press',0x8c602df8 if alternate else 0x8c602e34),(False,'release',0x8c647bb8)]:
    edge=(current^previous)&(current if rising else previous);queued=press if rising else release
    for i in range(8):
     bit=1<<i
     if edge&bit:queued|=bit
     if available&bit and queued&bit:wanted.append((target,i));queued&=~bit
    if rising:press=queued
    else:release=queued
   assert struct.unpack_from('<4I',native,0x88f55c)==(previous,current,press,release)
  assert [(t[0],t[1][0] if t[0] not in [0x8c647908,0x8c648234] else None) for t in actual]==wanted
  for t in actual:
   if t[0]==0x8c647bb8:assert t[1][1:3]==(2,0)
  count+=1
 report=dict(cases_passed=count,instruction_count=145,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_interrupt_input_events.c').read_bytes()).hexdigest(),scope='Full retail state/call comparisons; independent eight-channel edge/pending-event model, disabled path, both press dispatch variants, all/none/alternating availability and retained high bits. Six subordinate services injected; device backends unfinished.')
 (out/'analysis/interrupt_input_events_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'input event comparisons passed')
if __name__=='__main__':main()
