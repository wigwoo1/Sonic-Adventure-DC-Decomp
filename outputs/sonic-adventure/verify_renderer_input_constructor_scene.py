"""Use the original input initializer and retail caller arguments before drawing."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_filesystem_irq_scene.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())"
assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('renderer_filesystem_irq_scene_verification.json','renderer_input_constructor_scene_verification.json').replace('renderer_filesystem_irq_scene_frontier.json','renderer_input_constructor_scene_frontier.json')
marker=" phase(0x8c098a10,[],'original game heap initialization')"
assert code.count(marker)==1
code=code.replace(marker,"""
 exact(q.get(base[0],0x8c8a83b4)==0,'input root is absent before genuine constructor')
 # 010180 loads these two static buffers at 0101d8/0101da, then calls
 # 601a34 with mode zero at 0101dc/0101de. Preserve that caller contract.
 input_first=q.get(base[0],0x8c0102cc);input_second=q.get(base[0],0x8c0102c8)
 exact((input_first,input_second)==(0x8c67c680,0x8c6886a0),'independent original caller buffer literals')
 r=phase(0x8c601a34,[0,input_first,input_second],'original game/Maple input initialization with retail static-buffer arguments')
 root=q.get(base[0],0x8c8a83b4)
 exact(root==q.get(base[0],0x8c65ecb0)==0x8c8a72b8,'actual original constructor chooses static input context')
 exact(q.get(base[0],root+0xfc4)==0xac6886a0 and q.get(base[0],root+0xfc8)==0xac67c680,'actual constructor aligns and selects uncached buffer aliases')
 for channel in range(24):
  record=q.get(base[0],0x8c601ae8)+channel*0x34
  exact(q.get(base[0],record+0x30)==root+0x44c+channel*0x78,('all original input records retain their constructed status pointer',channel))
 exact(any(b['target']=='0x8c65ebb0' for b in r['boundaries']) and any(b['target']=='0x8c65f558' for b in r['boundaries']),'actual nested input constructors execute')
"""+marker)
marker="P/'analysis/renderer_filesystem_callback_connection.json'"
assert code.count(marker)==1
code=code.replace(marker,marker+",P/'verify_renderer_input_constructor_scene.py',P/'verify_renderer_filesystem_irq_scene.py',P/'analysis/movie-heap-caller/8c010180_instructions.tsv',P/'analysis/movie-revision-dependencies/8c601a34_instructions.tsv',P/'analysis/movie-revision-lower/8c646304_instructions.tsv',P/'analysis/movie-revision-input-services/8c65ebb0_instructions.tsv'")
code=code.replace('renderer_context_from_original_startup=True,','renderer_context_from_original_startup=True,input_context_from_original_constructor=True,input_buffers_from_retail_caller_literals=True,')
marker='records.append(dict(frame_events_state='
assert code.count(marker)==1
code=code.replace(marker,"records.append(dict(input_context_root=hex(q.get(m,0x8c8a83b4)),frame_events_state=")
exec(compile(code,str(parent),'exec'),globals())
