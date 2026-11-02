"""Retain every copy-unit assertion on the current scene-owner backend."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_model_camera_copy.py'
source=parent.read_text()
source=source.replace('model_camera_copy_verification.json','model_camera_scene_copy_verification.json')
for old,new in (('model_camera_copy_v2_fixtures','model_camera_scene_fixtures'),('write_watch_model_camera_copy_v2_state','write_watch_model_camera_scene_state')):source=source.replace(old,new)
for name in ('model_camera_copy_v2_reference','model_camera_copy_v2_fixtures','write_watch_model_camera_copy_v2','write_watch_model_camera_copy_v2_base','write_watch_model_camera_copy_v2_state','write_watch_model_camera_copy_v2_capture'):
 source=source.replace("'"+name+"'","'"+name.replace('model_camera_copy_v2','model_camera_scene')+"'")
context={'__name__':'retained_copy_unit','__file__':str(parent)}
exec(compile(source,str(parent),'exec'),context)
if __name__=='__main__':
 context['main']()
 target=P/'analysis/model_camera_scene_copy_verification.json';report=json.loads(target.read_text())
 report.update(canonical_backend='model_camera_scene.dll',all_parent_unit_assertions_retained=True)
 report['sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 target.write_text(json.dumps(report,indent=2)+'\n')
