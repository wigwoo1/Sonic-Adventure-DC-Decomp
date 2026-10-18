import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
 writer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_set.dll'))
 support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 processor=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_process.dll'))
 objects=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
 enqueue=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_request_enqueue.dll'))
 functions={}
 for address,name,hasio,owner in [(0x8c6671c4,'sa_sound_status_byte_get',False,lib),(0x8c666ba8,'sa_engine_frame_request_enqueue',False,enqueue),(0x8c10d25c,'sa_memory_move',False,enqueue),(0x8c666a68,'sa_sound_work_dispatch',False,processor),(0x8c643624,'sa_engine_frame_object_configure',False,objects),(0x8c65a568,'sa_engine_object_state_get',False,objects),(0x8c65a5cc,'sa_engine_object_state_set',False,objects),(0x8c65a5ac,'sa_engine_object_address_set',False,objects),(0x8c65a5d4,'sa_engine_object_length_set',False,objects),(0x8c65a5b4,'sa_engine_object_callback_set',False,objects),(0x8c65a5dc,'sa_engine_queue_consume_direct',False,support),(0x8c666c2c,'sa_engine_frame_transfer_begin',False,support),(0x8c669088,'sa_engine_frame_transfer_submit',False,support),(0x8c668f40,'sa_engine_frame_services',False,support),(0x8c65a518,'sa_engine_object_callback_dispatch',False,support),(0x8c66abfc,'sa_engine_pending_count_get',False,support),(0x8c65acac,'sa_engine_error_record',False,support),(0x8c666ad0,'sa_sound_command_complete',False,writer),(0x8c669144,'sa_sound_status_byte_set',False,writer),(0x8c66ac3c,'sa_sound_status_word_set',False,writer),(0x8c10d770,'sa_engine_transfer_copy_strided_io',True,support),(0x8c10ebac,'sa_engine_transfer_fifo_wait',False,support),(0x8c10f9a8,'sa_engine_transfer_fifo_ready_get_io',True,support),(0x8c66910c,'sa_sound_status_word_get',False,lib),(0x8c10d460,'sa_engine_transfer_read_strided_io',True,lib),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support)]:
  fn=getattr(owner,name);fn.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];functions[address]=(fn,hasio)
 startup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
 entries={2355523152: ('sa_engine_frame_object_pool_initialize', False), 2355523312: ('sa_engine_frame_queue_initialize', False), 2355521988: ('sa_engine_frame_request_pool_initialize', False), 2355524160: ('sa_engine_frame_device_layout_initialize', False), 2355522356: ('sa_engine_frame_work_initialize', False), 2355377376: ('sa_engine_frame_object_allocate', False), 2355471100: ('sa_engine_frame_debug_log', False), 2349911352: ('sa_engine_record_copy_dispatch', False), 2349911364: ('sa_engine_record_copy_loaded_dispatch', False), 2349911414: ('sa_engine_record_copy32_tail', False), 2349911418: ('sa_engine_record_copy28_tail', False), 2349911438: ('sa_engine_record_copy8_tail', False), 2349912860: ('sa_memory_fill', False), 2355473580: ('sa_engine_error_record', False)}
 for address,(name,hasio) in entries.items():
  helper=getattr(startup,name);helper.argtypes=abi;functions[address]=(helper,False)
 engine_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll')).sa_engine_context_initialize_native;engine_init.argtypes=abi
 transfer_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialize_host.dll')).sa_engine_transfer_initialize_host;transfer_init.argtypes=abi +[ctypes.POINTER(IO),ctypes.c_uint32]
 sound_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_frame_initialize;sound_init.argtypes=abi
 acquire=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialized_lifecycle.dll')).sa_engine_transfer_descriptor_acquire;acquire.argtypes=abi
 functions[0x8c10ec40]=(acquire,False)
 getters=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/transfer_status_getters.dll'))
 for a,n in [(0x8c10d428,'sa_transfer_status_first_get'),(0x8c10d444,'sa_transfer_status_second_get')]:
  h=getattr(getters,n);h.argtypes=abi;functions[a]=(h,False)
 request=processor.sa_sound_request_process;request.argtypes=abi;functions[0x8c65a378]=(request,False)
 service=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_service.dll')).sa_sound_driver_service;service.argtypes=abi;functions[0x8c643f98]=(service,False)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_irq_callback.dll')).sa_sound_driver_irq_callback;fn.argtypes=abi;cases=0
 for mode,bank,count,index in itertools.product([0,1],[0,0x200000],[0,1,3,31,32,33,256],[0,255]):
  address=0x1200
  value=1
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  mmio={0xa05f688c:0,0xa05f6900:0,0xa05f6920:0xffffffff,**{0xa05f7800+o:0 for o in range(0,0xc0,4)},**{0xa0801200+i:0x1234ab00 for i in range(0,512,4)}}
  native=bytearray(ram);registers=dict(mmio);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Read
  def read(ctx,a,n):
   if a not in registers or n!=4:errors.append(('read',a,n));return 0
   return registers[a]
  @Write
  def write(ctx,a,v,n):
   if a not in registers or n!=4:errors.append(('write',a,v,n))
   registers[a]=v
  io=IO(None,read,write)
  actual=[];expected=[]
  modeled=[0x8c04f7e0,0x8c602cb8,0x8c09ce40]
  def service_model(t,r,events):events.append(t);r[0]=0
  hooks={}
  for t in modeled:
   def hook(m,r,sr,t=t):service_model(t,r,expected);return sr
   hooks[t]=hook
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t in modeled:service_model(t,r,actual);return 0
   if t not in functions:errors.append(t);return -9
   function,hasio=functions[t];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return function(*args,None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65a378
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={"macl":0}
  for pc,initializer in [(0x8c6510a4,engine_init),(0x8c10d5a8,transfer_init),(0x8c65acc0,sound_init)]:
   s.pc=pc;s.pr=pr.value=0x8c000100
   reference(ram,s,fpu=f,mmio=mmio,special=special,vbr=0x8ca08000)
   args=[view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call]
   if initializer is transfer_init:args += [ctypes.byref(io),0x8ca08000]
   assert initializer(*args)==0 and not errors,errors
   assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio and macl.value==special['macl']
  processed=min(count,32)
  expected_packed=bytearray(512);requests=[]
  for memory in [ram,native]:
   struct.pack_into('<I',memory,0x7ac874,mode)
   struct.pack_into('<I',memory,0x8d35bc,address);struct.pack_into('<I',memory,0x8d35c4,address)
   struct.pack_into('<I',memory,0x8d356c,count);struct.pack_into('<I',memory,0x8d3574,index)
  for order in range(count):
   slot=(index+order)%256;offset=0x8d45a8+slot*28
   entry=bytearray((slot*7+i*13+3)&255 for i in range(28));struct.pack_into('<I',entry,20,1)
   for memory in [ram,native]:memory[offset:offset+28]=entry
   requests.append((slot,entry))
   if order<processed:
    expected_packed[order*16]=entry[0];expected_packed[order*16+2:order*16+16]=entry[4:18]
  for memory in (ram,native):
   struct.pack_into('<I',memory,0x754ec0,1)
   struct.pack_into('<I',memory,0x760970,0)
   struct.pack_into('<I',memory,0x67c1d4,1)
   struct.pack_into('<2I',memory,0x67c1c8,0xabcdef01,0x12345678)
  s.pc=0x8c051810;s.pr=pr.value=0x8c000100
  reference(ram,s,hooks=hooks,fpu=f,mmio=mmio,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio and macl.value==special["macl"]
  expected_buffer=bytearray(expected_packed)
  padded_count=processed+(processed&1)
  for record_index in range(padded_count):expected_buffer[record_index*16]=0
  assert native[0x8d43a0:0x8d45a0]==expected_buffer, (hex(struct.unpack_from('<I',native,0x8d3558)[0]),count,index,next((i for i,(a,b) in enumerate(zip(native[0x8d43a0:0x8d45a0],expected_buffer)) if a!=b),None))
  assert struct.unpack_from('<2I',native,0x8d355c)==(0,0)
  expected_g2=bytearray(struct.pack('<I',0x1234ab00)*128)
  if count:
   expected_g2[:padded_count*16]=expected_packed[:padded_count*16];expected_g2[0]=1
   assert struct.unpack_from('<I',native,0x8d398c)[0]==1
   assert struct.unpack_from('<3I',native,0x8d3588)==(1,2,0)
   assert struct.unpack_from('<2I',native,0x8d3998)==(0x8c666ad0,0x8c8d3558)
  else:
   assert struct.unpack_from('<I',native,0x8d398c)[0]==0
   assert struct.unpack_from('<3I',native,0x8d3588)==(0,0,0)
  assert b''.join(struct.pack('<I',registers[0xa0801200+i]) for i in range(0,512,4))==expected_g2
  assert struct.unpack_from('<I',native,0x8d356c)[0]==count-processed
  assert struct.unpack_from('<I',native,0x8d3574)[0]==((index+processed)%256 if count else index),(count,index,struct.unpack_from("<I",native,0x8d3574)[0])
  for order,(slot,entry) in enumerate(requests):
   expected_request=bytearray(entry)
   if order<processed:struct.pack_into('<I',expected_request,20,0)
   assert native[0x8d45a8+slot*28:0x8d45c4+slot*28]==expected_request
  assert actual==expected==modeled and struct.unpack_from('<I',native,0x754ec0)[0]==0
  assert struct.unpack_from('<I',native,0x8d35ac)[0]==0x8c7ac7a4
  transfer=struct.unpack_from('<I',native,0x5fbb14)[0]&0xffffff
  first=struct.unpack_from('<I',native,transfer+0xec)[0];second=struct.unpack_from('<I',native,transfer+0xdc)[0]
  assert struct.unpack_from('<I',native,0x89067c)[0]==first and struct.unpack_from('<I',native,0x890678)[0]==second
  assert struct.unpack_from('<2I',native,0x67c1c8)==((0x8c643f98,0x7fffffff) if second<first else (0xabcdef01,0x12345678))
  cases+=1
 (root/'analysis/sound_driver_irq_request_status_initialized_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native engine, transfer and sound startup followed by native driver IRQ callback051810, service643f98 and request processing65a378, packing, synchronous transfer and command completion. Full retail RAM/CPU/FPU/MMIO/MACL equality at startup boundaries and processing; independent payload, padding, pool wrap, 32-record batch boundary, preservation of excess requests, counters and status assertions. Real initialized 256-record pool and allocated work object. Caller supplies requests, destination offsets and transfer mode; countdown1/mode0/enable1/real initialized device and native status getters10d428/10d444; output globals and unsigned error condition checked, game/file/tail callbacks modeled; real interrupt delivery not simulated; host MMIO model, no audio playback.'),indent=2)+'\n')
 print(cases,'initialized sound request processing comparisons passed')
if __name__=='__main__':main()
