import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_registered_callback.dll')).sa_file_registered_callback;fn.argtypes=abi
backend=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_backend_poll.dll')).sa_file_backend_poll;backend.argtypes=abi
completion=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_completion_process.dll')).sa_file_completion_process;completion.argtypes=abi
device=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_status.dll')).sa_file_device_status;device.argtypes=abi
bios=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_bios_status.dll')).sa_file_bios_status;bios.argtypes=abi
startlib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_start.dll'))
start=startlib.sa_file_device_start;start.argtypes=abi
execute=startlib.sa_file_bios_execute;execute.argtypes=abi
cases=0
for gate,result,bank,active,error in itertools.product([0,1,2,0xffffffff],[0,1,2,4,0xffffffff,7,0xffffffef],[0,0x200000],[0,1],[0,6,16,8]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xa00020,gate)
 for o,v in [(0x673414,0x8ca00000),(0xa0000c,0x8ca02000),(0xa02040,0x8c6452d6),(0xa03014,10),(0xa0301c,3),(0xa03018,99),(0xa02010,0x8c6450ea),(0xbc,0x8cba0000),(0xa0303c,0x1234),(0xa0001c,0x8ca03000 if active else 0)]:struct.pack_into('<I',ram,o,v)
 struct.pack_into('<H',ram,0xa03048,1);struct.pack_into('<H',ram,0xa03050,2)
 events=[];actual=[]
 def service(m,regs,log):
  log.append(tuple(regs[i] for i in range(4,8)))
  assert regs[6]==0 and regs[7] in (1,2)
  if regs[7]==2:regs[0]=0
  else:
   dest=regs[5]&0xffffff
   for i,v in enumerate(struct.pack('<IIII',error,0xabc,0x1234,0x5678)):m[dest+i]=v
   regs[0]=result
 def status_hook(m,regs,status):service(m,regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c6005c2:return backend(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c5ffa12:return completion(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c6450ea:return device(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c10d94c:return bios(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c6452d6:return start(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c10d960:return execute(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert target==0x8cba0000;service(m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffcf0
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cba0000:status_hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 mapped={0:0,1:4,2:1,4:0x43,0xffffffff:5}.get(result,result)
 assert r[0]==(1 if gate==1 or not active else mapped)
 assert [event[3] for event in actual]==([] if gate==1 else [2]+([1] if active else []))
 def word(o):return struct.unpack_from('<I',native,o)[0]
 def half(o):return struct.unpack_from('<H',native,o)[0]
 if gate!=1 and active:
  assert actual[1][:2]==(0x1234,0x8cefffa4)
  pending=result in (1,4)
  assert word(0xa0001c)==(0x8ca03000 if pending else 0) and word(0xa0303c)==(0x1234 if pending else 0)
  assert half(0xa03050)==(2 if pending else mapped&255)
  assert word(0xa03014)==(13 if result==2 else 10)
  assert word(0xa03018)==(0x1234 if result in (1,2) else 99)
  if result==0xffffffff:
   mapped_error={0:-27,6:-33,16:-36,8:-26}[error]&0xffff
   assert half(0xa03052)==mapped_error and word(0xa03044)==0xabc
   assert word(0xa00034)==(1 if error==6 else 0)
 assert word(0xa00020)==(0 if gate!=1 and active else gate)
 cases+=1
report={'cases_passed':cases,'scope':'Connected native registered callback/backend poll/device start/BIOS execute/completion/device status/BIOS status. Four gate values, active/absent handles, BIOS states0/1/2/4/-1/unknown, four error classes and both FPU banks. Full retail RAM/CPU/FPU/SR/PR equality; independent BIOS dispatch order, state/progress/token/owner/error/media-change assertions. Only platform BIOS boundary modeled; status3 pending-query callback not covered.'}
(root/'analysis/file_registered_completion_bios_pipeline_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'registered completion BIOS pipeline comparisons passed')
