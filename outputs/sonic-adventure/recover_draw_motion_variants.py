"""Recover original motion/drawing selectors, traversal and callback proposals."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT_TEMPLATE=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT_TEMPLATE.read_text().split('functions=[];routines=[]')[0],str(PARENT_TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Motion/drawing selectors, hierarchy traversal and callbacks; names inferred.')
marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
chosen=chosen.replace(marker,'''        if w&0xf0ff==0x002a:return f'r[{n}]=*pr;'
        if w&0xf0ff==0x402a:return f'*pr=r[{n}];'
        if w&0xf0ff==0x4022:return f'r[{n}]-=4u;wr(m,r[{n}],*pr,4);'
        if w&0xf0ff==0x4026:return f'*pr=rd(m,r[{n}],4);r[{n}]+=4u;'
'''+marker)
BASE_CHOSEN=chosen;prior_guard=memory_guard
def memory_guard(w):
 n=w>>8&15
 if w&0xf0ff==0x4022:return f'if(!valid_ram(r[{n}]-4u,4u))return -3;'
 if w&0xf0ff==0x4026:return f'if(!valid_ram(r[{n}],4u))return -3;'
 return prior_guard(w)
inventory=json.loads((PROJECT/'analysis/draw_motion_variants_inventory.json').read_text());specs=[(int(r['entry'],16),Path(r['listing']).parent.name) for r in inventory['routines']]
support=PROJECT/'analysis/draw-motion-support/diagnostics.tsv';notes=list(csv.DictReader(support.open(),delimiter='\t'));assert len(notes)==33 and all(n['completed']=='true' and not n['error'] for n in notes)
specs.extend((int(n['address'],16),'draw-motion-support') for n in notes);assert len({a for a,_ in specs})==43
functions=[];routines=[]
for start,folder in specs:
 listing=PROJECT/f'analysis/{folder}/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/',folder+'/');exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_motion_variant_{start:08x}',len(rows));function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;')
 record.update(core_proposal=folder!='draw-motion-support',leaf_no_game_calls=not record['external_call_sites'],registration_requires_complete_behavior=True);functions.append(function);routines.append(record)
source=PROJECT/'source/sa_draw_motion_variants.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),PARENT_TEMPLATE,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TRANSLATOR,TEMPLATE,source,BINARY,support,PROJECT/'analysis/draw_motion_support_selection.json',PROJECT/'ghidra/export_draw_motion_variants.ps1',PROJECT/'ghidra/export_draw_motion_support.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[Path(r['listing']) for r in routines]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_registered_entries=0,reused_routine_credit=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='43 original motion/drawing proposals statically translated with original table/global callback loads and reached guards, including non-SP PR stack transfers. Existing native services are reused; unknown reached callees remain explicit. Per-body behavior/registration separate; compilation alone grants zero credit.')
(PROJECT/'analysis/draw_motion_variants_translation.json').write_text(json.dumps(report,indent=2)+'\n')
