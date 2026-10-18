import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_mount.dll')).sa_file_mount;fn.argtypes=abi
acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_reserved_handle.dll')).sa_file_reserved_handle_acquire;acquire.argtypes=abi
functions={0x8c600d82:acquire}
for address,name,dll in [(0x8c600cf0,'sa_file_release_wait','file_release_wait'),(0x8c6013a4,'sa_file_status_get','file_status_get'),(0x8c602618,'sa_file_elapsed_get','file_timer_helpers')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
clock=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll')).sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
for address,name,dll in [(2355100824, 'sa_file_mount_sector_read', 'file_mount_sector_read'), (2355105566, 'sa_file_read_sectors', 'file_read_sectors'), (2355105924, 'sa_file_read_aligned', 'file_read_aligned'), (2355105708, 'sa_file_read_unaligned', 'file_read_unaligned'), (2355100134, 'sa_file_volume_signature_check', 'file_volume_helpers')]:
 helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper
cases=0
for first,second,bank,scenario in itertools.product([0,0x8cb10000],[0,0x8cb20000],[0,0x200000],[0,1,2,3,4]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 for offset,value in [(0x673414,0x8ca01000),(0x673418,first),(0x67341c,second),(0xa01014,0x8ca02080),(0xa0101c,0),(0xa02080,0x8ca01000),(0xa0100c,0x8cb03000),(0xa0103c,0x8cb03000),(0xb0301c,0x8cb30000),(0xb03020,0x8cb40000),(0xb03048,0x8cb50000),(0xb0304c,0x8cb60000),(0xb03004,0x8cb70000),(0xb03040,0x8cb80000)]:struct.pack_into('<I',ram,offset,value)
 volume=bytearray(2048);volume[1:6]=b'CD001';volume[1+scenario]^=0x80
 events=[];actual=[]
 def put(m,o,data):
  for i,v in enumerate(data):m[o+i]=v
 def service(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8c64594c:regs[0]=0x8cb10000
  elif target==0x8cb30000:regs[0]=0x1234
  elif target==0x8cb50000 or target==0x8cb80000:
   put(m,0xa020d0,struct.pack('<H',1));put(m,0xa0101c,bytes(4));regs[0]=0
  elif target==0x8cb40000:
   put(m,regs[4]&0xffffff,struct.pack('<II',3,128));regs[0]=1
  elif target==0x8cb60000:regs[0]=0
  elif target==0x8cb70000:
   assert regs[5]==1 and regs[6]==0x8ca01040
   put(m,0xa01040,volume);regs[0]=0x1234
  else:raise AssertionError(hex(target))
 hooks={}
 for target in [0x8c64594c,0x8cb30000,0x8cb40000,0x8cb50000,0x8cb60000,0x8cb70000,0x8cb80000]:
  def hook(m,regs,status,target=target):service(target,m,regs,events);return status
  hooks[target]=hook
 def clock_hook(m,regs,status):regs[2]=0xffd8000c;regs[3]=0xffffffff;regs[0]=0;return status
 hooks[0x8c60260e]=clock_hook
 @Read
 def read(ctx,a,n):assert a==0xffd8000c and n==4;return 0xffffffff
 @Write
 def write(*args):raise AssertionError('unexpected write')
 io=IO(None,read,write)
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  args=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8c60260e:return clock(*args,ctypes.byref(io),None,call)
  if target in functions:return functions[target](*args,None,call)
  service(target,m,regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffd28
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert r[0]==0xfffffffb
 assert bytes(native[0xa01040:0xa01840])==bytes(volume)
 assert struct.unpack_from('<H',native,0xa01038)[0]==0
 assert [e[0] for e in actual if e[0]!=0x8c64594c]==[0x8cb30000,0x8cb50000,0x8cb40000,0x8cb60000,0x8cb70000,0x8cb80000,0x8cb50000]
 assert sum(e[0]==0x8c64594c for e in actual)==int(first==0)+int(second==0)
 cases+=1
report={'cases_passed':cases,'scope':'Native mount through successful first disc request and native volume sector read pipeline to signature rejection, full retail RAM/CPU/FPU/SR/PR equality. Device and registration callbacks modeled; all five CD001 bytes individually corrupted, registration slot variants and FPU banks. Independent -5 result, unmounted flag, exact volume bytes and device order. Successful directory mounting pending.'}
(root/'analysis/file_mount_native_volume_rejection_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'mount native volume rejection comparisons passed')