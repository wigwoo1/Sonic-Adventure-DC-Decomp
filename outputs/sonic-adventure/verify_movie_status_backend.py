"""Exhaust all sampled 16-bit inputs and check bus rejection/delegation/reset."""
import ctypes,hashlib,json
from pathlib import Path
import verify_movie_status as v
def main():
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);m=v.VideoMemory();bus=v.effects.Bus();v.effects.LIB.sa_video_signal_bus_init(ctypes.byref(m),view,ctypes.byref(bus),0xffffffff);checks=0
 assert m.port_data==0xffff;checks+=1;seen=set();out=ctypes.c_uint32()
 for sample in range(65536):
  m.port_data=sample;assert bus.read(bus.context,v.PORT,2,ctypes.byref(out))==0 and out.value==sample and m.port_data==sample;seen.add((out.value>>8)&3)
 assert seen=={0,1,2,3} and m.vertex.trace_count==4096 and m.vertex.trace_dropped==65536-4096
 assert [(e.kind,e.address,e.value,e.width) for e in m.vertex.trace[:4096]]==[(21,v.PORT,i,2) for i in range(4096)];checks+=1
 before=bytes(m)
 for width in (1,4,8):
  out.value=0xa5a5a5a5;assert bus.read(bus.context,v.PORT,width,ctypes.byref(out))==-3 and out.value==0xa5a5a5a5 and bytes(m)==before;checks+=1
 for width in (1,2,4,8):assert bus.write(bus.context,v.PORT,0,width)==-3 and bytes(m)==before;checks+=1
 assert bus.read(bus.context,v.PORT,2,None)==-3 and bytes(m)==before;checks+=1
 for address in (v.PORT-2,v.PORT+1,v.PORT+2,0x1f800030):
  out.value=0xa5a5a5a5;assert bus.read(bus.context,address,2,ctypes.byref(out))==-3 and out.value==0xa5a5a5a5 and bus.write(bus.context,address,0,2)==-3 and bytes(m)==before;checks+=2
 assert not any(ram) and not m.packet_count;checks+=1
 assert bus.write(bus.context,0xff00001c,0x905,4)==0 and bus.read(bus.context,0xff00001c,4,ctypes.byref(out))==0 and (m.ccr,m.ic_requests,m.oc_requests,out.value)==(0x105,1,0,0x105);checks+=1
 assert bus.write(bus.context,0xa05f6920,0x12345678,4)==0 and m.masks[0]==0x12345678;checks+=1
 assert bus.write(bus.context,0xff000038,0x10,4)==0 and bus.read(bus.context,0xff000038,4,ctypes.byref(out))==0 and out.value==0x10;checks+=1
 assert bus.cache(bus.context,0x8c001000,3)==0 and m.vertex.invalidates==1;checks+=1
 v.effects.LIB.sa_video_signal_bus_init(ctypes.byref(m),view,ctypes.byref(bus),0x200);assert m.port_data==0x200 and not m.ccr and not m.ic_requests and not m.oc_requests and not m.vertex.trace_count and not m.vertex.trace_dropped and not any(m.masks) and not any(m.vertex.qacr);checks+=1
 files=[Path(__file__),Path(v.__file__),v.P/'source/video_signal_bus.c',v.P/'source/video_signal_bus.h']
 report=dict(sampled_words_passed=65536,status_values_checked=4,backend_checks=checks,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Every 16-bit sampled word reads unchanged and covers all four status fields; exact first 4096 trace events/drop count, input clamp, read-only/width/null-output/neighbors/unsupported Area-7 rejection without mutation, retained RAM, inherited CCR/event-mask/queue/purge delegation and configured reset. No physical cable/GPIO direction or pixels are modeled.')
 (v.P/'analysis/movie_status_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Sampled words passed 65536; backend checks',checks)
if __name__=='__main__':main()
