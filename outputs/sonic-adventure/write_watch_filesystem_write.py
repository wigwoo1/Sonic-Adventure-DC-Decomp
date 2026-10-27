"""Use the unchanged complete-memory observer with new228-stage console ABI."""
from pathlib import Path
parent=Path(__file__).resolve().parent/'write_watch_buffer_initialize.py'
code=parent.read_text();marker='import movie_buffer_initialize_reference as old';assert code.count(marker)==1
code=code.replace(marker,'import movie_filesystem_write_reference as old')
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
