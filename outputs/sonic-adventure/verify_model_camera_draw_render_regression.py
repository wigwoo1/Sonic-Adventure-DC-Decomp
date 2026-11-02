"""Retain all complete original-frame assertions on the camera draw backend."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_model_ta_jobs.py'
source=parent.read_text()
changes=(('from model_ta_jobs_verified_state import','from model_camera_draw_render_state import'),('analysis/model_ta_jobs_verification.json','analysis/model_camera_draw_render_regression_verification.json'),('model-ta-jobs-verified-frames','model-camera-draw-render-regression-images'),('model-ta-jobs-frame-normal','model-camera-draw-render-regression-normal'),("P/'model_ta_jobs_verified_state.py'","P/'model_camera_draw_render_state.py'"),("P/'build_model_ta_jobs.ps1'","P/'build_model_camera_draw_update_rsp.ps1'"),('strict_full_build_sources=305','strict_full_build_sources=307'))
for before,after in changes:
 assert before in source,before;source=source.replace(before,after)
for name in ('model_ta_jobs_reference','model_ta_jobs_fixtures','write_watch_model_ta_jobs','write_watch_model_ta_jobs_base','write_watch_model_ta_jobs_state','write_watch_model_ta_jobs_capture'):
 source=source.replace("'"+name+"'","'"+name.replace('model_ta_jobs','model_camera_draw_update')+"'")
context={'__name__':'retained_original_frame_regression','__file__':str(parent)};exec(compile(source,str(parent),'exec'),context)
if __name__=='__main__':
 context['main']()
 target=P/'analysis/model_camera_draw_render_regression_verification.json';report=json.loads(target.read_text())
 report.update(backend_under_test='model_camera_draw_update.dll',original_parent_assertions_retained=True,retained_parent=str(parent))
 report['sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 target.write_text(json.dumps(report,indent=2)+'\n')
