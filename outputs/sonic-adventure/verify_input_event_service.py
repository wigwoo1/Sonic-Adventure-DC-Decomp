import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_event_service.dll')).sa_input_event_service
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c648470,0x8c647bde,0x8c64753c,0x8c64b4e4,0x8c64b1f0]
 for channel,mode,present,first,second,bank in itertools.product(range(8),[0,1],[0,1],[0,0xffffffff],[0,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;record=0x89c550+channel*0x148
  ram[record:record+0x148]=bytes(0x148);ram[record+2]=present;struct.pack_into('<I',ram,record+0x1c,0x8ca01000);struct.pack_into('<I',ram,0xa0101c,0x8ca02000)
  struct.pack_into('<I',ram,0x89c54c,0x8ca00000);struct.pack_into('<I',ram,0xa00008,mode);struct.pack_into('<I',ram,0x673ba8,11);struct.pack_into('<I',ram,0x894138+channel*128+8,18)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(r,t,pr):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr);r[0]=first if t==0x8c64753c else second if t==0x8c64b4e4 else 0;return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0]));return 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6482a0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  wanted=[0x8c648470] if mode else [] if not present else [0x8c647bde,0x8c64753c]+([] if first else [0x8c64b4e4]+([] if second else [0x8c64b1f0]))
  assert [x[0] for x in actual]==wanted and r[0]==(0 if mode else 0xffffff01 if not present or first else 0xffffff83 if second else 0)
  if not mode and present:
   assert actual[0][1]==(channel,18,1,11) and struct.unpack_from('<2I',native,record+0x30)==(2,11)
   assert native[record+8:record+10]==(bytes([1,1]) if not first and not second else bytes(2))
  count+=1
 report=dict(cases_passed=count,instruction_count=103,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_event_service.c').read_bytes()).hexdigest(),scope='Retail full state/call comparison across eight channels, mode tail path, presence, two backend error stages and both FPU banks. Independent call order, return codes, secondary callback arguments and record counters/flags. Five subordinate services injected; concrete device backend unfinished.')
 (out/'analysis/input_event_service_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'event service comparisons passed')
if __name__=='__main__':main()
