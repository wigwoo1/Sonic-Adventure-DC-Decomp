import ctypes,itertools,json,struct,shlex,hashlib
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
for address,name,dll in [(2355100096, 'sa_file_volume_word_get', 'file_volume_helpers'), (2355385598, 'sa_file_directory_load', 'file_directory_parser'), (2355385448, 'sa_file_directory_record_parse', 'file_directory_parser'), (2355385180, 'sa_file_directory_bytes_copy', 'file_directory_parser'), (2355385410, 'sa_file_name_checksum', 'file_filename_lookup'), (2355385160, 'sa_file_name_upper', 'file_filename_lookup'), (2355100926, 'sa_file_directory_parent_sector_adjust', 'file_directory_parent_sector_adjust')]:
 helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper
manifest=json.loads((root/'disc_inventory.json').read_text())
tracks=[shlex.split(line) for line in manifest['gdi'].splitlines()[1:] if line.strip()]
track=next(x for x in reversed(tracks) if x[2]=='4');base=int(track[1]);stride=int(track[3]);start=int(track[5]);source=ROOT/'unused'
source=__import__('pathlib').Path(manifest['source'])/track[4]
def disc_read(lba,count):
 data=bytearray()
 with source.open('rb') as stream:
  for i in range(count):
   stream.seek(start+(lba+i-base)*stride+(16 if stride==2352 else 0));block=stream.read(2048);assert len(block)==2048;data.extend(block)
 return bytes(data)
