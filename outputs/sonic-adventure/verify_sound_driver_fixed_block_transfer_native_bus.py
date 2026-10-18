import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
helpers={}
for a,name in [(0x8c643624,'sa_engine_frame_object_configure'),(0x8c65a568,'sa_engine_object_state_get'),(0x8c65a5cc,'sa_engine_object_state_set'),(0x8c65a5ac,'sa_engine_object_address_set'),(0x8c65a5d4,'sa_engine_object_length_set'),(0x8c65a5b4,'sa_engine_object_callback_set'),(0x8c65acac,'sa_engine_error_record')]:
 helper=getattr(lib,name);helper.argtypes=abi;helpers[a]=helper
consume=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll')).sa_engine_queue_consume_direct;consume.argtypes=abi;helpers[0x8c65a5dc]=consume
begin=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll')).sa_engine_frame_transfer_begin;begin.argtypes=abi;helpers[0x8c666c2c]=begin
submit=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll')).sa_engine_frame_transfer_submit;submit.argtypes=abi;helpers[0x8c669088]=submit
support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
for a,name in [(0x8c668f40,'sa_engine_frame_services'),(0x8c65a518,'sa_engine_object_callback_dispatch'),(0x8c66abfc,'sa_engine_pending_count_get')]:
 helper=getattr(support,name);helper.argtypes=abi;helpers[a]=helper
io_helpers={}
for a,name,hasio in [(0x8c10d770,'sa_engine_transfer_copy_strided_io',True),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True),(0x8c10eee0,'sa_engine_transfer_ready_wait',False),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True),(0x8c10ebac,'sa_engine_transfer_fifo_wait',False),(0x8c10f9a8,'sa_engine_transfer_fifo_ready_get_io',True),(0x8c10ee20,'sa_engine_transfer_channels_release',False),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True)]:
 helper=getattr(support,name)
 if hasio:io_helpers[a]=helper
 else:helper.argtypes=abi;helpers[a]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]

