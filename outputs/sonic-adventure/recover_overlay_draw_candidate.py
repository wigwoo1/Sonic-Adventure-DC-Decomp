"""Translate complete candidate draw control flow; behavioral scope is partial."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_overlay_model_blend.py'
exec(compile(PARENT.read_text().split('function,blend_record=recover')[0],str(PARENT),'exec'),globals())
header=header.replace('AL_MAIN model morph and word-copy dispatcher; names inferred.','AL_MAIN object draw candidate: complete static body, partial integration proof.')
chosen=overlay_code.replace('overlay-initializer-dependencies/','overlay-object-update-callers/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
function,record=recover(0xc9058a0,0xc90608e,'sa_overlay_object_draw_candidate_0c9058a0',911)
source=PROJECT/'source/sa_overlay_object_draw_candidate.c';source.write_text(header+function)
files=[Path(__file__),PARENT,PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,Path(record['listing'])]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=[record],instruction_rows=911,newly_recovered_entries=0,registration_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Complete static byte-checked candidate body. Behavioral integration is deliberately partial; no full-function recovery credit until remaining draw paths and dependencies are verified.')
(PROJECT/'analysis/overlay_draw_candidate_translation.json').write_text(json.dumps(report,indent=2)+'\n')
