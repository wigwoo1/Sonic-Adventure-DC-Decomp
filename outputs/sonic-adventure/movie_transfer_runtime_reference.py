"""Original driver-control word update with full interrupt and device state."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import movie_interrupt_context_reference as old
import console_service_reference as console_model
import sound_ram_reference as sound_model
import verify_boot_device as seed
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store
ENTRY,LOAD,POOL,ALLOC,ERROR,SET,CLEAR=(0x8c092fa0,0x8c093e60,0x8c65aa30,0x8c643ac0,0x8c65acac,0x8c5ff2cc,0x8c5ff2de)
CTX,OUT=0x8cd80000,0x8cda0000
MP=P/'analysis/movie_interrupt_context_translation.json';manifest=old.manifest;NEW=old.NEW
ROWS=old.ROWS;PER_BODY=old.PER_BODY;namespace=dict(old.namespace)
from verify_sh4_exception_state import ExceptionState
import transfer_runtime_reference_cpu as context_cpu
import transfer_runtime_reference as runtime_model
EXCEPTION_KEYS=old.EXCEPTION_KEYS
namespace['reference']=context_cpu.reference
native_exception=ExceptionState();native_runtime=runtime_model.State();runtime_attached=False
code=old.code
marker=' special.update({k:c.get(k,0) for k in EXCEPTION_KEYS})';assert code.count(marker)==1
code=code.replace(marker,marker+"\n special['returned']=0 # Per-entry host RTE telemetry; architectural SSR/SPC/SGR are carried.")
code=code.replace('tuple(special[k] for k in EXCEPTION_KEYS)','tuple(special[k] for k in EXCEPTION_KEYS),queue_reference.runtime.snapshot() if queue_reference.runtime else None')
marker='  try:'
# Add an independent software device boundary; all guest instructions still run.
marker=' try:\n  reference(';assert code.count(marker)==1
boundary=r""" def runtime_boundary(ram,regs,sr,pc,pr,fpu,special):
  runtime=queue_reference.runtime
  if runtime is None or not runtime.enabled or runtime.in_irq:return sr,pc,pr
  queue_reference.access_width=4
  def read(a):
   if sound_model.inside(a):return queue_reference.sound.read(a,4)
   return 0,queue_reference.base_read(a)
  def write(a,value):
   if sound_model.inside(a):return queue_reference.sound.write(a,value,4)
   queue_reference.base_write(a,value);return 0
  rc=runtime.tick(ram,read,write)
  if rc:stop(0x8c666e04,regs,sr,rc)
  if not runtime.pending:return sr,pc,pr
  mask=queue_reference.base_read(0xa05f6920)
  if not runtime.pending&mask:return sr,pc,pr
  if sr&0x10000000 or (sr>>4)&15>=6:runtime.blocked+=1;return sr,pc,pr
  before=State();before.r[:]=regs;before.sr=sr;before.pc=pc;before.pr=pr
  hidden=list(special['other']);before_f=bytes(fpu);saved_special=(special['gbr'],special['mach'],special['macl'],special['vbr'])
  bank=1 if sr&0x40000000 and sr&0x20000000 else 0
  physical=[None,None];physical[bank]=list(regs[:8]);physical[1-bank]=hidden
  entered=State();entered.r[:]=physical[1]+list(regs[8:]);entered.sr=sr|0x70000000;entered.pc=special['vbr']+0x600;entered.pr=pr
  special.update(ssr=sr,spc=pc,sgr=regs[15],intevt=0x360,returned=0,other=physical[0])
  runtime.in_irq=1
  try:reference(ram,entered,hooks=hooked,fpu=fpu,mmio=queue_reference,special=special,stop_pc=pc,trace=trace,vector_instruction=reference_vector,instruction_budget=c.get('instruction_budget',2000000),cache_writeback=queue_reference.writeback,cache_invalidate=queue_reference.invalidate,cache_purge=queue_reference.purge,cache_prefetch=queue_reference.prefetch)
  except Stopped:
   state.pr=entered.pr # A fault retains the reached interrupt-call link, not the suspended caller link.
   raise
  finally:runtime.in_irq=0
  runtime.deliveries+=1
  assert bytes(entered)==bytes(before) and bytes(fpu)==before_f and special['other']==hidden
  assert (special['gbr'],special['mach'],special['macl'],special['vbr'])==saved_special and special['returned']==1
  return sr,pc,pr
"""
code=code.replace(marker,boundary+marker)
code=code.replace('cache_prefetch=queue_reference.prefetch)\n except','cache_prefetch=queue_reference.prefetch,runtime_boundary=runtime_boundary)\n except')
exec(compile(code,'runtime_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')

effects=types.SimpleNamespace(**old.effects.__dict__);effects.CONNECTED=set(old.effects.CONNECTED)|NEW;effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_transfer_runtime.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
ordinary=effects.LIB.sa_movie_transfer_runtime_exception_observed;ordinary.argtypes=old.observed.argtypes;ordinary.restype=ctypes.c_int
running=effects.LIB.sa_movie_transfer_runtime_running_observed;running.argtypes=ordinary.argtypes+[ctypes.POINTER(runtime_model.State)];running.restype=ctypes.c_int
def observed(*args):return running(*args,ctypes.byref(native_runtime)) if runtime_attached else ordinary(*args)
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff,ctypes.byref(native_exception))
effects.fn=invoke
compare_code=old.compare_code
exec(compile(compare_code,'init_records_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats),local:={});_legacy_template_compare=local['compare']

Input=old.Input;seed_ram=old.seed_ram;sample=old.sample
VECTORS=(0x8c65e7a4,0x8c65e7d8);NORMAL,NESTED,EXIT=0x8c65e80c,0x8c65e958,0x8c65eaec
COUNTER=old.COUNTER;ASIC=old.ASIC;HANDLER=old.HANDLER;DESCRIPTOR=old.DESCRIPTOR;RECORD=old.RECORD;ROOT_CONTEXT=old.ROOT_CONTEXT

def prepare(ram,c):
 result=old.prepare(ram,c);effects.CONNECTED.update(old.effects.CONNECTED);return result
