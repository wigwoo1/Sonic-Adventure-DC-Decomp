"""Recover the original scene setup that produces the shared direction input."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original scene direction/state owner and its stage setup dependencies; names inferred.')
parts=[(0x8c012240,'model-camera-scene-owner')]+[(a,'model-camera-scene-support') for a in (0x8c602df2,0x8c64826e,0x8c014242,0x8c014308,0x8c012014,0x8c011d46)]
functions=[];records=[]
for start,folder in parts:
 current=chosen.replace('main-dispatch-model-traversal/',folder+'/').replace("('fipr ','ftrv ')","('fipr ','ftrv ','fsca ')")
 exec(compile(current,str(TEMPLATE),'exec'),globals())
 listing=PROJECT/f'analysis/{folder}/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+int(r['length']) for r in rows)
 function,record=recover(start,end,f'sa_camera_scene_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;')
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_model_camera_scene.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_model_camera_copy_v2_connected.c';connected=parent.read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
decl='\n'.join('DECL('+r['name']+');' for r in records)+'\n'
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);',decl+'DECL(sa_draw_float_state_wrapper_8c043a20);',1)
routes=''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for r in records)
connected=connected.replace(marker,marker+routes)
main=PROJECT/'source/sa_model_camera_scene_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_camera_copy_v2.ps1';build=PROJECT/'build_model_camera_scene.ps1';assert not build.exists()
text=recipe.read_text().replace('sa_model_camera_copy_v2_connected.c','sa_model_camera_scene_connected.c').replace('model_camera_copy_v2.dll','model_camera_scene.dll')
text=text.replace("'sa_model_camera_copy_v2.c'","'sa_model_camera_copy_v2.c','sa_model_camera_scene.c'");build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build]+[Path(r['listing']) for r in records]
report=dict(status='original_scene_owner_recovered_behavior_proof_separate',routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),direction_source='0x8c6ac5c0',direction_owner='0x8c012240',new_original_routine_credit=0,prior_sources_preserved=True,scope='Original stage-dependent scene setup, default direction producer and called stage/input helpers with reached RAM/FPU/return guards. No direction vector, loader result or game-callee output is supplied.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_scene_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
