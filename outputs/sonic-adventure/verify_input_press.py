import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/input_press.dll';lib=ctypes.CDLL(str(dll));counts={}
 targets=[0x8c647ca4,0x8c647cb4,0x8c647cc4]
 for name,entry,code in [('primary',0x8c602df8,1),('alternate',0x8c602e34,18)]:
  fn=getattr(lib,'sa_input_press_'+name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for channel,result,bank in itertools.product(range(8),[0,1,0xffffffff],[0,0x200000]):
   ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
   native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
   def effect(mem,r,sr,t,pr):
    trace=(t,tuple(r[4:8]),r[15],pr,struct.unpack_from('<3I',mem,r[15]&0xffffff))
    for i in range(4):r[i]=0x34560000+i
    if t==0x8c647ca4:r[0]=result
    return trace,sr^0x31
   @Call
   def call(ctx,t,m,f,sr,r,pr):
    if t not in targets:errors.append(hex(t));return -9
    trace,sr[0]=effect(native,r,sr[0],t,pr[0]);actual.append(trace);return 0
   def hook(t):
    def body(mem,r,sr):trace,sr=effect(mem,r,sr,t,s.pr);expected.append(trace);return sr
    return body
   f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and expected==actual and bytes(nf)==bytes(f)
   assert not errors and r[0]==(0xffffffff if result else 0)
   assert [x[0] for x in actual]==(targets[:1] if result else targets)
   assert actual[0][1][0]==channel
   if not result:
    assert actual[1][1][:2]==(channel,code)
    assert actual[2][1]==(channel,channel,0,0) and actual[2][4]==(0,0,0)
   count+=1
  counts[name]=count
 report=dict(cases_passed=counts,instruction_count_each=30,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={name:hashlib.sha256((out/'source'/('sa_input_press_'+name+'.c')).read_bytes()).hexdigest() for name in ['primary','alternate']},scope='Retail full-state comparisons for press helpers; independent query gate, event codes 1/18, channel forwarding, three zero stack arguments and return result assertions. Three lower services injected.')
 (out/'analysis/input_press_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'input press comparisons passed')
if __name__=='__main__':main()
