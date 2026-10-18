import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_interrupt_constructor import IO as InterruptIO,Read as InterruptRead,Write as InterruptWrite
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
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_load_initialize.dll')).sa_sound_driver_load_initialize;fn.argtypes=abi
 for a,name,dll in [(0x8c6436d0,'sa_sound_driver_mode_set','sound_driver_mode_set'),(0x8c643f6c,'sa_sound_initialize_wrapper','sound_initialize_wrapper'),(0x8c65acc0,'sa_engine_frame_initialize','engine_frame_initialize_native'),(0x8c643624,'sa_engine_frame_object_configure','engine_frame_object_configure'),(0x8c6435bc,'sa_engine_frame_object_release','engine_frame_object_configure'),(0x8c65a568,'sa_engine_object_state_get','engine_frame_object_configure'),(0x8c65a5cc,'sa_engine_object_state_set','engine_frame_object_configure'),(0x8c65a5ac,'sa_engine_object_address_set','engine_frame_object_configure'),(0x8c65a5d4,'sa_engine_object_length_set','engine_frame_object_configure'),(0x8c65a5b4,'sa_engine_object_callback_set','engine_frame_object_configure')]:
  h=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);h.argtypes=abi;funcs[a]=(h,False)
 cache_fn=ctypes.CDLL(str(ROOT/"work/sonic-adventure/build/engine_cache_writeback_io.dll")).sa_engine_cache_writeback_io
 cache_fn.argtypes=abi[:7]+[ctypes.POINTER(Cache)]+abi[7:]
 g2=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/g2_transfer_host.dll'))
 capture=g2.sa_g2_transfer_capture;capture.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Job)]
 execute=g2.sa_g2_transfer_execute;execute.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),P,P]
 event_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_event_dispatch.dll')).sa_interrupt_event_dispatch;event_dispatch.argtypes=abi
 engine_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll')).sa_engine_context_initialize_native;engine_init.argtypes=abi
 transfer_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialize_host.dll')).sa_engine_transfer_initialize_host;transfer_init.argtypes=abi+[ctypes.POINTER(IO),ctypes.c_uint32]
 acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialized_lifecycle.dll')).sa_engine_transfer_descriptor_acquire;acquire.argtypes=abi
 callback_register=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_callback_register.dll')).sa_setup_callback_register;callback_register.argtypes=abi;funcs[0x8c605234]=(callback_register,False)
 handler=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/task_delete_native.dll')).sa_interrupt_handler_primary_native;handler.argtypes=abi;funcs[0x8c64135a]=(handler,False)
 interrupt_functions={}
 for a,name,dll in [(0x8c64594c,'sa_interrupt_callback_create','interrupt_constructor'),(0x8c645c54,'sa_interrupt_callback_remove','interrupt_remove')]:
  h=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);h.argtypes=abi[:7]+[ctypes.POINTER(InterruptIO)]+abi[7:];interrupt_functions[a]=h

 upload_helpers={}
 pool_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
 for a,name in [(0x8c6434e0,'sa_engine_frame_object_allocate'),(0x8c65a2fc,'sa_engine_frame_debug_log'),(0x8c10cd38,'sa_engine_record_copy_dispatch'),(0x8c10cd44,'sa_engine_record_copy_loaded_dispatch'),(0x8c10cd76,'sa_engine_record_copy32_tail'),(0x8c10cd7a,'sa_engine_record_copy28_tail'),(0x8c10cd8e,'sa_engine_record_copy8_tail')]:
  upload_helpers[a]=getattr(pool_lib,name);upload_helpers[a].argtypes=abi
 queue_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_consume_direct_connected.dll'))
 entries={0x8c666c2c:("sa_engine_frame_transfer_begin",False),0x8c668f40:("sa_engine_frame_services",False),0x8c65a518:("sa_engine_object_callback_dispatch",False),0x8c66abfc:("sa_engine_pending_count_get",False),0x8c65a568:("sa_engine_object_state_get",False),0x8c65a5cc:("sa_engine_object_state_set",False),0x8c669088:("sa_engine_frame_transfer_submit",False),0x8c10d770:('sa_engine_transfer_copy_strided_io',True),0x8c10ee9c:('sa_engine_transfer_channels_arm',False),0x8c10fa60:('sa_engine_transfer_channel_arm_io',True),0x8c10eee0:('sa_engine_transfer_ready_wait',False),0x8c10fb48:('sa_engine_transfer_ready_get_io',True),0x8c10ebac:('sa_engine_transfer_fifo_wait',False),0x8c10f9a8:('sa_engine_transfer_fifo_ready_get_io',True),0x8c10ee20:('sa_engine_transfer_channels_release',False),0x8c10f9bc:('sa_engine_transfer_channel_release_io',True),0x8c1101f0:('sa_engine_transfer_busy_get_io',True),0x8c65acac:('sa_engine_error_record',False)}
 upload_io_helpers={}
 for a,(name,hasio) in entries.items():
  helper=getattr(queue_lib,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:]
  if hasio:upload_io_helpers[a]=helper
  else:upload_helpers[a]=helper
 upload_helpers[0x8c65a5dc]=queue_lib.sa_engine_queue_consume_direct;upload_helpers[0x8c65a5dc].argtypes=abi
 for a,name,dll in [(0x8c65a200,'sa_sound_driver_stop','sound_driver_stop'),(0x8c65a27c,'sa_sound_driver_start','sound_driver_start')]:
  upload_helpers[a]=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);upload_helpers[a].argtypes=abi
 reader=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_read_strided_io.dll')).sa_engine_transfer_read_strided_io;reader.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:];upload_io_helpers[0x8c10d460]=reader
 clear=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_memory_clear.dll')).sa_sound_driver_memory_clear;clear.argtypes=abi;upload_helpers[0x8c65a8c8]=clear
 for a,name,dll in [(0x8c65aaf4,'sa_sound_driver_info','sound_driver_info'),(0x8c667188,'sa_sound_driver_byte_get','sound_driver_getters'),(0x8c667150,'sa_sound_driver_word_get','sound_driver_getters'),(0x8c66910c,'sa_device_word_read','device_word_read')]:
  upload_helpers[a]=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);upload_helpers[a].argtypes=abi
 upload=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_transfer.dll')).sa_sound_driver_transfer;upload.argtypes=abi;funcs[0x8c643e6c]=(upload,False)
 for a,h in upload_helpers.items():funcs[a]=(h,False)
 for a,h in upload_io_helpers.items():funcs[a]=(h,True)
 async_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
 for a,(name,hasio) in {2349919208: ('sa_engine_transfer_complete_io', True), 2355523712: ('sa_engine_queue_push', False), 2355523480: ('sa_engine_queue_pop', False), 2355522832: ('sa_engine_queue_consume_context', False), 2349922804: ('sa_engine_transfer_descriptor_initialize', False), 2349922980: ('sa_engine_transfer_submit_io', True), 2355523078: ('sa_engine_transfer_error_count', False), 2355522604: ('sa_engine_frame_transfer_begin', False), 2355531584: ('sa_engine_frame_services', False), 2355471640: ('sa_engine_object_callback_dispatch', False), 2355538940: ('sa_engine_pending_count_get', False), 2355471720: ('sa_engine_object_state_get', False), 2355471820: ('sa_engine_object_state_set', False), 2355531912: ('sa_engine_frame_transfer_submit', False), 2349913968: ('sa_engine_transfer_copy_strided_io', True), 2349919900: ('sa_engine_transfer_channels_arm', False), 2349922912: ('sa_engine_transfer_channel_arm_io', True), 2349919968: ('sa_engine_transfer_ready_wait', False), 2349923144: ('sa_engine_transfer_ready_get_io', True), 2349919148: ('sa_engine_transfer_fifo_wait', False), 2349922728: ('sa_engine_transfer_fifo_ready_get_io', True), 2349919776: ('sa_engine_transfer_channels_release', False), 2349922748: ('sa_engine_transfer_channel_release_io', True), 2349924848: ('sa_engine_transfer_busy_get_io', True), 2355473580: ('sa_engine_error_record', False)}.items():
  h=getattr(async_lib,name);h.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];funcs[a]=(h,hasio)
 event_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_event_dispatch.dll')).sa_interrupt_event_dispatch;event_dispatch.argtypes=abi;funcs[0x8c645afa]=(event_dispatch,False)
 driver=(ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV').read_bytes()
 payload=driver[32:]
 cases=0
 for bank,fill,size,exhausted,prior in [(0,0,33408,0,0)]: 
  length=0;channel=0;busy=0
  callback=0x8c64241a;mode=0;bitmap=0
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  data=bytes((i*17+9)&255 for i in range(64));ram[0xa02000:0xa02040]=data
  ram[0x8d3400:0x8d35c0]=bytes([fill])*0x1c0
  struct.pack_into('<7I',ram,0xa00000,0,1,0x8ca02000,length,callback,0x12345678,0xabcdef01)
  mmio={0xa05f688c:0,0xa05f6900:0,0xa05f6920:0xffffffff,**{0xa05f7800+o:0 for o in range(0,0xc0,4)}}
  struct.pack_into('<7I',ram,0xa00040,0,1,0x8ca02080,length,callback,0x12345679,0xabcdef01)
  ram[0xa02080:0xa02100]=bytes((i*7+3)&255 for i in range(128))
  struct.pack_into('<I',ram,0x67c1c4,0)
  mmio[0xa0702c00]=0xabcdef01
  mmio.update({0xa0800000+i:0x55555555 for i in range(-4,0x200004,4)})
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
  actual=[];expected=[]
  modeled=[0x8c6016d6,0x8c600e2c,0x8c600f1e,0x8c600df4]
  def service(t,m,r,events):
   events.append((t,tuple(r[i] for i in range(4,8))))
   if t==0x8c6016d6:r[0]=0x8cb00000
   elif t==0x8c600e2c:
    for i in range(4):m[(r[5]&0xffffff)+i]=(size>>(8*i))&255
    r[0]=0
   elif t==0x8c600f1e:
    for i,v in enumerate(driver):m[0xd00000+i]=v
    r[0]=0
   else:r[0]=0
  hooks={}
  for t in modeled:
   def hook(m,r,sr,t=t):service(t,m,r,expected);return sr
   hooks[t]=hook
  scheduled=[];reference_scheduled=[]
  def hardware_transfer(memory,hardware,events):
   active=[c for c in range(4) if hardware.get(0xa05f7818+c*32,0)&1]
   assert len(active)==1,active
   c=active[0];base=0xa05f7800+c*32
   values=(ctypes.c_uint32*8)(*[hardware.get(base+i*4,0) for i in range(8)])
   job=Job();assert capture(c,values,ctypes.byref(job))==0
   @BusRead
   def bus_read(ctx,a,buffer,n):
    for i in range(n):buffer[i]=memory[(a&0xffffff)+i]
    return 0
   @BusWrite
   def bus_write(ctx,a,buffer,n):
    assert n%4==0
    for i in range(0,n,4):hardware[0xa0000000+a+i]=sum(buffer[i+j]<<(8*j) for j in range(4))
    return 0
   bus=Bus(None,bus_read,bus_write);amount=ctypes.c_uint32();event=ctypes.c_uint32()
   assert execute(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(amount),ctypes.byref(event))==0
   events.append((c,amount.value,event.value))
   hardware[base+24]=0
   context=sum(memory[0x67c1d0+i]<<(8*i) for i in range(4))&0xffffff
   return sum(memory[context+0x1ac+i]<<(8*i) for i in range(4))
  def reference_wait(m,regs,status):
   descriptor=hardware_transfer(m,mmio,reference_scheduled)
   irq=State();irq.r[:]=regs;irq.r[4]=reference_scheduled[-1][2];irq.sr=status;irq.pc=0x8c645afa;irq.pr=0x8c000100
   reference(m,irq,fpu=Fpu.from_buffer_copy(f),mmio=mmio,cache_writeback=ref_trace.append)
   return status
  hooks[0x8c666e02]=reference_wait
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
   if t==0x8c666e02:
    descriptor=hardware_transfer(m,registers,scheduled)
    ir=(ctypes.c_uint32*16)(*[r[i] for i in range(16)]);ir[4]=scheduled[-1][2]
    irq_sr=ctypes.c_uint32(sr[0]);irq_pr=ctypes.c_uint32(0x8c000100);irq_f=Fpu.from_buffer_copy(f.contents)
    return event_dispatch(m,ctypes.byref(irq_f),ctypes.byref(irq_sr),ir,ctypes.byref(irq_pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t in interrupt_functions:
    if t==0x8c64594c and sum(m[0x8900fc+i]<<(8*i) for i in range(4))!=0:errors.append('uncleared registration handle')
    return interrupt_functions[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(interrupt_io),None,call)
   if t in modeled:service(t,m,r,actual);return 0
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
  # Explicit callback fixture after native engine/transfer prerequisites.
  for memory in (ram,native):
   struct.pack_into('<I',memory,0x8900fc,prior)
   memory[0x892840:0x894040]=bytes(0x1800)
   for i in range(256):struct.pack_into('<I',memory,0x892840+16*i+12,0x80000000)
   if not exhausted:
    struct.pack_into('<I',memory,0x89285c,0);struct.pack_into('<I',memory,0x89286c,0)
   if prior:
    struct.pack_into('<4I',memory,0x892860,0x8c010000,0,0xabcdef01,0x80000020)
    struct.pack_into('<I',memory,0x893a80,0x8c892860)
  # Register modeled hardware channel0 completion through the retail constructor.
  s.pc=0x8c64594c;s.pr=pr.value=0x8c000100
  for i,v in enumerate([0xa60,0x8c10ebe8,0x58,0x8c7ac7a4],4):s.r[i]=r[i]=v
  reference(ram,s,fpu=f,mmio=mmio,vbr=interrupt_io.vbr)
  assert interrupt_functions[0x8c64594c](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(interrupt_io),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio
  assert r[0]==1 and struct.unpack_from('<4I',native,0x892850)==(0x8c10ebe8,0,0x8c7ac7a4,0x80000058)
  assert struct.unpack_from('<I',native,0x893ad8)[0]==0x8c892850
  table_base=struct.unpack_from('<I',ram,0x67c004)[0]&0xffffff
  table_before=bytes(ram[table_base+0x238:table_base+0x240])
  s.pc=0x8c093e60;s.pr=pr.value=0x8c000100
  try:reference(ram,s,hooks=hooks,fpu=f,special=special,vbr=interrupt_io.vbr,mmio=mmio,instruction_budget=120000000,cache_writeback=ref_trace.append)
  except AssertionError:
   print('reference stop',hex(s.pc),[hex(x) for x in s.r]);raise
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'] and registers==mmio
  assert struct.unpack_from('<I',native,0x67c1d0)[0]==0x8c8d3400
  assert struct.unpack_from('<2I',native,0x8d357c)==(0x8c8d3988,64)
  assert struct.unpack_from('<6I',native,0x8d3594)==(0x8c8d4188,64,2,2,0,1)
  assert struct.unpack_from('<2I',native,0x8d3564)==(0x8c8d45a8,256)
  assert struct.unpack_from('<I',native,0x8d35ac)[0]==0x8c7ac7a4
  assert struct.unpack_from('<I',native,0x8a380c)[0]==0x8c7ac7a4
  assert struct.unpack_from('<I',native,0x8d3584)[0]==1
  assert struct.unpack_from('<I',native,0x8d3578)[0]==0x8c8d3988
  assert native[0x8d3400]==9
  assert actual==expected and [e[0] for e in actual]==modeled
  assert actual[2][1][:3]==(0x8cb00000,(size+2047)>>11,0x0cd00000)
  created=2
  assert struct.unpack_from('<I',native,0x8900fc)[0]==created
  if created:
   assert struct.unpack_from('<4I',native,0x892840+16*created)==(0x8c051810,0,0,0x80000058)
   assert struct.unpack_from('<I',native,0x893a80)[0]==0x8c892840+16*created
   assert struct.unpack_from('<2I',native,table_base+0x238)==(0x8c645b1e,0)
  else:assert native[table_base+0x238:table_base+0x240]==table_before
  if prior and created!=2:assert struct.unpack_from('<I',native,0x89286c)[0]==0
  assert struct.unpack_from('<I',native,0x8d39a8)[0]==0
  assert struct.unpack_from('<4I',native,0x8d39ac)==(1,0x0cd00020,len(payload),0xffffffff)
  offset=struct.unpack_from('<h',binary,0x6436e0-0x10000)[0]
  assert struct.unpack_from('<I',native,0x8d3400+offset)[0]==1
  assert b''.join(struct.pack('<I',registers[0xa0800000+i]) for i in range(0,len(payload),4))==payload
  assert all(registers[0xa0800000+i]==0 for i in range(len(payload),0x200000,4))
  assert registers[0xa07ffffc]==registers[0xa0a00000]==0x55555555
  assert struct.unpack_from('<4I',native,0x892850)==(0x8c10ebe8,0,0x8c7ac7a4,0x80000058)
  assert scheduled==reference_scheduled==[(0,len(payload),0xa60)]*2
  assert trace==ref_trace
  cases+=1
 (out/'analysis/sound_driver_load_initialize_registered_upload_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Driver loader093e60 now calls native wrapper643f6c/initializer65acc0, object allocation/configuration/release. Native field setter6436d0 connected; context field independently checked as1. Callback registration605234, constructor64594c, removal645c54 and low handler install/reset native. One free-pool/no-prior fixture; independent driver handle2 and completion handle1, callback node, list head and actual native engine handler table checked. Driver upload, stop/start, memory clear, driver info/getters, queue/G2/channel helpers native. File operations modeled, read copies extracted retail MASARU.DRV to caller requested0cd00000. Exact uploaded33376-byte payload and remaining2MiB zero/guards checked; original caller invokes upload twice including after original object release. One combined success fixture; wait666e02 supplies synthetic hardware transfer/completion interrupt at the wait boundary, executes G2 host capture/execute then native645afa registry dispatch and recovered completion10ebe8 on preserved foreground register state; two uploads checked. Native64594c constructor registers channel0 completion before loader; node/head and persistence checked; hardware exception entry not integrated here. Callback pool explicitly seeded after native prerequisites; complete retail boot order unverified. Retail file size33408 checked for sector rounding and driver destination; released slot reused by second temporary upload object; final flag0, payload object fields and pool count1 checked; queue read/write indices advance to2. Native engine context, transfer startup and full frame initializer including pool/queue/request/layout/work allocation/debug-disabled/copy/acquisition helpers compared to full retail RAM/CPU/FPU/MMIO/MACL. Independent native frame pointer, pool/queue/request counts/bases, allocated work object and channel-0 descriptor/global alias checks across initial context fill and FPU banks. Logging enabled path and physical hardware not covered; no injected frame context.'),indent=2)+'\n')
 print(cases,'native driver loader initialization comparisons passed')
if __name__=='__main__':main()




