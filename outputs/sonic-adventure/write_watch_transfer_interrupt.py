"""Opt-in boundary-memory checker; original instructions and native DLL unchanged."""
import ctypes,types,hashlib,struct
import sound_ram_reference as sound_model
import movie_transfer_interrupt_reference as old
from windows_write_watch import WatchedMemory,Window,BoundaryJournal

namespace=dict(old.namespace)
code=old.code
for marker,replacement in (('hashlib.sha256(memory).digest()',"journal.reference_token('ram')"),('hashlib.sha256(queue_reference.vram).digest()',"journal.reference_token('vram')")):
 assert code.count(marker)==1;code=code.replace(marker,replacement)
code=code.replace('console_reference.snapshot()','console_reference.snapshot(),queue_reference.sound.snapshot(),journal.reference_token(\"sound_ram\")')
code=code.replace('tuple(queue_reference.registers.get(0x005f8000+i*4,0) for i in range(2048))','intern_snapshot(tuple(queue_reference.registers.get(0x005f8000+i*4,0) for i in range(2048)))')
exec(compile(code,'write_watch_original_boundaries','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.namespace=namespace;effects.reference_run=reference_run
def invoke(*args):return old.observed(*args,ctypes.byref(effects.console_state),ctypes.byref(old.native_stats),0xffffffff)
effects.fn=invoke
BaseReference=old._legacy_template_compare.__globals__['RtcReference']

class SoundReference(BaseReference):
 def __init__(self,*args):
  super().__init__(*args);c=args[-1];self.sound=sound_model.Reference(c.get('sound_seed',0xa5),not c.get('sound_unconfigured',False),c.get('sound_trace_capacity',0x90000))
  if c.get('sound_pattern')=='ramp':self.sound.ram[:]=bytes(range(256))*(sound_model.SIZE//256)
  self.writeback_events=[]
 def writeback(self,a):
  result=super().writeback(a);self.writeback_events.append(a);return result
 def __contains__(self,a):return sound_model.inside(a) or super().__contains__(a)
 def __getitem__(self,a):
  if not sound_model.inside(a):return super().__getitem__(a)
  status,value=self.sound.read(a,self.access_width);assert status==0;return value
 def __setitem__(self,a,value):
  if not sound_model.inside(a):return super().__setitem__(a,value)
  assert self.sound.write(a,value,self.access_width)==0

SoundMemory=sound_model.memory_type(effects.Bus);sound_model.declare(effects.LIB,effects.Bus,SoundMemory)

def watched_reference(journal,*args):
 ref=SoundReference(*args);owner=WatchedMemory(len(ref.vram));view=owner.view;view[:]=ref.vram
 ref.vram=view;ref.write_watch_owner=owner;journal.reference_region('vram',Window(owner))
 sound_owner=WatchedMemory(sound_model.SIZE);sound_view=sound_owner.view;sound_view[:]=ref.sound.ram;ref.sound.ram=sound_view;ref.sound_owner=sound_owner
 journal.reference_region('sound_ram',Window(sound_owner));return ref

def safe_token(journal,name):
 try:return journal.native_token(name)
 except BaseException as exc:
  journal.errors.append((name,'native observation error',repr(exc)));return ('write-watch error',)

compare_code=old.compare_code
compare_code=compare_code.replace('tuple(memory.registers)','intern_snapshot(tuple(memory.registers))')
compare_code=compare_code.replace('console_model.snapshot(console_state)))','console_model.snapshot(console_state),sound_model.snapshot(sound_memory),safe_token(journal,\"sound_ram\")))')
marker='other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,"""other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f)
 journal=BoundaryJournal(c.get('write_watch_legacy_hashes',False));namespace['journal']=journal
 metadata_cache={}
 def intern_snapshot(value):return metadata_cache.setdefault(value,value)
 namespace['intern_snapshot']=intern_snapshot
 actual_owner=WatchedMemory(len(m));expected_owner=WatchedMemory(len(m));actual=actual_owner.view;expected=expected_owner.view;actual[:]=m;expected[:]=m
 journal.reference_region('ram',Window(expected_owner));journal.native_region('ram',Window(actual_owner))""")
marker='ref=RtcReference(';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'ref=watched_reference(journal,')
marker='memory=RtcMemory();';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,'memory_owner=WatchedMemory(ctypes.sizeof(RtcMemory));memory_buffer=memory_owner.view;memory=RtcMemory.from_buffer(memory_buffer);')
marker=' @effects.OBSERVE';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,""" native_cache_writebacks=[]
 original_cache=type(bus.cache)(ctypes.cast(bus.cache,ctypes.c_void_p).value)
 @dict(effects.Bus._fields_)['cache']
 def cache_observer(ctx,a,op):
  status=original_cache(ctx,a,op)
  if not status and op==2:native_cache_writebacks.append(a)
  return status
 bus.cache=cache_observer
"""+marker)
compare_code=compare_code.replace(marker,""" journal.native_region('vram',Window(memory_owner,ctypes.addressof(memory.vram)-memory_owner.address,ctypes.sizeof(memory.vram)))
 sound_owner=WatchedMemory(sound_model.SIZE);sound_view=sound_owner.view;sound_view[:]=(bytes(range(256))*(sound_model.SIZE//256) if c.get('sound_pattern')=='ramp' else bytes([c.get('sound_seed',0xa5)])*sound_model.SIZE);sound_array=(ctypes.c_uint8*sound_model.SIZE).from_buffer(sound_view)
 sound_trace=(sound_model.Event*c.get('sound_trace_capacity',0x90000))();sound_memory=SoundMemory();parent_bus=bus;bus=effects.Bus()
 assert effects.LIB.sa_sound_ram_bus_init(ctypes.byref(sound_memory),ctypes.byref(parent_bus),None if c.get('sound_unconfigured') else sound_array,0 if c.get('sound_unconfigured') else sound_model.SIZE,sound_trace,len(sound_trace),ctypes.byref(bus))==0
 journal.native_region('sound_ram',Window(sound_owner))
"""+marker)
for marker,replacement in (('effects.ram_digest(mem)',"safe_token(journal,'ram')"),('hashlib.sha256(memory.vram).digest()',"safe_token(journal,'vram')")):
 assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,replacement)
marker=' assert rc==status';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,' journal.check()\n'+marker)
marker=' return dict(console_snapshot=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,""" assert sound_model.snapshot(sound_memory)==ref.sound.snapshot()
 sound_trace_bytes=ctypes.string_at(sound_memory.trace,sound_memory.trace_count*16)
 assert sound_trace_bytes==b''.join(struct.pack('<IIII',*e) for e in ref.sound.events[:ref.sound.capacity])
 assert native_cache_writebacks==ref.writeback_events
 metrics=journal.finish()
 return dict(cache_writeback_sequence=native_cache_writebacks,sound_ram_sha256=hashlib.sha256(sound_view).hexdigest(),sound_state=sound_model.snapshot(sound_memory),sound_trace_sha256=hashlib.sha256(sound_trace_bytes).hexdigest(),sound_trace_events=sound_memory.trace_count,sound_trace_dropped=sound_memory.trace_dropped,write_watch=metrics,console_snapshot=""")
marker='),path,actual,s,f';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'),path,bytearray(actual),s,f')
exec(compile(compare_code,'write_watch_full_comparison','exec'),dict(old._legacy_template_compare.__globals__,effects=effects,namespace=namespace,WatchedMemory=WatchedMemory,Window=Window,BoundaryJournal=BoundaryJournal,watched_reference=watched_reference,safe_token=safe_token,sound_model=sound_model,SoundMemory=SoundMemory,struct=struct),local:={})
compare=local['compare']
