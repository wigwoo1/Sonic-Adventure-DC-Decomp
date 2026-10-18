import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_disc_info.dll')).sa_file_device_disc_info;fn.argtypes=abi
bios=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_disc_info.dll')).sa_file_bios_disc_info;bios.argtypes=abi
cases=0
for result,bank in itertools.product([0,1,2,0xffffffff,0xffffffef],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xbc,0x8cba0000)
 events=[];actual=[]
 def service(regs,log):log.append(tuple(regs[i] for i in range(4,8)));regs[0]=result
 def hook(m,regs,status):service(regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c10d988:return bios(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert target==0x8cba0000;service(regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6451fc
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cba0000:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert len(actual)==1 and actual[0][0]==0x8ca00000 and actual[0][2:]==(0,4)
 assert r[0]==(0 if result==1 else 1) and r[4]==result
 cases+=1
report={'cases_passed':cases,'scope':'Native disc-info callback and BIOS operation4 wrapper, five BIOS returns and both banks. Full retail RAM/CPU/FPU/SR/PR equality; independent success inversion, output pointer and BIOS dispatch registers. BIOS boundary modeled; output RAM preservation checked when BIOS does not write.'}
(root/'analysis/file_device_disc_info_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'disc-info comparisons passed')
