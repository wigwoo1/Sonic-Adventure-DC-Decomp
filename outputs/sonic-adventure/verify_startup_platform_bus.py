"""Exercise new storage widths, aliases, cache-tag spacing and old routes."""
import ctypes,hashlib,json
from pathlib import Path
import verify_movie_startup as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 memory=v.StartupMemory();ram=v.effects.RAM_TYPE();bus=v.effects.Bus()
 v.effects.LIB.sa_startup_platform_bus_init(ctypes.byref(memory),ram,ctypes.byref(bus),0x55aa,0x12345678)
 assert list(memory.startup_registers)==[0]*v.BANK_WORDS and list(memory.system_registers)==[0]*9 and list(memory.cache_tags)==[0]*512
 writes=reads=0
 def store(address,value,n):return bus.write(bus.context,address,value,n)
 def load(address,n):
  value=ctypes.c_uint32(0x12345678);rc=bus.read(bus.context,address,n,ctypes.byref(value));return rc,value.value
 for address,width in zip(v.SYSTEM,v.WIDTHS):
  assert store(address,0xfedcba98,width)==0;writes+=1
  assert load(address,width)==(0,0xfedcba98&((1<<(width*8))-1));reads+=1
 for p in range(0x5f6800,0x5f7d00,4):
  if v.inherited(p):continue
  for alias in (0,0x80000000,0xa0000000):
   value=(p^alias^0x13579bdf)&0xffffffff;assert store(p|alias,value,4)==0;writes+=1
   for other in (0,0x80000000,0xa0000000):assert load(p|other,4)==(0,value);reads+=1
 for i in range(512):
  address=0xf4000000+i*32;value=0xa5a50000^i;assert store(address,value,4)==0;writes+=1
  assert load(address,4)==(0,value);reads+=1
 failures=0
 for address,width in zip(v.SYSTEM,v.WIDTHS):
  old=load(address,width)
  for n in (1,2,4):
   if n==width:continue
   assert store(address,0,n)==-3 and load(address,n)==(-3,0x12345678) and load(address,width)==old;failures+=3
 for address in (0x5f6811,0x805f6812,0xa05f6813,0xf4000004,0xf4000010):
  assert store(address,0,4)==-3 and load(address,4)==(-3,0x12345678);failures+=2
 # Existing policies retain their original handlers and backing fields.
 assert store(0xa05f7804,0x89abcdef,4)==0 and memory.transfer_registers[1]==0x89abcdef and load(0xa05f7804,4)==(0,0x89abcdef)
 assert store(0xa05f6800,0x12345678,4)==0 and memory.dma_registers[4]==0x12345678
 assert store(0xff00001c,0x828,4)==0 and memory.ccr==0x20 and memory.ic_requests==1 and memory.oc_requests==1
 assert store(0xa05f6920,0x76543210,4)==0 and memory.masks[0]==0x76543210
 expected_events=writes+reads+27+5
 vertex=memory.frame.graphics.vertex
 assert vertex.trace_count==4096 and vertex.trace_dropped==expected_events-4096
 reference=v.StartupReference([100,200],64,{})
 reference[0xffd8000c]=0xffffffff
 assert reference[0xffd8000c]==0xffffffff-100 and reference.clock_cursor==1
 assert reference[0xffd8000c]==0xffffffff-200 and reference.clock_cursor==2
 assert reference.system_registers[7]==0xffffffff
 files=[Path(__file__),Path(v.__file__),v.P/'source/startup_platform_bus.c',v.P/'source/startup_platform_bus.h',v.P/'analysis/movie_startup_connection.json']
 report=dict(system_registers=9,new_word_slots=sum(not v.inherited(p) for p in range(0x5f6800,0x5f7d00,4)),successful_writes=writes,successful_reads=reads,cache_tag_slots=512,invalid_access_checks=failures,old_route_cases=4,old_route_checks=11,trace_capacity=4096,trace_dropped=vertex.trace_dropped,trace_checks=2,retained_clock_routing_checks=5,
  dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Exact new register widths, all new word slots at physical/P1/P2 aliases, every 32-byte-spaced cache-tag word, rejected widths/alignments, and retained context/DMA/CCR/mask policies. Software storage only; no physical timer/IRQ/DMA/cache behavior.')
 (v.P/'analysis/startup_platform_bus_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in report if k!='sha256'})
if __name__=='__main__':main()
