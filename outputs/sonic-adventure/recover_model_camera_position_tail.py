"""Recover original camera position-write tail and selector helpers."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
parts=[(a,'model-camera-position-tail') for a in (0x8c01b3e0,0x8c01b158,0x8c01b040)]
functions=[];records=[]
for start,folder in parts:
 current=chosen.replace('main-dispatch-model-traversal/',folder+'/').replace("('fipr ','ftrv ')","('fipr ','ftrv ','fsca ')")
 exec(compile(current,str(TEMPLATE),'exec'),globals())
 listing=PROJECT/f'analysis/{folder}/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+int(r['length']) for r in rows)
 function,record=recover(start,end,f'sa_camera_position_tail_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;')
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_model_camera_position_tail.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_model_camera_state2_connected.c';connected=parent.read_text();marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);','\n'.join('DECL('+r['name']+');' for r in records)+'\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
connected=connected.replace(marker,marker+''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for r in records))
main=PROJECT/'source/sa_model_camera_position_tail_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_camera_state2.ps1';build=PROJECT/'build_model_camera_position_tail.ps1';assert not build.exists()
text=recipe.read_text().replace('sa_model_camera_state2_connected.c','sa_model_camera_position_tail_connected.c').replace('model_camera_state2.dll','model_camera_position_tail.dll').replace('model_camera_state2.rsp','model_camera_position_tail.rsp').replace("'sa_model_camera_state2.c'","'sa_model_camera_state2.c','sa_model_camera_position_tail.c'");build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build]+[Path(r['listing']) for r in records]
report=dict(status='original_state2_camera_rows_recovered_behavior_proof_separate',routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),new_original_routine_credit=0,prior_sources_preserved=True,scope='Complete statically recovered state-2 history, selection, transition and follow-up bodies. Original globals, pointer tables, dynamic callees and reached numerical/return/memory guards remain. Runtime branch qualification is separate.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_position_tail_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
