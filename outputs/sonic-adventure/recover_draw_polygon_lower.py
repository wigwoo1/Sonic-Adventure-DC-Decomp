"""Recover original clipping/projection primitives with the existing queue bus."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Polygon clipping/projection and header acquisition; names inferred.')
header=header.replace('#include "fpu_cleanup_transfer.h"','#include "fpu_cleanup_transfer.h"\n#include "vertex_bus.h"')
header=header.replace('uint8_t *m,uint32_t a,','uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,')
header=header.replace(' uint32_t value=0;for(unsigned i=0;i<n;i++)',' if((a&0x1f000000u)!=0x0c000000u){uint32_t value=0;if(!bus||!bus->read){*io_status=-2;return 0;}*io_status=bus->read(bus->context,a,n,&value);return value;}\n uint32_t value=0;for(unsigned i=0;i<n;i++)',1)
header=header.replace(' for(unsigned i=0;i<n;i++)m[',' if((a&0x1f000000u)!=0x0c000000u){if(!bus||!bus->write){*io_status=-2;return;}*io_status=bus->write(bus->context,a,value,n);return;}\n for(unsigned i=0;i<n;i++)m[',1)
header=header.replace('rd(m,','rd(m,bus,io_status,').replace('wr(m,','wr(m,bus,io_status,').replace('uint8_t *m,struct sa_fpu_banks *f','uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f')
header+='int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);\n'
header+='''static int valid_access(uint32_t a,unsigned n){
 if(valid_ram(a,n))return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
'''
# Keep the same reached-access guards, adding only the modeled bus domains.
old_memory_guard=memory_guard
def memory_guard(w):return old_memory_guard(w).replace('valid_ram(','valid_access(')
chosen=chosen.replace('main-dispatch-model-traversal/','draw-polygon-lower/')
marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
chosen=chosen.replace(marker,"        if text.startswith('fsqrt '):return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[{n}],&f->fpscr,&fr[{n}]);if(status)return status;}}'\n        if text.startswith('pref '):return f'if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[{n}],1u);if(status)return status;'\n"+marker)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
SPECS=((0x8c64f840,'near_clip'),(0x8c60607a,'header_uv'),(0x8c605fca,'header_colour'),(0x8c64fc68,'emit_uv'),(0x8c64fae8,'emit_colour'),(0x8c650144,'emit_projected'),(0x8c64fe0c,'emit_projected_uv'))
functions=[];routines=[]
for start,label in SPECS:
 listing=PROJECT/f'analysis/draw-polygon-lower/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 function,record=recover(start,end,f'sa_polygon_{label}_{start:08x}',len(rows))
 function=function.replace('uint32_t *stop){','uint32_t *stop,const struct sa_vertex_bus *bus){\n int io_status=0;(void)io_status;(void)bus;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_polygon_lower.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'source/vertex_bus.h',PROJECT/'source/vertex_bus.c',PROJECT/'ghidra/export_draw_polygon_lower.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='1720 original instruction rows in seven proposed clipping/header/projection primitives. Static guarded C, original numerical and conditional behavior; existing MMU-off queue bus for stores/PREF. Compilation and byte checks do not grant native recovery credit; per-body behavior verification remains separate.')
(PROJECT/'analysis/draw_polygon_lower_translation.json').write_text(json.dumps(report,indent=2)+'\n')
