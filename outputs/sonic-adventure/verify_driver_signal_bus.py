"""Finite borrowed IRQ observations; acknowledgements retain the old policy."""
import ctypes,hashlib,json
from pathlib import Path
import verify_movie_driver as v

def main():
 memory=v.DriverMemory();ram=v.effects.RAM_TYPE();bus=v.effects.Bus();lib=v.effects.LIB
 lib.sa_driver_signal_bus_init(ctypes.byref(memory),ram,ctypes.byref(bus),0,0)
 checks=0
 def load(address=0xa05f6900,n=4):
  out=ctypes.c_uint32(0x12345678);rc=bus.read(bus.context,address,n,ctypes.byref(out));return rc,out.value
 def snapshot():return memory.irq_configured,memory.irq_count,memory.irq_cursor,memory.normal_status,memory.frame.graphics.vertex.trace_count
 assert snapshot()[:4]==(0,0,0,0);checks+=1
 memory.normal_status=0x3f
 assert load()==(0,0x3f) and bus.write(bus.context,0xa05f6900,0x15,4)==0 and load()==(0,0x2a);checks+=3
 samples=(ctypes.c_uint32*3)(0,8,0x87654321);original=bytes(samples)
 assert lib.sa_driver_signal_set_samples(ctypes.byref(memory),samples,3)==0;checks+=1
 for address,wanted in zip((0x5f6900,0x805f6900,0xa05f6900),samples):
  assert load(address)==(0,wanted) and memory.normal_status==wanted;checks+=2
  assert bus.write(bus.context,address,0xffffffff,4)==0 and memory.normal_status==0;checks+=2
 before=snapshot();assert load()==(-4,0x12345678) and snapshot()==before;checks+=2
 assert bytes(samples)==original;checks+=1
 # Invalid configuration does not replace the old stream or restart it.
 assert lib.sa_driver_signal_set_samples(ctypes.byref(memory),None,1)==-3 and snapshot()==before;checks+=2
 assert lib.sa_driver_signal_set_samples(None,samples,3)==-3;checks+=1
 assert lib.sa_driver_signal_set_samples(ctypes.byref(memory),samples,3)==0 and memory.irq_cursor==0;checks+=2
 for address in (0x5f6900,0x805f6900,0xa05f6900):
  for n in (0,1,2,8):
   before=snapshot();assert load(address,n)==(-3,0x12345678) and snapshot()==before;checks+=2
  before=snapshot();assert bus.read(bus.context,address,4,None)==-3 and snapshot()==before;checks+=2
 for address in (0xa05f6901,0x805f6902,0x5f6903):
  before=snapshot();assert load(address)==(-3,0x12345678) and snapshot()==before;checks+=2
 assert load()==(0,0) and memory.irq_cursor==1;checks+=2
 assert lib.sa_driver_signal_set_samples(ctypes.byref(memory),None,0)==0;checks+=1
 before=snapshot();assert load()==(-4,0x12345678) and snapshot()==before;checks+=2
 # Unrelated routes still retain storage, cache and old mask behavior.
 assert bus.write(bus.context,0xa05f7804,0x89abcdef,4)==0 and load(0xa05f7804)==(0,0x89abcdef);checks+=2
 assert bus.write(bus.context,0xff00001c,0x828,4)==0 and memory.ccr==0x20 and memory.ic_requests==1 and memory.oc_requests==1;checks+=4
 assert bus.write(bus.context,0xa05f6920,0x55555555,4)==0 and load(0xa05f6920)==(0,0x55555555);checks+=2
 assert bytes(samples)==original;checks+=1
 assert memory.dma_operation==0;checks+=1
 for value in (0,0x8201,0xffffffff):
  assert bus.write(bus.context,0xffa00040,value,4)==0 and load(0xffa00040)==(0,value) and memory.dma_operation==value;checks+=3
 for width in (0,1,2,8):
  before=memory.dma_operation, memory.frame.graphics.vertex.trace_count
  assert bus.write(bus.context,0xffa00040,0,width)==-3 and load(0xffa00040,width)==(-3,0x12345678);checks+=2
  assert before==(memory.dma_operation,memory.frame.graphics.vertex.trace_count);checks+=1
 assert bus.read(bus.context,0xffa00040,4,None)==-3;checks+=1
 files=[Path(__file__),Path(v.__file__),v.P/'source/driver_signal_bus.c',v.P/'source/driver_signal_bus.h',v.P/'analysis/movie_driver_connection.json']
 digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 report=dict(checks_passed=checks,aliases=3,samples_borrowed_unchanged=True,exhaustion_status=-4,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Finite raw observations only. Reads consume one word; exhausted or invalid reads preserve cursor, status, output and trace. Writes retain W1C acknowledgement. No IRQ is generated and no physical delivery or timing is claimed.')
 (v.P/'analysis/driver_signal_bus_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Driver signal API checks passed',checks)
if __name__=='__main__':main()
