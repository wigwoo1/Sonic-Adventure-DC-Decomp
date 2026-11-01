"""Capture wait inputs at matched native boundaries; preserve all comparisons."""
import ctypes
import write_watch_renderer_frame_clock as previous
from renderer_frame_pacing_trace import FIELDS,attach
code=previous.code
def change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
change('def compare(m,s,f,c):','def compare(m,s,f,c):\n pacing_stores.clear()')
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
change(marker,marker+'''
  if target in (0x8c641f00,0x8c6416ec,0x8c651bc0,0x8c652210,0x8c642440,0x8c651f1e) or target==vb.value+0x600:
   boundaries[-1]['pacing_input']={hex(a):int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff),4),'little') for a in pacing_fields}
''')
change(' return dict(',' return dict(pacing_stores=list(pacing_stores),')
namespace=dict(previous.namespace,pacing_fields=FIELDS)
pacing_stores=attach(namespace['namespace']);namespace['pacing_stores']=pacing_stores
exec(compile(code,'read_only_original_frame_pacing_comparison','exec'),namespace,local:={})
compare=local['compare']
