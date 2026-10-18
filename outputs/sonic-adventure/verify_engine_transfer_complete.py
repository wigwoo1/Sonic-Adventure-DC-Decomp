import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_complete_io.dll')).sa_engine_transfer_complete_io
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
 cases=0
 for channel,counter,callback,bank in itertools.product(range(4),[0,0xffffffff],[0,0x8c64241a],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200);struct.pack_into('<I',ram,0xa00200,0x8ca00400)
  struct.pack_into('<I',ram,0x5fbb0c,0xa05f6900)
  struct.pack_into('<I',ram,0xa002ec+channel*4,counter)
  struct.pack_into('<I',ram,0xa00000,channel)
  struct.pack_into('<2I',ram,0xa0001c,callback,0x8ca01000)
  struct.pack_into('<2I',ram,0xa0002c,1,0)
  struct.pack_into('<I',ram,0xa00400+channel*32+20,1)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  writes=[];ref_writes=[];calls=[]
  @Read
  def read(ctx,a,n):errors.append(('read',a,n));return 0
  @Write
  def write(ctx,a,v,n):writes.append((a,v,n))
  io=IO(None,read,write)
  class Registers(dict):
   def __setitem__(self,a,v):ref_writes.append((a,v,4));super().__setitem__(a,v)
  mmio=Registers({0xa05f6900:0})
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   calls.append((t,r[4]))
   if t==0x8c64241a:return 0
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10ebe8;s.r[4]=0x8ca00000;s.r[5]=0x8ca00100
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,mmio=mmio)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<2I',native,0xa0002c)==(0,1)
  assert struct.unpack_from('<I',native,0xa00400+channel*32+20)[0]==0
  assert struct.unpack_from('<I',native,0xa002ec+channel*4)[0]==((counter+1)&0xffffffff)
  assert writes==ref_writes==[(0xa05f6900,1<<(channel+15),4)]
  assert calls==([(callback,0x8ca01000)] if callback else [])
  cases+=1
 (out/'analysis/engine_transfer_complete_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=39,scope='Full retail RAM/CPU/FPU and interrupt acknowledgement comparisons for transfer completion. Independent pending/completed flags, channel-state clearing, counter wrap and optional callback target/payload checks. Native dispatcher models verified retail RTS/NOP callback while reference executes it. Caller/channel states are fixtures; actual hardware execution/completion delivery remains absent.'),indent=2)+'\n')
 print(cases,'transfer completion comparisons passed')
if __name__=='__main__':main()
