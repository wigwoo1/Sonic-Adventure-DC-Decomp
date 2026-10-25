"""Opt-in boundary-memory checker; original instructions and native DLL unchanged."""
import ctypes,types
import verify_movie_driver_control as old
from windows_write_watch import WatchedMemory,Window,BoundaryJournal

namespace=dict(old.namespace)
code=old.code
for marker,replacement in (('hashlib.sha256(memory).digest()',"journal.reference_token('ram')"),('hashlib.sha256(queue_reference.vram).digest()',"journal.reference_token('vram')")):
 assert code.count(marker)==1;code=code.replace(marker,replacement)
exec(compile(code,'write_watch_original_boundaries','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.namespace=namespace;effects.reference_run=reference_run
def invoke(*args):return old.observed(*args,ctypes.byref(effects.console_state),ctypes.byref(old.native_stats),0xffffffff)
effects.fn=invoke
BaseReference=old.compare.__globals__['RtcReference']

def watched_reference(journal,*args):
 ref=BaseReference(*args);owner=WatchedMemory(len(ref.vram));view=owner.view;view[:]=ref.vram
 ref.vram=view;ref.write_watch_owner=owner;journal.reference_region('vram',Window(owner));return ref

def safe_token(journal,name):
 try:return journal.native_token(name)
 except BaseException as exc:
  journal.errors.append((name,'native observation error',repr(exc)));return ('write-watch error',)

compare_code=old.compare_code
marker='other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,"""other=effects.State.from_buffer_copy(s);ef=effects.Fpu.from_buffer_copy(f)
 journal=BoundaryJournal(c.get('write_watch_legacy_hashes',False));namespace['journal']=journal
 actual_owner=WatchedMemory(len(m));expected_owner=WatchedMemory(len(m));actual=actual_owner.view;expected=expected_owner.view;actual[:]=m;expected[:]=m
 journal.reference_region('ram',Window(expected_owner));journal.native_region('ram',Window(actual_owner))""")
marker='ref=RtcReference(';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'ref=watched_reference(journal,')
marker='memory=RtcMemory();';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,'memory_owner=WatchedMemory(ctypes.sizeof(RtcMemory));memory_buffer=memory_owner.view;memory=RtcMemory.from_buffer(memory_buffer);')
marker=' @effects.OBSERVE';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," journal.native_region('vram',Window(memory_owner,ctypes.addressof(memory.vram)-memory_owner.address,ctypes.sizeof(memory.vram)))\n"+marker)
for marker,replacement in (('effects.ram_digest(mem)',"safe_token(journal,'ram')"),('hashlib.sha256(memory.vram).digest()',"safe_token(journal,'vram')")):
 assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,replacement)
marker=' assert rc==status';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,' journal.check()\n'+marker)
marker=' return dict(console_snapshot=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,' metrics=journal.finish()\n return dict(write_watch=metrics,console_snapshot=')
marker='),path,actual,s,f';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'),path,bytearray(actual),s,f')
exec(compile(compare_code,'write_watch_full_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,WatchedMemory=WatchedMemory,Window=Window,BoundaryJournal=BoundaryJournal,watched_reference=watched_reference,safe_token=safe_token),local:={})
compare=local['compare']
