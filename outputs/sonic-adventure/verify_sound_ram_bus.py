"""Native policy boundary checks against independent byte storage and trace."""
import ctypes,hashlib,json,struct
from pathlib import Path
import verify_movie_driver_control as old
import sound_ram_reference as model
P=old.P;ROOT=old.ROOT;Bus=old.effects.Bus;Memory=model.memory_type(Bus)

def main():
 dll=ROOT/'work/sonic-adventure/build/movie_bulk_family.dll';lib=ctypes.CDLL(str(dll));model.declare(lib,Bus,Memory);checks=0;calls=[]
 @dict(Bus._fields_)['read']
 def read(ctx,address,width,out):calls.append(('read',address,width));out[0]=0x13579bdf;return -73
 @dict(Bus._fields_)['write']
 def write(ctx,address,value,width):calls.append(('write',address,value,width));return -74
 @dict(Bus._fields_)['cache']
 def cache(ctx,address,operation):calls.append(('cache',address,operation));return -75
 parent=Bus(None,read,write,cache)
 for configured,capacity in ((True,5),(True,0),(False,5)):
  expected=model.Reference(configured=configured,capacity=capacity);ram=(ctypes.c_uint8*model.SIZE).from_buffer_copy(expected.ram);trace=(model.Event*capacity)();state=Memory();bus=Bus()
  assert lib.sa_sound_ram_bus_init(ctypes.byref(state),ctypes.byref(parent),ram if configured else None,model.SIZE if configured else 0,trace if capacity else None,capacity,ctypes.byref(bus))==0;checks+=1
  for alias in (0,0x80000000,0xa0000000):
   for width in (1,2,4):
    for offset in (0,4,model.SIZE-width):
     address=alias|model.BASE+offset;value=0x89abcdef
     assert bus.write(bus.context,address,value,width)==expected.write(address,value,width);checks+=1
     out=ctypes.c_uint32(0xfeedface);status,result=expected.read(address,width)
     assert bus.read(bus.context,address,width,ctypes.byref(out))==status and out.value==(0xfeedface if status else result);checks+=2
     assert model.snapshot(state)==expected.snapshot() and bytes(ram)==expected.ram;checks+=2
  for address,width in ((0xa0800001,2),(0xa0800002,4),(0xa09fffff,2),(0xa09ffffe,4),(0xa0800000,8),(0xa0800000,0)):
   assert bus.write(bus.context,address,0xffffffff,width)==expected.write(address,0xffffffff,width)==-3;checks+=1
   out=ctypes.c_uint32(0x87654321);assert bus.read(bus.context,address,width,ctypes.byref(out))==-3 and out.value==0x87654321;checks+=2
  assert bus.read(bus.context,0xa0800000,4,None)==-3;checks+=1
  assert bus.read(bus.context,0xa0a00000,4,ctypes.byref(ctypes.c_uint32()))==-73 and bus.write(bus.context,0x8c123456,7,4)==-74 and bus.cache(bus.context,0xa0800000,3)==-75;checks+=3
  wanted=b''.join(struct.pack('<IIII',*e) for e in expected.events[:capacity]);assert bytes(trace)==wanted+bytes(len(bytes(trace))-len(wanted)) and model.snapshot(state)==expected.snapshot();checks+=2
 state=Memory();bus=Bus();ram=(ctypes.c_uint8*model.SIZE)()
 for args in ((None,ctypes.byref(parent),None,0,None,0,ctypes.byref(bus)),(ctypes.byref(state),None,None,0,None,0,ctypes.byref(bus)),(ctypes.byref(state),ctypes.byref(parent),ram,1,None,0,ctypes.byref(bus)),(ctypes.byref(state),ctypes.byref(parent),None,model.SIZE,None,0,ctypes.byref(bus)),(ctypes.byref(state),ctypes.byref(parent),ram,model.SIZE,None,1,ctypes.byref(bus))):
  assert lib.sa_sound_ram_bus_init(*args)==-3;checks+=1
 files=[Path(__file__),Path(model.__file__),P/'source/sound_ram_bus.c',P/'source/sound_ram_bus.h']
 report=dict(independent_checks=checks,software_memory_bytes=model.SIZE,borrowed_buffer_ownership=True,configured_and_unconfigured_tested=True,full_buffer_and_ordered_trace_checked=True,dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Native little-endian mutable RAM window and its independent reference agree for byte/word/long accesses, P0/P1/P2 aliases, end boundaries, alignment, invalid widths, unavailable storage, null arguments and bounded trace counts. Outside addresses/cache requests delegate with exact arguments/status. Standalone parent callbacks test bus delegation only; no game/firmware return is mocked in game execution. No physical AICA/ARM/timing/audio claim or routine credit.')
 (P/'analysis/sound_ram_bus_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Sound RAM bus checks passed:',checks)
if __name__=='__main__':main()
