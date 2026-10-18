import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_directory_parser.dll'));names=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll'))
fn=lib.sa_file_directory_load;fn.argtypes=abi
helpers={0x8c645468:lib.sa_file_directory_record_parse,0x8c64535c:lib.sa_file_directory_bytes_copy,0x8c645442:names.sa_file_name_checksum,0x8c645348:names.sa_file_name_upper}
for helper in helpers.values():helper.argtypes=abi
for address,name,dll in [(0x8c600d82,'sa_file_reserved_handle_acquire','file_reserved_handle'),(0x8c600f1e,'sa_file_read_sectors','file_read_sectors'),(0x8c601084,'sa_file_read_aligned','file_read_aligned'),(0x8c600fac,'sa_file_read_unaligned','file_read_unaligned'),(0x8c600cf0,'sa_file_release_wait','file_release_wait'),(0x8c6013a4,'sa_file_status_get','file_status_get')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;helpers[address]=helper
clock=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll')).sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
bridge=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_mount_sector_read.dll')).sa_file_mount_sector_read;bridge.argtypes=abi;helpers[0x8c5ffc98]=bridge
sectors=[bytearray(2048),bytearray(2048)]
filenames=[b'FIRST.MLT',b'SECOND.MLT',b'THIRD.MLT',b'FOURTH.MLT']
for i,name in enumerate(filenames):
 data=sectors[i//2];o=i%2*64;name+=b';1';data[o]=64;struct.pack_into('<I',data,o+2,100+i);struct.pack_into('<I',data,o+10,4096);data[o+32]=len(name);data[o+33:o+33+len(name)]=name
cases=0
for capacity,cached,fail_at,bank in itertools.product([1,2,3,4,5],[0,1],[0],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<4I',ram,0xb00000,99,capacity,123 if cached else 0,0)
 for o,v in [(0x673414,0x8ca10000),(0xa10014,0x8ca11000),(0xa11000,0x8ca10000),(0xa1000c,0x8ca12000),(0xa1003c,0x8ca12000),(0xa12004,0x8cb10000),(0xa12040,0x8cb20000),(0xa12048,0x8cb30000)]:struct.pack_into('<I',ram,o,v)
 events=[];actual=[]
 def put(m,o,data):
  for i,v in enumerate(data):m[o+i]=v
 def device(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8cb10000:
   index=regs[4]-123;assert index in [0,1] and regs[5]==1 and regs[6]==0x8ca00000
   put(m,0xa00000,sectors[index]);regs[0]=0x1234
  elif target==0x8cb20000:
   put(m,0xa11014,struct.pack('<I',1));put(m,0xa11050,struct.pack('<H',1));put(m,0xa1001c,bytes(4));regs[0]=0
  else:assert target==0x8cb30000;regs[0]=0
 hooks={}
 for target in [0x8cb10000,0x8cb20000,0x8cb30000]:
  def hook(m,regs,status,target=target):device(target,m,regs,events);return status
  hooks[target]=hook
 @Read
 def read(ctx,a,n):assert a==0xffd8000c and n==4;return 0xffffffff
 @Write
 def write(*args):raise AssertionError('unexpected write')
 io=IO(None,read,write)
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target in [0x8cb10000,0x8cb20000,0x8cb30000]:device(target,m,regs,actual);return 0
  args=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8c60260e:return clock(*args,ctypes.byref(io),None,call)
  return helpers[target](*args,None,call)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[5]=123;s.r[6]=0x8cb00000;s.r[7]=4096;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6454fe
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks,mmio={0xffd8000c:0xffffffff})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 read_count=0 if cached else 1 if fail_at==1 or capacity<=2 else 2
 read_failed=not cached and ((fail_at==1) or (fail_at==2 and read_count==2))
 result=0 if cached else 0xffffffef if read_failed else 0xfffffff4 if capacity<4 else 0
 assert r[0]==result and len(actual)==read_count*3
 assert [e[1][0] for e in actual if e[0]==0x8cb10000]==list(range(123,123+read_count))
 assert struct.unpack_from('<I',native,0xb00008)[0]==(0 if result else 123)
 parsed=0 if cached or fail_at==1 else min(capacity,2 if read_failed else 4)
 assert struct.unpack_from('<I',native,0xb00000)[0]==(99 if cached or read_failed else parsed)
 for i,name in enumerate(filenames[:parsed]):
  o=0xb00010+i*44;assert struct.unpack_from('<II',native,o)==(100+i,4096);assert native[o+10:o+11+len(name)]==name+b'\0'
 cases+=1
report={'cases_passed':cases,'scope':'Native directory loader/parser/copy/checksum/uppercase against full retail RAM/CPU/FPU/SR/PR, native mount-sector bridge, handle acquisition, read submission/wait/status/clock with modeled device read/start/poll callbacks. Capacities1..5, cached reuse, successful load and banks. Independent sector progression, partial records, count, result and cache invalidation. Actual disc/device read remains modeled.'}
(root/'analysis/file_directory_load_native_pipeline_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'native pipeline directory comparisons passed')
