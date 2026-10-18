import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_press_service.dll')).sa_input_press_service
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 targets=[0x8c647bde,0x8c64753c,0x8c64b4e4,0x8c660a7e]
 for channel,kind,first,second,third,bank in itertools.product(range(8),range(5),[0,0xffffffff],[0,0xffffffff],[0,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;record=0x89c550+channel*0x148;device=0x8a0fb0+channel*0x4c
  ram[record:record+0x148]=bytes(0x148);struct.pack_into('<I',ram,record+4,kind);struct.pack_into('<I',ram,device+0x1c,0x8ca02000)
  struct.pack_into('<I',ram,0x673bec+kind*80,11);struct.pack_into('<I',ram,0x894138+channel*128+8,18)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(r,t,pr):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr);r[0]={0x8c64753c:first,0x8c64b4e4:second,0x8c660a7e:third}.get(t,0);return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0]));return 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c64839a
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  wanted=[0x8c647bde,0x8c64753c]+([] if first else [0x8c64b4e4]+([] if second else [0x8c660a7e]))
  result=0xffffff01 if first else 0xffffff83 if second else 0xffffff01 if third else [15,33,65,122,0xffffff83][kind]
  assert [x[0] for x in actual]==wanted and r[0]==result and actual[0][1]==(channel,18,1,11)
  assert struct.unpack_from('<I',native,record+0x1c)[0]==0x8c000000+device and struct.unpack_from('<2I',native,record+0x30)==(2,11)
  assert native[record+9]==(1 if not first and not second and not third else 0)
  if not first and not second:assert struct.unpack_from('<I',native,device+0x30)[0]==0x8ca02000
  count+=1
 report=dict(cases_passed=count,instruction_count=107,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_press_service.c').read_bytes()).hexdigest(),scope='Eight channels, five device types, all three backend success/error stages and both FPU banks. Full retail RAM/register/FPU/call comparison and independent return, callback and device-record checks. Four subordinate services injected; device backend unfinished.')
 (out/'analysis/input_press_service_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'press service comparisons passed')
if __name__=='__main__':main()
