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
cases=0
for first,second,bank,scenario in itertools.product([0,0x8cb10000],[0,0x8cb20000],[0,0x200000],[0]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 for offset,value in [(0x673414,0x8ca01000),(0x673418,first),(0x67341c,second),(0xa01014,0x8ca02080),(0xa0101c,0),(0xa02080,0x8ca01000),(0xa01010,0x8ca03000),(0xa03004,4096),(0xa0100c,0x8c66d240),(0xa0103c,0x8c66d240),(0x66d240,0x8c644fe8),(0x66d25c,0x8cb30000),(0x66d260,0x8cb40000),(0x66d288,0x8c6452ea),(0x66d250,0x8c6450ea),(0x66d28c,0x8cb60000),(0x66d244,0x8c644ff6),(0x66d280,0x8c6452d6)]:struct.pack_into('<I',ram,offset,value)
 ram[0x80f0:0x80fe]=b'SONICADV    '+bytes([1,0])
 volume=original_volume;directory=original_directory
 struct.pack_into('<I',ram,0xbc,0x8cba0000)
 events=[];actual=[]
 def put(m,o,data):
  for i,v in enumerate(data):m[o+i]=v
 def service(target,m,regs,log):
  log.append((target,tuple(regs[i] for i in range(4,8))))
  if target==0x8c645328:regs[0]=0x8c66d240
  elif target==0x8cba0000:
   assert regs[6]==0
   if regs[7] in [2,3]:regs[0]=0
   elif regs[7]==1:
    assert regs[4]==0x1234
    put(m,regs[5]&0xffffff,struct.pack('<IIII',0,0,1,0));regs[0]=2
   elif regs[7]==0 and regs[4]==0x24:
    regs[0]=0
   else:
    assert regs[7]==0 and regs[4]==0x11,(hex(regs[4]),hex(regs[7]))
    o=regs[5]&0xffffff;request=struct.unpack('<4I',bytes(m[o:o+16]))
    log[-1]=(target,log[-1][1],request)
    assert request[1]==1 and request[2]==0x0ca01040 and request[3]==0
    put(m,request[2]&0xffffff,disc_read(request[0]-150,request[1]));regs[0]=0x1234
  elif target in [0x8c5fc0d8,0x8c5fc336]:regs[0]=0
  elif target==0x8c64594c:regs[0]=0x8cb10000
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
  else:raise AssertionError(hex(target))
 hooks={}
 for target in [0x8cba0000,0x8c64594c,0x8cb30000,0x8cb40000,0x8cb50000,0x8cb60000,0x8c5fc0d8,0x8c5fc336]:
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
 def read(ctx,a,n):assert a==0xffd8000c and n==4;return 0xffffffff
 @Write
 def write(*args):raise AssertionError('unexpected write')
 io=IO(None,read,write)
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  args=[m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl)]
  if target==0x8c60260e:return clock(*args,ctypes.byref(io),None,call)
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
 reference(ram,s,fpu=f,hooks=hooks)
 assert initialize(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert r[0]==0,(hex(r[0]),len(expected_records),root_lba,root_size,actual)
 assert struct.unpack_from('<H',native,0xa01038)[0]==1
 assert struct.unpack_from('<II',native,0xa01004)==(struct.unpack_from('<I',volume,140)[0]+150,struct.unpack_from('<I',volume,132)[0])
 assert struct.unpack_from('<3I',native,0xa03000)==(len(expected_records),4096,selected['lba']+150)
 for i,(lba,size,name) in enumerate(expected_records):
  o=0xa03010+i*44;assert struct.unpack_from('<II',native,o)==(lba,size);assert native[o+10:o+11+len(name)]==name+b'\0'
 assert [e[2][0] for e in actual if e[0]==0x8cba0000 and len(e)==3]==[base+16+150]+list(range(root_lba+150,root_lba+150+(root_size+2047)//2048))+list(range(selected['lba']+150,selected['lba']+150+(selected['size']+2047)//2048))
 assert [e[1][2:] for e in actual if e[0]==0x8cba0000 and len(e)==2 and e[1][3]==3]==[(0,3)]
 assert sum(e[0]==0x8cba0000 and e[1][3]==2 for e in actual)==2*sum(len(e)==3 for e in actual)+1
 assert sum(e[0]==0x8c64594c for e in actual)==int(first==0)+int(second==0)
 cases+=1
report={'cases_passed':cases,'root_records':len(root_records),'selected_records':len(expected_records),'selected_directory':'SONICADV','volume_sha256':hashlib.sha256(original_volume).hexdigest(),'root_directory_sha256':hashlib.sha256(original_directory).hexdigest(),'scope':'Original disc native context initialization and mount:768 root records and2069 SONICADV records. Native command24, registered callback, backend poll, completion processor, device status and BIOS status replace direct handle completion fixture. Full retail RAM/CPU/FPU/SR/PR and event equality; every selected record, sector request and mounted state independently checked. BIOS delivery/scheduling, cache, registration, disc-info and initial request callbacks remain modeled.'}
(root/'analysis/file_context_native_completion_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'BIOS execution boundary initialization and mount comparisons passed')