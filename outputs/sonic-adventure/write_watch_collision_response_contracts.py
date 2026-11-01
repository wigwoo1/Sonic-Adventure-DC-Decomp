"""Observe actual caller output slots without supplying or repairing their contents."""
from write_watch_collision_response_arguments import *
import write_watch_collision_response_arguments as previous
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
code=previous.code
assert code.count(marker)==1
code=code.replace(marker,marker+"""
  if target==0x8c0344dc or (target==0x8c63a75c and pr[0]==0x8c0729c6):
   def contract_words(a,n):
    if a&0x1f000000!=0x0c000000 or (a&0xffffff)>0x1000000-4*n:return None
    return [int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff)+4*j,4),'little') for j in range(n)]
   if target==0x8c0344dc:
    collision=contract_words(r[4]+0x38,1);info=contract_words(collision[0]+0x18,1) if collision else None
    boundaries[-1]['response_output_before_child']=dict(output=r[5],words=contract_words(r[5],3),collision_info=info[0] if info else None)
   else:boundaries[-1]['response_output_after_child']=dict(output=r[4],words=contract_words(r[4],3),state_words=contract_words(r[12]+0x44,3))
""")
exec(compile(code,'collision_response_actual_caller_contracts','exec'),dict(previous.compare.__globals__),captured:={})
compare=captured['compare']
