"""Guarded ABI adaptation of two already recovered original register services."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_geometry_stream.py'
exec(compile(PARENT.read_text().split("folder='draw-geometry-frontier'")[0],str(PARENT),'exec'),globals())
header=header.replace('if(valid_ram(a,n))return 1;','if(valid_ram(a,n))return 1;\n if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;')
folder='draw-view-registers';directory=PROJECT/'analysis'/folder
diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'))
assert len(notes)==2 and all(n['completed']=='true' and not n['error'] for n in notes)
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_view_register_{start:08x}',len(rows))
 assert function.count('sa_shared_service call,uint32_t *stop){')==1
 function=function.replace('sa_shared_service call,uint32_t *stop){','sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){\n int io_status=0;(void)io_status;(void)bus;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_view_registers.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/render_register_bus.h',PROJECT/'source/render_register_bus.c',diagnostics,PROJECT/'ghidra/export_draw_view_registers.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,scope='Two already registered original bodies adapted to reached guards and the explicit state/bus ABI. New host register storage is separately tested; no new original routine credit or GPU execution claim.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/draw_view_registers_translation.json').write_text(json.dumps(report,indent=2)+'\n')
print('Adapted original register services:',report['instruction_rows'],'rows; zero duplicate recovery credit.')
