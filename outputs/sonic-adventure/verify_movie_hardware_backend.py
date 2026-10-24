"""Check raw FIFO ownership, byte/word control storage and prior bus delegation."""
import ctypes,hashlib,itertools,json,random
from pathlib import Path
import verify_movie_hardware as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);m=v.HardwareMemory();bus=v.effects.Bus();out=ctypes.c_uint32();checks=0
 init=v.effects.LIB.sa_hardware_transfer_bus_init;configure=v.effects.LIB.sa_hardware_transfer_set_samples
 init(ctypes.byref(m),view,ctypes.byref(bus),0xa35a,0x12345678)
 assert not m.fifo_samples and not m.fifo_count and not m.fifo_cursor and m.control_word==0x12345678;checks+=1
 out.value=0xabcdef01;before=bytes(m);assert bus.read(bus.context,v.FIFO,4,ctypes.byref(out))==-4 and bytes(m)==before and out.value==0xabcdef01;checks+=1
 samples=[i*0x10040101&0xffffffff for i in range(5000)];storage=(ctypes.c_uint32*len(samples))(*samples);assert configure(ctypes.byref(m),storage,len(samples))==0;checks+=1
 for i,value in enumerate(samples):
  a=(0x005f688c,0x805f688c,0xa05f688c)[i%3];assert bus.read(bus.context,a,4,ctypes.byref(out))==0 and out.value==value and m.fifo_cursor==i+1
 assert list(storage)==samples and m.vertex.trace_count==4096 and m.vertex.trace_dropped==904;checks+=1
 assert [(e.kind,e.address,e.value,e.width) for e in m.vertex.trace[:4096]]==[(22,(0x005f688c,0x805f688c,0xa05f688c)[i%3],samples[i],4) for i in range(4096)];checks+=1
 out.value=0xabcdef01;before=bytes(m);assert bus.read(bus.context,v.FIFO,4,ctypes.byref(out))==-4 and out.value==0xabcdef01 and bytes(m)==before;checks+=1
 for n in (1,2,8):assert bus.read(bus.context,v.FIFO,n,ctypes.byref(out))==-3 and bytes(m)==before and out.value==0xabcdef01;checks+=1
 for n in (1,2,4,8):assert bus.write(bus.context,v.FIFO,0,n)==-3 and bytes(m)==before;checks+=1
 assert bus.read(bus.context,v.FIFO,4,None)==-3 and bytes(m)==before;checks+=1
 assert configure(ctypes.byref(m),None,1)==-3 and bytes(m)==before and configure(None,None,0)==-3;checks+=1
 assert configure(ctypes.byref(m),None,0)==0 and not m.fifo_samples and not m.fifo_count and not m.fifo_cursor;checks+=1
 rng=random.Random(v.CONTROL);control_cases=0
 for alias,n,offset in itertools.product((0x00702c00,0x80702c00,0xa0702c00),(1,2,4),range(4)):
  if offset%n or offset+n>4:continue
  for _ in range(8):
   initial=rng.getrandbits(32);value=rng.getrandbits(32);m.control_word=initial;mask=(1<<(n*8))-1;address=alias+offset
   assert bus.read(bus.context,address,n,ctypes.byref(out))==0 and out.value==(initial>>(offset*8))&mask
   assert bus.write(bus.context,address,value,n)==0 and m.control_word==(initial&~(mask<<(offset*8)))|((value&mask)<<(offset*8));control_cases+=1
 before=bytes(m);out.value=0xabcdef01
 for offset,n in ((0,0),(0,3),(0,8),(1,2),(1,4),(2,4),(3,2)):
  assert bus.read(bus.context,v.PORT+offset,n,ctypes.byref(out))==-3 and bytes(m)==before and out.value==0xabcdef01
  assert bus.write(bus.context,v.PORT+offset,0,n)==-3 and bytes(m)==before;checks+=2
 assert bus.read(bus.context,v.PORT,4,None)==-3 and bytes(m)==before;checks+=1
 assert bus.read(bus.context,0xff800030,2,ctypes.byref(out))==0 and out.value==0xa35a;checks+=1
 scan=(ctypes.c_uint32*2)(0x400,1);assert v.effects.LIB.sa_display_timing_set_samples(ctypes.byref(m),scan,2)==0
 for value in scan:assert bus.read(bus.context,v.previous.SCAN,4,ctypes.byref(out))==0 and out.value==value
 assert m.sample_cursor==2 and list(scan)==[0x400,1];checks+=1
 assert bus.write(bus.context,0xff00001c,0x905,4)==0 and bus.read(bus.context,0xff00001c,4,ctypes.byref(out))==0 and out.value==0x105 and m.ic_requests==1;checks+=1
 assert bus.write(bus.context,0xa05f6920,0x13579bdf,4)==0 and m.masks[0]==0x13579bdf;checks+=1
 assert bus.write(bus.context,0xa05f8040,0xabcdef,4)==0 and bus.read(bus.context,0xa05f8040,4,ctypes.byref(out))==0 and out.value==0xabcdef;checks+=1
 assert bus.cache(bus.context,0x8c001000,3)==0 and m.vertex.invalidates==1;checks+=1
 assert not any(ram);checks+=1
 assert configure(ctypes.byref(m),storage,len(samples))==0 and not m.fifo_cursor;checks+=1
 init(ctypes.byref(m),view,ctypes.byref(bus),0x200,0x98765432);assert not m.fifo_samples and not m.fifo_count and not m.fifo_cursor and m.control_word==0x98765432 and not m.samples and not m.sample_cursor and not m.ccr and not m.vertex.trace_count;checks+=1
 files=[Path(__file__),Path(v.__file__),v.P/'source/hardware_transfer_bus.c',v.P/'source/hardware_transfer_bus.h']
 report=dict(fifo_sampled_words_passed=len(samples),control_subword_cases=control_cases,backend_checks=checks,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='5000 raw FIFO words across three aliases, caller storage immutability, exact trace/drop counts, exhaustion/rejection without mutation, borrowed input reset and reconfiguration; randomized byte/word/longword control reads/writes with alias coherence and unaffected-byte checks. Prior scan/A/V/CCR/mask/render/cache delegation and RAM retention are checked. No physical FIFO/AICA/G2 behavior, clock generation, audio or pixels is claimed.')
 (v.P/'analysis/movie_hardware_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('FIFO words',len(samples),'control cases',control_cases,'backend checks',checks)
if __name__=='__main__':main()
