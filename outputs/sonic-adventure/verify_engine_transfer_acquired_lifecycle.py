import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Cache,Read,Write,Writeback
def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();cases=0
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_acquired_lifecycle.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 acquire=lib.sa_engine_transfer_descriptor_acquire;acquire.argtypes=abi
 initialize=lib.sa_engine_transfer_descriptor_initialize;initialize.argtypes=abi
 arm=lib.sa_engine_transfer_channels_arm;arm.argtypes=abi
 channel_arm=lib.sa_engine_transfer_channel_arm;channel_arm.argtypes=abi
 complete=lib.sa_engine_transfer_complete_io;complete.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]
 submit=lib.sa_engine_transfer_submit;submit.argtypes=abi
 busy_get=lib.sa_engine_transfer_busy_get;busy_get.argtypes=abi
 cache_fn=lib.sa_engine_cache_writeback_io;cache_fn.argtypes=abi[:7]+[ctypes.POINTER(Cache)]+abi[7:]
 for channel,busy,mode,length,counter,bank,callback in itertools.product([0,1,2,3],[0],[0,1],[0,32,96],[0,0xffffffff],[0,0x200000],[0,0x8c64241a]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200);struct.pack_into('<I',ram,0xa00200,0x8ca00400)
  struct.pack_into('<I',ram,0xa00400+channel*32+24,busy)
  struct.pack_into('<I',ram,0xa002dc+channel*4,counter)
  struct.pack_into('<I',ram,0xa002ec+channel*4,counter)
  struct.pack_into('<I',ram,0x5fbb0c,0xa05f6900)
  for index in range(4):struct.pack_into('<I',ram,0xa00204+index*52,index)
  struct.pack_into('<5I',ram,0xf00000,1,1,1,callback,0x8ca01000)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);trace=[];ref_trace=[];errors=[];writes=[];calls=[]
  @Read
  def read(ctx,a,n):errors.append(('read',a,n));return 0
  @Write
  def write(ctx,a,v,n):writes.append((a,v,n))
  @Writeback
  def writeback(ctx,a):trace.append(a);return 0
  cache=Cache(IO(None,read,write),writeback)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c1101f0:return busy_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t==0x8c5fc58a:return cache_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache),None,call)
   if t==0x8c10fa60:return channel_arm(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t==0x8c64241a:calls.append((t,r[4]));return 0
   errors.append(('call',t));return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.r[4]=0x8ca00000;s.r[5]=0xa0801200;s.r[6]=0x8ca02000;s.r[7]=length;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10f9f4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={'macl':0}
  s.r[4]=r[4]=0x8ca00000;s.r[5]=r[5]=channel;s.pc=0x8c10ec40
  reference(ram,s,fpu=f,special=special)
  assert acquire(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  descriptor=struct.unpack_from('<I',native,0xa00000)[0];desc_offset=descriptor&0xffffff
  assert descriptor==0x8ca00204+channel*52
  s.r[4]=r[4]=descriptor;s.r[5]=r[5]=0xa0801200;s.r[6]=r[6]=0x8ca02000;s.r[7]=r[7]=length;s.pr=pr.value=0x8c000100;s.pc=0x8c10f9f4
  reference(ram,s,fpu=f,special=special)
  assert initialize(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  s.r[4]=r[4]=descriptor;s.r[5]=r[5]=mode;s.pr=pr.value=0x8c000100;s.pc=0x8c10faa4
  before=bytes(native[0xa00400+channel*32:0xa00420+channel*32])
  reference(ram,s,fpu=f,cache_writeback=ref_trace.append,special=special)
  assert submit(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and trace==ref_trace
  assert r[0]==(0x90180 if busy else 0)
  assert struct.unpack_from('<I',native,0xa002dc+channel*4)[0]==(counter if busy else (counter+1)&0xffffffff)
  if busy:
   assert not trace and native[0xa00400+channel*32:0xa00420+channel*32]==before
   assert struct.unpack_from('<I',native,desc_offset+0x2c)[0]==0
  else:
   assert trace==[0x0ca02000+i*32 for i in range(length//32)]
   expected_flags=7 if mode else 5
   assert struct.unpack_from('<7I',native,0xa00400+channel*32)==(0x00801200,0x0ca02000,0x80000000|length,1,expected_flags,1,1), (channel,busy,mode,length,hex(expected_flags),struct.unpack_from('<7I',native,0xa00400+channel*32))
   assert struct.unpack_from('<I',native,desc_offset+0x2c)[0]==1
  s.pr=pr.value=0x8c000100;s.pc=0x8c10ee9c
  reference(ram,s,fpu=f,special=special)
  assert arm(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert struct.unpack_from('<I',native,desc_offset+24)[0]==2
  assert struct.unpack_from('<I',native,0xa00400+channel*32+28)[0]==1
  assert struct.unpack_from('<I',native,0x7ac8c0)[0]==1
  assert not writes and not calls
  ref_writes=[]
  class Registers(dict):
   def __setitem__(self,a,v):ref_writes.append((a,v,4));super().__setitem__(a,v)
  mmio=Registers({0xa05f6900:0})
  s.r[4]=r[4]=descriptor;s.pr=pr.value=0x8c000100;s.pc=0x8c10ebe8
  reference(ram,s,fpu=f,special=special,mmio=mmio)
  assert complete(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(cache.registers),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert struct.unpack_from('<2I',native,desc_offset+44)==(0,1)
  assert struct.unpack_from('<I',native,0xa00400+channel*32+20)[0]==0
  assert struct.unpack_from('<I',native,0xa002ec+channel*4)[0]==((counter+1)&0xffffffff)
  assert writes==ref_writes==[(0xa05f6900,1<<(channel+15),4)]
  assert calls==([(callback,0x8ca01000)] if callback else [])
  cases+=1
 (ROOT/'outputs/sonic-adventure/analysis/engine_transfer_acquired_lifecycle_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native acquired descriptor -> initialization -> successful submission -> four-channel arm -> explicit completion handler. Full retail RAM/CPU/FPU/MACL comparisons and independent states/counters/cache traces/acknowledgement/callback checks. No manual descriptor or pending-state construction. Fixed channel table fixture and Python scheduling remain; completion is directly invoked without actual data transfer or hardware interrupt delivery. Optional native no-op callback models retail RTS/NOP.'),indent=2)+'\n')
 print(cases,'acquired transfer lifecycle comparisons passed')
if __name__=='__main__':main()


