"""Qualify original list-state transitions and actual PVR commands in the scene."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_frame_pacing.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
def rendering_change(a,b):
 global code
 assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
code=code.replace('import renderer_frame_pacing_fixtures as q','import renderer_frame_rendering_fixtures as q').replace('renderer_frame_pacing_verification.json','renderer_frame_rendering_verification.json').replace('renderer_frame_pacing_frontier.json','renderer_frame_rendering_frontier.json')
rendering_change('records.append(dict(pacing_stores=',"records.append(dict(pvr_register_writes=result['register_writes'],ASIC_acknowledgements=result['ack_writes'],pacing_stores=")
rendering_change("P/'disc_inventory.json',asset,","P/'disc_inventory.json',asset,P/'verify_renderer_frame_rendering.py',P/'renderer_frame_rendering_fixtures.py',P/'write_watch_renderer_frame_rendering.py',")
exec(compile(code,str(parent),'exec'),globals())
