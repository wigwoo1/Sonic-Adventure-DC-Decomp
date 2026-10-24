"""Independent bounded VRAM/normal-interrupt storage checks; no hardware delivery."""
import ctypes,hashlib,json
from pathlib import Path
import verify_boot_interrupt as v

def check():
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);m=v.InterruptMemory();bus=v.effects.Bus()
 v.effects.LIB.sa_interrupt_bus_init(ctypes.byref(m),view,ctypes.byref(bus));n=0
 assert not m.normal_status and not any(m.vram);n+=1
 for alias in (0x005f6900,0x805f6900,0xa05f6900):
  m.normal_status=0xdeadbeef;out=ctypes.c_uint32()
  assert bus.read(bus.context,alias,4,ctypes.byref(out))==0 and out.value==0xdeadbeef
  assert bus.write(bus.context,alias,0x10203048,4)==0 and m.normal_status==0xce8d8ea7
  assert bus.write(bus.context,alias,0,4)==0 and m.normal_status==0xce8d8ea7
  assert bus.write(bus.context,alias,0xffffffff,4)==0 and not m.normal_status;n+=4
 for alias in (0x05000000,0x85000000,0xa5000000):
  for offset in (0,4,0x7ffffc):
   value=0x98765432^offset;out=ctypes.c_uint32()
   assert bus.write(bus.context,alias+offset,value,4)==0 and m.vram[offset//4]==value
   assert bus.read(bus.context,alias+offset,4,ctypes.byref(out))==0 and out.value==value;n+=2
 for address in (0xa5000001,0xa57ffffd,0xa5800000,0xa4fffffc,0xa4000000,0xa05f6901,0xa05f68fc,0xa05f6904):
  out=ctypes.c_uint32(0xcafebabe)
  assert bus.read(bus.context,address,4,ctypes.byref(out))==-3 and out.value==0xcafebabe
  assert bus.write(bus.context,address,0,4)==-3;n+=2
 for address in (v.ACK,0xa5000000):
  for width in (1,2,8):
   out=ctypes.c_uint32(0xcafebabe)
   assert bus.read(bus.context,address,width,ctypes.byref(out))==-3 and out.value==0xcafebabe
   assert bus.write(bus.context,address,0,width)==-3;n+=2
 for a in (v.DMA[0],0xa05f8144):
  out=ctypes.c_uint32();assert bus.write(bus.context,a,0xabcdef01,4)==0
  assert bus.read(bus.context,a,4,ctypes.byref(out))==0 and out.value==0xabcdef01;n+=2
 assert not any(ram) and not m.packet_count;n+=1
 m.vertex.trace_count=4096;old=m.vertex.trace_dropped
 assert bus.write(bus.context,0xa5000000,0x55,4)==0 and m.vram[0]==0x55 and m.vertex.trace_dropped==old+1;n+=1
 v.effects.LIB.sa_interrupt_bus_init(ctypes.byref(m),view,ctypes.byref(bus))
 assert not any(m.vram) and not m.normal_status and not m.vertex.trace_count and not any(m.dma_registers);n+=1
 return dict(independent_checks=n,vram_bytes=8<<20,hardware_interrupt_delivery=False,hardware_dma_completion=False)

if __name__=='__main__':
 report=check();files=[Path(__file__),v.PROJECT/'verify_boot_interrupt.py',v.PROJECT/'source/interrupt_bus.c',v.PROJECT/'source/interrupt_bus.h']
 report['sha256']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files}
 report['dll_sha256']=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest()
 (v.PROJECT/'analysis/boot_interrupt_bus_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report,indent=2))
