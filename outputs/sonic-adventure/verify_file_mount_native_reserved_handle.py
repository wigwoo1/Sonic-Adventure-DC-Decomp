import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_mount.dll')).sa_file_mount;fn.argtypes=abi
acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_reserved_handle.dll')).sa_file_reserved_handle_acquire;acquire.argtypes=abi
cases=0
for first,second,bank in itertools.product([0,0x8cb10000],[0,0x8cb20000],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 for offset,value in [(0x673414,0x8ca01000),(0x673418,first),(0x67341c,second),(0xa01014,0x8ca02080),(0xa0101c,0x8ca02080),(0xa02080,0x8ca01000)]:struct.pack_into('<I',ram,offset,value)
 events=[];actual=[]
 def service(target,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  regs[0]=0x8cb10000 if target==0x8c64594c else 0
 hooks={}
 for target in [0x8c64594c]:
  def hook(m,regs,status,target=target):service(target,regs,events);return status
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c600d82:return acquire(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  service(target,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffd28
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert r[0]==0xfffffffe
 assert sum(e[0]==0x8c64594c for e in actual)==int(first==0)+int(second==0)
 cases+=1
report={'cases_passed':cases,'scope':'Native mount no-reserved-handle failure path with retail RAM/CPU/FPU/SR/PR comparison; existing/missing registration slots and FPU banks. Native reserved-handle acquisition rejects a handle already owned by its context; callback registration hooked. Independent -2 result, registration count. Successful mounting and disc/device behavior unverified.'}
(root/'analysis/file_mount_native_reserved_handle_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'mount no-handle comparisons passed')