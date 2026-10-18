"""Recover original animation format routing and keyframe interpolation."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Motion format routing, keyframe samplers and interpolators; names inferred.')
chosen=chosen.replace('main-dispatch-model-traversal/','draw-motion-formats/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
SPECS=((0x8c60d054,'format_one'),(0x8c60d094,'format_two'),(0x8c60d0d4,'format_three'),(0x8c60d140,'format_four'),(0x8c60d180,'format_mesh'),(0x8c60d19e,'format_six_seven'),(0x8c60d1c8,'format_eight'),(0x8c60c322,'sample_position'),(0x8c60c3b8,'sample_scale'),(0x8c60c488,'sample_rotation'),(0x8c60c526,'sample_rotation_yxz'),(0x8c60beb0,'linear_vector'),(0x8c60bf5e,'linear_angles'),(0x8c60c026,'cubic_vector'),(0x8c60c19c,'cubic_angles'))
functions=[];routines=[]
for start,label in SPECS:
 listing=PROJECT/f'analysis/draw-motion-formats/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 function,record=recover(start,end,f'sa_motion_{label}_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;');functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_motion_formats.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'ghidra/export_draw_motion_formats.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_recovered_entries=len(routines),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Static guarded original format callbacks, position/scale/angle keyframe search and linear/cubic interpolation. Distinct motion mesh output remains a lower dependency; behavior proof separate.')
(PROJECT/'analysis/draw_motion_formats_translation.json').write_text(json.dumps(report,indent=2)+'\n')
