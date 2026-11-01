"""Retain actual empty-frame producers on the additive native TA backend."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_renderer_frame_submit_state.py';code=parent.read_text()
code=code.replace('import renderer_frame_submit_clock_fixtures as q','import renderer_frame_ta_fixtures as q')
code=code.replace('renderer_frame_submit_state_verification.json','renderer_frame_ta_state_verification.json').replace('renderer_frame_submit_state_frontier.json','renderer_frame_ta_state_frontier.json')
code=code.replace("P/'renderer_frame_submit_clock_fixtures.py'","P/'renderer_frame_submit_clock_fixtures.py',P/'renderer_frame_ta_fixtures.py',P/'write_watch_renderer_frame_ta.py',P/'renderer_frame_ta_dma_reference.py',P/'verify_renderer_frame_submit_state.py'")
code=code.replace("R/'work/sonic-adventure/build/renderer_context_owner.dll'","R/'work/sonic-adventure/build/renderer_frame_ta.dll'")
exec(compile(code,str(parent),'exec'),globals())