original_volume=disc_read(base+16,1)
assert original_volume[:7]==b'\x01CD001\x01'
root_lba=struct.unpack_from('<I',original_volume,158)[0];root_size=struct.unpack_from('<I',original_volume,166)[0]
original_directory=disc_read(root_lba,(root_size+2047)//2048)
expected_records=[];pos=0
while pos<root_size:
 length=original_directory[pos]
 if not length:pos=(pos//2048+1)*2048;continue
 record=original_directory[pos:pos+length];name=record[33:33+record[32]]
 if record[25]&2:name=b'.' if name==b'\0' else b'..' if name==b'\1' else name
 else:name=name[:-2]
 expected_records.append((struct.unpack_from('<I',record,2)[0],struct.unpack_from('<I',record,10)[0],name));pos+=length
select=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_directory_select.dll')).sa_file_directory_select;select.argtypes=abi;functions[0x8c60041a]=select
lookup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll')).sa_file_filename_lookup;lookup.argtypes=abi;functions[0x8c6455ce]=lookup
selected=next(x for x in manifest['files'] if x['path']=='SONICADV' and x['directory'])
selected_data=disc_read(selected['lba'],(selected['size']+2047)//2048)
root_records=expected_records;expected_records=[];pos=0
while pos<selected['size']:
 length=selected_data[pos]
 if not length:pos=(pos//2048+1)*2048;continue
 record=selected_data[pos:pos+length];name=record[33:33+record[32]]
 if record[25]&2:name=b'.' if name==b'\0' else b'..' if name==b'\1' else name
 else:name=name[:-2]
 expected_records.append((struct.unpack_from('<I',record,2)[0],struct.unpack_from('<I',record,10)[0],name));pos+=length
if len(expected_records)>=2:expected_records[1]=(expected_records[0][0],expected_records[1][1],expected_records[1][2])
fold=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll')).sa_file_name_equal_folded;fold.argtypes=abi;functions[0x8c6453e2]=fold
functions[0x8c5ffd28]=fn
initialize=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_context_initialize.dll')).sa_file_context_initialize;initialize.argtypes=abi
for address,name,dll in [(2355105232, 'sa_file_directory_storage_initialize', 'file_directory_storage'), (2355102820, 'sa_file_handle_open_initialize', 'file_handle_open_initialize')]:
 helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper
getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_interface_get.dll')).sa_file_device_interface_get;getter.argtypes=abi;functions[0x8c645328]=getter
device_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_initialize.dll')).sa_file_device_initialize;device_init.argtypes=abi;functions[0x8c644fe8]=device_init
low_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_low_initialize.dll')).sa_file_device_low_initialize;low_init.argtypes=abi;functions[0x8c10d974]=low_init
for address,name,dll in [(2355384310, 'sa_file_device_read', 'file_device_read'), (2349914424, 'sa_file_bios_request', 'file_bios_request')]:
 helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper
for address,name in [(0x8c6452d6,'sa_file_device_start'),(0x8c10d960,'sa_file_bios_execute')]:
 helper=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_start.dll')),name);helper.argtypes=abi;functions[address]=helper
for address,name,dll in [(0x8c6452ea,'sa_file_device_command24','file_device_command24'),(0x8c5ffcf0,'sa_file_registered_callback','file_registered_callback'),(0x8c6005c2,'sa_file_backend_poll','file_backend_poll'),(0x8c5ffa12,'sa_file_completion_process','file_completion_process'),(0x8c6450ea,'sa_file_device_status','file_device_status'),(0x8c10d94c,'sa_file_bios_status','file_bios_status')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
for address,name in [(0x8c6451fc,'sa_file_device_disc_info'),(0x8c10d988,'sa_file_bios_disc_info')]:
 helper=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_disc_info.dll')),name);helper.argtypes=abi;functions[address]=helper
disc_request=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_disc_request.dll')).sa_file_device_disc_request;disc_request.argtypes=abi;functions[0x8c6451c8]=disc_request
version=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_device_version_request.dll')).sa_file_device_version_request;version.argtypes=abi;functions[0x8c64530a]=version
registration=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_callback_register.dll')).sa_file_callback_register_io;registration.argtypes=abi[:7]+[ctypes.c_uint32,ctypes.POINTER(IO)]+abi[7:]
for address,name in [(0x8c64135a,'sa_file_driver_vblank_register'),(0x8c6412c2,'sa_file_driver_callback_install')]:
 helper=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_driver_callback_install.dll')),name);helper.argtypes=abi;functions[address]=helper
for address,name,dll in [(0x8c6004d4,'sa_file_backend_open','file_backend_open'),(0x8c5ffa08,'sa_file_error_set','file_error_set'),(0x8c600df4,'sa_file_close','file_handle_metadata')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
file_record=next(x for x in manifest['files'] if x['path']=='SONICADV/E_00B7.MLT')
file_sectors=(file_record['size']+2047)//2048
file_bytes=disc_read(file_record['lba'],file_sectors)
for address,name,dll in [(0x8c0939b0,'sa_sound_bank_load','sound_bank_load'),(0x8c6016d6,'sa_file_open_dispatch','file_open_dispatch'),(0x8c600e2c,'sa_file_size_get','file_handle_metadata')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
for address,name,dll in [(0x8c093ce0,'sa_sound_bank_download','sound_bank_download'),(0x8c6434e0,'sa_engine_frame_object_allocate','engine_frame_initialize_native'),(0x8c643624,'sa_engine_frame_object_configure','engine_frame_object_configure'),(0x8c6435bc,'sa_engine_frame_object_release','engine_frame_object_configure'),(0x8c65a568,'sa_engine_object_state_get','engine_frame_object_configure'),(0x8c65a5cc,'sa_engine_object_state_set','engine_frame_object_configure'),(0x8c65a5ac,'sa_engine_object_address_set','engine_frame_object_configure'),(0x8c65a5d4,'sa_engine_object_length_set','engine_frame_object_configure'),(0x8c65a5b4,'sa_engine_object_callback_set','engine_frame_object_configure'),(0x8c10cd38,'sa_engine_record_copy_dispatch','engine_frame_initialize_native'),(0x8c10cd44,'sa_engine_record_copy_loaded_dispatch','engine_frame_initialize_native'),(0x8c10cd76,'sa_engine_record_copy32_tail','engine_frame_initialize_native'),(0x8c10cd7a,'sa_engine_record_copy28_tail','engine_frame_initialize_native'),(0x8c10cd8e,'sa_engine_record_copy8_tail','engine_frame_initialize_native'),(0x8c65acac,'sa_engine_error_record','engine_frame_initialize_native'),(0x8c10d81c,'sa_memory_fill','engine_frame_initialize_native')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
for address,name,dll in [(0x8c643970,'sa_sound_multi_unit_download','sound_multi_unit_download'),(0x8c65a730,'sa_sound_bank_section_configure','sound_bank_section_configure')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
bank_records=[struct.unpack_from('<8I',file_bytes,32+i*32) for i in range(struct.unpack_from('<I',file_bytes,8)[0])]
for address,name,dll in [(0x8c643904,'sa_sound_bank_section_transfer','sound_bank_section_transfer'),(0x8c65a900,'sa_sound_bank_section_lookup','sound_bank_section_lookup')]:
 helper=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helper.argtypes=abi;functions[address]=helper
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
io_functions={}
for address,name,hasio,owner in [(0x8c10d770,'sa_engine_transfer_copy_strided_io',True,support),(0x8c10ebac,'sa_engine_transfer_fifo_wait',False,support),(0x8c10f9a8,'sa_engine_transfer_fifo_ready_get_io',True,support),(0x8c66910c,'sa_sound_status_word_get',False,lib),(0x8c10d460,'sa_engine_transfer_read_strided_io',True,lib),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support)]:
 fn=getattr(owner,name);fn.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];io_functions[address]=(fn,hasio)
word=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_bank_word_write.dll')).sa_sound_bank_word_write;word.argtypes=abi;functions[0x8c667200]=word
consumer_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_consume_direct_connected.dll'))
for address,name in [(0x8c65a5dc,'sa_engine_queue_consume_direct'),(0x8c666c2c,'sa_engine_frame_transfer_begin'),(0x8c668f40,'sa_engine_frame_services'),(0x8c65a518,'sa_engine_object_callback_dispatch'),(0x8c66abfc,'sa_engine_pending_count_get'),(0x8c669088,'sa_engine_frame_transfer_submit')]:
 helper=getattr(consumer_lib,name);helper.argtypes=abi;functions[address]=helper
cases=0
for first,second,bank,scenario in itertools.product([0,0x8cb10000],[0,0x8cb20000],[0,0x200000],[0]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 for offset,value in [(0x67c004,0x8cb08000),(0x673414,0x8ca01000),(0x673418,first),(0x67341c,second),(0xa01014,0x8ca02080),(0xa0101c,0),(0xa02080,0x8ca01000),(0xa01010,0x8ca03000),(0xa03004,4096),(0xa0100c,0x8c66d240),(0xa0103c,0x8c66d240),(0x66d240,0x8c644fe8),(0x66d25c,0x8c6451c8),(0x66d260,0x8c6451fc),(0x66d288,0x8c6452ea),(0x66d250,0x8c6450ea),(0x66d28c,0x8c64530a),(0x66d244,0x8c644ff6),(0x66d280,0x8c6452d6)]:struct.pack_into('<I',ram,offset,value)
 ram[0x80f0:0x80fe]=b'SONICADV    '+bytes([1,0])
 volume=original_volume;directory=original_directory
 struct.pack_into('<I',ram,0xbc,0x8cba0000)
 events=[];actual=[];mmio={0xa05f6920:0x123,0xa05f6924:0x456,0xa05f688c:0,**{0xa0801400+i:0 for i in range(0,0x300,4)},**{0xa05f7818+slot*32:0 for slot in range(4)}};native_mmio=dict(mmio)
 for record in bank_records:
  if 0<record[5]<0x80000000:
   for offset in range(0,record[5]+16,4):mmio[0xa0800000+record[2]+offset]=0xcccccccc
 native_mmio=dict(mmio)
 def put(m,o,data):
  for i,v in enumerate(data):m[o+i]=v
 def service(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8c645328:regs[0]=0x8c66d240
  elif target==0x8cba0000:
   assert regs[6]==0
   if regs[7] in [2,3]:regs[0]=0
   elif regs[7]==0 and regs[4]==0x18:
    assert regs[5]==0;regs[0]=0x1234
   elif regs[7]==0 and regs[4]==0x28:
    o=regs[5]&0xffffff;dest,zero=struct.unpack('<II',bytes(m[o:o+8]));assert dest==0x8c88e938 and zero==0
    put(m,dest&0xffffff,b'0'*12+b'1234');regs[0]=0x1234
   elif regs[7]==4:
    put(m,regs[4]&0xffffff,struct.pack('<II',3,128));regs[0]=0
   elif regs[7]==1:
    assert regs[4]==0x1234
    put(m,regs[5]&0xffffff,struct.pack('<IIII',0,0,next((e[2][1] for e in reversed(log) if len(e)==3),1),0));regs[0]=2
   elif regs[7]==0 and regs[4]==0x24:
    regs[0]=0
   else:
    assert regs[7]==0 and regs[4]==0x11,(hex(regs[4]),hex(regs[7]))
    o=regs[5]&0xffffff;request=struct.unpack('<4I',bytes(m[o:o+16]))
    log[-1]=(target,log[-1][1],request)
    assert request[1]>=1 and request[2] in (0x0ca01040,0x0cb20000,0x0cd00000) and request[3]==0
    put(m,request[2]&0xffffff,disc_read(request[0]-150,request[1]));regs[0]=0x1234
  elif target in [0x8c5fc0d8,0x8c5fc336]:regs[0]=0
  elif target==0x8c658c20:
   assert regs[4]==1;regs[0]=1
  elif target==0x8cb30000:regs[0]=0x1234
  elif target==0x8cb50000 or target==0x8cb80000:
   put(m,0xa020d0,struct.pack('<H',1));put(m,0xa0101c,bytes(4));regs[0]=0
  elif target==0x8cb40000:
   put(m,regs[4]&0xffffff,struct.pack('<II',3,128));regs[0]=1
  elif target==0x8cb60000:regs[0]=0
  elif target==0x8cb70000:
   assert regs[5]==1 and regs[6]==0x8ca01040
   assert regs[5]==1 and regs[6]==0x8ca01040
   put(m,0xa01040,disc_read(regs[4]-150,1));regs[0]=0x1234
  elif target==0x8c65a2fc:regs[0]=0
  else:raise AssertionError(hex(target))
 hooks={}
 for target in [0x8c65a2fc,0x8cba0000,0x8c658c20,0x8cb30000,0x8cb40000,0x8cb50000,0x8cb60000,0x8c5fc0d8,0x8c5fc336]:
  def hook(m,regs,status,target=target):
   if target==0x8cba0000 and regs[7]==0 and regs[4]==0x24:
    nested=State();nested.r[:]=regs;nested.r[4]=0x8ca01000;nested.sr=status;nested.pr=s.pr;nested.pc=0x8c5ffcf0
    reference(m,nested,fpu=f,hooks=hooks)
    regs[:]=list(nested.r);status=nested.sr
    # BIOS command service returns after its scheduled callback.
    regs[0]=0;return status
   service(target,m,regs,events);return status
  hooks[target]=hook
 def clock_hook(m,regs,status):regs[2]=0xffd8000c;regs[3]=0xffffffff;regs[0]=0;return status
 hooks[0x8c60260e]=clock_hook
 @Read
 def read(ctx,a,n):
  assert n==4
  return 0xffffffff if a==0xffd8000c else native_mmio[a]
 @Write
 def write(ctx,a,v,n):assert (a in native_mmio or 0xa0800000<=a<0xa0a00000) and n==4;native_mmio[a]=v
 io=IO(None,read,write)
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  args=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8c64594c:return registration(*args,0x8c000000,ctypes.byref(io),None,call)
  if target==0x8c60260e:return clock(*args,ctypes.byref(io),None,call)
  if target in io_functions:
   helper,hasio=io_functions[target]
   return helper(*args,*([ctypes.byref(io)] if hasio else []),None,call)
  if target in functions:return functions[target](*args,None,call)
  if target==0x8cba0000 and regs[7]==0 and regs[4]==0x24:
   regs[4]=0x8ca01000
   status_code=functions[0x8c5ffcf0](*args,None,call)
   regs[0]=0;return status_code
  service(target,m,regs,actual);return 0
 struct.pack_into('<I',ram,0xf00000,0x8c66d240)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600038;s.r[4]=1;s.r[5]=0x8ca01000;s.r[6]=4096;s.r[7]=0x8ca03000
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks,vbr=0x8c000000,mmio=mmio)
 assert initialize(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert native_mmio==mmio
 assert native_mmio[0xa05f6920]==(0x123|(1<<14) if second==0 else 0x123)
 assert native_mmio[0xa05f6924]==0x456
 assert r[0]==0,(hex(r[0]),len(expected_records),root_lba,root_size,actual)
 assert struct.unpack_from('<H',native,0xa01038)[0]==1
 assert struct.unpack_from('<II',native,0xa01004)==(struct.unpack_from('<I',volume,140)[0]+150,struct.unpack_from('<I',volume,132)[0])
 assert struct.unpack_from('<3I',native,0xa03000)==(len(expected_records),4096,selected['lba']+150)
 for i,(lba,size,name) in enumerate(expected_records):
  o=0xa03010+i*44;assert struct.unpack_from('<II',native,o)==(lba,size);assert native[o+10:o+11+len(name)]==name+b'\0'
 assert [e[2][0] for e in actual if e[0]==0x8cba0000 and len(e)==3]==[base+16+150]+list(range(root_lba+150,root_lba+150+(root_size+2047)//2048))+list(range(selected['lba']+150,selected['lba']+150+(selected['size']+2047)//2048))
 assert [e[1][2:] for e in actual if e[0]==0x8cba0000 and len(e)==2 and e[1][3]==3]==[(0,3)]
 assert sum(e[0]==0x8cba0000 and e[1][3]==2 for e in actual)==2*sum(len(e)==3 for e in actual)+2
 count=int(first==0)+int(second==0)
 assert sum(e[0]==0x8c658c20 for e in actual)==int(first==0)
 assert struct.unpack_from('<I',native,0x673418)[0]==(first or 1)
 assert struct.unpack_from('<I',native,0x67341c)[0]==(second or (2 if first==0 else 1))
 if count:
  head=0x8c892850
  if first==0:assert struct.unpack_from('<I',native,0x893a80)[0]==head
  if second==0:assert struct.unpack_from('<I',native,0x893ad0)[0]==0x8c892850+(16 if first==0 else 0)
  callbacks=[0x8c5ffcf0,0x8c5ffcd6] if count==2 else [0x8c5ffcf0 if first==0 else 0x8c5ffcd6]
  for i,callback in enumerate(callbacks,1):
   assert struct.unpack_from('<I',native,0x892840+i*16)[0]==callback
   assert struct.unpack_from('<I',native,0x892848+i*16)[0]==0x8ca01000
 assert sum(e[0]==0x8cba0000 and e[1][3]==4 for e in actual)==1
 assert sum(e[0]==0x8cba0000 and e[1][3]==0 and e[1][0]==0x18 for e in actual)==1
 assert struct.unpack_from('<I',native,0xa0102c)[0]==0x10304
 assert sum(e[0]==0x8cba0000 and e[1][3]==0 and e[1][0]==0x28 for e in actual)==1
 if first==0:assert struct.unpack_from('<II',native,0xb08238)==(0x8c645b1e,0)
 # Continue from the mounted retail context into native file open/read/close.
 filename=b'e_00b7.mlt\0'
 for memory in (ram,native):memory[0xb10000:0xb10000+len(filename)]=filename
 def run_stage(address,arguments):
  s.pc=address;s.pr=0x8c000100;s.r[15]=0x8cf00000;s.r[4:4+len(arguments)]=arguments
  r[:]=s.r;sr.value=s.sr;pr.value=s.pr
  special={'macl':macl.value}
  reference(ram,s,fpu=f,hooks=hooks,vbr=0x8c000000,mmio=mmio,special=special)
  assert functions[address](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events and native_mmio==mmio and macl.value==special['macl']
  return r[0]
 for memory in (ram,native):
  struct.pack_into('<I',memory,0x673420,0x8c66c4b4);struct.pack_into('<I',memory,0xf00000,0)
  struct.pack_into('<I',memory,0x67c1d0,0x8cc00000);struct.pack_into('<I',memory,0xc0017c,0x8cc01000);struct.pack_into('<I',memory,0xc001c0,0x1400)
  struct.pack_into('<I',memory,0x5fbb14,0x8cc02000);struct.pack_into('<I',memory,0xc02000,0xa05f7800)
  for slot in range(4):struct.pack_into('<I',memory,0xc02004+slot*52,slot)
 assert run_stage(0x8c0939b0,[0x8cb10000,0,0,0])==0
 assert native[0xd00000:0xd00000+len(file_bytes)]==file_bytes
 assert struct.unpack_from('<I',native,0xa020e8)[0]==file_sectors
 assert struct.unpack_from('<H',native,0xa02120)[0]==0
 assert struct.unpack_from('<II',native,0xa020dc)==(file_record['lba']+150,file_record['size'])
 assert [e[2][:3] for e in actual if len(e)==3 and e[0]==0x8cba0000][-1]==(file_record['lba']+150,file_sectors,0x0cd00000)
 for record in bank_records:
  if 0<record[5]<0x80000000:
   copied=b''.join(struct.pack('<I',native_mmio[0xa0800000+record[2]+offset]) for offset in range(0,record[5],4))
   assert copied==file_bytes[record[4]:record[4]+record[5]]
   assert all(native_mmio[0xa0800000+record[2]+offset]==0xcccccccc for offset in range(record[5],record[5]+16,4) if not any(0<other[5]<0x80000000 and other[2]<=record[2]+offset<other[2]+other[5] for other in bank_records))
 offsets={0x42534d53:(0,0x1e4),0x42504d53:(0x80,0x264),0x42534f53:(0x100,0x2e4),0x52535053:(0x180,0x364),0x42504653:(0x200,0x3e4),0x424f4653:(0x280,0x464),0x57504653:(0x288,0x46c)}
 table_writes=[]
 for record in bank_records:
  if 0<record[3]<0x80000000:
   bus,offset=offsets[record[0]];index=record[1]
   assert struct.unpack_from('<II',native,0xc00000+offset+index*8)==record[2:4]
   table_writes.extend([(0x1400+bus+index*8,record[2]),(0x1404+bus+index*8,record[3])])
 for address,value in table_writes:assert native_mmio[0xa0800000+address]==value
 assert struct.unpack_from('<I',native,0xc01020)[0]==0
 assert struct.unpack_from('<I',native,0xc01000)[0]==0 and struct.unpack_from('<I',native,0xc00184)[0]==0
 cases+=1
report={'cases_passed':cases,'file_path':file_record['path'],'file_size':file_record['size'],'file_sectors':file_sectors,'scope':'Native original-disc startup/mount followed by game-facing sound bank load, open dispatch/backend lookup/size/read/close and native device/BIOS completion path. Full retail RAM/CPU/FPU/SR/PR/MACL/MMIO/events equality; independent exact sector bytes at0cd00000, file metadata, seven-sector progress, closed handle and download dispatch. Native bank-download wrapper/object allocation/configuration/release connected with independently checked cleanup; Native multi-unit parser and section configure connected; Native section-transfer wrapper and section lookup connected with independent destination/source/length and payload-byte assertions; Native table-word writer and G2 copy/arm/ready/FIFO/release helpers connected; Native queue consumer, transfer begin, frame services, object callback dispatch and pending-count getter connected; all four bank payloads copied to modeled sound RAM with independent byte and tail-guard checks; debug logging modeled; BIOS/cache/driver-enable modeled; no audio execution.'}
(root/'analysis/sound_bank_load_mounted_queue_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'mounted-disc sound bank load comparisons passed')
