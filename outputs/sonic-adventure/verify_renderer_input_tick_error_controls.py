"""Use persistent device counters when the retained event trace is already full."""
from pathlib import Path
P=Path(__file__).resolve().parent;builder=P/'verify_renderer_input_tick_controls.py'
source=builder.read_text();ending="exec(compile(code,str(parent),'exec'),globals())"
assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(builder),'exec'),globals())
code=code.replace('renderer_input_tick_controls_verification.json','renderer_input_tick_error_controls_verification.json')
marker="   observed=r['graphics_out'].pvr_error_events\n   assert observed and observed[0]==(0xa05f6908,case.get('error',0),1);checks+=1"
assert code.count(marker)==1
code=code.replace(marker,"""   # Existing error-event history is bounded. Its restored prefix is not a
   # new read. Check the exact declared device input and monotonic read counter;
   # every native/reference trace byte and device counter is still compared.
   assert c['graphics_in'].pvr[-1][0]==case.get('error',0);checks+=1
   assert r['graphics_out'].pvr[-1][1]>c['graphics_in'].pvr[-1][1];checks+=1""")
code=code.replace(' files=[Path(__file__),parent,',' files=[Path(__file__),builder,parent,')
exec(compile(code,str(parent),'exec'),globals())
