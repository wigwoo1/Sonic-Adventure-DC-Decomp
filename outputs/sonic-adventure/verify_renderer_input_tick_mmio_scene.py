"""Qualify normal frame control flow after genuine input and interrupt producers."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_input_tick_scene.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())"
assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('import renderer_input_tick_fixtures as q','import renderer_input_tick_mmio_fixtures as q')
code=code.replace('renderer_input_tick_scene_verification.json','renderer_input_tick_mmio_scene_verification.json').replace('renderer_input_tick_scene_frontier.json','renderer_input_tick_mmio_scene_frontier.json')
code=code.replace('build/renderer_input_tick.dll','build/renderer_input_tick_mmio.dll')
marker="P/'analysis/renderer_input_tick_connection.json'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'verify_renderer_input_tick_mmio_scene.py',P/'renderer_input_tick_mmio_fixtures.py',P/'renderer_input_tick_mmio_reference.py',P/'write_watch_renderer_input_tick_mmio.py',P/'analysis/renderer_input_tick_mmio_connection.json'")
marker=" phase(0x8c6050b0,[],'actual original whole frame finalization/submission wrapper')";assert code.count(marker)==1
code=code.replace(marker,"""
 frame_counter=q.get(base[0],q.get(base[0],0x8c60520c))
 frame=phase(0x8c6050b0,[],'actual original whole frame finalization/submission wrapper')
 exact(frame['runtime_state'][8]==2 and frame['runtime_state'][9:11]==(0,0),'two complete IRQ/RTE paths with no pending borrowed interrupt frame')
 exact(q.get(base[0],0x8c8a30c0)==0,'original frame submission clears busy state')
 exact(sum(b['target']=='0x8c601e2a' for b in frame['boundaries'])==1,'original input update executes inside whole frame')
 exact(sum(b['target']=='0x8c646868' for b in frame['boundaries'])==24,'original input update visits all 24 status accessors')
 exact(q.get(base[0],q.get(base[0],0x8c60520c))==(frame_counter+1)&0xffffffff,'independent whole-frame counter increment')
 frame_source=q.get(base[0],q.get(base[0],0x8c60521c))
 for i,literal in enumerate((0x8c605220,0x8c605224,0x8c605228,0x8c60522c,0x8c605230)):
  exact(q.get(base[0],q.get(base[0],literal))==q.get(base[0],frame_source+4*i),('independent final five-word context publication',i))
 exact(not any(b['target']=='0x8c650b60' and b['pr'] in ('0x8c651d92','0x8c651eae') for b in frame['boundaries']),'frame does not use SDK watchdog recovery')
""")
code=code.replace("dict(valid_texture_descriptors=138,","dict(whole_6050b0_frame_control_flow_return_qualified=True,valid_texture_descriptors=138,")
exec(compile(code,str(parent),'exec'),globals())
