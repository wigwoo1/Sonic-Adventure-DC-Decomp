"""Independent VBR operation, bounded mask backend and API guard checks."""
import ctypes,hashlib,json,random
from pathlib import Path
import verify_boot_event as v

def main():
 fn=v.effects.LIB.sa_sh4_stc_vbr;fn.argtypes=[ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint];fn.restype=ctypes.c_int
 rng=random.Random(0x0022);states=[0,0x20000000,0x40000000,0x600003f3]+[rng.getrandbits(32) for _ in range(508)];cases=0
 for sr in states:
  for value in (0,1,0xffffffff,0x80000000,0x8ce50000,0x0ce50000,0xace50000,0x12345678):
   for delay in (0,1):
    words=(ctypes.c_uint32*3)(0x13579bdf,0xa5a5a5a5,0x2468ace0)
    status=fn(value,sr,ctypes.cast(ctypes.byref(words,4),ctypes.POINTER(ctypes.c_uint32)),delay)
    expected=0 if sr&0x40000000 else -5 if delay else -4
    assert status==expected and list(words)==[0x13579bdf,value if not expected else 0xa5a5a5a5,0x2468ace0]
    cases+=1
 ram=bytearray(16<<20);view=v.effects.RAM_TYPE.from_buffer(ram);m=v.EventMemory();bus=v.effects.Bus();v.effects.LIB.sa_event_bus_init(ctypes.byref(m),view,ctypes.byref(bus));checks=0
 for i,physical in enumerate((0x005f6920,0x005f6924)):
  for alias in (0,0x80000000,0xa0000000):
   a=alias|physical;value=0x89abcdef^a;out=ctypes.c_uint32()
   assert bus.write(bus.context,a,value,4)==0 and m.masks[i]==value
   assert bus.read(bus.context,a,4,ctypes.byref(out))==0 and out.value==value;checks+=2
   for width in (1,2,8):
    out.value=0xa5a5a5a5;assert bus.write(bus.context,a,0,width)==-3 and bus.read(bus.context,a,width,ctypes.byref(out))==-3 and out.value==0xa5a5a5a5;checks+=1
 for a in (0xa05f691c,0xa05f6928,0xa05f6921,0xa05f6925):
  out=ctypes.c_uint32(0xa5a5a5a5);assert bus.write(bus.context,a,0,4)==-3 and bus.read(bus.context,a,4,ctypes.byref(out))==-3 and out.value==0xa5a5a5a5;checks+=1
 for a in (0xa5000000,0xffa00020,0xa05f8144):
  out=ctypes.c_uint32();assert bus.write(bus.context,a,0x12345678,4)==0 and bus.read(bus.context,a,4,ctypes.byref(out))==0 and out.value==0x12345678;checks+=1
 m.normal_status=0xf;assert bus.write(bus.context,0xa05f6900,8,4)==0 and m.normal_status==7;checks+=1
 assert not any(ram) and not m.packet_count;checks+=1
 m.vertex.trace_count=4096;old=m.vertex.trace_dropped
 assert bus.write(bus.context,0xa05f6920,0x11223344,4)==0 and m.masks[0]==0x11223344 and m.vertex.trace_dropped==old+1;checks+=1
 v.effects.LIB.sa_event_bus_init(ctypes.byref(m),view,ctypes.byref(bus));assert not any(m.masks) and not m.normal_status and not any(m.vram) and not m.vertex.trace_count;checks+=1
 # Additional state is mandatory in the new ABI. Reject missing pointers before
 # dispatch, preserving the old visible/extra machine and guest RAM contents.
 c=dict(entry=v.REPLACE,mode=0);ram,s,f=v.prepare(v.previous.previous.previous.previous.seed_ram(),c);view=v.effects.RAM_TYPE.from_buffer(ram);v.effects.LIB.sa_event_bus_init(ctypes.byref(m),view,ctypes.byref(bus))
 lo=ctypes.c_uint32(0x12345678);gbr=ctypes.c_uint32(0);vb=ctypes.c_uint32(0x8ce50000);extra=v.effects.Extra();extra.mach=0xabcdef01;extra.other[:]=list(range(8));fpul=ctypes.cast(ctypes.byref(f,v.effects.Fpu.fpul.offset),ctypes.POINTER(ctypes.c_uint32));before=(bytes(s),bytes(f),bytes(extra),bytes(ram),lo.value,gbr.value,vb.value)
 for e,p in ((None,ctypes.byref(vb)),(ctypes.byref(extra),None)):
  result=v.effects.fn(view,ctypes.byref(s),ctypes.byref(f),fpul,ctypes.byref(lo),v.REPLACE,None,v.effects.OBSERVE(),ctypes.byref(bus),ctypes.byref(gbr),e,p)
  assert result==-3 and before==(bytes(s),bytes(f),bytes(extra),bytes(ram),lo.value,gbr.value,vb.value) and not m.vertex.trace_count;checks+=1
 files=[Path(__file__),v.PROJECT/'verify_boot_event.py']+[v.PROJECT/'source'/n for n in ('sh4_vector_base.c','sh4_vector_base.h','sh4_integer_state.c','sh4_integer_state.h','event_register_bus.c','event_register_bus.h')]
 report=dict(vbr_kernel_cases_passed=cases,backend_and_api_checks=checks,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},reference_document='https://www.st.com/resource/en/user_manual/cd00147165.pdf',reference_scope='SH-4 STC VBR operation and privileged status, printed page 444/PDF page 443. Existing host -4 privilege / -5 slot-fault conventions remain; exception delivery is not implemented.',hardware_irq_delivery=False,scope='8192 independently expected VBR/MD/delay cases include arbitrary reserved SR bits, aliases/unaligned register values and preserved output on failure. Two software event mask words, aliases, invalid widths/alignment/neighbors, inherited VRAM/ack/DMA/render delegation, overflow/reset and new API missing-state guards. No asynchronous IRQs, hardware event vectoring or DMA is supplied.')
 (v.PROJECT/'analysis/boot_event_state_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('vbr_kernel_cases_passed','backend_and_api_checks')})

if __name__=='__main__':main()
