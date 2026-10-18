"""Recover original geometry/view proposals with GBR and the existing queue bus."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;GBR_PARENT=PROJECT/'recover_draw_gbr_geometry.py'
exec(compile(GBR_PARENT.read_text().split("folder='draw-gbr-geometry'")[0],str(GBR_PARENT),'exec'),globals())
header=header.replace('Renderer initialization, geometry and explicit GBR state; names inferred.','Unsigned-label vector interpolation; explicit GBR/queue state.')
# Reuse the already verified polygon primitive bus adaptation, preserving all
# ordinary RAM and floating-move paths. The old source files remain unchanged.
header=header.replace('#include "fpu_cleanup_transfer.h"','#include "fpu_cleanup_transfer.h"\n#include "vertex_bus.h"')
header=header.replace('uint8_t *m,uint32_t a,','uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,')
header=header.replace(' uint32_t value=0;for(unsigned i=0;i<n;i++)',' if((a&0x1f000000u)!=0x0c000000u){uint32_t value=0;if(!bus||!bus->read){*io_status=-2;return 0;}*io_status=bus->read(bus->context,a,n,&value);return value;}\n uint32_t value=0;for(unsigned i=0;i<n;i++)',1)
header=header.replace(' for(unsigned i=0;i<n;i++)m[',' if((a&0x1f000000u)!=0x0c000000u){if(!bus||!bus->write){*io_status=-2;return;}*io_status=bus->write(bus->context,a,value,n);return;}\n for(unsigned i=0;i<n;i++)m[',1)
header=header.replace('rd(m,','rd(m,bus,io_status,').replace('wr(m,','wr(m,bus,io_status,').replace('uint8_t *m,struct sa_fpu_banks *f','uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f')
header+='''static int valid_access(uint32_t a,unsigned n){
 if(valid_ram(a,n))return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
'''
geometry_prior_guard=memory_guard
def memory_guard(w):
 guard=geometry_prior_guard(w)
 return guard if w>>8 in (0xc2,0xc6) else guard.replace('valid_ram(','valid_access(')
marker="        assert not text.startswith('f'),text";assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""        if text.startswith('pref '):return f'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],1u);if(status)return status;'
        if text.startswith('ocbi '):return f'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],0u);if(status)return status;'
"""+marker)
folder='draw-continuation-frontier';directory=PROJECT/'analysis'/folder
diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'))
assert len(notes)==1 and all(n['completed']=='true' and not n['error'] for n in notes)
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_vector_curve_{start:08x}',len(rows))
 assert function.count('sa_shared_service call,uint32_t *stop){')==1
 function=function.replace('sa_shared_service call,uint32_t *stop){','sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){\n int io_status=0;(void)io_status;(void)bus;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
source=PROJECT/'source/sa_draw_vector_curve.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,PROJECT/'recover_draw_polygon_lower.py',PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus_lighting.c',diagnostics,PROJECT/'ghidra/export_draw_continuation_frontier.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),explicit_special_register='GBR',queue_backend='source/vertex_bus_lighting.c',newly_registered_entries=0,reused_routine_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='One observed original vector-curve helper. Original static C with explicit GBR, reached guards and existing MMU-off coherent-RAM/store-queue bus including OCBI/PREF. Ordinary RAM accesses are preserved. Behavior/registration separate; missing original callees remain explicit.')
(PROJECT/'analysis/draw_vector_curve_translation.json').write_text(json.dumps(report,indent=2)+'\n')
