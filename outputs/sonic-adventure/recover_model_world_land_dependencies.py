"""Recover eight original USA world/land dependencies; Japanese source supplies names only."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
functions=[];records=[]
parts=[(0x8c051e56,'draw_static'),(0x8c051f64,'draw_motion'),(0x8c052048,'draw_added'),(0x8c0520c8,'prepare_draw'),(0x8c052a00,'pool_clear'),(0x8c094dc4,'original_zero_hook'),(0x8c09d9c6,'load_allocate'),(0x8c09f3e0,'relocate')]
for start,label in parts:
 current=chosen.replace('main-dispatch-model-traversal/','model-world-land-dependencies/')
 exec(compile(current,str(TEMPLATE),'exec'),globals())
 listing=PROJECT/f'analysis/model-world-land-dependencies/{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 end=max(int(row['address'],16)+int(row['length']) for row in rows)
 function,record=recover(start,end,f'sa_world_{label}_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;')
 functions.append(function);records.append(record)
assert len(records)==8 and sum(row['instruction_rows'] for row in records)==546
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original USA world/land producer entries; names are source-informed.')
source=PROJECT/'source/sa_model_world_land_dependencies.c'
assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_model_world_land_connected.c'
connected=parent.read_text();marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);'
assert connected.count(marker)==1
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);',
 '\n'.join('DECL('+row['name']+');' for row in records)+'\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
connected=connected.replace(marker,marker+''.join(
 f'\n if(target=={row["address"]}u)return {row["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);'
 for row in records))
main=PROJECT/'source/sa_model_world_land_dependencies_connected.c'
assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_world_land.ps1';build=PROJECT/'build_model_world_land_dependencies.ps1'
text=recipe.read_text().replace('sa_model_world_land_connected.c','sa_model_world_land_dependencies_connected.c')
text=text.replace('model_world_land.dll','model_world_land_dependencies.dll').replace('model_world_land.rsp','model_world_land_dependencies.rsp')
text=text.replace("'sa_model_world_land.c'","'sa_model_world_land.c','sa_model_world_land_dependencies.c'")
assert not build.exists();build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',
 TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build]+[Path(row['listing']) for row in records]
report=dict(status='original_USA_world_land_dependency_rows_recovered_behavior_proof_separate',routines=records,
 instruction_rows=546,new_original_routine_credit=0,prior_sources_preserved=True,upstream_game_C_imported=False,
 scope='Original USA bytes and control flow statically emitted as C. Source-informed names do not import Japanese bodies. Existing memory/FPU/return guards and dynamic callee boundaries remain. Behavioral qualification is separate.',
 sha256={str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in files})
out=PROJECT/'analysis/model_world_land_dependencies_translation.json'
assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
print('Recovered',len(records),'world/land dependency entries / 546 original rows; behavior not yet qualified.')
