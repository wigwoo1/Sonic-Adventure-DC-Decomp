"""Recover the reached original camera preset callback and shared render setters."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
chosen=chosen.replace('main-dispatch-model-traversal/','model-camera-update-support-v2/')
chosen=chosen.replace("('fipr ','ftrv ')","('fipr ','ftrv ','fsca ')")
marker="        assert not text.startswith('f'),text";assert chosen.count(marker)==1
chosen=chosen.replace(marker,"        if w>>8==0xc2:return f'wr(m,*gbr+0x{(w&255)*4:x}u,r[0],4);'\n"+marker)
old_memory_guard=memory_guard
def memory_guard(w):
 if w>>8==0xc2:return f'if(!valid_ram(*gbr+0x{(w&255)*4:x}u,4u))return -3;'
 return old_memory_guard(w)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Reached original camera preset and shared render setters; names inferred.')
entries=(0x8c01e6a6,0x8c038bb8,0x8c610cf8,0x8c610d30,0x8c610d3e,0x8c63aee8,0x8c63ae2c,0x8c63adc0,0x8c63ae90,0x8c038c24)
functions=[];records=[]
for start in entries:
 listing=PROJECT/f'analysis/model-camera-update-support-v2/{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+int(r['length']) for r in rows)
 function,record=recover(start,end,f'sa_camera_update_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sx;(void)sr;')
 if start==0x8c038c24:function=function.replace('sa_shared_service call,uint32_t *stop){','sa_shared_service call,uint32_t *stop,uint32_t *gbr){')
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_model_camera_update_v2.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_model_camera_position_v2_connected.c';connected=parent.read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
decl='\n'.join('DECL('+r['name']+');' for r in records)+'\n'
decl=decl.replace('DECL(sa_camera_update_8c038c24);','int sa_camera_update_8c038c24(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*);')
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);',decl+'DECL(sa_draw_float_state_wrapper_8c043a20);',1)
routes=''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for r in records)
routes=routes.replace('sa_camera_update_8c038c24(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);','sa_camera_update_8c038c24(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr);')
connected=connected.replace(marker,marker+routes)
main=PROJECT/'source/sa_model_camera_update_v2_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_camera_position_v2.ps1';build=PROJECT/'build_model_camera_update_v2.ps1';assert not build.exists()
text=recipe.read_text().replace('sa_model_camera_position_v2_connected.c','sa_model_camera_update_v2_connected.c').replace('model_camera_position_v2.dll','model_camera_update_v2.dll')
text=text.replace("'sa_model_camera_position_v2.c'","'sa_model_camera_position_v2.c','sa_model_camera_update_v2.c'");build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build]+[Path(r['listing']) for r in records]
report=dict(status='original_rows_statically_recovered_behavior_proof_separate',routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),new_original_routine_credit=0,prior_sources_preserved=True,explicit_live_GBR_pointer=True,scope='Original dynamically selected camera preset and shared render-state setters. Reached memory/FPU/return guards; original caller arguments and incomplete-service stops retained.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_update_v2_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
