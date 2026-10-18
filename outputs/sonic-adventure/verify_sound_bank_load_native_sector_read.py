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
 producer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_commit.dll'))
 commit=producer.sa_sound_request_commit;commit.argtypes=abi;functions[0x8c65a464]=(commit,False)
 wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_command_type2.dll')).sa_sound_command_type2;wrapper.argtypes=abi;functions[0x8c643e08]=(wrapper,False)
 dispatcher=ctypes.CDLL(str(ROOT/"work/sonic-adventure/build/sound_index_dispatch.dll")).sa_sound_index_dispatch;dispatcher.argtypes=abi
 low=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll'))
 for address,name in [(0x8c0933ba,'sa_sound_low_index'),(0x8c643c18,'sa_sound_midi_pack'),(0x8c643b9c,'sa_sound_midi_enqueue')]:
  helper=getattr(low,name);helper.argtypes=abi;functions[address]=(helper,False)
 copy20=producer.sa_sound_request_copy20;copy20.argtypes=abi;functions[0x8c10cd82]=(copy20,False)
 functions[0x8c10ec40]=(acquire,False)
 slots=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_slot_setup.dll'))
 for address,name in [(0x8c643ac0,'sa_sound_slot_acquire_wrapper'),(0x8c65aa30,'sa_sound_slot_acquire')]:
  helper=getattr(slots,name);helper.argtypes=abi;functions[address]=(helper,False)
 table_init=slots.sa_sound_object_table_initialize;table_init.argtypes=abi
 note_on=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_control.dll')).sa_sound_volume_control;note_on.argtypes=abi
 dispatch_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll'))
 volume_dispatch=dispatch_lib.sa_sound_volume_dispatch;volume_dispatch.argtypes=abi
 division=dispatch_lib.sa_signed_division;division.argtypes=abi;functions[0x8c10ca5c]=(division,False)
 functions[0x8c093240]=(note_on,False)
 type5=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_command_type1.dll')).sa_sound_command_type1;type5.argtypes=abi;functions[0x8c643d88]=(type5,False)
 lookup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_lookup.dll')).sa_sound_bank_section_lookup;lookup.argtypes=abi;functions[0x8c65a900]=(lookup,False)
 bank_transfer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_section_transfer.dll')).sa_sound_bank_section_transfer;bank_transfer.argtypes=abi
 for address,name,dll in [(0x8c65a730,'sa_sound_bank_section_configure','sound_bank_section_configure'),(0x8c667200,'sa_sound_bank_word_write','sound_bank_word_write'),(0x8c6435bc,'sa_engine_frame_object_release','engine_frame_object_configure'),(0x8c643904,'sa_sound_bank_section_transfer','sound_bank_section_transfer')]:
  helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=(helper,False)
 multi=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_multi_unit_download.dll')).sa_sound_multi_unit_download;multi.argtypes=abi
 functions[0x8c643970]=(multi,False)
 download=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_download.dll')).sa_sound_bank_download;download.argtypes=abi;functions[0x8c093ce0]=(download,False)
 load=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_load.dll')).sa_sound_bank_load;load.argtypes=abi
 metadata=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_handle_metadata.dll'))
 for address,name in [(0x8c600e2c,'sa_file_size_get'),(0x8c600df4,'sa_file_close')]:
  helper=getattr(metadata,name);helper.argtypes=abi;functions[address]=(helper,False)
 for address,name,dll in [(0x8c600f1e,'sa_file_read_sectors','file_read_sectors'),(0x8c601084,'sa_file_read_aligned','file_read_aligned'),(0x8c600fac,'sa_file_read_unaligned','file_read_unaligned'),(0x8c600cf0,'sa_file_release_wait','file_release_wait'),(0x8c6013a4,'sa_file_status_get','file_status_get')]:
  helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=(helper,False)
 clock=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll')).sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:];functions[0x8c60260e]=(clock,True)
 fn=processor.sa_sound_request_process;fn.argtypes=abi;cases=0
 bank_data=(ROOT/'work/sonic-adventure/extracted/files/SONICADV/E_00B7.MLT').read_bytes()
 for mode,bank in itertools.product([0,1],[0,0x200000]):
  address=0x1200
  value=1
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  mmio={0xa05f688c:0,0xa05f6900:0,0xa05f6920:0xffffffff,**{0xa05f7800+o:0 for o in range(0,0xc0,4)},**{0xa0801200+i:0x1234ab00 for i in range(0,2048,4)}}
  records=[struct.unpack_from('<8I',bank_data,32+i*32) for i in range(struct.unpack_from('<I',bank_data,8)[0])]
  for record in records:
   if 0<record[5]<0x80000000:
    for offset in range(0,record[5],4):mmio[0xa0800000+record[2]+offset]=0
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
  mmio[0xffd8000c]=registers[0xffd8000c]=0xffffffff
  reference_files=[];native_files=[];reference_devices=[];native_devices=[]
  def put(m,offset,data):
   for i,v in enumerate(data):m[offset+i]=v
  def device(target,m,regs,events):
   events.append((target,tuple(regs[i] for i in range(4,8))))
   if target==0x8cb20000:
    assert regs[4]==0 and regs[5]==(len(bank_data)+2047)//2048 and regs[6]==0x0cd00000 and regs[7]==3
    put(m,0xd00000,bank_data+bytes(regs[5]*2048-len(bank_data)));regs[0]=0x123
   elif target==0x8cb30000:
    put(m,0xb01014,struct.pack('<I',(len(bank_data)+2047)//2048));put(m,0xb01050,struct.pack('<H',1));put(m,0xb0201c,bytes(4));regs[0]=0
   else:assert target==0x8cb10000;regs[0]=0
  def file_service(target,m,regs,events):
   events.append((target,regs[4],regs[5],regs[6]))
   if target==0x8c6016d6:
    assert regs[4]==0x8cb00000 and regs[5]==0
    regs[0]=0x8cb01000
    for offset,data in [(0xc,struct.pack('<I',len(bank_data))),(0x4c,struct.pack('<H',1)),(0x50,struct.pack('<H',1))]:
     for i,v in enumerate(data):m[0xb01000+offset+i]=v
    for offset,value in [(0xb01000,0x8cb02000),(0xb01008,0),(0xb01010,(len(bank_data)+2047)//2048),(0xb01014,0),(0xb0200c,0x8cb03000),(0xb0201c,0),(0xb0203c,0x8cb03000),(0xb03004,0x8cb20000),(0xb03040,0x8cb30000),(0xb03048,0x8cb10000)]:put(m,offset,struct.pack('<I',value))
    put(m,0xb0104e,struct.pack('<H',3))
   elif target==0x8c600e2c:
    assert regs[4]==0x8cb01000
    output=regs[5]&0xffffff
    for i,v in enumerate(struct.pack('<I',len(bank_data))):m[output+i]=v
    regs[0]=0
   elif target==0x8c600f1e:
    assert regs[4]==0x8cb01000 and regs[5]==(len(bank_data)+2047)//2048 and regs[6]==0x0cd00000
    for i,v in enumerate(bank_data+bytes(regs[5]*2048-len(bank_data))):m[0xd00000+i]=v
    regs[0]=0
   else:
    assert target==0x8c600df4 and regs[4]==0x8cb01000
    regs[0]=0
  hooks={}
  for target in [0x8c6016d6]:
   def hook(m,regs,status,target=target):file_service(target,m,regs,reference_files);return status
   hooks[target]=hook
  for target in [0x8cb10000,0x8cb20000,0x8cb30000]:
   def hook(m,regs,status,target=target):device(target,m,regs,reference_devices);return status
   hooks[target]=hook
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c6016d6:file_service(t,m,r,native_files);return 0
   if t in [0x8cb10000,0x8cb20000,0x8cb30000]:device(t,m,r,native_devices);return 0
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
  for memory in [ram,native]:
   struct.pack_into('<I',memory,0x7ac874,mode)
   struct.pack_into('<I',memory,0x8d35c0,0x1400)
   memory[0xb00000:0xb0000b]=b'E_00B7.MLT\0'
   struct.pack_into('<I',memory,0xf00000,0)
  configure=objects.sa_engine_frame_object_configure;configure.argtypes=abi
  for pc,entry,arguments in [(0x8c0939b0,load,[0x8cb00000,0,0,0])]:
   s.pc=pc;s.pr=pr.value=0x8c000100
   for i,data in enumerate(arguments):s.r[4+i]=r[4+i]=data
   reference(ram,s,fpu=f,mmio=mmio,special=special,hooks=hooks)
   assert entry(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
   assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio and macl.value==special['macl']
   assert r[0]==0,(hex(pc),hex(r[0]))
  offsets={0x42534d53:(0,0x1e4),0x42504d53:(0x80,0x264),0x42534f53:(0x100,0x2e4),0x52535053:(0x180,0x364),0x42504653:(0x200,0x3e4),0x424f4653:(0x280,0x464),0x57504653:(0x288,0x46c)}
  for record in records:
   bus_offset,context_offset=offsets[record[0]];index=record[1]
   assert struct.unpack_from('<II',native,0x8d3400+context_offset+index*8)==record[2:4]
   assert [registers[0xa0801400+bus_offset+index*8],registers[0xa0801404+bus_offset+index*8]]==list(record[2:4])
   if 0<record[5]<0x80000000:
    expected=struct.unpack_from('<'+'I'*(record[5]//4),bank_data,record[4])
    assert tuple(registers[0xa0800000+record[2]+offset] for offset in range(0,record[5],4))==expected
  assert struct.unpack_from('<I',native,0x8d3584)[0]==1
  assert struct.unpack_from('<I',native,0x8d39a8)[0]==0
  assert struct.unpack_from('<I',native,0x8d39c8)[0]==0
  assert reference_files==native_files and [event[0] for event in native_files]==[0x8c6016d6]
  assert native[0xd00000:0xd00000+len(bank_data)]==bank_data
  assert struct.unpack_from('<H',native,0xb0104c)[0]==0 and struct.unpack_from('<I',native,0xb0100c)[0]==len(bank_data)
  assert native_devices==reference_devices and [event[0] for event in native_devices]==[0x8cb20000,0x8cb30000,0x8cb10000]
  assert struct.unpack_from('<I',native,0xb01014)[0]==(len(bank_data)+2047)//2048
  cases+=1
 (root/'analysis/sound_bank_load_native_sector_read_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Original extracted E_00B7.MLT bank passed through native game-facing and multi-unit download, section configuration and G2 helpers with full retail RAM/CPU/FPU/MMIO equality. Independent all section allocation table and four real payload byte writes and both temporary object cleanup. Native load wrapper calls native size/read/close wrappers with explicit host open and device read/start/poll hooks; native disc/filesystem internals not executed; hardware readiness modeled, no audio or filesystem service execution.'},indent=2)+'\n')
 print(cases,'real bank load with native sector read comparisons passed')
if __name__=='__main__':main()
