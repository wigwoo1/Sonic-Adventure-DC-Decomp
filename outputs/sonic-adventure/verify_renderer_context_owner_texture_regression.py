"""Retain original producer-chain execution and expose its actual quad buffer."""
from pathlib import Path
P=Path(__file__).resolve().parent
parent=P/'verify_player_texture_draw.py';code=parent.read_text()
code=code.replace('import player_texture_draw_fixtures as q','import renderer_context_owner_fixtures as q')
marker='import renderer_context_owner_fixtures as q';assert code.count(marker)==1
code=code.replace(marker,marker+'\nimport write_watch_renderer_context_owner_capture as capture\nq.fast=capture')
code=code.replace('player_texture_draw_verification.json','renderer_context_owner_texture_regression_verification.json').replace('player_texture_draw_frontier.json','renderer_context_owner_texture_regression_frontier.json')
marker="command_packets=result['command_packets'],";assert code.count(marker)==1
code=code.replace(marker,marker+"sprite_command_buffers=result['sprite_command_buffers'],vertex_events=result['vertex_events'] if label=='actual stored visible particle draw with loaded textures' else [],vertex_counts=result['vertex_counts'],")
marker="P/'analysis/player_texture_draw_state_verification.json',";assert code.count(marker)==1
code=code.replace(marker,marker+"P/'verify_player_texture_draw.py',P/'write_watch_player_texture_draw_capture.py',")
marker=" exact(r['graphics_out'].vram==texture_vram,'drawing preserves existing uploaded texture VRAM')";assert code.count(marker)==1
code=code.replace(marker,marker+'''
 buffers=r['sprite_command_buffers'];exact(len(buffers)==1 and buffers[0]['bytes']==160,'independent one32-byte header plusfour32-byte vertices')
 words=buffers[0]['words'];exact([words[8+8*i] for i in range(4)]==[0xe0000000]*3+[0xf0000000],'independent threevertex commands andfinal strip-end')
 bindings=[b for b in r['boundaries'] if b['target']=='0x8c606a88'];exact(len(bindings)==1 and bindings[0]['arguments'][0]==132,'actual original draw selects expected sprite texture132')
 exact(r['graphics_out'].qacr==(12,12),'actual original storequeue configuration targets declared RAM command staging')
''')
code=code.replace('persistent_graphics_device_state=True,','persistent_graphics_device_state=True,retail_pixel_trace_values_published=False,')
code=code.replace("P/'player_texture_draw_fixtures.py'","P/'renderer_context_owner_fixtures.py'")
code=code.replace("P/'player_texture_link_reference.py'","P/'renderer_context_owner_reference.py'")
code=code.replace("P/'write_watch_player_texture_draw_capture.py'","P/'write_watch_renderer_context_owner_capture.py',P/'write_watch_renderer_context_owner_state.py'")
code=code.replace("R/'work/sonic-adventure/build/player_texture_link.dll'","R/'work/sonic-adventure/build/renderer_context_owner.dll'")
code=code.replace('native_DLL_unchanged=True,','native_DLL_unchanged=False,native_DLL_is_additive_extension=True,')
exec(compile(code,str(parent),'exec'),globals())
