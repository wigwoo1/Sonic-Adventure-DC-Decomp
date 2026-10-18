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
 fn=processor.sa_sound_request_process;fn.argtypes=abi;cases=0
 for mode,bank,count,index in itertools.product([0,1],[0,0x200000],[0,1,3],[0,3]):
  address=0x1200
  value=1
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca01000);struct.pack_into('<I',ram,0xa01000,0xa05f7800);struct.pack_into('<I',ram,0xa010d4,mode)
  for slot in range(4):struct.pack_into('<I',ram,0xa01004+slot*52,slot)
  ram[0xa00000:0xa00004]=bytes([0x55])*4
  mmio={0xa05f688c:0,**{0xa0801200+i:0x1234ab00 for i in range(0,512,4)}}
  for slot in range(4):mmio[0xa05f7818+slot*32]=0
  struct.pack_into('<I',ram,0x67c1d0,0x8ca03000)
  struct.pack_into('<2I',ram,0xa0315c,0,0)
  struct.pack_into('<I',ram,0xa031c4,address);struct.pack_into('<I',ram,0xa031bc,address);struct.pack_into('<I',ram,0xa03158,0x8ca02000)
  ram[0xa02000:0xa02200]=bytes([0x55])*512
  struct.pack_into('<I',ram,0xa03178,0x8ca00000)
  struct.pack_into('<7I',ram,0xa00000,1,0,0,0,0,0,0)
  struct.pack_into('<I',ram,0xa03160,0)
  struct.pack_into('<5I',ram,0xa03164,0x8ca04000,4,count,0,index)
  expected_packed=bytearray([0x55]*512);requests=[]
  for slot in range(4):
   entry=bytearray((slot*7+i*13+3)&255 for i in range(28));struct.pack_into('<I',entry,20,0)
   ram[0xa04000+slot*28:0xa0401c+slot*28]=entry
  for order in range(count):
   slot=(index+order)%4;offset=0xa04000+slot*28
   struct.pack_into('<I',ram,offset+20,1);entry=bytes(ram[offset:offset+28]);requests.append((slot,entry))
   expected_packed[order*16]=entry[0];expected_packed[order*16+2:order*16+16]=entry[4:18]
  native=bytearray(ram);registers=dict(mmio);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Read
  def read(ctx,a,n):
   if a not in registers or n!=4:errors.append(('read',a,n));return 0
   return registers[a]
  @Write
  def write(ctx,a,v,n):
   if not 0xa0801200<=a<0xa0801400 or n!=4:errors.append(('write',a,v,n))
   registers[a]=v
  io=IO(None,read,write)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in functions:errors.append(t);return -9
   function,hasio=functions[t];args=[m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl)]
   if hasio:args.append(ctypes.byref(io))
   return function(*args,None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65a378
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={"macl":0};reference(ram,s,fpu=f,mmio=mmio,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and registers==mmio and macl.value==special["macl"]
  expected_buffer=bytearray(expected_packed)
  padded_count=count+(count&1)
  for record_index in range(padded_count):expected_buffer[record_index*16]=0
  assert native[0xa02000:0xa02200]==expected_buffer
  assert struct.unpack_from('<2I',native,0xa0315c)==(0,0)
  expected_g2=bytearray(struct.pack('<I',0x1234ab00)*128)
  if count:
   expected_g2[:padded_count*16]=expected_packed[:padded_count*16];expected_g2[0]=1
   assert struct.unpack_from('<I',native,0xa00004)[0]==1
   assert struct.unpack_from('<3I',native,0xa03188)==(1,2,0)
   assert struct.unpack_from('<2I',native,0xa00010)==(0x8c666ad0,0x8ca03158)
  else:
   assert struct.unpack_from('<I',native,0xa00004)[0]==0
   assert struct.unpack_from('<3I',native,0xa03188)==(0,0,0)
  assert b''.join(struct.pack('<I',registers[0xa0801200+i]) for i in range(0,512,4))==expected_g2
  assert struct.unpack_from('<I',native,0xa0316c)[0]==0
  assert struct.unpack_from('<I',native,0xa03174)[0]==((index+count)%4 if count else index),(count,index,struct.unpack_from("<I",native,0xa03174)[0])
  for slot,entry in requests:
   expected_request=bytearray(entry);struct.pack_into('<I',expected_request,20,0)
   assert native[0xa04000+slot*28:0xa0401c+slot*28]==expected_request
  cases+=1
 (root/'analysis/sound_request_process_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native sound work dispatcher configures object, synchronous consumer transfers packed buffer, real transfer callback dispatches native command completion with status read-modify-write and work clearing. Complete retail RAM/CPU/FPU/MMIO comparisons plus independent even-record padding, copied/status bytes, preserved tail, emptied work flags/counters, object state, callback payload and transfer counters for 0/1/32 records, transfer modes and banks. Context/table/object/bus fixtures; request selection traverses a wrapping 4-record pool, reads zero status byte and native enqueue packs/releases active records; independent selected record/payload, circular index and pending-counter assertions. Native startup not executed here.'),indent=2)+'\n')
 print(cases,'sound request processing comparisons passed')
if __name__=='__main__':main()
