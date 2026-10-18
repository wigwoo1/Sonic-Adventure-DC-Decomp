import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_mount_sector_read.dll')).sa_file_mount_sector_read;fn.argtypes=abi
acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_reserved_handle.dll')).sa_file_reserved_handle_acquire;acquire.argtypes=abi
cases=0
for count,busy,result,bank in itertools.product([0,1,7],[0,1],[0,0xffffffef],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 for offset,value in [(0x673414,0x8ca01000),(0xa01014,0x8ca02080),(0xa0101c,0x8ca02080 if busy else 0),(0xa02080,0x8ca01000)]:struct.pack_into('<I',ram,offset,value)
 events=[];actual=[]
 def service(regs,log):log.append(tuple(regs[i] for i in range(4,8)));regs[0]=result
 def hook(m,regs,status):service(regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c600d82:return acquire(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert target==0x8c600f1e;service(regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=42;s.r[5]=count;s.r[6]=0x8cb00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffc98
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8c600f1e:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert r[0]==(0xfffffff3 if busy else result)
 assert actual==([] if busy else [(0x8ca02080,count,0x8cb00000,count*2048)])
 if not busy:assert struct.unpack_from('<4I',native,0xa02084)==(0,42,count*2048,count)
 cases+=1
report={'cases_passed':cases,'scope':'Native mount-sector bridge and reserved-handle acquisition with full retail RAM/CPU/FPU/SR/PR equality. Counts0/1/7, owned/free handle, read return0/-17, FPU banks; independent request fields and read arguments. Sector read wrapper hooked in this comparison; no disc/device execution.'}
(root/'analysis/file_mount_sector_read_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'mount sector bridge comparisons passed')
