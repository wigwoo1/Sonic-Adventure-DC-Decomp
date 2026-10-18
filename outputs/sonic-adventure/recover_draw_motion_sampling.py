"""Recover original key searches, linear curves and paired-motion dispatch."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PREVIOUS=PROJECT/'recover_draw_motion_variants.py'
exec(compile(PREVIOUS.read_text().split('inventory=json.loads')[0],str(PREVIOUS),'exec'),globals())
header=header.replace('Motion/drawing selectors, hierarchy traversal and callbacks; names inferred.','Key search, linear interpolation and paired-motion dispatch; names inferred.')
assert BASE_CHOSEN.count("('fipr ','ftrv ')")==1
BASE_CHOSEN=BASE_CHOSEN.replace("('fipr ','ftrv ')","('fipr ','ftrv ','fsrra ')")
marker="        assert not text.startswith('f'),text";assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"        if w>>8==0xca:return f'r[0]^=0x{w&255:x}u;'\n"+marker)
folder='draw-motion-sampling';directory=PROJECT/'analysis'/folder
diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'))
assert len(notes)==9 and all(n['completed']=='true' and not n['error'] for n in notes)
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_motion_sampling_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;')
 functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
source=PROJECT/'source/sa_draw_motion_sampling.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,diagnostics,PROJECT/'ghidra/export_draw_motion_sampling.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Nine original search/interpolation/paired-motion dispatch proposals. Full source from original instructions and guarded callback targets; complete behavior evidence and registration separate. No injected interpolation or game-service results.')
(PROJECT/'analysis/draw_motion_sampling_translation.json').write_text(json.dumps(report,indent=2)+'\n')
