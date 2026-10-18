import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
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
 frame_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
 frame_initialize=frame_lib.sa_engine_frame_initialize;frame_initialize.argtypes=abi
 extra=frame_lib.sa_engine_frame_object_pool_initialize;extra.argtypes=abi;funcs[2355523152]=(extra,False)
 extra=frame_lib.sa_engine_frame_queue_initialize;extra.argtypes=abi;funcs[2355523312]=(extra,False)
 extra=frame_lib.sa_engine_frame_request_pool_initialize;extra.argtypes=abi;funcs[2355521988]=(extra,False)
 extra=frame_lib.sa_engine_frame_device_layout_initialize;extra.argtypes=abi;funcs[2355524160]=(extra,False)
 extra=frame_lib.sa_engine_frame_work_initialize;extra.argtypes=abi;funcs[2355522356]=(extra,False)
 extra=frame_lib.sa_engine_frame_object_allocate;extra.argtypes=abi;funcs[2355377376]=(extra,False)
 extra=frame_lib.sa_engine_frame_debug_log;extra.argtypes=abi;funcs[2355471100]=(extra,False)
 extra=frame_lib.sa_engine_record_copy_dispatch;extra.argtypes=abi;funcs[2349911352]=(extra,False)
 extra=frame_lib.sa_engine_record_copy_loaded_dispatch;extra.argtypes=abi;funcs[2349911364]=(extra,False)
 extra=frame_lib.sa_engine_record_copy32_tail;extra.argtypes=abi;funcs[2349911414]=(extra,False)
 extra=frame_lib.sa_engine_record_copy28_tail;extra.argtypes=abi;funcs[2349911418]=(extra,False)
 extra=frame_lib.sa_engine_record_copy8_tail;extra.argtypes=abi;funcs[2349911438]=(extra,False)
 extra=frame_lib.sa_memory_fill;extra.argtypes=abi;funcs[2349912860]=(extra,False)
 extra=frame_lib.sa_engine_error_record;extra.argtypes=abi;funcs[2355473580]=(extra,False)
 fn=lib.sa_engine_queue_consume_direct;fn.argtypes=abi
 cache_fn=ctypes.CDLL(str(ROOT/"work/sonic-adventure/build/engine_cache_writeback_io.dll")).sa_engine_cache_writeback_io
 cache_fn.argtypes=abi[:7]+[ctypes.POINTER(Cache)]+abi[7:]
 g2=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/g2_transfer_host.dll'))
 capture=g2.sa_g2_transfer_capture;capture.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Job)]
 execute=g2.sa_g2_transfer_execute;execute.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),P,P]
 event_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_event_dispatch.dll')).sa_interrupt_event_dispatch;event_dispatch.argtypes=abi
 engine_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll')).sa_engine_context_initialize_native;engine_init.argtypes=abi
 transfer_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialize_host.dll')).sa_engine_transfer_initialize_host;transfer_init.argtypes=abi+[ctypes.POINTER(IO),ctypes.c_uint32]
 acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialized_lifecycle.dll')).sa_engine_transfer_descriptor_acquire;acquire.argtypes=abi
 mode_set=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_mode_set.dll')).sa_engine_frame_mode_set;mode_set.argtypes=abi
 object_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
 object_configure=object_lib.sa_engine_frame_object_configure;object_configure.argtypes=abi
 object_allocate=frame_lib.sa_engine_frame_object_allocate;object_allocate.argtypes=abi
 object_release=object_lib.sa_engine_frame_object_release;object_release.argtypes=abi
 for address,name in [(0x8c65a5ac,'sa_engine_object_address_set'),(0x8c65a5d4,'sa_engine_object_length_set'),(0x8c65a5b4,'sa_engine_object_callback_set')]:
  setter=getattr(object_lib,name);setter.argtypes=abi;funcs[address]=(setter,False)
 cases=0
 for length,channel,busy,bank,fill in itertools.product([0,32,96],[0],[0],[0,0x200000],[0,0x55]):
  callback=0x8c64241a;mode=0;bitmap=0
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  data=bytes((i*17+9)&255 for i in range(64));ram[0xa02000:0xa02040]=data
  ram[0x8d3400:0x8d35c0]=bytes([fill])*0x1c0
  mmio={0xa05f688c:0,0xa05f6900:0,0xa05f6920:0xffffffff,**{0xa05f7800+o:0 for o in range(0,0xc0,4)}}
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
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65a5dc;s.r[4]=0x8c8d39a8;s.r[5]=0x1200
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
  assert frame_initialize(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and registers==mmio
  assert struct.unpack_from('<I',native,0x67c1d0)[0]==0x8c8d3400
  descriptor=struct.unpack_from('<I',native,0x8d35ac)[0]
  assert descriptor==0x8c7ac7a4
  s.pc=0x8c6436d0;s.r[4]=r[4]=1;s.pr=pr.value=0x8c000100
  reference(ram,s,fpu=f,special=special)
  assert mode_set(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<I',native,0x8d35a8)[0]==1
  objects=[]
  for index in range(2):
   s.pc=0x8c6434e0;s.r[4]=r[4]=0x8ca01900;s.pr=pr.value=0x8c000100
   reference(ram,s,fpu=f,special=special)
   assert object_allocate(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
   assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   object_address=struct.unpack_from('<I',native,0xa01900)[0];objects.append(object_address)
   assert object_address==0x8c8d39a8+index*32
   struct.pack_into('<I',ram,s.r[15]&0xffffff,0x12345678+index);struct.pack_into('<I',native,r[15]&0xffffff,0x12345678+index)
   s.pc=0x8c643624;s.r[4]=r[4]=object_address;s.r[5]=r[5]=0x8ca02000+index*128;s.r[6]=r[6]=length;s.r[7]=r[7]=callback;s.pr=pr.value=0x8c000100
   reference(ram,s,fpu=f,special=special)
   assert object_configure(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
   assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   assert struct.unpack_from('<6I',native,object_address&0xffffff)==(1,0,0x8ca02000+index*128,length,callback,0x12345678+index)
  assert struct.unpack_from('<I',native,0x8d3584)[0]==3
  s.pc=0x8c65a5dc;s.r[4]=r[4]=0x8c8d39a8;s.r[5]=r[5]=0x1200;s.pr=pr.value=0x8c000100
  reference(ram,s,fpu=f,mmio=mmio,cache_writeback=ref_trace.append)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert r[0]==0 and not callbacks
  assert struct.unpack_from('<I',native,0x8d39ac)[0]==2
  assert struct.unpack_from('<3I',native,0x8d3588)==(1,0,0x8c8d39a8)
  assert struct.unpack_from('<2I',native,0x8d35a0)==(1,0)
  assert struct.unpack_from('<2I',native,0x8d4188)==(0,0)
  assert struct.unpack_from('<2I',native,(descriptor&0xffffff)+0x1c)==(0x8c668f40,0x8c8d39a8)
  assert struct.unpack_from('<I',native,0x8d61a8)[0]==(0x90180 if busy else 0)
  assert struct.unpack_from('<I',native,0x67c350)[0]==busy
  assert trace==ref_trace==([] if busy else [0x0ca02000+i*32 for i in range(length//32)])
  if not busy:
   assert tuple(registers[0xa05f7800+channel*32+o] for o in range(0,28,4))==(0x00801200,0x0ca02000,0x80000000|length,0,5,1,1)
   assert struct.unpack_from('<I',native,(descriptor&0xffffff)+0x2c)[0]==1
  s.pc=0x8c65a5dc;s.r[4]=r[4]=0x8c8d39c8;s.r[5]=r[5]=0x1280;s.pr=pr.value=0x8c000100
  reference(ram,s,fpu=f,mmio=mmio,cache_writeback=ref_trace.append)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and registers==mmio
  assert struct.unpack_from('<2I',native,0x8d35a0)==(1,1)
  assert struct.unpack_from('<2I',native,0x8d4190)==(0x8c8d39c8,0x1280)
  for item in range(2):
   values=(ctypes.c_uint32*8)(*[registers.get(0xa05f7800+channel*32+i*4,0) for i in range(8)])
   job=Job();assert capture(channel,values,ctypes.byref(job))==0
   g2_data=bytearray([0x55]*128);source_address=0x0ca02000+item*128;destination=0x00801200+item*128
   before=bytes(native[source_address&0xffffff:(source_address&0xffffff)+128])
   @BusRead
   def bus_read(ctx,a,buffer,n):
    assert source_address<=a and a+n<=source_address+128
    for i,v in enumerate(native[a&0xffffff:(a&0xffffff)+n]):buffer[i]=v
    return 0
   @BusWrite
   def bus_write(ctx,a,buffer,n):
    assert destination<=a and a+n<=destination+128
    g2_data[a-destination:a-destination+n]=bytes(buffer[:n]);return 0
   bus=Bus(None,bus_read,bus_write);transferred=ctypes.c_uint32();event=ctypes.c_uint32()
   assert execute(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(transferred),ctypes.byref(event))==0
   assert transferred.value==length and event.value==0xa60+channel*32
   assert g2_data==before[:length]+bytes([0x55])*(128-length)
   assert bytes(native[source_address&0xffffff:(source_address&0xffffff)+128])==before
   # Host completion boundary clears the hardware start/busy bit before the handler.
   registers[0xa05f7818+channel*32]=mmio[0xa05f7818+channel*32]=0
   s.pc=0x8c645afa;s.r[4]=r[4]=event.value;s.pr=pr.value=0x8c000100
   reference(ram,s,fpu=f,mmio=mmio,cache_writeback=ref_trace.append)
   assert event_dispatch(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
   assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
   assert struct.unpack_from('<I',native,0x8d39ac)[0]==1
   assert struct.unpack_from('<3I',native,0x8d3588)==(2,2*(item+1),0x8c8d39c8 if item==0 else 0)
   assert struct.unpack_from('<2I',native,(descriptor&0xffffff)+0x2c)==((1,0) if item==0 else (0,1))
   assert callbacks==[0x12345678+i for i in range(item+1)]
   assert registers[0xa05f7800+channel*32+20]==(1 if item==0 else 0) and registers[0xa05f6900]==1<<(channel+15)
  assert trace==ref_trace==[0x0ca02000+i*32 for i in range(length//32)]+[0x0ca02080+i*32 for i in range(length//32)]
  assert struct.unpack_from("<2I",native,0x8d35a0)==(2,0)
  assert struct.unpack_from("<2I",native,0x8d4190)==(0,0)
  assert struct.unpack_from("<I",native,0x8d39cc)[0]==1
  for index,object_address in enumerate(objects):
   object_offset=object_address&0xffffff;retained=bytes(native[object_offset+4:object_offset+32])
   s.pc=0x8c6435bc;s.r[4]=r[4]=object_address;s.pr=pr.value=0x8c000100
   reference(ram,s,fpu=f,special=special)
   assert object_release(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
   assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
   assert r[0]==0 and struct.unpack_from('<I',native,object_offset)[0]==0
   assert bytes(native[object_offset+4:object_offset+32])==retained
   assert struct.unpack_from('<I',native,0x8d3584)[0]==2-index
  cases+=1
 (out/'analysis/engine_frame_native_object_continuation_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native engine context and transfer startup plus descriptor acquisition create transfer table, descriptors and real event registrations. Mode-1 direct consumer submits first object and queues second; C host copies both, event dispatcher reaches registered native completion, and real frame services start next transfer then drain queue. Full retail CPU/RAM/FPU/MMIO comparisons at startup/acquisition/submission/completion boundaries plus independent data, source/tail, callback payload, queue, object state, counters and cache traces. Native full frame startup creates context/queues/request pool/device layout/work object/channel-0 descriptor; asynchronous mode selected through recovered setter afterward. Object allocation, configuration and post-completion release execute natively; caller supplies source bytes, lengths and callback arguments. Bus inputs remain fixtures; host scheduler clears busy bit and supplies event code. Physical exception entry and production runtime not covered.'),indent=2)+'\n')
 print(cases,'native-object lifecycle comparisons passed')
if __name__=='__main__':main()
