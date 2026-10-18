import ctypes,itertools,json,struct,hashlib
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_transfer.dll'))
fn=lib.sa_sound_driver_transfer;fn.argtypes=abi
error=lib.sa_engine_error_register;error.argtypes=abi
objects=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
helpers={}
for a,name in [(0x8c6435bc,'sa_engine_frame_object_release'),(0x8c643624,'sa_engine_frame_object_configure'),(0x8c65a568,'sa_engine_object_state_get'),(0x8c65a5cc,'sa_engine_object_state_set'),(0x8c65a5ac,'sa_engine_object_address_set'),(0x8c65a5d4,'sa_engine_object_length_set'),(0x8c65a5b4,'sa_engine_object_callback_set'),(0x8c65acac,'sa_engine_error_record')]:
 helpers[a]=getattr(objects,name);helpers[a].argtypes=abi
pool_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll'))
for a,name in [(0x8c6434e0,'sa_engine_frame_object_allocate'),(0x8c65a2fc,'sa_engine_frame_debug_log'),(0x8c10cd38,'sa_engine_record_copy_dispatch'),(0x8c10cd44,'sa_engine_record_copy_loaded_dispatch'),(0x8c10cd76,'sa_engine_record_copy32_tail'),(0x8c10cd7a,'sa_engine_record_copy28_tail'),(0x8c10cd8e,'sa_engine_record_copy8_tail')]:
 helpers[a]=getattr(pool_lib,name);helpers[a].argtypes=abi
queue_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_consume_direct_connected.dll'))
entries={0x8c666c2c:("sa_engine_frame_transfer_begin",False),0x8c668f40:("sa_engine_frame_services",False),0x8c65a518:("sa_engine_object_callback_dispatch",False),0x8c66abfc:("sa_engine_pending_count_get",False),0x8c65a568:("sa_engine_object_state_get",False),0x8c65a5cc:("sa_engine_object_state_set",False),0x8c669088:("sa_engine_frame_transfer_submit",False),0x8c10d770:('sa_engine_transfer_copy_strided_io',True),0x8c10ee9c:('sa_engine_transfer_channels_arm',False),0x8c10fa60:('sa_engine_transfer_channel_arm_io',True),0x8c10eee0:('sa_engine_transfer_ready_wait',False),0x8c10fb48:('sa_engine_transfer_ready_get_io',True),0x8c10ebac:('sa_engine_transfer_fifo_wait',False),0x8c10f9a8:('sa_engine_transfer_fifo_ready_get_io',True),0x8c10ee20:('sa_engine_transfer_channels_release',False),0x8c10f9bc:('sa_engine_transfer_channel_release_io',True),0x8c1101f0:('sa_engine_transfer_busy_get_io',True),0x8c65acac:('sa_engine_error_record',False)}
io_helpers={}
for a,(name,hasio) in entries.items():
 helper=getattr(queue_lib,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:]
 if hasio:io_helpers[a]=helper
 else:helpers[a]=helper
helpers[0x8c65a5dc]=queue_lib.sa_engine_queue_consume_direct;helpers[0x8c65a5dc].argtypes=abi
for a,name,dll in [(0x8c65a200,'sa_sound_driver_stop','sound_driver_stop'),(0x8c65a27c,'sa_sound_driver_start','sound_driver_start')]:
 helpers[a]=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helpers[a].argtypes=abi
reader=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_read_strided_io.dll')).sa_engine_transfer_read_strided_io;reader.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:];io_helpers[0x8c10d460]=reader
clear=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_memory_clear.dll')).sa_sound_driver_memory_clear;clear.argtypes=abi;helpers[0x8c65a8c8]=clear
for a,name,dll in [(0x8c65aaf4,'sa_sound_driver_info','sound_driver_info'),(0x8c667188,'sa_sound_driver_byte_get','sound_driver_getters'),(0x8c667150,'sa_sound_driver_word_get','sound_driver_getters'),(0x8c66910c,'sa_device_word_read','device_word_read')]:
 helpers[a]=getattr(ctypes.CDLL(str(ROOT/('work/sonic-adventure/build/'+dll+'.dll'))),name);helpers[a].argtypes=abi
