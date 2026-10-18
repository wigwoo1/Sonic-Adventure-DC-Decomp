import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_backend_request.dll')).sa_input_backend_request
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];resume=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_backend_request.dll')).sa_input_backend_request_resume;resume.argtypes=fn.argtypes;count=0;suspensions=0
 targets=[0x8c646856,0x8c646868,0x8c646b3c,0x8c646982,0x8c646884,0x8c646b36,0x8c66086a]
 for channel,present,flags,allocation_wait,completion_wait,busy,packet,bank in itertools.product(range(8),[0,1],[0,2],[0,2],[0,2],[0,1],[0,1],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x6724f4+channel*4,channel);ram[0xa00000]=present;ram[0xa00004]=packet;struct.pack_into('<I',ram,0xa01000,flags)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];indices=[[0,0],[0,0]]
  def effect(r,t,pr,side):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr)
   if t==0x8c646856:r[0]=0x8ca00000
   elif t==0x8c646868:r[0]=0x8ca01000
   elif t==0x8c646982:r[0]=0xffffffff if indices[side][0]<allocation_wait else 7;indices[side][0]+=1
   elif t==0x8c646884:r[0]=0 if indices[side][1]<completion_wait else 1;indices[side][1]+=1
   elif t==0x8c646b36:r[0]=busy
   else:r[0]=0
   return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in targets:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0],1));return 1 if t==0x8c66086a else 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr,0));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[5]=0x8ca02000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6608a4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in targets},f)
  args=(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
  result=fn(*args);steps=0
  while result==1:
   assert pr.value in [0x8c660906,0x8c66091a,0x8c660932,0x8c660a68]
   steps+=1;assert steps<20;result=resume(*args)
  assert result==0;suspensions+=steps
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  active=present and flags
  assert r[0]==(0xffffff30 if active and busy else 0xffffff01)
  assert indices==([[allocation_wait+1,completion_wait+1]]*2 if active else [[0,0]]*2)
  yields=sum(t[0]==0x8c66086a for t in actual)
  assert yields==(allocation_wait+completion_wait+int(busy) if active else 1)
  count+=1
 report=dict(cases_passed=count,instruction_count=216,suspensions_resumed=suspensions,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_backend_request.c').read_bytes()).hexdigest(),scope='Full retail state/call comparison of absent/unsupported devices, allocation/completion waits, busy status and invalid packet codes across eight channels and both FPU banks. Seven lower services injected, with yield returning a control-transfer status and explicit continuation reentry. CPU context restore integration remains unverified.')
 (out/'analysis/input_backend_resume_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'backend continuation comparisons passed')
if __name__=='__main__':main()
