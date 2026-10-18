import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_record_create.dll')).sa_sound_voice_record_create
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
selector=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_record_create.dll')).sa_sound_voice_slot_select
selector.argtypes=fn.argtypes
update=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_records_update.dll')).sa_sound_voice_records_update
update.argtypes=fn.argtypes
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
start=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_start_dispatch.dll')).sa_sound_voice_start_dispatch
start.argtypes=fn.argtypes
setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_program_setup.dll')).sa_sound_program_setup
setup.argtypes=fn.argtypes
pack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_midi_pack
pack.argtypes=fn.argtypes
enqueue=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_midi_enqueue
enqueue.argtypes=fn.argtypes
producer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_commit.dll'))
commit=producer.sa_sound_request_commit;commit.argtypes=fn.argtypes
copy20=producer.sa_sound_request_copy20;copy20.argtypes=fn.argtypes
copy_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_engine_record_copy_loaded_dispatch
copy_dispatch.argtypes=fn.argtypes
processor=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_process.dll')).sa_sound_request_process;processor.argtypes=fn.argtypes
work_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_request_process.dll')).sa_sound_work_dispatch;work_dispatch.argtypes=fn.argtypes
objects=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
configure=objects.sa_engine_frame_object_configure;configure.argtypes=fn.argtypes
object_functions={}
for address,name in [(0x8c65a568,"sa_engine_object_state_get"),(0x8c65a5cc,"sa_engine_object_state_set"),(0x8c65a5ac,"sa_engine_object_address_set"),(0x8c65a5d4,"sa_engine_object_length_set"),(0x8c65a5b4,"sa_engine_object_callback_set")]:
 helper=getattr(objects,name);helper.argtypes=fn.argtypes;object_functions[address]=helper
packed_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_request_enqueue.dll'))
packer=packed_lib.sa_engine_frame_request_enqueue;packer.argtypes=fn.argtypes
move=packed_lib.sa_memory_move;move.argtypes=fn.argtypes
from verify_engine_cache_writeback import IO,Read,Write
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
writer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_set.dll'))
support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
abi=fn.argtypes;functions={}
for address,name,hasio,owner in [(0x8c6671c4,'sa_sound_status_byte_get',False,lib),(0x8c643624,'sa_engine_frame_object_configure',False,objects),(0x8c65a568,'sa_engine_object_state_get',False,objects),(0x8c65a5cc,'sa_engine_object_state_set',False,objects),(0x8c65a5ac,'sa_engine_object_address_set',False,objects),(0x8c65a5d4,'sa_engine_object_length_set',False,objects),(0x8c65a5b4,'sa_engine_object_callback_set',False,objects),(0x8c65a5dc,'sa_engine_queue_consume_direct',False,support),(0x8c666c2c,'sa_engine_frame_transfer_begin',False,support),(0x8c669088,'sa_engine_frame_transfer_submit',False,support),(0x8c668f40,'sa_engine_frame_services',False,support),(0x8c65a518,'sa_engine_object_callback_dispatch',False,support),(0x8c66abfc,'sa_engine_pending_count_get',False,support),(0x8c65acac,'sa_engine_error_record',False,support),(0x8c666ad0,'sa_sound_command_complete',False,writer),(0x8c669144,'sa_sound_status_byte_set',False,writer),(0x8c66ac3c,'sa_sound_status_word_set',False,writer),(0x8c10d770,'sa_engine_transfer_copy_strided_io',True,support),(0x8c10ebac,'sa_engine_transfer_fifo_wait',False,support),(0x8c10f9a8,'sa_engine_transfer_fifo_ready_get_io',True,support),(0x8c66910c,'sa_sound_status_word_get',False,lib),(0x8c10d460,'sa_engine_transfer_read_strided_io',True,lib),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support)]:
 helper=getattr(owner,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];functions[address]=(helper,hasio)
