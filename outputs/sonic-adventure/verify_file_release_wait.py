import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_release_wait.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cleanup=lib.sa_file_release_wait;cleanup.argtypes=abi
timers=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll'))
clock=timers.sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
elapsed=timers.sa_file_elapsed_get;elapsed.argtypes=abi
threshold=struct.unpack_from('<I',binary,0x600dec-0x10000)[0]
status_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_status_get.dll')).sa_file_status_get;status_get.argtypes=abi
cases=0
for kind,bank,epoch in itertools.product([0,1,2,3,4,5,0x40,0xffff],[0,0x200000],[0,0xfffffff0]):
 present=active=1;length=0
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 ram[0xa00000:0xa00060]=bytes((i*13+7)&255 for i in range(96))
 struct.pack_into('<I',ram,0xa00000,0x8ca01000);struct.pack_into('<I',ram,0xa0101c,0x8ca00000);struct.pack_into('<I',ram,0xa0000c,length);struct.pack_into('<H',ram,0xa0004c,active);struct.pack_into('<H',ram,0xa00050,kind)
 struct.pack_into('<I',ram,0xa00040,0)
 struct.pack_into('<I',ram,0xa0103c,0x8ca02000);struct.pack_into('<I',ram,0xa02048,0x8cb10000)
 struct.pack_into('<H',ram,0xa00052,0xfff1)
 before=bytes(ram[0xa00000:0xa00060]);reference_ticks=[];native_ticks=[];reference_polls=[];native_polls=[];errors=[]
 def tick(events):
  value=(epoch+len(events)*(threshold+1))&0xffffffff;events.append(value);return value
 def clock_hook(m,regs,status):
  value=tick(reference_ticks);regs[2]=0xffd8000c;regs[3]=(~value)&0xffffffff;regs[0]=value;return status
 def poll_hook(m,regs,status):reference_polls.append(1);regs[0]=0;return status
 @Read
 def read(ctx,address,n):
  if address!=0xffd8000c or n!=4:errors.append((address,n))
  return (~tick(native_ticks))&0xffffffff
 @Write
 def write(*args):errors.append('unexpected write')
 io=IO(None,read,write)
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  arguments=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8cb10000:native_polls.append(1);regs[0]=0;return 0
  if target==0x8c6013a4:return status_get(*arguments,None,call)
  if target==0x8c60260e:return clock(*arguments,ctypes.byref(io),None,call)
  if target==0x8c602618:return elapsed(*arguments,None,call)
  errors.append(hex(target));return -9
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000 if present else 0;s.r[5]=0x8cb00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600e2c
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'macl':0};f=Fpu();nf=Fpu.from_buffer_copy(f)
 f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 s.pc=0x8c600cf0
 reference(ram,s,fpu=f,special=special,hooks={0x8c60260e:clock_hook,0x8cb10000:poll_hook})
 assert cleanup(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 assert native[0xa00000:0xa00060]==before
 expected=(0xffffffe0 if kind==0 else 0 if kind==1 else 0xffffffeb if kind in [2,3,4] else 0xfffffff1 if kind in [5,0x40] else 0xffffffe7)
 assert r[0]==expected and not errors,errors
 assert reference_ticks==native_ticks and len(native_ticks)==(2 if kind in [2,3,4] else 1)
 assert reference_polls==native_polls==[1]
 cases+=1
(root/'analysis/file_release_wait_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native release-wait/status/timer/elapsed helpers compared against full retail RAM/CPU/FPU/SR/PR/MACL. Explicit no-op device polling hook and shared modeled counter sequence. Independent completion0, state0 error-32, pending timeout-21, state5/40 signed stored error and unknown-25. Counter wrap and FPU banks. Device internals and progress-reset path pending.'},indent=2)+'\n')
print(cases,'file release wait comparisons passed')