driver=(ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV').read_bytes()
assert len(driver)==33408 and hashlib.sha256(driver).hexdigest()=='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9'
assert driver[:6]==b'SDRV\x01\x01'
payload_length=struct.unpack_from('<I',driver,8)[0];assert payload_length==len(driver)-32
pool_count_offset=struct.unpack_from('<h',binary,0x64360c-0x10000)[0]
cases=0
for bank,control in [(0,0xabcdef01)]:
 rejection='success';result=0;mask=6;handle=0x8cb20000
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0x67c1c8,0xabcdef01,0x12345678)
 struct.pack_into('<I',ram,0xb10008,0x8cb40000)
 struct.pack_into('<I',ram,0xb40000,0 if rejection=='magic' else 0x56524453)
 ram[0xb40004]=0 if rejection=='version' else 1;ram[0xb40005]=2 if rejection=='type' else 0
 struct.pack_into('<I',ram,0x67379c,0)
 struct.pack_into('<I',ram,0xb40008,64)
 struct.pack_into('<I',ram,0x67c1d0,0x8cb08000)
 ram[0xb20000:0xb20040]=bytes([0x99])*64;struct.pack_into('<2I',ram,0xb20000,0,1)
 struct.pack_into('<I',ram,0xb08000+380,handle)
 struct.pack_into('<I',ram,0xb08000+pool_count_offset,0)
 payload=driver[32:];ram[0xb40000:0xb40000+len(driver)]=driver
 struct.pack_into('<I',ram,0x5fbb14,0x8cb09000);struct.pack_into('<I',ram,0xb09000,0xa05f7800)
 for channel in range(4):struct.pack_into('<I',ram,0xb09004+channel*52,channel)
 mmio={0xa0702c00:control,0xa05f688c:0,**{0xa05f7818+32*i:0 for i in range(4)},**{0xa0800000+i:0x55555555 for i in range(0,68,4)}};registers=dict(mmio)
 struct.pack_into('<I',ram,0x67c1c4,0)
 mmio.update({0xa0800000+i:0x55555555 for i in range(-4,0x200004,4)});registers=dict(mmio)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[];control_writes=[]
 targets=[struct.unpack_from('<I',binary,a-0x10000)[0] for a in [0x643f34,0x643f38,0x643f40,0x643f44,0x643f48,0x643f4c,0x643f58,0x643f5c,0x643f60]]
 def service(t,m,r,events):
  events.append((t,tuple(r[i] for i in range(4,8))))
  if t==targets[2]:
   a=r[4]&0xffffff
   for i in range(4):m[a+i]=(handle>>(8*i))&255
  r[0]=0
 hooks={}
 for t in targets:
  if t in helpers:continue
  def hook(m,r,sr,t=t):service(t,m,r,expected);return sr
  hooks[t]=hook
 @Read
 def read(ctx,a,n):
  if a not in registers or n!=4:errors.append(('read',a,n));return 0
  return registers[a]
 @Write
 def write(ctx,a,v,n):
  if a not in registers or n!=4:errors.append(('write',a,n));return
  registers[a]=v
  if a==0xa0702c00:control_writes.append(v)
 io=IO(None,read,write)
 @Call
 def call(ctx,t,m,f,sr,r,pr):
  if t in io_helpers:return io_helpers[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if t in helpers:return helpers[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if t not in targets:errors.append(t);return -9
  service(t,m,r,actual);return 0
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.pc=0x8c643e6c;s.pr=0x8c000100;s.sr=0x70000003|(mask<<4)
 s.r[4]=0 if rejection=='null' else 0x8cb10000
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,hooks=hooks,fpu=f,mmio=mmio,instruction_budget=60000000)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==expected
 assert r[0]==result and r[14]==0x2234000e and r[15]==0x8cf00000
 assert struct.unpack_from('<2I',native,0x67c1c8)==((0x8c643e6c,result) if result else (0xabcdef01,0x12345678))
 assert [e[0] for e in actual]==[t for t in targets if t not in helpers]
 assert struct.unpack_from('<I',native,0x67379c)[0]==handle
 assert struct.unpack_from('<5I',native,0xb20004)==(1,0x8cb40020,payload_length,0xffffffff,0)
 template=struct.unpack_from('<I',binary,0x6435ac-0x10000)[0]&0xffffff
 assert struct.unpack_from('<I',native,0xb20018)[0]==1
 assert native[0xb2001c:0xb20020]==binary[template+28-0x10000:template+32-0x10000]
 assert native[0xb20020:0xb20040]==bytes([0x99])*32
 assert registers==mmio
 assert b''.join(struct.pack('<I',registers[0xa0800000+i]) for i in range(0,payload_length,4))==payload
 assert all(registers[0xa0800000+i]==0 for i in range(payload_length,0x200000,4))
 assert registers[0xa07ffffc]==registers[0xa0a00000]==0x55555555
 assert native[0xb40020:0xb40020+payload_length]==payload
 assert not actual
 assert control_writes==[control|1,control|1,control&0xfffffffe]
 assert registers[0xa0702c00]==control&0xfffffffe
 assert struct.unpack_from('<I',native,0x8a3408)[0]==control|1
 assert struct.unpack_from('<I',native,0x67c1c0)[0]==control&0xfffffffe
 assert struct.unpack_from('<I',native,0xb20000)[0]==0
 assert struct.unpack_from('<I',native,0xb08000+pool_count_offset)[0]==0
 assert struct.unpack_from('<I',native,0x67c1d4)[0]==1
 assert r[12]==0x2234000c and r[13]==0x2234000d
 def lit(a):return struct.unpack_from('<h',binary,a-0x8c010000)[0]
 assert struct.unpack_from('<I',native,0xb08000+lit(0x8c65ab62)-28)[0]==32
 assert native[0xb08000+lit(0x8c65ab64)]==payload[32]
 assert native[0xb08000+lit(0x8c65ab66)]==payload[33]
 assert native[0xb08000+lit(0x8c65ac64)]==payload[34]
 for a,b in [(0x8c65ac6a,0x8c65ac68),(0x8c65ac6e,0x8c65ac6c),(0x8c65ac72,0x8c65ac70),(0x8c65ac76,0x8c65ac74),(0x8c65ac7a,0x8c65ac78),(0x8c65ac7e,0x8c65ac7c)]:
  offset=32+lit(a);wanted=int.from_bytes(payload[offset:offset+4].ljust(4,b'\0'),'little')
  assert struct.unpack_from('<I',native,0xb08000+lit(b))[0]==wanted
 assert native[0xb08000+lit(0x8c65ac66)]==0
 cases+=1
report={'cases_passed':cases,'driver_sha256':hashlib.sha256(driver).hexdigest(),'driver_file_bytes':len(driver),'payload_bytes':payload_length,'instructions_recovered':91,'scope':'Full original-instruction RAM/CPU/SR/PR/FPU/service-event equality on valid SDR V driver header, retail MASARU.DRV payload33376, one FPU bank/control fixture. Original2097152-iteration delay executed without patching. Independent remaining modeled call order and native five object fields/tail, queue args, enable flag, callback/release arguments, preserved stack/R12/R13/R14 and zero result. Object allocation/configuration/release, debug logger, record copy dispatcher/tail and state getter/setters/error recorder native; independent allocation flag and pool-counter checks; Queue/G2 transfer and channel control native; Native sound stop/start and control read/write connected; Native full2MiB memory clear connected; Native driver-info/getters connected; no routine services modeled; info fields checked against uploaded payload, original object callback inactive/native dispatcher; independent stop/start control writes for patterned control; exact modeled sound RAM payload/tail checked; hardware/actual driver execution unverified.'}
(out/'analysis/sound_driver_transfer_retail_payload_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'driver transfer success comparisons passed')




