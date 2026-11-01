"""Read-only capture of actual line-wrapper arguments; execution is unchanged."""
import write_watch_player_response_clip as previous
from write_watch_player_response_clip import *
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
code=previous.record_input_code
assert code.count(marker)==1
code=code.replace(marker,marker+"""
  if target==0x8c63ce24:
   def packet_valid(a,n):return a&0x1f000000==0x0c000000 and (a&0xffffff)<=0x1000000-n
   def packet_words(a,n):return [int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff)+4*j,4),'little') for j in range(n)] if packet_valid(a,4*n) else None
   header=packet_words(r[4],4)
   boundaries[-1]['line_wrapper_input']=dict(header=header,points=packet_words(header[0],6) if header else None,colours=packet_words(header[1],2) if header else None)
""")
exec(compile(code,'collision_response_actual_line_arguments','exec'),dict(previous.compare.__globals__),captured:={})
compare=captured['compare']
