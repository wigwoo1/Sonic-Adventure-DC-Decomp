"""Original hierarchy matrix preparation, generator, copy and retrieval bodies."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Hierarchy matrix preparation/generation/copy/retrieval; names inferred.')
header+='''
/* Production default retains the original unsupported-layout infinite loop.
 * Focused research builds stop only after its first BRA/NOP cycle. */
#ifndef SA_BOUND_RETAIL_STALLS
#define SA_BOUND_RETAIL_STALLS 0
#endif
#if SA_BOUND_RETAIL_STALLS
#define SA_HIERARCHY_RETAIL_STALL(stop) do{*(stop)=0x8c05809cu;return -4;}while(0)
#else
#define SA_HIERARCHY_RETAIL_STALL(stop) goto L8c05809c
#endif
'''
chosen=chosen.replace('main-dispatch-model-traversal/','draw-hierarchy/')
marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
chosen=chosen.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
specs=((0x8c0583a0,'prepare'),(0x8c057bc0,'generate'),(0x8c63a770,'copy'),(0x8c058434,'get'),(0x8c63a5ec,'scale'),(0x8c638f94,'bank_store'))
functions=[];routines=[]
for start,label in specs:
 listing=PROJECT/f'analysis/draw-hierarchy/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 function,record=recover(start,end,f'sa_draw_hierarchy_{label}_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;')
 if start==0x8c057bc0:
  lines=function.splitlines();i=next(i for i,s in enumerate(lines) if s.startswith('L8c05809c: '));assert lines[i].count('goto L8c05809c;')==1
  assert binary[0x05809c-0x10000:0x0580a0-0x10000]==bytes.fromhex('feaf0900')
  lines[i]=lines[i].replace('goto L8c05809c;','SA_HIERARCHY_RETAIL_STALL(stop);');function='\n'.join(lines)+'\n'
 record['first_registry_entry']=start!=0x8c638f94;functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_hierarchy.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,BINARY,source,PROJECT/'ghidra/export_draw_hierarchy.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
prior=('model_hierarchy_prepare','model_hierarchy_prepare_native','model_hierarchy_generator','model_hierarchy_matrix','model_matrix_copy','model_matrix_scale','model_matrix_bank_save')
files.extend(PROJECT/'source'/f'{n}.c' for n in prior)
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=5,existing_entry_extensions=1,reused_routine_credit=0,retail_stall=dict(address='0x8c05809c',original_bytes='feaf0900',source_default='Original BRA-to-self/NOP loop retained',research_define='SA_BOUND_RETAIL_STALLS=1',research_status=-4,research_stop='After one original BRA/NOP cycle, before the second BRA'),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Six original guarded bodies: matrix hierarchy wrapper/generator/copy/retrieval/vector-input scale and extended existing bank store. Original layout bit-length dispatch and unsupported-layout stall retained in production source; research bound explicit. Previous limited/injected-vector sources preserved, no duplicate credit.')
(PROJECT/'analysis/draw_hierarchy_translation.json').write_text(json.dumps(report,indent=2)+'\n')
