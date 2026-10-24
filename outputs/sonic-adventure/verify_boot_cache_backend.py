"""Independent CCR command/readback, bus-guard, overflow and reset checks."""
import ctypes,hashlib,json,random
from pathlib import Path
import verify_boot_cache as v
def main():
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);m=v.CacheMemory();bus=v.effects.Bus();v.effects.LIB.sa_cache_register_bus_init(ctypes.byref(m),view,ctypes.byref(bus));rng=random.Random(0x808)
 values=[0,0xffffffff]+[1<<bit for bit in range(32)]+[0xffffffff^(1<<bit) for bit in range(32)]+[rng.getrandbits(32) for _ in range(64)];events=[];ic=oc=0
 for value in values:
  out=ctypes.c_uint32(0xa5a5a5a5);assert bus.write(bus.context,v.CCR,value,4)==0
  ic=(ic+bool(value&0x800))&0xffffffff;oc=(oc+bool(value&8))&0xffffffff
  assert (m.ccr,m.ic_requests,m.oc_requests)==(value&~0x808,ic,oc)
  assert bus.read(bus.context,v.CCR,4,ctypes.byref(out))==0 and out.value==value&~0x808
  events.extend(((19,v.CCR,value,4),(20,v.CCR,value&~0x808,4)))
 assert [(e.kind,e.address,e.value,e.width) for e in m.vertex.trace[:m.vertex.trace_count]]==events
 guards=0;before=bytes(m)
 for width in (1,2,8):
  out=ctypes.c_uint32(0xa5a5a5a5);assert bus.write(bus.context,v.CCR,0,width)==-3 and bus.read(bus.context,v.CCR,width,ctypes.byref(out))==-3 and out.value==0xa5a5a5a5 and bytes(m)==before;guards+=2
 assert bus.read(bus.context,v.CCR,4,None)==-3 and bytes(m)==before;guards+=1
 for a in (0xff000018,0xff00001d,0xff000020,0x1f00001c):
  out=ctypes.c_uint32(0xa5a5a5a5);assert bus.write(bus.context,a,0,4)==-3 and bus.read(bus.context,a,4,ctypes.byref(out))==-3 and out.value==0xa5a5a5a5 and bytes(m)==before;guards+=2
 # Overflow never suppresses coherent-state changes or invalidation requests.
 m.vertex.trace_count=4096;m.ic_requests=m.oc_requests=0xffffffff
 out=ctypes.c_uint32(99);assert bus.write(bus.context,v.CCR,0x808,4)==0 and bus.read(bus.context,v.CCR,4,ctypes.byref(out))==0
 assert (m.ccr,m.ic_requests,m.oc_requests,m.vertex.trace_dropped,out.value)==(0,0,0,2,0);guards+=1
 # Delegate the already qualified queue-address registers and purge operation.
 out=ctypes.c_uint32();assert bus.write(bus.context,0xff000038,0x10,4)==0 and bus.read(bus.context,0xff000038,4,ctypes.byref(out))==0 and out.value==0x10;guards+=1
 assert bus.cache(bus.context,0x8c001000,3)==0 and m.vertex.invalidates==1;guards+=1
 assert not any(ram) and not m.packet_count;guards+=1
 v.effects.LIB.sa_cache_register_bus_init(ctypes.byref(m),view,ctypes.byref(bus));assert not m.ccr and not m.ic_requests and not m.oc_requests and not m.vertex.trace_count and not m.vertex.trace_dropped and not any(m.masks) and not any(m.vertex.qacr);guards+=1
 files=[Path(__file__),Path(v.__file__),v.P/'source/cache_register_bus.c',v.P/'source/cache_register_bus.h']
 report=dict(ccr_write_read_cases=len(values),backend_checks=guards,reference_document='https://www.st.com/resource/en/user_manual/cd00147165.pdf',reference_scope='Table 18/PDF pages 77-79: CCR OCI/ICI read zero; coherent storage, reserved-bit retention and immediate request counters are explicit host policies.',dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Independent command/readback and exact ordered traces for all individual bits, complements, zeros/ones and random reserved-bit fixture inputs; invalid widths/null output/neighbors/unsupported Area-7 mapping, no mutation on rejection, trace/counter overflow, inherited delegation, retained RAM and reset. No physical cache-line or cycle/exception model.')
 (v.P/'analysis/boot_cache_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CCR cases',len(values),'backend checks',guards)
if __name__=='__main__':main()
