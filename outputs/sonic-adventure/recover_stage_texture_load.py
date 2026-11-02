"""Recover three original USA stage/act texture loaders; Japanese source supplies names only."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
functions=[];records=[]
parts=[(0x8c099220,'stage_textures'),(0x8c0995e0,'act_textures'),(0x8c0908b6,'special_stage_query')]
for start,label in parts:
 current=chosen.replace('main-dispatch-model-traversal/','stage-texture-load/')
 exec(compile(current,str(TEMPLATE),'exec'),globals())
 listing=PROJECT/f'analysis/stage-texture-load/{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 end=max(int(row['address'],16)+int(row['length']) for row in rows)
 function,record=recover(start,end,f'sa_world_{label}_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;')
 functions.append(function);records.append(record)
assert len(records)==3 and sum(row['instruction_rows'] for row in records)==337
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original USA world/land producer entries; names are source-informed.')
source=PROJECT/'source/sa_stage_texture_load.c'
assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_stage_world_render_gate_connected.c'
connected=parent.read_text();marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);'
assert connected.count(marker)==1
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);',
 '\n'.join('DECL('+row['name']+');' for row in records)+'\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
connected=connected.replace(marker,marker+''.join(
 f'\n if(target=={row["address"]}u)return {row["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);'
 for row in records))
main=PROJECT/'source/sa_stage_texture_load_connected.c'
assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_stage_world_render_gate.ps1';build=PROJECT/'build_stage_texture_load.ps1'
text=recipe.read_text().replace('sa_stage_world_render_gate_connected.c','sa_stage_texture_load_connected.c')
text=text.replace('stage_world_render_gate.dll','stage_texture_load.dll').replace('stage_world_render_gate.rsp','stage_texture_load.rsp')
text=text.replace("'sa_stage_world_render_gate.c'","'sa_stage_world_render_gate.c','sa_stage_texture_load.c'")
assert not build.exists();build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',
 TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build]+[Path(row['listing']) for row in records]
report=dict(status='original_USA_stage_texture_rows_recovered_behavior_proof_separate',routines=records,
 instruction_rows=337,new_original_routine_credit=0,prior_sources_preserved=True,upstream_game_C_imported=False,
 scope='Original USA bytes and control flow statically emitted as C. Source-informed names do not import Japanese bodies. Existing memory/FPU/return guards and dynamic callee boundaries remain. Behavioral qualification is separate.',
 sha256={str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in files})
out=PROJECT/'analysis/stage_texture_load_translation.json'
assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
print('Recovered',len(records),'stage texture entries / 337 original rows; behavior not yet qualified.')
