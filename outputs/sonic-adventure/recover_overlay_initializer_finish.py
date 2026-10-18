"""Recover the final initializer dependencies and their original tail helpers."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
GENERATOR=PROJECT/'recover_overlay_model_blend.py'
prefix=GENERATOR.read_text().split('function,blend_record=recover')[0]
exec(compile(prefix,str(GENERATOR),'exec'),globals())
header=header.replace('AL_MAIN model morph and word-copy dispatcher; names inferred.', 'AL_MAIN animation, appearance, accessories and material tails; names inferred.')
functions=[];routines=[]
specs=((0xc90bd40,0xc90bdb6,59,'animation_start'),(0xc90853e,0xc908676,134,'appearance_select'),(0xc9048ec,0xc904aea,237,'accessory_install'),(0xc90fb82,0xc90fc54,105,'tail_palette'),(0xc90fca0,0xc90fd42,81,'tail_material_158'),(0xc90fd42,0xc90fde4,81,'tail_material_161'),(0xc9084e6,0xc9084fc,11,'appearance_mode'),(0xc9084fc,0xc908520,18,'appearance_pair'),(0xc908530,0xc90853e,7,'appearance_links'))
for start,end,count,label in specs:
 chosen=overlay_code.replace('overlay-initializer-dependencies/','overlay-initializer-finish/') if start in (0xc9084e6,0xc9084fc,0xc908530) else overlay_code
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_overlay_{label}_{start:08x}',count)
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sr;')
 record.update(module='AL_MAIN.BIN',load_address='0x0c900000',memory_guards='Dynamic RAM transfer and entry-PR guards publish exact reached instruction; no hardware exception delivery.')
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_overlay_initializer_finish.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),GENERATOR,PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'ghidra/export_overlay_initializer_finish.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(module='AL_MAIN.BIN',load_address='0x0c900000',source=source.relative_to(PROJECT).as_posix(),routines=routines,newly_recovered_entries=8,retranslated_existing_entry='0xc908530',reused_setter_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Nine complete static C bodies / 733 byte-checked instruction rows: the remaining three initializer helpers, three lower appearance setters and three outer material tails. Dynamic RAM and arithmetic/return guards retain reached state. Existing RNG reused separately; native behavior verified independently.')
(PROJECT/'analysis/overlay_initializer_finish_translation.json').write_text(json.dumps(report,indent=2)+'\n')
