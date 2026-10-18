"""Original compiler-table callback and two context-word helpers."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;CONTEXT_PARENT=PROJECT/'recover_draw_view_registers.py'
exec(compile(CONTEXT_PARENT.read_text().split("folder='draw-view-registers'")[0],str(CONTEXT_PARENT),'exec'),globals())
functions=[];routines=[];diagnostics=[];exports=[]
for folder,count in (('draw-frame-context',1),('draw-frame-context-support',2)):
 directory=PROJECT/'analysis'/folder;diagnostic=directory/'diagnostics.tsv';diagnostics.append(diagnostic);notes=list(csv.DictReader(diagnostic.open(),delimiter='\t'));assert len(notes)==count and all(n['completed']=='true' and not n['error'] for n in notes)
 exports.append(PROJECT/'ghidra'/('export_'+folder.replace('-','_')+'.ps1'))
 for note in notes:
  start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
  chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
  function,record=recover(start,end,f'sa_frame_context_{start:08x}',len(rows))
  assert function.count('sa_shared_service call,uint32_t *stop){')==1
  function=function.replace('sa_shared_service call,uint32_t *stop){','sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){\n int io_status=0;(void)io_status;(void)bus;')
  function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
  for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
  function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
  functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
source=PROJECT/'source/sa_draw_frame_context.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),CONTEXT_PARENT,PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+diagnostics+exports+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,scope='Actual compiler-table callback 8c661300 and its two original context helpers. Byte-checked guarded static C; behavior, caller and registration evidence remain separate.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/draw_frame_context_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Recovered compiler/context proposals:',report['instruction_rows'],'rows, zero recovery credit until verified.')
