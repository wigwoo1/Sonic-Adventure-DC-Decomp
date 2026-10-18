import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_status.dll')).sa_file_device_status;fn.argtypes=abi
bios=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_bios_status.dll')).sa_file_bios_status;bios.argtypes=abi
cases=0
for result,error,bank in itertools.product([0,1,2,3,4,0xffffffff,7,0xffffffef],[0,1,2,3,4,5,6,7,11,16,8,0xffffffff],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa04000:0xa0400c]=bytes([0xaa])*12
 struct.pack_into('<I',ram,0xbc,0x8cba0000)
 events=[];actual=[]
 def service(m,regs,log):
  log.append(tuple(regs[i] for i in range(4,8)))
  dest=regs[5]&0xffffff
  for i,v in enumerate(struct.pack('<IIII',error,0xabcdef,0x1234,0x5678)):m[dest+i]=v
  regs[0]=result
 def hook(m,regs,status):service(m,regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c10d94c:return bios(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert target==0x8cba0000;service(m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x1234;s.r[5]=0x8ca04000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6450ea
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cba0000:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 expected={0:0,1:4,2:1,3:0x41,4:0x43,0xffffffff:5}.get(result,result)
 assert len(actual)==1 and actual[0][:2]==(0x1234,0x8cefffe8) and actual[0][2:]==(0,1) and r[0]==expected
 out=list(struct.unpack_from('<III',native,0xa04000));sentinel=0xaaaaaaaa
 if result==1:assert out==[0x1234,sentinel,0x5678]
 elif result==2:assert out==[0x1234,sentinel,sentinel]
 elif result==0xffffffff:
  mapped={0:-27,1:-28,2:-29,3:-30,4:-31,5:-32,6:-33,7:-34,11:-35,16:-36}.get(error,-26)&0xffffffff
  assert out==[mapped,0xabcdef,sentinel]
 else:assert out==[sentinel]*3
 cases+=1
report={'cases_passed':cases,'scope':'Native device-status formatter against full retail RAM/CPU/FPU/SR/PR and callback equality. BIOS states0/1/2/3/4/-1/unknown, ten mapped errors plus unknown errors and both banks. Independent status mapping, partial output writes, untouched fields and request/output arguments. Native BIOS status wrapper10d94c executes; BIOS pointer dispatch modeled, R6=0/R7=1 independently asserted.'}
(root/'analysis/file_device_status_bios_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'device status comparisons passed')
