import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_read.dll')).sa_file_device_read;fn.argtypes=abi
cases=0
for mode,dest,result,count,bank in itertools.product([0,1,3],[0x8cb00000,0x8cb00001,0xa0b00000],[0,0x1234],[0,7],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;events=[];actual=[]
 def service(target,m,regs,log):
  if target==0x8c10d938:
   o=regs[5]&0xffffff;words=struct.unpack('<4I',bytes(m[o:o+16]));log.append((target,regs[4],words));regs[0]=result
  else:assert target in [0x8c5fc0d8,0x8c5fc336];log.append((target,regs[4],regs[5]));regs[0]=0
 hooks={}
 for target in [0x8c10d938,0x8c5fc0d8,0x8c5fc336]:
  def hook(m,regs,status,target=target):service(target,m,regs,events);return status
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,status,regs,pr):service(target,m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=45166;s.r[5]=count;s.r[6]=dest;s.r[7]=mode;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c644ff6
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert r[0]==(0xffffffee if mode==3 else result)
 if mode==3:assert not actual
 else:
  address=(dest&0x1fffffff)|(0xa0000000 if mode==0 else 0)
  assert actual[0]==(0x8c10d938,0x10+mode,(45166,count,address,0))
  cached=result!=0 and dest>>28!=0xa
  assert len(actual)==1+int(cached)
  if cached:assert actual[1]==(0x8c5fc336 if dest&31 else 0x8c5fc0d8,dest,count*2048)
 cases+=1
report={'cases_passed':cases,'scope':'Native device read request formatter with full retail RAM/CPU/FPU/SR/PR equality. Modes0/1/3, cached/uncached and unaligned buffers, count0/7, rejected/accepted requests and banks. Independent command ID, request block, destination alias, error and cache-helper selection. BIOS request and cache helper bodies modeled; no device data transfer.'}
(root/'analysis/file_device_read_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'device read comparisons passed')
