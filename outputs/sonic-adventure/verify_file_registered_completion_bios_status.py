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
cases=0
for gate,result,bank,active in itertools.product([0,1,2,0xffffffff],[0,0xffffffef],[0,0x200000],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xa00020,gate)
 for o,v in [(0x673414,0x8ca00000),(0xa0000c,0x8ca02000),(0xa02040,0x8cb10000),(0xa02010,0x8c6450ea),(0xbc,0x8cba0000),(0xa0303c,0x1234),(0xa0001c,0x8ca03000 if active else 0)]:struct.pack_into('<I',ram,o,v)
 events=[];actual=[]
 def service(target,regs,log):log.append((target,tuple(regs[i] for i in range(4,8))));regs[0]=result
 def start_hook(m,regs,status):service(0x8cb10000,regs,events);return status
 def status_hook(m,regs,status):service(0x8cba0000,regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c6005c2:return backend(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c5ffa12:return completion(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c6450ea:return device(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if target==0x8c10d94c:return bios(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert target in [0x8cb10000,0x8cba0000];service(target,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffcf0
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cb10000:start_hook,0x8cba0000:status_hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert r[0]==(1 if gate==1 else result)
 assert [event[0] for event in actual]==([] if gate==1 else [0x8cb10000]+([0x8cba0000] if active else []))
 if gate!=1:
  assert actual[0][1]==args
  if active:
   assert actual[1][1][0]==0x1234 and actual[1][1][1]==0x8cefffa4
   assert actual[1][1][2:]==(0,1)
   assert struct.unpack_from('<I',native,0xa0001c)[0]==0 and struct.unpack_from('<I',native,0xa0303c)[0]==0
   assert struct.unpack_from('<H',native,0xa03050)[0]==result&255
 assert struct.unpack_from('<I',native,0xa00020)[0]==(0 if gate!=1 and active else gate)
 cases+=1
report={'cases_passed':cases,'scope':'Connected native registered callback, backend poll and completion processor. Gate0/1/2/ffffffff, active/absent handles, device results0/-17 and both banks. Full retail RAM/CPU/FPU/SR/PR and device event equality; independent callback order, token/output arguments, gate reset and owner/token/state transitions. Native device status formatter and BIOS status wrapper connected; BIOS boundary and device-start remain modeled.'}
(root/'analysis/file_registered_completion_bios_status_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'registered native completion comparisons passed')
