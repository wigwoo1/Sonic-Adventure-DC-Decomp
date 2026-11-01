"""Read actual allocated actor/state/collision inputs at the owner response call."""
from write_watch_collision_response_contracts import *
import write_watch_collision_response_contracts as previous
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
code=previous.code
assert code.count(marker)==1
code=code.replace(marker,marker+"""
  if target==0x8c072946:
   def owner_read(a,n=4):
    if a&0x1f000000!=0x0c000000 or (a&0xffffff)>0x1000000-n:return None
    return int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff),n),'little')
   collision=owner_read(r[4]+0x38);info=owner_read(collision+0x18) if collision else None
   index=owner_read(collision+0x16,2) if collision else None
   signed_index=index-0x10000 if index is not None and index&0x8000 else index
   shapes=owner_read(info+0xc) if info else None
   boundaries[-1]['owner_response_input']=dict(actor=r[4],aux=r[5],state=r[6],actor_flags=owner_read(r[4]+4,2),collision=collision,collision_flags=owner_read(collision+2,2) if collision else None,info=info,provider=owner_read(collision+0xc) if collision else None,index=signed_index,count=owner_read(info+4,2) if info else None,shapes=shapes,selected_shape_flags=owner_read((shapes+0x34*signed_index)&0xffffffff) if shapes and signed_index is not None else None,velocity=[owner_read(r[5]+4+4*j) for j in range(3)],old_normal=[owner_read(r[6]+0x50+4*j) for j in range(3)])
""")
exec(compile(code,'movement_owner_actual_response_inputs','exec'),dict(previous.compare.__globals__),captured:={})
compare=captured['compare']
