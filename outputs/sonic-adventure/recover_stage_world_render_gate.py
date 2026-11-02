"""Original world rendering/resource dependencies with explicit live GBR input."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
GBR_PARENT=PROJECT/'recover_draw_gbr_geometry.py'
exec(compile(GBR_PARENT.read_text().split("folder='draw-gbr-geometry'")[0],str(GBR_PARENT),'exec'),globals())
header=header.replace('#include "fpu_cleanup_transfer.h"','#include "fpu_cleanup_transfer.h"\n#include "vertex_bus.h"\n#include "sh4_integer_state.h"')
header='/* Original USA world-render/resource dependencies; behavior proof separate. */\n'+header
chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/','stage-world-render-gate/')
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
parts=[(0x8c02e7e4,'matrix_restore'),(0x8c037108,'model_draw'),(0x8c038da0,'render_state'),(0x8c040942,'motion_draw'),(0x8c09da74,'file_size'),(0x8c09f5d6,'chunk1'),(0x8c09f900,'chunk3'),(0x8c09f966,'chunk4')]
for start,label in parts:
 listing=PROJECT/f'analysis/stage-world-render-gate/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 function,record=recover(start,end,f'sa_stage_world_{label}_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,'sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){\n (void)bus;(void)integer;(void)vbr;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 functions.append(function);records.append(record)
assert len(records)==8 and sum(r['instruction_rows'] for r in records)==339
source=PROJECT/'source/sa_stage_world_render_gate.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_model_stage_resources_connected.c';text=parent.read_text()
vdecl=next(line for line in text.splitlines() if line.startswith('#define VDECL('));assert text.count(vdecl)==1
text=text.replace(vdecl,vdecl+'\n'+'\n'.join('VDECL('+r['name']+');' for r in records),1)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace(marker,marker+''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);' for r in records))
connected=PROJECT/'source/sa_stage_world_render_gate_connected.c';assert not connected.exists();connected.write_text(text)
prior=PROJECT/'build_model_stage_resources.ps1';build=PROJECT/'build_stage_world_render_gate.ps1'
recipe=prior.read_text().replace('sa_model_stage_resources_connected.c','sa_stage_world_render_gate_connected.c')
recipe=recipe.replace("'sa_model_stage_resources.c'","'sa_model_stage_resources.c','sa_stage_world_render_gate.c'").replace('model_stage_resources.dll','stage_world_render_gate.dll').replace('model_stage_resources.rsp','stage_world_render_gate.rsp')
assert not build.exists();build.write_text(recipe)
files=[Path(__file__),GBR_PARENT,SAMPLING,PREVIOUS,PARENT_TEMPLATE,TRANSLATOR,TEMPLATE,BINARY,source,parent,connected,prior,build]+[Path(r['listing']) for r in records]
report=dict(status='original_USA_world_render_rows_emitted_behavior_proof_separate',routines=records,instruction_rows=339,explicit_live_GBR=True,GBR_accesses_keep_RAM_guards=True,new_wait_or_return_guard_relaxations=False,new_original_routine_credit=0,prior_sources_preserved=True,upstream_game_C_imported=False,all_paths_qualified=False,scope='Byte-checked static C with the existing explicit-GBR translation and original numeric/return/memory guards. V ABI carries live GBR/bus/integer/VBR inputs; no fixed GBR or fabricated callee results.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
target=PROJECT/'analysis/stage_world_render_gate_translation.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n');print('Recovered eight render/resource entries / 339 original rows; behavior not yet qualified.')
