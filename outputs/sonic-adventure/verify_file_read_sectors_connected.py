import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_read_sectors.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cleanup=lib.sa_file_read_sectors;cleanup.argtypes=abi
functions={}
for address,name,dll in [(0x8c601084,'sa_file_read_aligned','file_read_aligned'),(0x8c600fac,'sa_file_read_unaligned','file_read_unaligned'),(0x8c600cf0,'sa_file_release_wait','file_release_wait'),(0x8c6013a4,'sa_file_status_get','file_status_get')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
clock=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll')).sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
cases=0
for present,active,busy,position,request,alignment,token,bank in itertools.product([1],[1],[0],[0,8,10],[0,4,20],[0,1],[0x123],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa00000:0xa00060]=bytes((i*13+7)&255 for i in range(96))
 for offset,value in [(0,0x8ca01000),(8,100),(0x10,10),(0x14,position)]:struct.pack_into('<I',ram,0xa00000+offset,value)
 struct.pack_into('<H',ram,0xa0004c,active);struct.pack_into('<H',ram,0xa0004e,3)
 struct.pack_into('<I',ram,0xa0101c,0x8cb00000 if busy else 0);struct.pack_into('<I',ram,0xa0100c,0x8ca02000)
 struct.pack_into('<I',ram,0xa0103c,0x8ca03000);struct.pack_into('<I',ram,0xa03048,0x8cb30000)
 struct.pack_into('<I',ram,0xa02004,0x8cb10000);struct.pack_into('<I',ram,0xa02040,0x8cb20000)
 before=bytes(ram[0xa00000:0xa00060]);native_calls=[];reference_calls=[]
 def put(m,offset,data):
  for i,v in enumerate(data):m[offset+i]=v
 def device(target,m,regs,events):
  events.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8cb10000:
   put(m,regs[6]&0xffffff,bytes((i*7+3)&255 for i in range(regs[5]*2048)));regs[0]=token
  else:
   if target==0x8cb20000:
    count=int.from_bytes(bytes(m[0xa0001c+i] for i in range(4)),'little')
    put(m,0xa00014,struct.pack('<I',position+count));put(m,0xa00050,struct.pack('<H',1));put(m,0xa0101c,bytes(4))
   regs[0]=0
 def read_hook(m,regs,status):device(0x8cb10000,m,regs,reference_calls);return status
 def start_hook(m,regs,status):device(0x8cb20000,m,regs,reference_calls);return status
 def poll_hook(m,regs,status):device(0x8cb30000,m,regs,reference_calls);return status
 @Read
 def read(ctx,address,n):assert address==0xffd8000c and n==4;return 0xffffffff
 @Write
 def write(*args):raise AssertionError('unexpected timer write')
 io=IO(None,read,write)
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target in [0x8cb10000,0x8cb20000,0x8cb30000]:device(target,m,regs,native_calls);return 0
  args=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8c60260e:return clock(*args,ctypes.byref(io),None,call)
  helper=functions.get(target)
  return helper(*args,None,call) if helper else -9
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000 if present else 0;s.r[5]=request;s.r[6]=0x8cb00000+alignment;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600e2c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 s.pc=0x8c600f1e
 reference(ram,s,fpu=f,special=special,hooks={0x8cb10000:read_hook,0x8cb20000:start_hook,0x8cb30000:poll_hook},mmio={0xffd8000c:0xffffffff})
 assert cleanup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 effective=min(request,10-position) if position<10 else 0
 assert r[0]==0
 assert native_calls==reference_calls and [event[0] for event in native_calls]==([0x8cb10000,0x8cb20000,0x8cb30000] if effective else [0x8cb30000])
 assert struct.unpack_from('<I',native,0xa00014)[0]==position+effective
 assert struct.unpack_from('<H',native,0xa00050)[0]==1 and struct.unpack_from('<I',native,0xa0101c)[0]==0
 if effective:
  assert native_calls[0][1]==(100+position,effective,0x8cb00000+alignment,0 if alignment else 3)
  assert native[0xb00000+alignment:0xb00000+alignment+effective*2048]==bytes((i*7+3)&255 for i in range(effective*2048))
 cases+=1
(root/'analysis/file_read_sectors_connected_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native sector-read wrapper, aligned/unaligned submission and release/status/clock helpers against full retail RAM/CPU/FPU/SR/PR/MACL. Explicit device fixtures copy sector bytes and complete at start, then poll; EOF, zero requests, clamping, alignment and banks. Independent destination data/progress/status/call-order assertions. Device internals and delayed retry path not executed.'},indent=2)+'\n')
print(cases,'connected sector read comparisons passed')
