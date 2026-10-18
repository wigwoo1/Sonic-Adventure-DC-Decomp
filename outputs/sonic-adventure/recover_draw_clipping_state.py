"""Static clipping proposals with MACH, both integer banks and scoped probes."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PARENT=PROJECT/'recover_draw_vector_blend.py'
exec(compile(PARENT.read_text().split("folder='draw-post-angle-frontier'")[0],str(PARENT),'exec'),globals())
header=header.replace('Sequential two-vector interpolation','Clipping proposals with integer banks and MACH')
header=header.replace('#include "vertex_bus.h"','#include "vertex_bus.h"\n#include "sh4_integer_state.h"')
marker="        assert not text.startswith('f'),text";assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""        prior=rows.get(a-2,(0,''))[0]
        delay=int(prior in (0x000b,0x002b) or prior>>12 in (0xa,0xb) or prior>>8 in (0x8d,0x8f) or prior&0xf0ff in (0x400b,0x402b,0x0003,0x0023))
        if w&0xf0ff==0x0002:return f'status=sa_sh4_stc_sr(*sr,&r[{n}],{delay}u);if(status)return status;'
        if w&0xf0ff==0x400e:return f'status=sa_sh4_ldc_sr(r[{n}],sr,r,integer,{delay}u);if(status)return status;'
        if w&0xf00f==0x300d:return f'sa_sh4_dmuls_state(r[{n}],r[{m}],macl,integer);'
        if w&0xf0ff==0x000a:return f'r[{n}]=integer->mach;'
        if text.startswith('flds '):return f'*fpul=f->bank[(f->fpscr>>21)&1u][{n}];'
"""+marker)
marker="return f'*stop=0x{a:x}u;'+memory_guard(rows[a][0])+raw_simple(a)"
assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,marker+"+(f'if(checkpoint==0x{a:x}u){{*stop=0x{a+2:x}u;return 2;}}' if a in CHECKPOINTS else '')")
# This family conditionally tail-transfers into another original clipper body.
marker='for target in (a+4+2*signed(w&255,8),fall):todo.append(target);edges.add((a,target))'
assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""for target in (a+4+2*signed(w&255,8),fall):
                if target in rows:todo.append(target)
                else:calls.add(a)
                edges.add((a,target))""")
marker="code=f'take={condition};'+simple(a+2)+f'if(take)goto L{target:x};goto L{a+4:x};'"
assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""jump=f'goto L{target:x};' if target in rows else f'if(!call)return -2;return call(ctx,0x{target:x}u,m,f,sr,r,pr);'
                code=f'take={condition};'+simple(a+2)+f'if(take){{{jump}}}goto L{a+4:x};'""")
marker="else:code=f'if({condition})goto L{target:x};goto L{a+2:x};'"
assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""else:
                jump=f'goto L{target:x};' if target in rows else f'if(!call)return -2;return call(ctx,0x{target:x}u,m,f,sr,r,pr);'
                code=f'if({condition}){{{jump}}}goto L{a+2:x};'""")
audit=PROJECT/'analysis/draw_primitive_sr_transitions.json'
probes=[dict(entry=int(r['pattern_start'],16),checkpoint=int(r['pc'],16),kind='sr_mask',owner=int(r['entry_export'],16)) for r in json.loads(audit.read_text())['sr_transitions']]
folder='draw-primitive-frontier';directory=PROJECT/'analysis'/folder
diagnostics=directory/'diagnostics.tsv';notes=list(csv.DictReader(diagnostics.open(),delimiter='\t'))
notes=[n for n in notes if int(n['address'],16) in {0x8c037ee0,0x8c037ff4,0x8c038516,0x8c038802}]
assert len(notes)==4 and all(n['completed']=='true' and not n['error'] for n in notes)
functions=[];routines=[]
for note in notes:
 start=int(note['address'],16);listing=directory/f'{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 for row in rows:
  if row['instruction'].startswith('dmuls.l '):probes.append(dict(entry=int(row['address'],16),checkpoint=int(row['address'],16)+2,kind='signed_product',owner=start))
 own=[p for p in probes if p['owner']==start];CHECKPOINTS={p['checkpoint'] for p in own}
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_clipping_state_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,'sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t entry,uint32_t checkpoint){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)checkpoint;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 switch='switch(entry){'+''.join(f'case 0x{a:x}u:goto L{a:x};' for a in sorted({start}|{p['entry'] for p in own}))+'default:return -2;}'
 assert function.count(f' goto L{start:x};')==1
 function=function.replace(f' goto L{start:x};',' '+switch,1)
 functions.append(function);record['registration_requires_complete_behavior']=True;routines.append(record)
assert len(probes)==24
source=PROJECT/'source/sa_draw_clipping_state.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT,GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,TRANSLATOR,TEMPLATE,BINARY,source,diagnostics,audit,PROJECT/'source/sh4_integer_state.c',PROJECT/'source/sh4_integer_state.h',PROJECT/'source/sh4_signed_product.c',PROJECT/'source/sh4_signed_product.h']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),test_only_probe_entries=probes,newly_registered_entries=0,reused_routine_credit=0,scope='Four original full-body native clipping proposals with explicit MACH/inactive integer-bank state, original SR masking and typed fault stops. Test-only interior entries/checkpoint status 2 qualify the real eight SR sequences and sixteen signed-product pairs; they do not prove complete clipping. Guest exception vectoring, interrupts/timing and complete callers remain unfinished. Older APIs and sources preserved.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/draw_clipping_state_translation.json').write_text(json.dumps(report,indent=2)+'\n')
