"""Independent purge-policy and new native-disc ABI rejection checks."""
import ctypes,hashlib,json
from pathlib import Path
import verify_boot_disc as v

def main():
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);memory=v.previous.previous.EventMemory();bus=v.effects.Bus();v.effects.LIB.sa_event_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));checks=0
 expected=[]
 for region in (0,0x80000000,0xa0000000):
  for offset in (0,1,31,32,0xffffe0,0xffffff):
   a=region|0xc000000|offset;assert bus.cache(bus.context,a,3)==0;expected.append((18,a,0,0));checks+=1
 assert memory.vertex.invalidates==18 and [(e.kind,e.address,e.value,e.width) for e in memory.vertex.trace[:memory.vertex.trace_count]]==expected;checks+=1
 before=bytes(memory)
 for a in (0,0x0bffffff,0x0d000000,0x2c000000,0x4c000000,0x6c000000,0xcc000000,0xec000000):
  assert bus.cache(bus.context,a,3)==-3 and bytes(memory)==before;checks+=1
 memory.vertex.trace_count=4096;old=memory.vertex.invalidates
 assert bus.cache(bus.context,0x8c123456,3)==0 and memory.vertex.invalidates==old+1 and memory.vertex.trace_dropped==1;checks+=1
 assert not any(ram) and not memory.packet_count;checks+=1
 v.effects.LIB.sa_event_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));assert not memory.vertex.invalidates and not memory.vertex.trace_count and not memory.vertex.trace_dropped;checks+=1
 seed=v.previous.previous.previous.previous.previous.previous.seed_ram();ram,s,f=v.prepare(seed,dict(entry=v.previous.LOAD,mode=0));view=v.effects.RAM_TYPE.from_buffer(ram)
 lo=ctypes.c_uint32(0x12345678);gbr=ctypes.c_uint32(0);vbr=ctypes.c_uint32(0x8ce50000);extra=v.effects.Extra();q=v.platform.Queue();fpul=ctypes.cast(ctypes.byref(f,v.effects.Fpu.fpul.offset),v.effects.core.P)
 before=(bytes(ram),bytes(s),bytes(f),bytes(extra),bytes(q),lo.value,gbr.value,vbr.value)
 for null in ('integer','vbr','queue','vector'):
  result=v.effects.fn(view,ctypes.byref(s),ctypes.byref(f),fpul,ctypes.byref(lo),s.pc,None,v.effects.OBSERVE(),None,ctypes.byref(gbr),None if null=='integer' else ctypes.byref(extra),None if null=='vbr' else ctypes.byref(vbr),None if null=='queue' else ctypes.byref(q),0 if null=='vector' else v.BIOS)
  assert result==-3 and before==(bytes(ram),bytes(s),bytes(f),bytes(extra),bytes(q),lo.value,gbr.value,vbr.value);checks+=1
 files=[Path(__file__),Path(v.__file__),v.P/'source/vertex_bus_disc.c',v.P/'source/sa_boot_disc_connected.c',v.P/'connect_boot_disc.py']
 report=dict(backend_and_api_checks=checks,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Independent coherent-RAM purge aliases/unaligned operands, invalid regions with unchanged state, trace overflow, reset, retained RAM, and all four new-ABI missing-state guards. No physical cache coherency or hardware fault delivery claim.')
 (v.P/'analysis/boot_disc_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Backend/API checks passed',checks)
if __name__=='__main__':main()
