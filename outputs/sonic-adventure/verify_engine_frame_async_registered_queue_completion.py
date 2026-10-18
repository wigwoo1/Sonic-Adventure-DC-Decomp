import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_interrupt_constructor import IO as InterruptIO,Read as InterruptRead,Write as InterruptWrite
from verify_g2_transfer_host import Job,Bus,Read as BusRead,Write as BusWrite
from verify_engine_cache_writeback import IO,Read,Write,Cache,Writeback

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 entries={0x8c10ebe8:("sa_engine_transfer_complete_io",True),0x8c667080:("sa_engine_queue_push",False),0x8c666f98:("sa_engine_queue_pop",False),0x8c666d10:("sa_engine_queue_consume_context",False),0x8c10f9f4:("sa_engine_transfer_descriptor_initialize",False),0x8c10faa4:("sa_engine_transfer_submit_io",True),0x8c666e06:("sa_engine_transfer_error_count",False),0x8c666c2c:("sa_engine_frame_transfer_begin",False),0x8c668f40:("sa_engine_frame_services",False),0x8c65a518:("sa_engine_object_callback_dispatch",False),0x8c66abfc:("sa_engine_pending_count_get",False),0x8c65a568:("sa_engine_object_state_get",False),0x8c65a5cc:("sa_engine_object_state_set",False),0x8c669088:("sa_engine_frame_transfer_submit",False),0x8c10d770:('sa_engine_transfer_copy_strided_io',True),0x8c10ee9c:('sa_engine_transfer_channels_arm',False),0x8c10fa60:('sa_engine_transfer_channel_arm_io',True),0x8c10eee0:('sa_engine_transfer_ready_wait',False),0x8c10fb48:('sa_engine_transfer_ready_get_io',True),0x8c10ebac:('sa_engine_transfer_fifo_wait',False),0x8c10f9a8:('sa_engine_transfer_fifo_ready_get_io',True),0x8c10ee20:('sa_engine_transfer_channels_release',False),0x8c10f9bc:('sa_engine_transfer_channel_release_io',True),0x8c1101f0:('sa_engine_transfer_busy_get_io',True),0x8c65acac:('sa_engine_error_record',False)}
 funcs={}
 for address,(name,hasio) in entries.items():
  f=getattr(lib,name);f.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];funcs[address]=(f,hasio)
 fn=lib.sa_engine_queue_consume_direct;fn.argtypes=abi
 cache_fn=ctypes.CDLL(str(ROOT/"work/sonic-adventure/build/engine_cache_writeback_io.dll")).sa_engine_cache_writeback_io
 cache_fn.argtypes=abi[:7]+[ctypes.POINTER(Cache)]+abi[7:]
 g2=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/g2_transfer_host.dll'))
 capture=g2.sa_g2_transfer_capture;capture.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Job)]
 execute=g2.sa_g2_transfer_execute;execute.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),P,P]
 event_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_event_dispatch.dll')).sa_interrupt_event_dispatch;event_dispatch.argtypes=abi
 constructor=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_constructor.dll')).sa_interrupt_callback_create;constructor.argtypes=abi[:7]+[ctypes.POINTER(InterruptIO)]+abi[7:]
 cases=0
 for length,channel,busy,bank in itertools.product([0,32,96],[0,1,2,3],[0],[0,0x200000]):
  callback=0x8c64241a;mode=0;bitmap=0
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200);struct.pack_into('<I',ram,0xa00200,0xa05f7800)
  struct.pack_into('<I',ram,0xa002d4,mode)
  for slot in range(4):struct.pack_into('<I',ram,0xa00204+slot*52,slot)
  data=bytes((i*17+9)&255 for i in range(64));ram[0xa02000:0xa02040]=data
  struct.pack_into('<I',ram,0x67c1d0,0x8ca01000)
  struct.pack_into('<7I',ram,0xa00000,0,1,0x8ca02000,length,callback,0x12345678,0xabcdef01)
  mmio={0xa05f688c:0,0xa05f6900:0,0xa05f6920:0x10203040}
  for slot in range(4):
   mmio[0xa05f7818+slot*32]=(bitmap>>slot)&1
   mmio[0xa05f781c+slot*32]=0xabcdef01
  for offset in range(0,64,4):mmio[0xa0801200+offset]=0x55555555
  descriptor=0x8ca00204+channel*52
  struct.pack_into('<6I',ram,0xa01194,0x8ca01400,4,3,3,0,1)
  struct.pack_into('<I',ram,0xa011ac,descriptor)
  mmio[0xa05f7818+channel*32]=busy
  for offset in range(0,28,4):mmio.setdefault(0xa05f7800+channel*32+offset,0x55555555)
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
  @InterruptRead
  def interrupt_read(ctx,a,v):
   if a not in registers:errors.append(('interrupt read',a));return -9
   v[0]=registers[a];return 0
  @InterruptWrite
  def interrupt_write(ctx,a,v):
   if a not in registers:errors.append(('interrupt write',a));return -9
   registers[a]=v;return 0
  interrupt_io=InterruptIO(0x8ca08000,0,None,interrupt_read,interrupt_write)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
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
  reference(ram,s,fpu=f,mmio=mmio,cache_writeback=ref_trace.append)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert r[0]==0 and not callbacks
  assert struct.unpack_from('<I',native,0xa00004)[0]==2
  assert struct.unpack_from('<3I',native,0xa01188)==(1,0,0x8ca00000)
  assert struct.unpack_from('<2I',native,0xa011a0)==(0,0)
  assert struct.unpack_from('<2I',native,0xa01418)==(0,0)
  assert struct.unpack_from('<2I',native,(descriptor&0xffffff)+0x1c)==(0x8c668f40,0x8ca00000)
  assert struct.unpack_from('<I',native,0x8d61a8)[0]==(0x90180 if busy else 0)
  assert struct.unpack_from('<I',native,0x67c350)[0]==busy
  assert trace==ref_trace==([] if busy else [0x0ca02000+i*32 for i in range(length//32)])
  if not busy:
   assert tuple(registers[0xa05f7800+channel*32+o] for o in range(0,28,4))==(0x00801200,0x0ca02000,0x80000000|length,0,5,1,1)
   assert struct.unpack_from('<I',native,(descriptor&0xffffff)+0x2c)[0]==1
  values=(ctypes.c_uint32*8)(*[registers.get(0xa05f7800+channel*32+i*4,0) for i in range(8)])
  job=Job();assert capture(channel,values,ctypes.byref(job))==0
  g2_data=bytearray([0x55]*256);before=bytes(native[0xa02000:0xa02100])
  @BusRead
  def bus_read(ctx,a,buffer,n):
   assert 0x0ca02000<=a and a+n<=0x0ca02100
   for i,v in enumerate(native[a&0xffffff:(a&0xffffff)+n]):buffer[i]=v
   return 0
  @BusWrite
  def bus_write(ctx,a,buffer,n):
   assert 0x00801200<=a and a+n<=0x00801300
   g2_data[a-0x00801200:a-0x00801200+n]=bytes(buffer[:n]);return 0
  bus=Bus(None,bus_read,bus_write);transferred=ctypes.c_uint32();event=ctypes.c_uint32()
  assert execute(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(transferred),ctypes.byref(event))==0
  assert transferred.value==length and event.value==0xa60+channel*32
  assert g2_data==before[:length]+bytes([0x55])*(256-length)
  assert bytes(native[0xa02000:0xa02100])==before
  # Host completion boundary clears the hardware start/busy bit before the handler.
  registers[0xa05f7818+channel*32]=mmio[0xa05f7818+channel*32]=0
  # Free-slot fixture; constructor creates the entry and hardware event mask.
  for memory in (ram,native):
   memory[0x892840:0x894040]=bytes(0x1800)
   for slot in range(256):struct.pack_into('<I',memory,0x892840+slot*16+12,0x80000000)
   struct.pack_into('<I',memory,0x89285c,0)
  s.pc=0x8c64594c;s.pr=pr.value=0x8c000100
  for i,v in enumerate([event.value,0x8c10ebe8,0x58,descriptor],4):s.r[i]=r[i]=v
  reference(ram,s,fpu=f,mmio=mmio,vbr=interrupt_io.vbr)
  assert constructor(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(interrupt_io),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert r[0]==1 and struct.unpack_from('<4I',native,0x892850)==(0x8c10ebe8,0,descriptor,0x80000058)
  assert struct.unpack_from('<I',native,0x893840+(event.value>>4)*4)[0]==0x8c892850
  assert registers[0xa05f6920]==0x10203040|(1<<(15+channel))
  # Submit another object while the first descriptor is active.
  for memory in (ram,native):struct.pack_into('<7I',memory,0xa00040,0,1,0x8ca02000,length,callback,0x12345679,0)
  s.pc=0x8c667080;s.pr=pr.value=0x8c000100
  s.r[4]=r[4]=0x8ca00040;s.r[5]=r[5]=0x1200
  reference(ram,s,fpu=f,mmio=mmio,cache_writeback=ref_trace.append)
  push=funcs[0x8c667080][0]
  assert push(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert struct.unpack_from('<I',native,0xa011a4)[0]==1
  s.pc=0x8c645afa;s.r[4]=r[4]=event.value;s.pr=pr.value=0x8c000100
  reference(ram,s,fpu=f,mmio=mmio,cache_writeback=ref_trace.append)
  complete=event_dispatch
  assert complete(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert struct.unpack_from('<I',native,0xa00004)[0]==1
  assert struct.unpack_from('<I',native,0xa00044)[0]==2
  assert struct.unpack_from('<I',native,0xa011a4)[0]==0
  assert struct.unpack_from('<I',native,0xa01190)[0]==0x8ca00040
  assert struct.unpack_from('<2I',native,(descriptor&0xffffff)+0x1c)==(0x8c668f40,0x8ca00040)
  assert callbacks==[0x12345678]
  # Hardware executes the second transfer that the first completion submitted.
  values=(ctypes.c_uint32*8)(*[registers.get(0xa05f7800+channel*32+i*4,0) for i in range(8)])
  assert capture(channel,values,ctypes.byref(job))==0
  assert execute(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(transferred),ctypes.byref(event))==0
  assert transferred.value==length and event.value==0xa60+channel*32
  assert g2_data==before[:length]+bytes([0x55])*(256-length)
  registers[0xa05f7818+channel*32]=mmio[0xa05f7818+channel*32]=0
  s.pc=0x8c645afa;s.r[4]=r[4]=event.value;s.pr=pr.value=0x8c000100
  reference(ram,s,fpu=f,mmio=mmio,cache_writeback=ref_trace.append)
  assert complete(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert struct.unpack_from('<I',native,0xa00044)[0]==1
  assert struct.unpack_from('<I',native,0xa01190)[0]==0
  assert struct.unpack_from('<I',native,0xa011a4)[0]==0
  assert callbacks==[0x12345678,0x12345679]
  assert registers[0xa05f7800+channel*32+20]==0 and registers[0xa05f6900]==1<<(channel+15)
  assert trace==ref_trace
  cases+=1
 (out/'analysis/engine_frame_async_registered_queue_completion_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Retail RAM/CPU/FPU/MMIO/cache trace comparisons of direct consumer mode 1, queue push/pop and native context consumer descriptor initialization/submission. Independent object busy state, current object, wrapped/drained queue, descriptor frame callback/payload, programmed channel and busy-submit error checks. C host transfer capture/execute copies bytes, then native registered event dispatcher645afa routes10ebe8 completion and executes real frame services and object dispatcher with second object queued via native667080. First completion pops/submits next transfer through native666d10; second host transfer/event completes it. Independent two callbacks, queue drain, descriptor/current object transitions. Independent ready/current-object/counter/completion/acknowledgement and user-callback checks. Context/bus/descriptor fixtures and completion scheduling synthetic; native constructor64594c builds registry node/head and enables event mask from free-slot fixture; dispatcher routes captured channel event through recovered completion. Independent registration handle/node/head/mask. Hardware exception entry, automatic scheduling and full native engine registration order not covered.'),indent=2)+'\n')
 print(cases,'asynchronous frame completion comparisons passed')
if __name__=='__main__':main()


