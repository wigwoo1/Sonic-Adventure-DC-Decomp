"""Read-only particle packet inputs; retain every existing exact comparison."""
from write_watch_player_collision_attach import *
import write_watch_player_collision_attach as previous
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
particle_input_code=previous.record_input_code
assert particle_input_code.count(marker)==1
particle_input_code=particle_input_code.replace(marker,marker+"""
  if target==0x8c09e778 and r[4]&0x1f000000==0x0c000000 and (r[4]&0xffffff)<=0x1000000-32:
   raw=ctypes.string_at(ctypes.addressof(mem.contents)+(r[4]&0xffffff),32)
   boundaries[-1]['particle_sprite_input']=list(struct.unpack('<8I',raw))
""")
local={}
exec(compile(particle_input_code,'particle_read_only_inputs','exec'),dict(previous.compare.__globals__),local)
compare=local['compare']
