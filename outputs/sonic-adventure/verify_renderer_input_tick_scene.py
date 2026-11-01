"""Run genuine input initialization and tick callbacks in the texture/frame chain."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_input_constructor_scene.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())"
assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('import renderer_filesystem_irq_fixtures as q','import renderer_input_tick_fixtures as q')
code=code.replace('renderer_input_constructor_scene_verification.json','renderer_input_tick_scene_verification.json').replace('renderer_input_constructor_scene_frontier.json','renderer_input_tick_scene_frontier.json')
code=code.replace('build/renderer_filesystem_callback.dll','build/renderer_input_tick.dll')
marker="P/'analysis/renderer_filesystem_callback_connection.json'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'verify_renderer_input_tick_scene.py',P/'renderer_input_tick_fixtures.py',P/'renderer_input_tick_reference.py',P/'write_watch_renderer_input_tick.py',P/'analysis/renderer_input_tick_connection.json'")
marker='records.append(dict(input_context_root=';assert code.count(marker)==1
code=code.replace(marker,"records.append(dict(input_tick_calls=path.count(0x8c646024),input_submission_calls=path.count(0x8c6463ac),input_update_calls=path.count(0x8c601e2a),input_context_root=")
exec(compile(code,str(parent),'exec'),globals())
