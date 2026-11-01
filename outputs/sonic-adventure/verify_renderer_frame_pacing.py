"""Trace wait inputs and their original store producers in the accepted scene."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_frame_clock_scene.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def pacing_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
code=code.replace('import renderer_frame_clock_fixtures as q','import renderer_frame_pacing_fixtures as q').replace('renderer_frame_clock_scene_verification.json','renderer_frame_pacing_verification.json').replace('renderer_frame_clock_scene_frontier.json','renderer_frame_pacing_frontier.json')
pacing_change('records.append(dict(frame_clock_out=',"records.append(dict(pacing_stores=result['pacing_stores'],frame_clock_out=")
pacing_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,P/'verify_renderer_frame_pacing.py',P/'renderer_frame_pacing_fixtures.py',P/'renderer_frame_pacing_trace.py',P/'write_watch_renderer_frame_pacing.py',")
exec(compile(code,str(parent),'exec'),globals())
