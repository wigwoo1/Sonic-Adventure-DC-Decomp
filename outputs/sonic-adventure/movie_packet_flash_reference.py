"""Original packet instructions with explicit mutable host flash storage."""
from movie_packet_startup_reference import *
import movie_packet_startup_reference as old
import flash_storage_reference as flash_model

namespace=dict(old.namespace);code=old.code
marker='  rc,result=console_reference.service(m,channel,regs[7],regs[4],regs[5],regs[6])'
assert code.count(marker)==1
code=code.replace(marker,'  if channel==1 and flash_reference is not None:rc,result=flash_reference.service(m,regs[7],regs[4],regs[5],regs[6])\n  else:rc,result=console_reference.service(m,channel,regs[7],regs[4],regs[5],regs[6])')
marker='queue_reference.runtime.snapshot() if queue_reference.runtime else None'
assert code.count(marker)==2
# The final exception tuple keeps its old shape; only event tuples gain flash.
event=code.index(' def event(');end=code.index(' def stop(',event)
part=code[event:end];assert part.count(marker)==1
part=part.replace(marker,marker+",(flash_reference.snapshot(),hashlib.sha256(flash_reference.data).digest()) if flash_reference is not None else None")
code=code[:event]+part+code[end:]
namespace['flash_reference']=None
exec(compile(code,'packet_flash_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__)
effects.CONNECTED=set(old.effects.CONNECTED);effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_packet_flash.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
console_model.declare(effects.LIB);flash_model.declare(effects.LIB)
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
native_flash=flash_model.State();native_flash_bytes=None;storage_attached=False
ordinary=effects.LIB.sa_movie_packet_flash_exception_observed
ordinary.argtypes=old.ordinary.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_packet_flash_running_observed
running.argtypes=old.running.argtypes;running.restype=ctypes.c_int
storage=effects.LIB.sa_movie_packet_flash_storage_observed
storage.argtypes=running.argtypes+[ctypes.POINTER(flash_model.State)];storage.restype=ctypes.c_int

def observed(*args):
 if storage_attached:
  assert runtime_attached,'Explicit storage entry requires declared persistent runtime'
  return storage(*args,ctypes.byref(native_runtime),ctypes.byref(native_flash))
 return running(*args,ctypes.byref(native_runtime)) if runtime_attached else ordinary(*args)

for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes
 getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
native_stats=old.native_stats.__class__()

def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff,ctypes.byref(native_exception))
effects.fn=invoke;compare_code=old.compare_code
exec(compile(compare_code,'packet_flash_legacy_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={})
_legacy_template_compare=local['compare']

# Explicit profile metadata, not a firmware partition-query result. These extents
# are documented in the Flycast primary source; the game queries them itself.
STANDARD_PARTITIONS=((0x1a000,0x2000),(0x18000,0x2000),(0x1c000,0x4000),(0x10000,0x8000),(0,0x10000))

def setup_flash(c):
 global storage_attached,native_flash_bytes
 storage_attached=bool(c.get('full_flash_storage'))
 assert effects.LIB.sa_flash_storage_init(ctypes.byref(native_flash))==0
 native_flash_bytes=None
 if not storage_attached:return None
 image=console_model.inputs(c)[1]
 parts=c.get('flash_partitions',STANDARD_PARTITIONS)
 values=(flash_model.Partition*len(parts))(*(flash_model.Partition(a,n) for a,n in parts))
 native_flash_bytes=(ctypes.c_uint8*len(image))(*image)
 assert effects.LIB.sa_flash_storage_configure(ctypes.byref(native_flash),native_flash_bytes,len(image),values,len(parts),int(c.get('flash_writable',False)))==0
 return flash_model.Reference(image,parts,c.get('flash_writable',False))

def native_flash_snapshot():
 return (flash_model.snapshot(native_flash),hashlib.sha256(bytes(native_flash_bytes)).digest()) if storage_attached else None

def bind_vectors(vbr):
 old.bind_vectors(vbr);ROWS.update(old.ROWS);effects.CONNECTED.update(old.effects.CONNECTED)

prepare=old.prepare
