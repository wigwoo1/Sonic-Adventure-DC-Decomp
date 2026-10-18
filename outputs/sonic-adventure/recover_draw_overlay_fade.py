"""Recover original overlay fade/text/quad bodies and their render-state wrapper."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Overlay fading labels, text layout and quads; names inferred.')
BASE_CHOSEN=chosen
SPECS=((0xc90c384,'label'),(0xc90c274,'text'),(0xc90bfc0,'glyph'),(0xc90c160,'backdrop'),(0x8c605480,'filter_state'),(0x8c63a804,'vector_translate'))
functions=[];routines=[]
for start,label in SPECS:
 overlay=start<0x10000000;folder='overlay-draw-state-services' if label=='vector_translate' else 'draw-overlay-fade' if overlay else 'draw-overlay-fade-main';listing=PROJECT/f'analysis/{folder}/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 BINARY=ROOT/('work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN' if overlay else 'work/sonic-adventure/extracted/1ST_READ.BIN');binary=BINARY.read_bytes()
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/')
 if overlay:chosen=chosen.replace('0x8C010000','0x0C900000')
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_overlay_fade_{label}_{start:08x}',len(rows));function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;')
 if label=='vector_translate':
  marker=f'L{start:x}: ';assert function.count(marker)==1;function=function.replace(marker,marker+'if(f->fpscr&0x180000u)return -1;',1)
 record.update(existing_registered_entry=label=='vector_translate',module='AL_MAIN.BIN' if overlay else '1ST_READ.BIN',load_address='0x0c900000' if overlay else '0x8c010000');functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_overlay_fade.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,source,PROJECT/'ghidra/export_draw_overlay_fade.ps1',PROJECT/'ghidra/export_draw_overlay_fade_main.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java',ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN',ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='663 original instruction rows: 607 in four overlay fade/text/quad proposals and a main filter-state wrapper, plus a guarded 56-row existing vector-translation helper with its narrow-entry mode contract retained and zero new registration credit. Static guarded C retaining original state/attributes/matrix and lower rendering calls. Behavior and per-entry registration separate; no source credit from compilation alone.')
(PROJECT/'analysis/draw_overlay_fade_translation.json').write_text(json.dumps(report,indent=2)+'\n')
