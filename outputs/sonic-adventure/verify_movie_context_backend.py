"""Verify channel-register storage, aliases, rejection and inherited interfaces."""
import ctypes,hashlib,json,random
from pathlib import Path
import verify_movie_context as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);m=v.ContextMemory();bus=v.effects.Bus();out=ctypes.c_uint32();checks=0
 init=v.effects.LIB.sa_transfer_context_bus_init;init(ctypes.byref(m),view,ctypes.byref(bus),0xa35a,0x12345678);assert not any(m.transfer_registers);checks+=1
 rng=random.Random(v.INIT);expected=[0]*48;word_cases=0;expected_events=[]
 for index in range(48):
  for alias in (0x005f7800,0x805f7800,0xa05f7800):
   value=rng.getrandbits(32);a=alias+index*4;assert bus.write(bus.context,a,value,4)==0;expected[index]=value;expected_events.append((26,a,value,4))
   for other in (0x005f7800,0x805f7800,0xa05f7800):
    assert bus.read(bus.context,other+index*4,4,ctypes.byref(out))==0 and out.value==value;expected_events.append((25,other+index*4,value,4))
   assert list(m.transfer_registers)==expected;word_cases+=1
 before=bytes(m);out.value=0xabcdef01
 for address,width in ((0xa05f7800,1),(0xa05f7800,2),(0xa05f7800,8),(0xa05f7801,4),(0xa05f78bf,4),(0xa05f77fc,4),(0xa05f78c0,4)):
  assert bus.read(bus.context,address,width,ctypes.byref(out))==-3 and out.value==0xabcdef01 and bytes(m)==before
  assert bus.write(bus.context,address,0,width)==-3 and bytes(m)==before;checks+=2
 assert bus.read(bus.context,0xa05f7800,4,None)==-3 and bytes(m)==before;checks+=1
 assert bus.read(bus.context,0xff800030,2,ctypes.byref(out))==0 and out.value==0xa35a;checks+=1
 assert bus.read(bus.context,v.previous.PORT,4,ctypes.byref(out))==0 and out.value==0x12345678;checks+=1
 fifo=(ctypes.c_uint32*2)(0x21,0);assert v.effects.LIB.sa_hardware_transfer_set_samples(ctypes.byref(m),fifo,2)==0
 for value in fifo:assert bus.read(bus.context,v.previous.FIFO,4,ctypes.byref(out))==0 and out.value==value
 assert m.fifo_cursor==2 and list(fifo)==[0x21,0] and bus.read(bus.context,v.previous.FIFO,4,ctypes.byref(out))==-4;checks+=1
 scan=(ctypes.c_uint32*2)(0x400,1);assert v.effects.LIB.sa_display_timing_set_samples(ctypes.byref(m),scan,2)==0
 for value in scan:assert bus.read(bus.context,v.previous.previous.SCAN,4,ctypes.byref(out))==0 and out.value==value
 assert m.sample_cursor==2 and list(scan)==[0x400,1];checks+=1
 assert bus.write(bus.context,0xff00001c,0x905,4)==0 and bus.read(bus.context,0xff00001c,4,ctypes.byref(out))==0 and out.value==0x105 and m.ic_requests==1;checks+=1
 assert bus.write(bus.context,0xa05f6920,0x13579bdf,4)==0 and m.masks[0]==0x13579bdf;checks+=1
 assert bus.write(bus.context,0xa05f8040,0xabcdef,4)==0 and bus.read(bus.context,0xa05f8040,4,ctypes.byref(out))==0 and out.value==0xabcdef;checks+=1
 assert bus.cache(bus.context,0x8c001000,3)==0 and m.vertex.invalidates==1;checks+=1
 assert not any(ram);checks+=1
 trace=[(e.kind,e.address,e.value,e.width) for e in m.vertex.trace[:576]]
 assert trace==expected_events;checks+=1
 m.vertex.trace_count=4096;m.vertex.trace_dropped=7;assert bus.write(bus.context,0xa05f7800,0x87654321,4)==0 and bus.read(bus.context,0xa05f7800,4,ctypes.byref(out))==0 and out.value==0x87654321 and m.vertex.trace_count==4096 and m.vertex.trace_dropped==9;checks+=1
 init(ctypes.byref(m),view,ctypes.byref(bus),0x200,0);assert not any(m.transfer_registers) and not m.fifo_samples and not m.sample_count and not m.ccr and not m.vertex.trace_count;checks+=1
 files=[Path(__file__),Path(v.__file__),v.P/'source/transfer_context_bus.c',v.P/'source/transfer_context_bus.h']
 report=dict(register_word_cases=word_cases,register_alias_reads=word_cases*3,backend_checks=checks,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All 48 words and three aliases, coherent randomized writes with unaffected-word checks, exact events/drop behavior, width/alignment/fence/null rejection without mutation, reset, unchanged RAM, and prior FIFO/scan/control/A/V/CCR/mask/render/cache delegation. Storage does not advance DMA, IRQs or hardware timing.')
 (v.P/'analysis/movie_context_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Context register cases',word_cases,'alias reads',word_cases*3,'backend checks',checks)
if __name__=='__main__':main()
