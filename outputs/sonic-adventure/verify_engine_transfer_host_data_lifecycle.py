import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Cache,Read,Write,Writeback
from verify_g2_transfer_host import Job,Bus,Read as BusRead,Write as BusWrite
def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();cases=0
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialized_lifecycle.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 acquire=lib.sa_engine_transfer_descriptor_acquire;acquire.argtypes=abi
 event_dispatch=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/interrupt_event_dispatch.dll')).sa_interrupt_event_dispatch;event_dispatch.argtypes=abi
 initialize=lib.sa_engine_transfer_descriptor_initialize;initialize.argtypes=abi
 arm=lib.sa_engine_transfer_channels_arm;arm.argtypes=abi
 channel_arm=lib.sa_engine_transfer_channel_arm_io;channel_arm.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
 complete=lib.sa_engine_transfer_complete_io;complete.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
 submit=lib.sa_engine_transfer_submit_io;submit.argtypes=channel_arm.argtypes
 busy_get=lib.sa_engine_transfer_busy_get_io;busy_get.argtypes=channel_arm.argtypes
 cache_fn=lib.sa_engine_cache_writeback_io;cache_fn.argtypes=abi[:7]+[ctypes.POINTER(Cache)]+abi[7:]
 engine_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_initialize_native.dll')).sa_engine_context_initialize_native;engine_init.argtypes=abi
 transfer_init=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialize_host.dll')).sa_engine_transfer_initialize_host;transfer_init.argtypes=abi+[ctypes.POINTER(IO),ctypes.c_uint32]
 g2=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/g2_transfer_host.dll'))
 capture=g2.sa_g2_transfer_capture;capture.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Job)]
 execute=g2.sa_g2_transfer_execute;execute.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),P,P]
 for channel,busy,mode,length,counter,bank,callback in itertools.product([0,1,2,3],[0],[0,1],[0,32,96],[0],[0,0x200000],[0,0x8c64241a]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  registers={0xa05f6920:0xffffffff,0xa05f6900:0,**{0xa05f7800+o:0 for o in range(0,0xc0,4)}}
  native_registers=dict(registers)
  struct.pack_into('<5I',ram,0xf00000,1,1,1,callback,0x8ca01000)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);trace=[];ref_trace=[];errors=[];writes=[];calls=[];handler_calls=[];jobs=[];g2_data=bytearray((i*13+channel)&255 for i in range(256));before_g2=bytes(g2_data)
  @Read
  def read(ctx,a,n):
   if a not in native_registers or n!=4:errors.append(('read',a,n));return 0
   return native_registers[a]
  @Write
  def write(ctx,a,v,n):
   writes.append((a,v,n));native_registers[a]=v
   if 0xa05f7800<=a<0xa05f7880 and (a-0xa05f7800)%32==24 and v&1:
    captured_channel=(a-0xa05f7800)//32
    values=(ctypes.c_uint32*8)(*[native_registers[0xa05f7800+captured_channel*32+i*4] for i in range(8)])
    job=Job();result=capture(captured_channel,values,ctypes.byref(job))
    if result==0:jobs.append(job)
    else:errors.append(('capture',result))
  @Writeback
  def writeback(ctx,a):trace.append(a);return 0
  cache=Cache(IO(None,read,write),writeback)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c1101f0:return busy_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache.registers),None,call)
   if t==0x8c5fc58a:return cache_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache),None,call)
   if t==0x8c10fa60:return channel_arm(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache.registers),None,call)
   if t==0x8c10ebe8:
    handler_calls.append((t,r[4]))
    return complete(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache.registers),None,call)
   if t==0x8c64241a:calls.append((t,r[4]));return 0
   errors.append(('call',t));return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.r[4]=0x8ca00000;s.r[5]=0xa0801200;s.r[6]=0x8ca02000;s.r[7]=length;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10f9f4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={'macl':0}
  s.pc=0x8c6510a4
  reference(ram,s,fpu=f,special=special)
  assert engine_init(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  s.pr=pr.value=0x8c000100;s.pc=0x8c10d5a8
  reference(ram,s,fpu=f,special=special,mmio=registers,vbr=0x8ca08000)
  assert transfer_init(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,ctypes.byref(cache.registers),0x8ca08000)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and native_registers==registers
  assert struct.unpack_from('<I',native,0x5fbb14)[0]==0x8c7ac7a0
  writes.clear()
  s.pr=pr.value=0x8c000100
  s.r[4]=r[4]=0x8ca00000;s.r[5]=r[5]=channel;s.pc=0x8c10ec40
  reference(ram,s,fpu=f,special=special)
  assert acquire(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  descriptor=struct.unpack_from('<I',native,0xa00000)[0];desc_offset=descriptor&0xffffff
  assert descriptor==0x8c7ac7a4+channel*52
  s.r[4]=r[4]=descriptor;s.r[5]=r[5]=0xa0801200;s.r[6]=r[6]=0x8ca02000;s.r[7]=r[7]=length;s.pr=pr.value=0x8c000100;s.pc=0x8c10f9f4
  reference(ram,s,fpu=f,special=special)
  assert initialize(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  s.r[4]=r[4]=descriptor;s.r[5]=r[5]=mode;s.pr=pr.value=0x8c000100;s.pc=0x8c10faa4
  before=bytes(native[0xa00400+channel*32:0xa00420+channel*32])
  reference(ram,s,fpu=f,cache_writeback=ref_trace.append,special=special,mmio=registers)
  assert submit(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache.registers),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and trace==ref_trace
  assert r[0]==(0x90180 if busy else 0)
  assert struct.unpack_from('<I',native,0x7ac87c+channel*4)[0]==(counter if busy else (counter+1)&0xffffffff)
  if busy:
   assert not trace and native[0xa00400+channel*32:0xa00420+channel*32]==before
   assert struct.unpack_from('<I',native,desc_offset+0x2c)[0]==0
  else:
   assert trace==[0x0ca02000+i*32 for i in range(length//32)]
   expected_flags=7 if mode else 5
   assert tuple(native_registers[0xa05f7800+channel*32+o] for o in range(0,28,4))==(0x00801200,0x0ca02000,0x80000000|length,1,expected_flags,1,1), (channel,busy,mode,length,hex(expected_flags),tuple(native_registers[0xa05f7800+channel*32+o] for o in range(0,28,4)))
   assert struct.unpack_from('<I',native,desc_offset+0x2c)[0]==1
  s.pr=pr.value=0x8c000100;s.pc=0x8c10ee9c
  reference(ram,s,fpu=f,special=special,mmio=registers)
  assert arm(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert struct.unpack_from('<I',native,desc_offset+24)[0]==2
  assert native_registers[0xa05f7800+channel*32+28]==1
  assert struct.unpack_from('<I',native,0x7ac8c0)[0]==1
  assert not calls
  assert native_registers==registers
  assert len(jobs)==1
  job=jobs.pop();assert (job.channel,job.g2_address,job.root_address,job.length,job.direction)==(channel,0x00801200,0x0ca02000,0x80000000|length,1)
  @BusRead
  def bus_read(ctx,a,buffer,n):
   if a<0x00801200 or a+n>0x00801300:errors.append(('bus_read',a,n));return -8
   for i,v in enumerate(g2_data[a-0x00801200:a-0x00801200+n]):buffer[i]=v
   return 0
  @BusWrite
  def bus_write(ctx,a,buffer,n):
   if a<0x0ca02000 or a+n>0x0ca02100:errors.append(('bus_write',a,n));return -8
   native[a&0xffffff:(a&0xffffff)+n]=bytes(buffer[:n]);return 0
  bus=Bus(None,bus_read,bus_write);transferred=ctypes.c_uint32();completion_event=ctypes.c_uint32()
  assert execute(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(transferred),ctypes.byref(completion_event))==0 and not errors
  assert transferred.value==length and completion_event.value==0xa60+channel*32
  ram[0xa02000:0xa02000+length]=before_g2[:length]
  assert native==ram and g2_data==before_g2
  writes.clear()
  ref_writes=[]
  class Registers(dict):
   def __setitem__(self,a,v):ref_writes.append((a,v,4));super().__setitem__(a,v)
  mmio=Registers(registers)
  s.r[4]=r[4]=completion_event.value;s.pr=pr.value=0x8c000100;s.pc=0x8c645afa
  reference(ram,s,fpu=f,special=special,mmio=mmio)
  assert event_dispatch(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert struct.unpack_from('<2I',native,desc_offset+44)==(0,1)
  assert native_registers[0xa05f7800+channel*32+20]==0
  assert struct.unpack_from('<I',native,0x7ac88c+channel*4)[0]==((counter+1)&0xffffffff)
  assert writes==ref_writes==[(0xa05f7800+channel*32+20,0,4),(0xa05f6900,1<<(channel+15),4)]
  assert calls==([(callback,0x8ca01000)] if callback else [])
  assert handler_calls==[(0x8c10ebe8,descriptor)]
  assert native_registers==mmio
  cases+=1
 (ROOT/'outputs/sonic-adventure/analysis/engine_transfer_host_data_lifecycle_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native engine/transfer startup, acquire/init/submit/arm, register-trigger captured job, authored C host G2 byte transfer, derived event and recovered registered completion. Independent copied bytes/source preservation/job snapshot/event assertions plus full retail state comparison before/after modeled DMA. Reference DMA effect is independently applied, not retail CPU instructions. No actual hardware timing/exception entry; bus region and event scheduling remain test-driven. Only G2-to-root direction integrated here. Full game boot/renderer/audio absent.'),indent=2)+'\n')
 print(cases,'host-data transfer lifecycle comparisons passed')
if __name__=='__main__':main()



