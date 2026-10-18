"""Recover the AL_MAIN parameter approach/event and scale-smoothing bodies."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PARENT=PROJECT/'recover_overlay_model_blend.py'
exec(compile(PARENT.read_text().split('function,blend_record=recover')[0],str(PARENT),'exec'),globals())
header=header.replace('AL_MAIN model morph and word-copy dispatcher; names inferred.', 'AL_MAIN parameter update and scale smoothing; names inferred.')
exec(compile(overlay_code.replace('overlay-initializer-dependencies/','overlay-selector-initializer/'),str(TEMPLATE),'exec'),globals())
functions=[];routines=[]
for start,end,count,label in ((0xc905618,0xc905666,39,'scale_spring'),(0xc905666,0xc905872,236,'parameter_update')):
 function,record=recover(start,end,f'sa_overlay_{label}_{start:08x}',count)
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sr;')
 record.update(module='AL_MAIN.BIN',load_address='0x0c900000',memory_guards='Dynamic RAM and entry-PR guards publish reached state; no hardware exception delivery.')
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_overlay_parameter_update.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT,PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source]+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,newly_recovered_entries=2,instruction_rows=275,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Two static guarded native C bodies. Scale smoothing and parameter clamps/events/approach; update conditionally tail-branches into the previously recovered morph. These bodies do not draw pixels.')
(PROJECT/'analysis/overlay_parameter_update_translation.json').write_text(json.dumps(report,indent=2)+'\n')
