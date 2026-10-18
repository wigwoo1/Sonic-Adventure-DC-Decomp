import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_g2_transfer_host import Job,Bus,Read as BusRead,Write as BusWrite
from verify_engine_cache_writeback import IO,Read,Write,Cache,Writeback

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 entries={2355523152: ('sa_engine_frame_object_pool_initialize', False), 2355523312: ('sa_engine_frame_queue_initialize', False), 2355521988: ('sa_engine_frame_request_pool_initialize', False), 2355524160: ('sa_engine_frame_device_layout_initialize', False), 2355522356: ('sa_engine_frame_work_initialize', False), 2355377376: ('sa_engine_frame_object_allocate', False), 2355471100: ('sa_engine_frame_debug_log', False), 2349911352: ('sa_engine_record_copy_dispatch', False), 2349911364: ('sa_engine_record_copy_loaded_dispatch', False), 2349911414: ('sa_engine_record_copy32_tail', False), 2349911418: ('sa_engine_record_copy28_tail', False), 2349911438: ('sa_engine_record_copy8_tail', False), 2349912860: ('sa_memory_fill', False), 2355473580: ('sa_engine_error_record', False)}
 funcs={}
 for address,(name,hasio) in entries.items():
  f=getattr(lib,name);f.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];funcs[address]=(f,hasio)
 fn=lib.sa_engine_frame_initialize;fn.argtypes=abi
 cache_fn=ctypes.CDLL(str(ROOT/"work/sonic-adventure/build/engine_cache_writeback_io.dll")).sa_engine_cache_writeback_io
 cache_fn.argtypes=abi[:7]+[ctypes.POINTER(Cache)]+abi[7:]
 g2=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/g2_transfer_host.dll'))
 capture=g2.sa_g2_transfer_capture;capture.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Job)]
 execute=g2.sa_g2_transfer_execute;execute.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),P,P]
 event_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_event_dispatch.dll')).sa_interrupt_event_dispatch;event_dispatch.argtypes=abi
 engine_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll')).sa_engine_context_initialize_native;engine_init.argtypes=abi
 transfer_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialize_host.dll')).sa_engine_transfer_initialize_host;transfer_init.argtypes=abi+[ctypes.POINTER(IO),ctypes.c_uint32]
 acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialized_lifecycle.dll')).sa_engine_transfer_descriptor_acquire;acquire.argtypes=abi
 cases=0
 for bank,fill in itertools.product([0,0x200000],[0,0x55]):
  length=0;channel=0;busy=0
  callback=0x8c64241a;mode=0;bitmap=0
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  data=bytes((i*17+9)&255 for i in range(64));ram[0xa02000:0xa02040]=data
  ram[0x8d3400:0x8d35c0]=bytes([fill])*0x1c0
  struct.pack_into('<7I',ram,0xa00000,0,1,0x8ca02000,length,callback,0x12345678,0xabcdef01)
  mmio={0xa05f688c:0,0xa05f6900:0,0xa05f6920:0xffffffff,**{0xa05f7800+o:0 for o in range(0,0xc0,4)}}
  struct.pack_into('<7I',ram,0xa00040,0,1,0x8ca02080,length,callback,0x12345679,0xabcdef01)
  ram[0xa02080:0xa02100]=bytes((i*7+3)&255 for i in range(128))
  native=bytearray(ram);registers=dict(mmio);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[];callbacks=[]
  @Read
  def read(ctx,a,n):
   if n!=4 or a not in registers:errors.append(('read',a,n));return 0
   return registers[a]
  @Write
  def write(ctx,a,v,n):
   if n!=4 or a not in registers:errors.append(('write',a,v,n));return
   registers[a]=v
  io=IO(None,read,write);trace=[];ref_trace=[]
  @Writeback
  def writeback(ctx,a):trace.append(a);return 0
  cache=Cache(io,writeback)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c10ec40:return acquire(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t==0x8c5fc58a:return cache_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache),None,call)
   if t==0x8c64241a:callbacks.append(r[4]);return 0
   if t not in funcs:errors.append(t);return -9
   function,hasio=funcs[t]
   args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return function(*args,None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65a5dc;s.r[4]=0x8ca00000;s.r[5]=0x1200
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={'macl':0}
  s.pc=0x8c6510a4
  reference(ram,s,fpu=f,special=special)
  assert engine_init(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  s.pc=0x8c10d5a8;s.pr=pr.value=0x8c000100
  reference(ram,s,fpu=f,special=special,mmio=mmio,vbr=0x8ca08000)
  assert transfer_init(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,ctypes.byref(io),0x8ca08000)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  s.pc=0x8c65acc0;s.pr=pr.value=0x8c000100
  reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and registers==mmio
  assert struct.unpack_from('<I',native,0x67c1d0)[0]==0x8c8d3400
  assert struct.unpack_from('<2I',native,0x8d357c)==(0x8c8d3988,64)
  assert struct.unpack_from('<6I',native,0x8d3594)==(0x8c8d4188,64,0,0,0,0)
  assert struct.unpack_from('<2I',native,0x8d3564)==(0x8c8d45a8,256)
  assert struct.unpack_from('<I',native,0x8d35ac)[0]==0x8c7ac7a4
  assert struct.unpack_from('<I',native,0x8a380c)[0]==0x8c7ac7a4
  assert struct.unpack_from('<I',native,0x8d3584)[0]==1
  assert struct.unpack_from('<I',native,0x8d3578)[0]==0x8c8d3988
  assert native[0x8d3400]==9
  cases+=1
 (out/'analysis/engine_frame_initialize_native_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native engine context, transfer startup and full frame initializer including pool/queue/request/layout/work allocation/debug-disabled/copy/acquisition helpers compared to full retail RAM/CPU/FPU/MMIO/MACL. Independent native frame pointer, pool/queue/request counts/bases, allocated work object and channel-0 descriptor/global alias checks across initial context fill and FPU banks. Logging enabled path and physical hardware not covered; no injected frame context.'),indent=2)+'\n')
 print(cases,'native frame startup comparisons passed')
if __name__=='__main__':main()
