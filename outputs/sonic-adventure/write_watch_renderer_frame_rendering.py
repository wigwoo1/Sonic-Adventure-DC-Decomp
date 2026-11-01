"""Read-only list-state and PVR-write evidence on the accepted native build."""
import write_watch_renderer_frame_pacing as previous
from renderer_frame_pacing_trace import FIELDS
fields=tuple(sorted(set(FIELDS)|{0x8c8a2f34,0x8c8a2f5c,0x8c8a2f60,0x8c8a2d78}|set(range(0x8c8a3098,0x8c8a30d0,4))|set(range(0x8c8a3238,0x8c8a32c0,4))))
code=previous.code
marker='(0x8c641f00,0x8c6416ec,0x8c651bc0,0x8c652210,0x8c642440,0x8c651f1e)'
assert code.count(marker)==1
code=code.replace(marker,marker[:-1]+',0x8c651570,0x8c6516a0,0x8c65194a)')
namespace=dict(previous.namespace,pacing_fields=fields)
exec(compile(code,'read_only_original_render_list_state_comparison','exec'),namespace,local:={})
compare=local['compare']
