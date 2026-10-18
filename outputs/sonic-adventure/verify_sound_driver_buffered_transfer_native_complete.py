import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_buffered_transfer.dll')).sa_sound_driver_buffered_transfer;fn.argtypes=abi
division=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll')).sa_signed_division;division.argtypes=abi
from verify_engine_cache_writeback import IO,Read,Write
support=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_async_completion.dll'))
reader=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_status_get.dll'))
helpers={}
for a,name,hasio,owner in [(0x8c10d460,'sa_engine_transfer_read_strided_io',True,reader),(0x8c10ee9c,'sa_engine_transfer_channels_arm',False,support),(0x8c10fa60,'sa_engine_transfer_channel_arm_io',True,support),(0x8c10eee0,'sa_engine_transfer_ready_wait',False,support),(0x8c10fb48,'sa_engine_transfer_ready_get_io',True,support),(0x8c10ee20,'sa_engine_transfer_channels_release',False,support),(0x8c10f9bc,'sa_engine_transfer_channel_release_io',True,support),(0x8c1101f0,'sa_engine_transfer_busy_get_io',True,support)]:
 helper=getattr(owner,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];helpers[a]=(helper,hasio)
buffer_read=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_buffer_read.dll')).sa_sound_buffer_read;buffer_read.argtypes=abi
error_record=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll')).sa_engine_error_record;error_record.argtypes=abi
objects=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
for a,name in [(0x8c643624,'sa_engine_frame_object_configure'),(0x8c65a568,'sa_engine_object_state_get'),(0x8c65a5cc,'sa_engine_object_state_set'),(0x8c65a5ac,'sa_engine_object_address_set'),(0x8c65a5d4,'sa_engine_object_length_set'),(0x8c65a5b4,'sa_engine_object_callback_set')]:
 helper=getattr(objects,name);helper.argtypes=abi;helpers[a]=(helper,False)
block=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_block_transfer.dll')).sa_sound_driver_block_transfer;block.argtypes=abi
clock=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_timer_helpers.dll')).sa_file_clock_get_io;clock.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:];helpers[0x8c60260e]=(clock,True)
for a,name,hasio in [(0x8c65a5dc,'sa_engine_queue_consume_direct',False),(0x8c666c2c,'sa_engine_frame_transfer_begin',False),(0x8c669088,'sa_engine_frame_transfer_submit',False),(0x8c668f40,'sa_engine_frame_services',False),(0x8c65a518,'sa_engine_object_callback_dispatch',False),(0x8c66abfc,'sa_engine_pending_count_get',False),(0x8c10d770,'sa_engine_transfer_copy_strided_io',True),(0x8c10ebac,'sa_engine_transfer_fifo_wait',False),(0x8c10f9a8,'sa_engine_transfer_fifo_ready_get_io',True)]:
 helper=getattr(support,name);helper.argtypes=abi[:7]+([ctypes.POINTER(IO)] if hasio else [])+abi[7:];helpers[a]=(helper,hasio)
