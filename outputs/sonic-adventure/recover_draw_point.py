"""Recover the original explicit/implicit point transform with reached guards."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original point transform with explicit/implicit matrix; name inferred.')
chosen=chosen.replace('main-dispatch-model-traversal/','overlay-draw-missing-helpers/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
listing=PROJECT/'analysis/overlay-draw-missing-helpers/8c638ecc_instructions.tsv'
rows=list(csv.DictReader(listing.open(),delimiter='\t'));start=0x8c638ecc;end=0x8c638f24
assert {int(r['address'],16) for r in rows}==set(range(start,end,2))
function,routine=recover(start,end,'sa_draw_point_transform_8c638ecc',len(rows))
function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sx;')
source=PROJECT/'source/sa_draw_point.c';source.write_text(header+function)
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,listing,PROJECT/'source/model_point_transform.c']
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=[routine],instruction_rows=len(rows),newly_registered_entries=1,reused_routine_credit=0,prior_limited_source='source/model_point_transform.c',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Complete contiguous original point-transform body, with explicit FIPR and implicit FTRV paths, dynamic FMOV width/bank and reached RAM/FPU/return guards. Real pinned numerical support; older injected-vector experiment preserved with zero duplicate credit.')
(PROJECT/'analysis/draw_point_translation.json').write_text(json.dumps(report,indent=2)+'\n')
