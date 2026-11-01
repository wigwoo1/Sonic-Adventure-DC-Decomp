"""Compose actual startup/context producers before original textured particle draw."""
from pathlib import Path
P = Path(__file__).resolve().parent
parent = P / 'verify_player_texture_draw.py'
code = parent.read_text()


def replace_once(before, after):
    global code
    assert code.count(before) == 1, before
    code = code.replace(before, after)


replace_once('import player_texture_draw_fixtures as q', 'import renderer_context_producer_fixtures as q')
code = code.replace('player_texture_draw_verification.json', 'renderer_context_producer_verification.json')
code = code.replace('player_texture_draw_frontier.json', 'renderer_context_producer_frontier.json')
replace_once("command_packets=result['command_packets'],", "command_packets=result['command_packets'],sprite_command_buffers=result['sprite_command_buffers'],vertex_events=result['vertex_events'] if label=='actual stored visible particle draw with loaded textures' else [],vertex_counts=result['vertex_counts'],polygon_context_root=hex(q.get(m,0x8c88fcd4)),polygon_context_words=[q.get(m,q.get(m,0x8c88fcd4)+4*i) for i in range(5)] if q.get(m,0x8c88fcd4) else [],")
replace_once("P/'player_texture_draw_fixtures.py'", "P/'renderer_context_producer_fixtures.py'")
replace_once("P/'write_watch_player_texture_draw.py'", "P/'write_watch_renderer_context_owner_capture.py',P/'write_watch_renderer_context_owner_state.py',P/'verify_player_texture_draw.py'")
replace_once("P/'player_texture_link_reference.py'", "P/'renderer_context_owner_reference.py'")
replace_once("R/'work/sonic-adventure/build/player_texture_link.dll'", "R/'work/sonic-adventure/build/renderer_context_owner.dll'")
replace_once('persistent_graphics_device_state=True,', 'persistent_graphics_device_state=True,retail_pixel_trace_values_published=False,renderer_context_is_declared_input=False,renderer_context_from_original_startup=True,')
replace_once(" phase(0x8c098a10,[],'original game heap initialization')", """
 exact(q.get(base[0],0x8c88fcd4)==q.get(base[0],0x8c88fcb4)==0,'original null context/view roots before actual producers')
 phase(0x8c602128,[0x8cb00000,0x100000],'original system-heap initialization on declared caller arena')
 phase(0x8c604702,[1,0,1,15],'original display startup and context-input record producer')
 exact(q.get(base[0],0x8c88fdb4)==96 and q.get(base[0],0x8c88fdd4)==0x8c88fcd4,'independent original record size/output-pointer field')
 exact(q.get(base[0],0x8c88fcd4)==0,'display setup alone does not fabricate polygon context')
 phase(0x8c604dce,[-1,0,0x20000,0],'original allocated polygon-context initialization',count=0)
 exact(q.get(base[0],0x8c88fcd4)==0x8c8a33c0,'independent original indirect context root')
 phase(0x8c098a10,[],'original game heap initialization')""")
replace_once(" q.put(base[0],0x8c67c2c8,0);phase(0x8c663140,[],'original graphics allocator constructor,declared platform mode0');phase(0x8c641160,[],'original driver default installation')", " exact(q.get(base[0],0x8c88fcd4)==0x8c8a33c0,'actual context survives game/filesystem/texture-table initialization;no later graphics allocator reset')")
replace_once(" screen=q.get(base[0],0x8c604ce0);q.put(base[0],screen+4,q.bits(640));q.put(base[0],screen+8,q.bits(480));phase(0x8c604ba0,[],'original640x480view builder')", " screen=q.get(base[0],0x8c604ce0);exact(q.get(base[0],screen+4)==q.bits(640) and q.get(base[0],screen+8)==q.bits(480),'actual startup-selected640x480 screen');phase(0x8c604ba0,[],'original640x480view builder')")
replace_once(" exact(r['graphics_out'].vram==texture_vram,'drawing preserves existing uploaded texture VRAM')", """ exact(r['graphics_out'].vram==texture_vram,'drawing preserves existing uploaded texture VRAM')
 buffers=r['sprite_command_buffers'];exact(len(buffers)==1 and buffers[0]['bytes']==160,'independent one32-byte header plusfour32-byte vertices')
 words=buffers[0]['words'];exact([words[8+8*i] for i in range(4)]==[0xe0000000]*3+[0xf0000000],'independent threevertex commands andfinal strip-end')
 bindings=[b for b in r['boundaries'] if b['target']=='0x8c606a88'];exact(len(bindings)==1 and bindings[0]['arguments'][0]==132,'actual draw selects original sprite texture132')
 owner=q.get(base[0],0x8c88fdd8)&0x8fffffff;cursor=int(buffers[0]['ram_address'],16)
 exact(0x8cb00000<=owner<=cursor<cursor+160<=0x8cc00000,'quad command buffer lies inside actual owned system-heap arena')
 exact(q.get(base[0],0x8c88fcd4)==0x8c8a33c0,'actual original polygon context remains selected')
""")
replace_once("scope='Ordinary original heap/filesystem/GPU-memory constructor/resource loading followed by real task/player/collision/child/spawn/view/draw. Match complete original/native state andevery boundary per call;carry actual equal RAM/CPU/FPU andmatched graphics outputs at normal returns,including all8MiBVRAM/registers/storequeues/DMA. External timing/sample services retain their declared scenario inputs. Source retail images remain local. Fault state is discarded,never reentered.'", "scope='Raw original RAM with null polygon/view roots. Actual602128 system heap,604702 display/config record and604dce/656940 allocated context execute before real filesystem/resource/player/particle calls. No polygon-context pointer,cursor,view dimensions,allocator/callee result orprivate frame is supplied. Complete original/native state andordered boundaries match;matched normal-return graphics state persists. External GPU identity/display-mode/scan/IRQ samples andcaller arena/scene/matrix inputs remain declared;not full hardware continuity,reset boot,scheduler,frame consumption ordisplayed pixels. Retail inputs remain local;raw pixel traces filtered.'")
exec(compile(code, str(parent), 'exec'), globals())
