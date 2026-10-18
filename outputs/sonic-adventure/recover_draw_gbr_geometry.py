"""Recover renderer initialization/geometry with explicit original GBR state."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
SAMPLING=PROJECT/'recover_draw_motion_sampling.py'
exec(compile(SAMPLING.read_text().split("folder='draw-motion-sampling'")[0],str(SAMPLING),'exec'),globals())
header=header.replace('Key search, linear interpolation and paired-motion dispatch; names inferred.','Renderer initialization, geometry and explicit GBR state; names inferred.')
assert BASE_CHOSEN.count("('fipr ','ftrv ','fsrra ')")==1
BASE_CHOSEN=BASE_CHOSEN.replace("('fipr ','ftrv ','fsrra ')","('fipr ','ftrv ','fsrra ','fsca ')")
marker="        assert not text.startswith('f'),text";assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""        if w&0xf0ff==0x401e:return f'*gbr=r[{n}];'
        if w==0x0018:return '*sr|=1u;'
        if w==0x0008:return '*sr&=~1u;'
        if w>>8==0xc6:return f'r[0]=rd(m,*gbr+0x{(w&255)*4:x}u,4);'
        if w>>8==0xc2:return f'wr(m,*gbr+0x{(w&255)*4:x}u,r[0],4);'
"""+marker)
old_memory_guard=memory_guard
def memory_guard(w):
 if w>>8 in (0xc2,0xc6):return f'if(!valid_ram(*gbr+0x{(w&255)*4:x}u,4u))return -3;'
 return old_memory_guard(w)
folder='draw-gbr-geometry';directory=PROJECT/'analysis'/folder
diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'))
assert len(notes)==7 and all(n['completed']=='true' and not n['error'] for n in notes)
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_gbr_geometry_{start:08x}',len(rows))
 assert function.count('sa_shared_service call,uint32_t *stop){')==1
 function=function.replace('sa_shared_service call,uint32_t *stop){','sa_shared_service call,uint32_t *stop,uint32_t *gbr){')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
source=PROJECT/'source/sa_draw_gbr_geometry.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,diagnostics,PROJECT/'analysis/draw-gbr-state/transfers.tsv',PROJECT/'ghidra/export_draw_gbr_geometry.ps1',PROJECT/'ghidra/export_draw_gbr_state.ps1',PROJECT/'ghidra/ExportGbrStateWriters.java',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,explicit_special_register='GBR',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Seven original renderer setup/geometry/sampling proposals with explicit live GBR pointer, original LDC and guarded GBR-relative RAM loads/stores. Older sources/ABIs preserved; behavior and registration separate. No guessed fixed GBR operand or injected game-service result.')
(PROJECT/'analysis/draw_gbr_geometry_translation.json').write_text(json.dumps(report,indent=2)+'\n')
