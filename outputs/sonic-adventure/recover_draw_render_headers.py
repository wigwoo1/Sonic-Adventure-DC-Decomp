"""Recover expanded original render-state, texture-header and visibility bodies."""
import csv,hashlib,json,struct
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Expanded render-state, texture header and visibility; names inferred.')
chosen=chosen.replace('main-dispatch-model-traversal/','draw-render-headers/')
BRAF_TARGETS={};switches=[]
for pc,table in ((0x8c654fc0,0x8c654fcc),(0x8c6550a2,0x8c6550a8)):
 assert binary[pc-0x8c010000:pc-0x8c010000+4]==bytes.fromhex('23000900')
 targets=[pc+4+struct.unpack_from('<h',binary,table-0x8c010000+i*2)[0] for i in range(12)]
 BRAF_TARGETS[pc]=set(targets);switches.append(dict(pc=hex(pc),table=hex(table),selector_count=12,entries=[hex(a) for a in targets],guard='Original bounded table loads/BRAF preserved; outside-proposal computed targets stop -3.'))
cfg='        if w==0xB or w>>12 in (0xA,0xB) or w&0xF0FF in (0x400B,0x402B):';assert chosen.count(cfg)==1
chosen=chosen.replace(cfg,'''        if w==0x0023:
            assert a in BRAF_TARGETS and a+2 in rows
            seen.add(a+2);todo.extend(BRAF_TARGETS[a]);continue
'''+cfg)
marker='        elif w>>12==0xA:';assert chosen.count(marker)==1
chosen=chosen.replace(marker,'''        elif w==0x0023:
            code=f'target=0x{a+4:x}u+r[0];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{t:x}u:goto L{t:x};' for t in sorted(BRAF_TARGETS[a]))+f'default:*stop=0x{a:x}u;return -3;}}'
            slots.add(a+2);terminal=True
'''+marker)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
SPECS=((0x8c654e60,'state'),(0x8c655620,'texture_header'),(0x8c64f5bc,'visibility_z'),(0x8c654b60,'size_code'))
functions=[];routines=[]
for start,label in SPECS:
 listing=PROJECT/f'analysis/draw-render-headers/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 function,record=recover(start,end,f'sa_render_{label}_{start:08x}',len(rows));function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;')
 record['computed_switches']=[s for s in switches if start<=int(s['pc'],16)<end];functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_render_headers.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'ghidra/ExpandDrawRenderStateBody.java',PROJECT/'ghidra/export_draw_render_headers.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),expanded_state_rows=644,prior_state_rows=582,added_state_rows=62,missing_case_heads_recovered=18,newly_registered_entries=0,reused_routine_credit=0,computed_switches=switches,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='1017 original instruction rows: full expanded 644-row state body, 315-row texture header, 19-row visibility and 39-row size converter. Original twelve-selector BRAF tables decoded, all eighteen previously missing case heads included. Static guarded C; behavior/registration separate.')
(PROJECT/'analysis/draw_render_headers_translation.json').write_text(json.dumps(report,indent=2)+'\n')
