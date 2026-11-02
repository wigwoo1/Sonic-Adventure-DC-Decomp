"""Re-run the retained model/frame assertions on the guarded callback backend."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
parent=P/'verify_model_float_render.py'
source=parent.read_text()
for old,new in (("analysis/model_float_render_verification.json","analysis/model_ta_stream_bounded_render_verification.json"),("model-float-render-verified-v2","model-ta-stream-bounded-render-regression"),("model_float_color_fixtures","model_ta_stream_bounded_fixtures"),("write_watch_model_float_color_state","write_watch_model_ta_stream_bounded_state")):
 assert old in source,old
 source=source.replace(old,new)
context={'__name__':'retained_regression','__file__':str(parent)}
exec(compile(source,str(parent),'exec'),context)
if __name__=='__main__':
 context['main']()
 target=P/'analysis/model_ta_stream_bounded_render_verification.json'
 report=json.loads(target.read_text())
 files=[Path(__file__),parent]
 files += [P/(name+'.py') for name in ('model_ta_stream_bounded_reference','model_ta_stream_bounded_fixtures','write_watch_model_ta_stream_bounded','write_watch_model_ta_stream_bounded_base','write_watch_model_ta_stream_bounded_state','write_watch_model_ta_stream_bounded_capture')]
 report['retained_test_parent']=str(parent)
 report['backend_under_test']='model_ta_stream_bounded.dll'
 report['original_test_assertions_retained']=True
 report['sha256'].update({str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n')
