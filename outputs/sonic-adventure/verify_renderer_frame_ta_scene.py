"""Submit actual texture-backed particle commands through the TA DMA endpoint."""
from pathlib import Path
P=Path(__file__).resolve().parent;builder=P/'verify_renderer_frame_submit.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('import renderer_frame_submit_fixtures as q','import renderer_frame_ta_fixtures as q')
code=code.replace('renderer_frame_submit_verification.json','renderer_frame_ta_scene_verification.json').replace('renderer_frame_submit_frontier.json','renderer_frame_ta_scene_frontier.json')
marker="  result,path,m,s,f=q.fast.compare(m,s,f,c)";assert code.count(marker)==1
code=code.replace(marker,"  if entry==0x8c6050b0:c['ticks']=list(q.FRAME_TICKS)\n"+marker)
code=code.replace("R/'work/sonic-adventure/build/renderer_context_owner.dll'","R/'work/sonic-adventure/build/renderer_frame_ta.dll'")
marker="P/'write_watch_renderer_frame_submit.py'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'renderer_frame_ta_fixtures.py',P/'renderer_frame_ta_reference.py',P/'write_watch_renderer_frame_ta.py',P/'renderer_frame_ta_dma_reference.py',P/'analysis/renderer_frame_ta_connection.json',P/'verify_renderer_frame_submit.py'")
code=code.replace('native_DLL_unchanged=True,','native_DLL_unchanged=False,native_game_C_unchanged=True,native_backend_is_additive_extension=True,')
exec(compile(code,str(parent),'exec'),globals())
