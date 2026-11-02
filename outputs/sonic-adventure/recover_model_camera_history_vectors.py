"""Recover the original in-place vector subtraction used by camera history."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
chosen=chosen.replace('main-dispatch-model-traversal/','model-camera-history-vector-support/').replace("('fipr ','ftrv ')","('fipr ','ftrv ','fsca ')")
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
start=0x8c63a7e4;listing=PROJECT/f'analysis/model-camera-history-vector-support/{start:08x}_instructions.tsv'
rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+int(r['length']) for r in rows)
function,record=recover(start,end,f'sa_camera_history_vector_{start:08x}',len(rows))
function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;')
source=PROJECT/'source/sa_model_camera_history_vectors.c';assert not source.exists();source.write_text(header+function)
parent=PROJECT/'source/sa_model_camera_draw_update_connected.c';connected=parent.read_text();marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);','DECL('+record['name']+');\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
connected=connected.replace(marker,marker+f'\n if(target=={record["address"]}u)return {record["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);')
main=PROJECT/'source/sa_model_camera_history_vectors_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_camera_draw_update_rsp.ps1';build=PROJECT/'build_model_camera_history_vectors.ps1';assert not build.exists()
text=recipe.read_text().replace('sa_model_camera_draw_update_connected.c','sa_model_camera_history_vectors_connected.c').replace('model_camera_draw_update.dll','model_camera_history_vectors.dll').replace('model_camera_draw_update.rsp','model_camera_history_vectors.rsp').replace("'sa_model_camera_draw_update.c'","'sa_model_camera_draw_update.c','sa_model_camera_history_vectors.c'");build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build,listing]
report=dict(status='original_camera_movement_history_rows_recovered_behavior_proof_separate',routines=[record],instruction_rows=len(rows),new_original_routine_credit=0,prior_sources_preserved=True,scope='Original 13-row in-place three-component vector subtraction 8c63a7e4, including reached dynamic loads, original register advances and delay-slot store. Dynamic RAM and reached FPU/return guards retained.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_history_vectors_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
