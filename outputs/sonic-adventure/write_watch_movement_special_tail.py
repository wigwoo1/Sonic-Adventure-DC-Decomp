"""Use the unchanged complete-memory observer with new228-stage console ABI."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'write_watch_buffer_initialize.py'
code=parent.read_text();marker='import movie_buffer_initialize_reference as old';assert code.count(marker)==1
code=code.replace(marker,'import movement_special_tail_reference as old')
marker="assert console_ref.service(expected,0,0,0,0,0)==(0,None)";assert code.count(marker)==1
code=code.replace(marker,"assert console_ref.service(expected,0,0,0,0,0)==(0,0)")
exec(compile(code,str(parent),'exec'),globals())

# Read completed native MACL without changing execution or comparison logic.
marker=' return dict(';assert compare_code.count(marker)==1
address_compare_code=compare_code.replace(marker,' return dict(final_macl=ml.value,')
exec(compile(address_compare_code,'record_address_full_comparison','exec'),dict(compare.__globals__),address_local:={})
compare=address_local['compare']

# Observe the mandatory TAS purge independently of packet/result expectations.
purge_code=address_compare_code
marker=' native_cache_writebacks=[]';assert purge_code.count(marker)==1
purge_code=purge_code.replace(marker,marker+'\n native_cache_purges=[]')
marker='  if not status and op==2:native_cache_writebacks.append(a)';assert purge_code.count(marker)==1
purge_code=purge_code.replace(marker,marker+'\n  if not status and op==3:native_cache_purges.append(a)')
marker=' return dict(';assert purge_code.count(marker)==1
purge_code=purge_code.replace(marker,marker+'cache_purge_sequence=native_cache_purges,')
# Read-only request inputs at the actual original packet-builder boundary.
# Existing complete-memory/device journaling still compares every state byte.
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
assert purge_code.count(marker)==1
purge_code=purge_code.replace(marker,marker+"""
  if target==0x8c65f810 and r[5] in (12,13) and r[7]==2:
   def write_read32(address):return int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(address&0xffffff),4),'little')
   def write_valid(address,size):return address&0x1f000000==0x0c000000 and address&0xffffff<=0x1000000-size
   if write_valid(r[6],8) and write_valid(r[15],8):
    source=write_read32(r[15]);words=write_read32(r[15]+4)
    if words<=128 and (words==0 or write_valid(source,words*4)):
     boundaries[-1]['write_request_input']=dict(mapped_unit=r[4],command=r[5],parameters=[write_read32(r[6]),write_read32(r[6]+4)],source=hex(source),words=words,data=list(ctypes.string_at(ctypes.addressof(mem.contents)+(source&0xffffff),words*4)) if words else [])
""")
exec(compile(purge_code,'request_purge_full_comparison','exec'),dict(compare.__globals__),purge_local:={})
compare=purge_local['compare']

# A direct glyph leaf requires its caller's already-configured store queues.
# Seed only the explicit external-device INPUT state on both sides; the text
# caller instead executes its own original QACR writes. No game result/frame
# or instruction/observer expectation is supplied or changed.
prior_watched_reference=watched_reference
def font_watched_reference(journal,*args):
 ref=prior_watched_reference(journal,*args);values=args[-1].get('vertex_qacr',(0,0))
 assert len(values)==2 and all(x in (0,4,8,12,16,20,24,28) for x in values)
 ref.qacr[:]=values
 return ref
font_code=purge_code
marker="effects.LIB.sa_rtc_sample_bus_init(ctypes.byref(memory),view,ctypes.byref(bus),c.get('port_data',0),c.get('control_word',0))"
assert font_code.count(marker)==1
font_code=font_code.replace(marker,marker+"\n memory.vertex.qacr[:]=c.get('vertex_qacr',(0,0))")
exec(compile(font_code,'font_device_input_full_comparison','exec'),dict(compare.__globals__,watched_reference=font_watched_reference),font_local:={})
compare=font_local['compare']

# Read-only capture of actual floating arguments at the three original math
# wrappers. Full-state/ordered comparisons have already checked these banks.
angle_input_code=font_code
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
assert angle_input_code.count(marker)==1
angle_input_code=angle_input_code.replace(marker,marker+"\n  if target in (0x8c10d008,0x8c10d058,0x8c10d0f8):\n   bank=(fp.contents.fpscr>>21)&1;boundaries[-1]['angle_input']=dict(fr4=int(fp.contents.banks[bank][4]),fr5=int(fp.contents.banks[bank][5]),fpscr=int(fp.contents.fpscr))")
exec(compile(angle_input_code,'contact_angle_input_full_comparison','exec'),dict(compare.__globals__),angle_input_local:={})
compare=angle_input_local['compare']

# Capture only inputs at the real0751c0 callback boundary. Execution and the
# complete-state observer remain unchanged; no result or frame is supplied.
record_input_code=angle_input_code
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
assert record_input_code.count(marker)==1
record_input_code=record_input_code.replace(marker,marker+"\n  if target==0x8c0751c0:\n   def record_words(a):\n    return [int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff)+4*j,4),'little') for j in range(3)] if a&0x1f000000==0x0c000000 and (a&0xffffff)<=0x1000000-12 else None\n   boundaries[-1]['record_callback_input']=dict(actor_position=record_words(r[5]+0x20),character_position=record_words(r[4]+0x20),saved_position=record_words(r[15]+4))")
exec(compile(record_input_code,'record_callback_input_full_comparison','exec'),dict(compare.__globals__),record_input_local:={})
compare=record_input_local['compare']
