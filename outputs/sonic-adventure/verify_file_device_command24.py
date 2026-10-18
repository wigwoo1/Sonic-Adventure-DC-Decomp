import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_command24.dll')).sa_file_device_command24;fn.argtypes=abi
request=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_bios_request.dll')).sa_file_bios_request;request.argtypes=abi
cases=0
for arguments,result,bank in itertools.product([(0,0,0,0),(45166,7,0x8cb00000,1),(0xffffffff,0x80000000,0x12345678,0xffffffff)],[0,0x1234,0xffffffee],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;events=[];actual=[];struct.pack_into('<I',ram,0xbc,0x8cba0000)
 def service(target,m,regs,log):
  if target==0x8cba0000:
   assert regs[6]==0 and regs[7]==0
   o=regs[5]&0xffffff;words=struct.unpack('<4I',bytes(m[o:o+16]));log.append((target,regs[4],words));regs[0]=result
  else:assert target in [0x8c5fc0d8,0x8c5fc336];log.append((target,regs[4],regs[5]));regs[0]=0
 hooks={}
 for target in [0x8cba0000,0x8c5fc0d8,0x8c5fc336]:
  def hook(m,regs,status,target=target):service(target,m,regs,events);return status
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c10d938:return request(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  service(target,m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4:8]=arguments;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6452ea
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert r[0]==result
 assert actual==[(0x8cba0000,0x24,arguments)]
 cases+=1
report={'cases_passed': cases, 'scope': 'Native device command24 formatter and BIOS request wrapper against full retail RAM/CPU/FPU/SR/PR. Zero, representative and signed/high-bit arguments, zero/token/error returns and banks. Independent exact four-word payload, command24, R6/R7 zero and result. BIOS boundary modeled; asynchronous callback delivery and completion state unverified.'}
(root/'analysis/file_device_command24_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'device command24 comparisons passed')
