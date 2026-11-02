"""Recover camera positioning, preset dispatch and shared-tail/projection services."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
folder=PROJECT/'analysis/model-camera-position-v2';folder.mkdir(exist_ok=False)
parts=[(0x8c019400,'model-camera-lifecycle',None),(0x8c019f4a,'model-camera-lifecycle',None),(0x8c01a100,'model-camera-lifecycle',True),(0x8c0130f8,'model-camera-lifecycle',False)]
parts.extend((a,'model-camera-position-support',None) for a in (0x8c01a680,0x8c6404c8,0x8c64041c,0x8c64048c,0x8c64065c,0x8c60957e))
origins=[]
for start,parent_folder,segment in parts:
 original=PROJECT/f'analysis/{parent_folder}/{0x8c01a100 if segment is not None else start:08x}_instructions.tsv'
 rows=list(csv.DictReader(original.open(),delimiter='\t'))
 if segment is not None:rows=[r for r in rows if (int(r['address'],16)>=0x8c01a100)==segment]
 with (folder/f'{start:08x}_instructions.tsv').open('w',newline='') as stream:
  writer=csv.DictWriter(stream,fieldnames=('address','length','bytes','instruction','flows'),delimiter='\t');writer.writeheader();writer.writerows(rows)
 origins.append(original)
chosen=chosen.replace('main-dispatch-model-traversal/','model-camera-position-v2/')
chosen=chosen.replace("('fipr ','ftrv ')","('fipr ','ftrv ','fsca ')")
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original camera positioning, preset and shared-tail services; names inferred.')
functions=[];records=[]
for start,_,_ in parts:
 listing=folder/f'{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 end=max(int(r['address'],16)+int(r['length']) for r in rows)
 function,record=recover(start,end,f'sa_camera_position_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sx;(void)sr;')
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_model_camera_position_v2.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_model_camera_owner_v3_connected.c';connected=parent.read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
decl='\n'.join('DECL('+r['name']+');' for r in records)+'\n'
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);',decl+'DECL(sa_draw_float_state_wrapper_8c043a20);',1)
routes=''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for r in records)
connected=connected.replace(marker,marker+routes)
main=PROJECT/'source/sa_model_camera_position_v2_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_camera_owner_v3.ps1';build=PROJECT/'build_model_camera_position_v2.ps1';assert not build.exists()
text=recipe.read_text().replace('sa_model_camera_owner_v3_connected.c','sa_model_camera_position_v2_connected.c').replace('model_camera_owner_v3.dll','model_camera_position_v2.dll')
text=text.replace("'sa_model_camera_owner_v3.c'","'sa_model_camera_owner_v3.c','sa_model_camera_position_v2.c'");build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build]+origins+[Path(r['listing']) for r in records]
report=dict(status='original_rows_statically_recovered_behavior_proof_separate',routines=records,instruction_rows=sum(r['instruction_rows'] for r in records),shared_tail_split=dict(caller='0x8c01a100',caller_rows=86,jump='0x8c01a1a8',literal='0x8c01a234',target='0x8c0130f8',target_rows=123,PR_restored_before_tail=True,delay_slot='mov.l @r15+,r14'),new_original_routine_credit=0,prior_sources_preserved=True,scope='Byte-checked static bodies with reached RAM/FPU/return guards. Shared tail remains an explicit service boundary; no callback/position/return substitution.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_position_v2_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