from verify_engine_cache_writeback import IO,Read,Write
clock=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll')).sa_file_clock_get_io
clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
for helper in io_helpers.values():helper.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for counter_start,selector,rejection,bitmap,bank,sr0 in itertools.product([1,0xfffffffe],[-1,0,1,2,255,256],['success','context','object','state'],[0,15],[0,0x200000],[0,0x70000063]):
 transfer_answer=0
 configure_answer={'success':0,'context':0x0d000001,'object':0x07000103,'state':0x7fffffff}[rejection]
 entry=0x8c66854e;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_fixed_block_transfer.dll')).sa_sound_driver_fixed_block_transfer
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 clock_target=0x8c60260e;configure_target=0x8c643624;transfer_target=struct.unpack_from('<I',binary,0x6690fc-0x10000)[0];callback_target=struct.unpack_from('<I',binary,0x666d00-0x10000)[0];error_target=0x8c667b7a
 timestamps=lit(0x8c6686d8);struct.pack_into('<3I',ram,timestamps,0xa5a5a5a5,0xa5a5a5a5,0xa5a5a5a5)
 for channel in (-1,0,1,2):struct.pack_into('<I',ram,0xa08020+4*channel,0x8ca10000+64*(channel+1))
 mode=0x1200;address=0x8ca20000;length=32;struct.pack_into('<I',ram,0xf00000,length)
 target_handle=0x8ca10000+64*(ctypes.c_int8(selector).value+1)
 object_base=target_handle&0xffffff;initial_object=(1,2 if rejection=='state' else 1,0x55,0x66,0x77,0x88,0x99)
 struct.pack_into('<7I',ram,object_base,*initial_object)
 struct.pack_into('<I',ram,0x67c1d0,0 if rejection=='context' else 0x8ca09000)
 if rejection=='object':struct.pack_into('<I',ram,0xa08020+4*ctypes.c_int8(selector).value,0);target_handle=0
 struct.pack_into('<I',ram,0x5fbb14,0x8ca00200);struct.pack_into('<I',ram,0xa00200,0xa05f7800)
 struct.pack_into('<I',ram,0xa002d4,0)
 for channel in range(4):struct.pack_into('<I',ram,0xa00204+channel*52,channel)
 data=bytes((i*17+9)&255 for i in range(length+16));ram[0xa20000:0xa20000+len(data)]=data
 timer_reads=[[],[]]
 def timer_value(which):
  value=(counter_start-0x1000*len(timer_reads[which]))&0xffffffff;timer_reads[which].append(value);return value
 class TimerMap(dict):
  def __getitem__(self,a):
   return timer_value(1) if a==0xffd8000c else super().__getitem__(a)
 mmio=TimerMap({0xffd8000c:0,0xa05f688c:0})
 for channel in range(4):
  mmio[0xa05f7818+channel*32]=(bitmap>>channel)&1
  mmio[0xa05f781c+channel*32]=0xabcdef01
 for offset in range(0,length+16,4):mmio[0xa0801200+offset]=0x55555555
 registers=dict(mmio)
 @Read
 def read(ctx,a,n):
  if n!=4 or a not in registers:errors.append(('bus-read',a,n));return 0
  return timer_value(0) if a==0xffd8000c else registers[a]
 @Write
 def write(ctx,a,v,n):
  if n!=4 or a not in registers or a==0xffd8000c:errors.append(('bus-write',a,v,n));return
  registers[a]=v
 io=IO(None,read,write)
 context_offset=struct.unpack_from('<h',binary,0x65a6b4-0x10000)[0]
 struct.pack_into('<I',ram,0xa09000+context_offset,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];ticks=[0,0];configure_stack=[];configure_calls=[];reference_stack=[];submit_stack=[];reference_submit_stack=[];copy_calls=[]
 def service(a,r,which):
  if a==clock_target:ticks[which]+=1;return 0x1000*ticks[which]
  if a==configure_target:return configure_answer
  if a==transfer_target:return transfer_answer
  if a==callback_target:return 999
  assert a==error_target;return 777
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==transfer_target:copy_calls.append((a,r[4],r[5],r[6],r[7]))
  if a==transfer_target:submit_stack.append(sum(int(m[(r[15]&0xffffff)+j])<<(8*j) for j in range(4)))
  if a==configure_target:configure_stack.append(sum(int(m[(r[15]&0xffffff)+j])<<(8*j) for j in range(4)))
  if a==clock_target:return clock(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if a==configure_target:configure_calls.append(tuple(r[i] for i in range(4,8)))
  if a in io_helpers:return io_helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=service(a,r,0);return 0
 def hook(a):
  def run(m,r,sr):
   if a==transfer_target:reference_submit_stack.append(struct.unpack_from('<I',m,r[15]&0xffffff)[0])
   if a==configure_target:reference_stack.append(struct.unpack_from('<I',m,r[15]&0xffffff)[0])
   events.append((a,r[4],r[5],r[6],r[7]));r[0]=service(a,r,1);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=selector&0xffffffff;s.r[6]=mode;s.r[7]=address;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,mmio=mmio,hooks={a:hook(a) for a in [*addresses,transfer_target,callback_target] if a not in (entry,clock_target,*helpers,*io_helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and configure_stack==[0]
 assert configure_calls==[(target_handle,address,length,0)]
 expected_sequence=[]
 if configure_answer:expected_sequence.append(error_target)

 assert [x[0] for x in calls]==expected_sequence
 if not configure_answer:
  assert copy_calls[0]==(transfer_target,address,(mode+struct.unpack_from('<I',binary,0x6690f8-0x10000)[0])&0xffffffff,4,length//4)
  assert struct.unpack_from('<I',native,lit(0x8c669100))[0]==transfer_answer
 for x in calls:
  if x[0]==error_target:assert x[1]==0x8c000000+lit(0x8c6686e4 if configure_answer else 0x8c6686ec)
 timer_count=0
 assert timer_reads[0]==timer_reads[1]==[(counter_start-0x1000*i)&0xffffffff for i in range(timer_count)]
 expected_times=[(~v)&0xffffffff for v in timer_reads[0]]+[0xa5a5a5a5]*(3-timer_count)
 assert struct.unpack_from('<3I',native,timestamps)==tuple(expected_times)
 assert r[0]==configure_answer
 if configure_answer:
  assert struct.unpack_from('<7I',native,object_base)==initial_object
  assert struct.unpack_from('<2I',native,0x67c1c8)==(configure_target,configure_answer)
 else:
  assert struct.unpack_from('<5I',native,object_base+4)==(1,address,length,0,0)
  assert submit_stack==[1]
  current_offset=struct.unpack_from('<h',binary,0x666ce4-0x10000)[0]
  assert struct.unpack_from('<I',native,0xa09000+current_offset)[0]==0
  assert struct.unpack_from('<I',native,0xa09000+current_offset-8)[0]==1
  assert struct.unpack_from('<I',native,0xa09000+current_offset-4)[0]==2
  assert struct.unpack_from('<I',native,object_base+24)[0]==0
  assert struct.unpack_from('<I',native,lit(0x8c65a6cc))[0]==transfer_answer
 assert dict(mmio)==registers
 copied=b''.join(struct.pack('<I',registers[0xa0801200+i]) for i in range(0,length+16,4))
 assert copied==(bytes([0x55])*(length+16) if configure_answer else data[:length]+bytes([0x55])*16)
 assert native[0xa20000:0xa20000+len(data)]==data
 for channel in range(4):assert registers[0xa05f781c+channel*32]==(0 if not configure_answer and bitmap&(1<<channel) else 0xabcdef01)
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_fixed_block_transfer_native_bus_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native55-instruction fixed32-byte driver transfer66854e connected to configuration/consumer/begin/submit/frame services/callback dispatch/pending getter and G2 strided copy/channel controls. Full retail RAM/CPU/FPU/SR/PR/MACL,MMIO and diagnostics match. Independent exact32-byte payload,16-byte preserved destination tail,unchanged source,channel release,object idle/context counters/global result checked. Context/null object/busy configuration rejections preserve destination/channels. No timer reads. G2 bus registers simulated; physical audio/full-game operation unverified.'),indent=2)+'\n')
print(cases,'driver fixed block transfer native bus comparisons passed')
