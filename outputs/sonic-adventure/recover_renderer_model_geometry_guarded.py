"""Additive link correction of the frozen checked geometry draft; no new credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
old=P/'source/sa_renderer_model_geometry_checked.c'
prior=P/'analysis/renderer_model_geometry_checked_translation.json';proof=json.loads(prior.read_text())
for name,h in proof['sha256'].items():assert sha(name)==h,name
text=old.read_text();marker='void sa_motion_publish_stop(void *,uint32_t);'
assert text.count(marker)==1
text=text.replace(marker,'#include "sa_draw_motion_stop.h"')
target=P/'source/sa_renderer_model_geometry_guarded.c';assert not target.exists();target.write_text(text)
recipe=P/'build_renderer_model_geometry.ps1'
build=P/'build_renderer_model_geometry_guarded.ps1';assert not build.exists()
build.write_text(recipe.read_text().replace('sa_renderer_model_geometry_checked.c','sa_renderer_model_geometry_guarded.c').replace('renderer_model_geometry.dll','renderer_model_geometry_guarded.dll'))
files=[Path(__file__),old,prior,target,recipe,build,P/'source/sa_draw_motion_stop.h']
report=dict(status='guarded_geometry_inline_link_correction_behavior_proof_separate',original_rows_reused=271,new_original_routine_credit=0,prior_sources_preserved=True,change='Use existing static inline stop-publication helper; all generated instruction bodies and RAM guards are identical to the frozen checked draft.',routines=proof['routines'],sha256={str(f):sha(f) for f in files})
out=P/'analysis/renderer_model_geometry_guarded_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