copy=0x8c669014;transfer=0x8c6684c4;cases=0
for index,start,count,bank,sr0 in itertools.product([0,1],[-1,0,63],[0,1,63,64,128],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 handle=0x8ca08000;buffer=0x8ca30000;source=0x1200
 struct.pack_into('<2I',ram,0xa08030,source,source)
 struct.pack_into('<I',ram,0xf00000,buffer)
 struct.pack_into('<I',ram,0x67c1d0,0x8ca09000)
 for channel in range(2):
  struct.pack_into('<I',ram,0xa08018+4*channel,0x8ca10000+64*channel)
  struct.pack_into('<7I',ram,0xa10000+64*channel,1,1,0,0,0,0,0)
 struct.pack_into('<I',ram,0x5fbb14,0x8ca01000);struct.pack_into('<I',ram,0xa01000,0xa05f7800);struct.pack_into('<I',ram,0xa010d4,0)
 for channel in range(4):struct.pack_into('<I',ram,0xa01004+channel*52,channel)
 mmio={0xa05f688c:0,0xffd8000c:0x12345678}
 for channel in range(4):mmio[0xa05f7818+channel*32]=0
 for offset in range(-256,16384,4):mmio[0xa0800000+source+offset]=(0xabcdef01+offset*0x12345)&0xffffffff
 registers=dict(mmio);initial_registers=dict(mmio);errors=[]
 @Read
 def read(ctx,a,n):
  if n!=4 or a not in registers:errors.append(('read',a,n));return 0
  return registers[a]
 @Write
 def write(ctx,a,v,n):
  if n!=4 or a not in registers:errors.append(('write',a,v,n));return
  registers[a]=v
 io=IO(None,read,write)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);calls=[];events=[];leafcalls=[];consumes=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c65acac:return error_record(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c65a5dc:consumes.append((a,r[4],r[5],r[6],r[7],None))
  if a in helpers:
   helper,hasio=helpers[a];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return helper(*args,None,call)
  if a in (0x8c65a5dc,0x8c667b7a):
   leafcalls.append((a,r[4],r[5],r[6],r[7],None));r[0]=123;return 0
  assert a in (copy,transfer),a
  stack=sum(int(m[(r[15]&0xffffff)+j])<<(j*8) for j in range(4)) if a==transfer else None
  calls.append((a,r[4],r[5],r[6],r[7],stack))
  if a==copy:return buffer_read(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  return block(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 def hook(a):
  def run(m,r,sr):
   stack=struct.unpack_from('<I',m,r[15]&0xffffff)[0] if a==transfer else None
   events.append((a,r[4],r[5],r[6],r[7],stack))
   if a==copy:m[r[5]&0xffffff:(r[5]&0xffffff)+r[6]]=m[r[4]&0xffffff:(r[4]&0xffffff)+r[6]]
   r[0]=123;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=index;s.r[6]=start&0xffffffff;s.r[7]=count;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c6687f4
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,mmio=mmio,hooks={a:hook(a) for a in (0x8c667b7a,)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and leafcalls==events and not errors and mmio==registers
 expected=[];active=buffer;other=buffer+256
 def signed_mod64(v):return (abs(v)%64)*(-1 if v<0 else 1)
 for outer in range(count):
  ring=signed_mod64(start+outer)
  expected.append((copy,source+ring*256,active,256))
  for inner in range(64//count):
   next_ring=signed_mod64(ring-start+64)
   dest=source+next_ring*256
   expected.append((copy,dest,other,256))
   expected.append((transfer,handle,index,dest,active,256))
   ring=next_ring;active,other=other,active
 assert len(calls)==len(expected),(index,start,count,len(calls),len(expected),calls[:3])
 for actual,wanted in zip(calls,expected):assert actual[:len(wanted)]==wanted,(count,start,actual,wanted)
 expected_buffers={buffer:bytes(256),buffer+256:bytes(256)};oracle=dict(initial_registers)
 for wanted in expected:
  if wanted[0]==copy:expected_buffers[wanted[2]]=b''.join(struct.pack('<I',oracle[0xa0800000+wanted[1]+j]) for j in range(0,256,4))
  else:
   payload=expected_buffers[wanted[4]]
   for j in range(0,256,4):oracle[0xa0800000+wanted[3]+j]=struct.unpack_from('<I',payload,j)[0]
 assert registers==oracle
 for address,payload in expected_buffers.items():assert native[address&0xffffff:(address&0xffffff)+256]==payload
 transfers=[x for x in expected if x[0]==transfer]
 if transfers:
  final=transfers[-1];obj=0xa10000+64*index
  assert struct.unpack_from('<5I',native,obj+4)==(1,final[4],256,0,0)
  consume_calls=consumes
  assert len(consume_calls)==len(transfers)
  for observed,wanted in zip(consume_calls,transfers):assert observed[1:3]==(0x8ca10000+64*index,wanted[3])
 cases+=1
(out/'analysis/sound_driver_buffered_transfer_native_complete_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native buffered routine/division,G2 read,block wrapper/configuration/timer,queue consumer/begin/submit/frame services/object dispatcher/pending getter,G2 read/write/channel/FIFO helpers connected. Full retail RAM/CPU/FPU/SR/PR/MMIO and diagnostic traces match. Independent sequential sound-memory read/write oracle,exact buffers,transfer counts/ring offsets/arguments,idle descriptor/selected object checked. Channels0/1,start-1/0/63,count0/1/63/64/128,banks/SR. G2/timer registers synthetic; physical audio/full-game runtime unverified.'),indent=2)+'\n')
print(cases,'buffered transfer native completion comparisons passed')
