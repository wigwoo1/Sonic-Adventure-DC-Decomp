"""Follow actual allocated particle draw with the original whole frame wrapper."""
from pathlib import Path
P=Path(__file__).resolve().parent
builder=P/'verify_renderer_context_producer_matrix.py';source=builder.read_text()
ending="exec(compile(code,str(parent),'exec'),globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('import renderer_context_producer_disc_fixtures as q','import renderer_frame_submit_fixtures as q')
code=code.replace('renderer_context_producer_matrix_verification.json','renderer_frame_submit_verification.json')
code=code.replace('renderer_context_producer_matrix_frontier.json','renderer_frame_submit_frontier.json')
marker=" phase(q.get(base[0],child+0x18),[child],'actual stored child cleanup')";assert code.count(marker)==1
code=code.replace(marker," phase(0x8c6050b0,[],'actual original whole frame finalization/submission wrapper')\n"+marker)
marker="P/'verify_renderer_context_producer_disc.py'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'verify_renderer_context_producer_matrix.py'")
marker="P/'renderer_context_producer_fixtures.py'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'renderer_frame_submit_fixtures.py',P/'write_watch_renderer_frame_submit.py'")
exec(compile(code,str(parent),'exec'),globals())