def literal(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
table=literal(0x8c094078);reserved_cursor=literal(0x8c09407c);ordinary_cursor=literal(0x8c094220);cases=0
ids=[0,1,0x42,0x7c,0x308,0x309,0x30d,0x30e,0x4be,0x4bf,0x4c6,0x4c7,0x4ef,0x4f0,0x4f7,0x4f8,0x4f9]
for identity,pattern,bank,mode in itertools.product(ids,['free','busy','eligible'],[0,0x200000],[0,1]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 reserved=identity in [0,0x42,0x43,0x7c,0xa1,0xa0,0xef,0x229,0x22a,0x423,0x40f,0x438,0x29b,0x403,0x405,0x4f9] or 0x308<identity<0x30e or 0x4be<identity<0x4c7 or 0x4ef<identity<0x4f8
 base=32 if reserved else 0
 struct.pack_into('<I',ram,reserved_cursor,32);struct.pack_into('<I',ram,ordinary_cursor,0)
 for slot in range(38):struct.pack_into('<2I',ram,table+slot*52,10,0 if pattern=='free' else 1)
 if pattern=='eligible':struct.pack_into('<I',ram,table+(base+2)*52,1)
 struct.pack_into('<I',ram,literal(0x8c094a00),0);struct.pack_into('<I',ram,literal(0x8c094a04),0)
 struct.pack_into('<I',ram,0x67c1d0,0x8ca09000)
 devices=literal(0x8c093348);command_buffer=literal(0x8c093344)
 struct.pack_into('<2I',ram,devices,0x8ca08000,0x8ca08008)
 ram[0xa08001]=0;ram[0xa08009]=1
 ram[command_buffer:command_buffer+8]=bytes([0x55])*8
 ring_offset=struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0];ring=0xa09000+ring_offset;ring_base=0xa10000
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0)
 struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,0,0,0)
 struct.pack_into("<I",ram,0xa09178,0x8ca30000)
 struct.pack_into("<7I",ram,0xa30000,1,0,0,0,0,0,0)
 struct.pack_into('<I',ram,0x5fbb14,0x8ca01000)
 struct.pack_into('<I',ram,0xa01000,0xa05f7800)
 struct.pack_into('<I',ram,0xa010d4,mode)
 for slot in range(4):struct.pack_into('<I',ram,0xa01004+slot*52,slot)
 struct.pack_into('<I',ram,0xa091bc,0x1200);struct.pack_into('<I',ram,0xa091c4,0x1200)
 mmio={0xa05f688c:0,**{0xa0801200+i:0x1234ab00 for i in range(0,512,4)}}
 for slot in range(4):mmio[0xa05f7818+slot*32]=0
 registers=dict(mmio)
 @Read
 def read(ctx,a,n):
  if a not in registers or n!=4:errors.append(('read',a,n));return 0
  return registers[a]
 @Write
 def write(ctx,a,v,n):
  if not 0xa0801200<=a<0xa0801400 or n!=4:errors.append(('write',a,v,n))
  registers[a]=v
 io=IO(None,read,write)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=identity;s.r[5]=0xabcdef01;s.r[6]=5;s.r[7]=0x12345678;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c0940da
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 errors=[];actual=[];events=[];command_stack=[];requests=[];reference_requests=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in functions:
   function,hasio=functions[a];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return function(*args,None,call)
  if a==0x8c643b9c:return enqueue(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c093140:return setup(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c093b2a:return start(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c093f02:return selector(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==struct.unpack_from('<I',binary,0x093340-0x10000)[0]:
   command_stack.append(tuple(sum(m[(r[15]&0xffffff)+4*j+i]<<(8*i) for i in range(4)) for j in range(3)))
   offset=r[5]&0xffffff;neighbors=bytes(m[offset+i] for i in range(4,8))
   result=pack(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if bytes(m[offset+i] for i in range(4,8))!=neighbors:errors.append('packer changed adjacent bytes');return -9
   return result
  if a==0x8c666a68:return work_dispatch(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c643624:return configure(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a in object_functions:return object_functions[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c666ba8:return packer(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10d25c:return move(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10cd44:return copy_dispatch(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10cd82:return copy20(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==struct.unpack_from('<I',binary,0x643c0c-0x10000)[0]:
   requests.append(bytes(m[(r[4]&0xffffff)+i] for i in range(20)))
   return commit(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  actual.append((a,tuple(r[i] for i in range(4,8))));r[0]=0;return 0
 special={'macl':0}
 reference(ram,s,fpu=f,special=special)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 expected=base if pattern=='free' else base+2 if pattern=='eligible' else 0xffffffff
 assert r[0]==(0xffffffff if expected==0xffffffff else 44),(identity,pattern,r[0],expected)
 assert macl.value==special['macl']
 if expected!=0xffffffff:
  assert struct.unpack_from('<12I',native,table+expected*52)==(0xffffffff,120,0xabcdef01,17,identity,0,0x12345678,0,0,0,0,0)
 hooks={}
 for address in addresses:
  if address in (0x8c0948a0,0x8c093b2a,0x8c093140,0x8c643c18,0x8c643b9c,0x8c65a464,0x8c10cd82,0x8c10cd44,0x8c666ba8,0x8c10d25c,0x8c65a378,0x8c666a68,0x8c643624,*object_functions,*functions):continue
  def hook(m,regs,status,address=address):
   if address==struct.unpack_from('<I',binary,0x643c0c-0x10000)[0]:reference_requests.append(bytes(m[(regs[4]&0xffffff)+i] for i in range(20)))
   events.append((address,tuple(regs[i] for i in range(4,8))));regs[0]=0;return status
  hooks[address]=hook
 for tick in range(2):
  s.pc=0x8c0948a0;s.pr=0x8c000100;r[:]=s.r;sr.value=s.sr;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks=hooks,mmio=mmio)
  assert update(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events
  if expected!=0xffffffff:
   assert struct.unpack_from('<I',native,table+expected*52+4)[0]==120-tick
   assert struct.unpack_from('<I',native,table+expected*52+12)[0]==16
 if expected!=0xffffffff:
  if expected<32:
   command=struct.unpack_from('<I',binary,0x093340-0x10000)[0]
   assert command_stack and command_stack[0][0:2]==(((expected&15)|0xb0),0x20)
   assert native[command_buffer:command_buffer+4]==bytes([127,1,expected&15,3])
  else:
   target=struct.unpack_from('<I',binary,0x093bb8-0x10000)[0]
   mapping=literal(0x8c093bb4)
   handle=struct.unpack_from('<I',native,mapping+(expected-30)*4)[0]
   position=literal(0x8c094a14);boundary=0;group=0
   for _ in range(2048):
    value,kind=struct.unpack_from('<ii',native,position)
    if value<0:break
    if value<identity:boundary=value;group=kind;break
    position+=8
   command_identity=identity if boundary==0 else identity-boundary-1
   if boundary==0:group=0
   assert any(a==target and args[:3]==(handle,group,command_identity) for a,args in actual)
 count=struct.unpack_from('<I',native,ring+20)[0]
 assert count==len(requests) and struct.unpack_from('<I',native,ring+24)[0]==count
 assert requests==[bytes(ram[ring_base+j*28:ring_base+j*28+20]) for j in range(count)]
 for j,request in enumerate(requests):
  assert native[ring_base+j*28:ring_base+j*28+20]==request
  assert struct.unpack_from('<I',native,ring_base+j*28+20)[0]==1
 if expected!=0xffffffff and expected<32:
  assert len(requests)==5
  request_base=literal(0x8c643c00);data0=literal(0x8c643c04)-request_base;data1=literal(0x8c643c08)-request_base
  assert (requests[-1][data0],requests[-1][data1],requests[-1][data1+1],requests[-1][9])==(127,1,expected&15,3)
 # A busy driver must preserve pending records; readiness is then changed by the bus fixture.
 for bus in (mmio,registers):bus[0xa0801200]=0x1234ab01
 pending_before=bytes(native[ring_base:ring_base+256*28])
 s.pc=0x8c65a378;s.pr=0x8c000100;r[:]=s.r;sr.value=s.sr;pr.value=s.pr
 reference(ram,s,fpu=f,special=special,hooks=hooks,mmio=mmio)
 assert processor(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events and registers==mmio and not errors
 assert bytes(native[ring_base:ring_base+256*28])==pending_before
 assert struct.unpack_from('<I',native,ring+20)[0]==count
 assert struct.unpack_from('<I',native,ring+28)[0]==0
 assert struct.unpack_from('<2I',native,ring+4)==(0,0)
 assert struct.unpack_from('<I',native,0xa30004)[0]==0
 for bus in (mmio,registers):bus[0xa0801200]=0x1234ab00
 s.pc=0x8c65a378;s.pr=0x8c000100;r[:]=s.r;sr.value=s.sr;pr.value=s.pr
 reference(ram,s,fpu=f,special=special,hooks=hooks,mmio=mmio)
 assert processor(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and actual==events
 assert struct.unpack_from('<I',native,ring+20)[0]==0
 assert struct.unpack_from('<I',native,ring+28)[0]==count
 assert not errors,errors
 assert registers==mmio
 assert struct.unpack_from('<I',native,ring+4)[0]==0
 assert struct.unpack_from('<I',native,ring+8)[0]==0
 assert struct.unpack_from('<I',native,0xa30004)[0]==int(count>0)
 if count:
  assert struct.unpack_from('<2I',native,0xa30008)==(0x8ca20000,(count+(count&1))*16)
  assert struct.unpack_from('<2I',native,0xa30010)==(0x8c666ad0,0x8ca09158)
 for j,request in enumerate(requests):
  assert struct.unpack_from('<I',native,ring_base+j*28+20)[0]==0
  assert native[0xa20000+j*16:0xa20000+(j+1)*16]==bytes(2)+request[4:18]
 expected_bus=bytearray(struct.pack('<I',0x1234ab00)*128)
 if count:
  packed=b''.join(bytes([request[0],0])+request[4:18] for request in requests)+bytes(16*(count&1))
  expected_bus[:len(packed)]=packed
  expected_bus[0]=1
 assert b''.join(struct.pack('<I',registers[0xa0801200+i]) for i in range(0,512,4))==expected_bus
 cases+=1
(out/'analysis/sound_voice_create_update_request_readiness_verification.json').write_text(json.dumps({'cases_passed':cases,'scope':'Native/reference full RAM/CPU/SR/PR/FPU comparisons. Independent reserved/ordinary selection for boundary identities, free/busy/priority eligible tables and both FPU banks. All 37 table entries initialized; valid cursor starts32/0 only. Native48-instruction caller plus native193-instruction selector, independent created12-word record and MACL checks. Function name provisional; Native530-instruction updater connected for two ticks; independent creation transition17->16 and lifetimes120->119 plus modeled start command slot checked. Native35-instruction start dispatcher connected for ordinary/reserved slots. Existing native128-instruction program setup093140 connected; no added function coverage for alias. Native162-instruction MIDI packer connected, explicit context and two device-object fixture, final controller packet bytes/guards independently checked. Native MIDI enqueue643b9c connected; five ordinary setup request records and final packet fields independently checked. Native request commit65a464 and copy20 connected; explicit empty256-slot request ring, independent stored20-byte records/tags/count/write-index checks. Native request consumer65a378 and packed request builder666ba8/memory move connected. Independent drained ring/read-index/cleared tags and exact16-byte packed records checked. Native work dispatcher666a68 and object configuration643624 connected. Independently checked even record padding, busy flag, object address/length/callback. Both transfer modes0/1 and FPU banks covered. Native synchronous queue consumption, transfer helpers and command completion connected. Independent exact modeled sound bus bytes, cleared packed tags/counters and callback object state checked. Native readiness byte getter6671c4 connected via native word getter and strided bus read. Independent busy-driver pass preserves all queued records, counts/read index and idle transfer object, followed by ready-driver transfer of the same requests. Sound bus synthetic; real audio not exercised.'},indent=2)+'\n')
print(cases,'sound record creation/update/request readiness/transfer comparisons passed')
