import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_request.dll')).sa_input_device_request
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];resume=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_request.dll')).sa_input_device_request_resume;resume.argtypes=fn.argtypes;count=0;transfers=0
 sequences=[[0],[1,0],[0xffffff30,0],[0xffffff01]]
 for channel,limit,flag,seq,bank in itertools.product(range(8),[0,1,3],[0,1],sequences,[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;record=0x89c550+channel*0x148;ram[record:record+0x148]=bytes(0x148)
  struct.pack_into('<2I',ram,record+0x30,4,11);struct.pack_into('<I',ram,0x894114,limit);struct.pack_into('<I',ram,0x66d3a0,flag);struct.pack_into('<I',ram,0x894138+channel*128+8,18)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];indices=[0,0]
  def effect(r,t,pr,side):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr)
   if t==0x8c6608a4:r[0]=seq[min(indices[side],len(seq)-1)];indices[side]+=1
   else:r[0]=0
   return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in [0x8c6608a4,0x8c647bde]:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0],1));return 1
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr,0));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[5]=0x8ca01000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c64753c
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,{t:hook(t) for t in [0x8c6608a4,0x8c647bde]},f)
  args=(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
  result=fn(*args);steps=0
  while result==1:
   assert pr.value in [0x8c64756e,0x8c6475bc];steps+=1;assert steps<20;result=resume(*args)
  assert result==0;transfers+=steps
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  attempts=0;failures=0;remaining=limit
  while True:
   value=seq[min(attempts,len(seq)-1)];attempts+=1
   if value==0 or (flag and value==0xffffff30):break
   failures+=1
   if remaining==0:break
   remaining-=1
  assert indices==[attempts,attempts] and struct.unpack_from('<I',native,record+0x34)[0]==11+failures
  callback=not(flag and value==0xffffff30)
  assert len(actual)==attempts+int(callback) and r[0]==(0 if value==0 else 0xffffff01 if value in [0xffffff01,0xffffff30] and (value==0xffffff01 or flag) else 0xffff0000)
  if callback:assert actual[-1][1]==(channel,18,4,11+failures)
  count+=1
 report=dict(cases_passed=count,instruction_count=88,call_returns_resumed=transfers,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_device_request.c').read_bytes()).hexdigest(),scope='Eight channels, retry limits, special-error flag, success/error response sequences and both FPU banks. Full retail state/call comparison plus independent retry counts, counters, callback arguments and mapped results. Two lower services injected with transfer status and explicit native caller continuation reentry; actual context restoration through callers unfinished.')
 (out/'analysis/input_device_request_resume_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'device request caller continuation comparisons passed')
if __name__=='__main__':main()
