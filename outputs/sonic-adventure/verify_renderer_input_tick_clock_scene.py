"""Repeat from original producers with enough declared samples for final timing."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_input_tick_mmio_scene.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())"
assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('import renderer_input_tick_mmio_fixtures as q','import renderer_input_tick_clock_fixtures as q')
code=code.replace('renderer_input_tick_mmio_scene_verification.json','renderer_input_tick_clock_scene_verification.json').replace('renderer_input_tick_mmio_scene_frontier.json','renderer_input_tick_clock_scene_frontier.json')
marker="P/'analysis/renderer_input_tick_mmio_connection.json'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'verify_renderer_input_tick_clock_scene.py',P/'renderer_input_tick_clock_fixtures.py',P/'verify_renderer_input_tick_mmio_scene.py'")
exec(compile(code,str(parent),'exec'),globals())
