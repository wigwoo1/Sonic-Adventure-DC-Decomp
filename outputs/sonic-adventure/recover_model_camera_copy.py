"""Complete the original 0..64-byte computed record-copy body, preserving its CFG."""
import csv,hashlib,json,struct
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
folder=PROJECT/'analysis/model-camera-record-copy';raw=folder/'copy_range.tsv'
all_rows=list(csv.DictReader(raw.open(),delimiter='\t'))
allowed=set(range(0x8c10cd44,0x8c10cd50,2))|set(range(0x8c10cd56,0x8c10cd98,2))
rows=[dict(address=r['address'],length='2',bytes=int(r['opcode'],16).to_bytes(2,'little').hex(),instruction=r['instruction'],flows=r['references']) for r in all_rows if int(r['address'],16) in allowed]
assert {int(r['address'],16) for r in rows}==allowed and len(rows)==39
listing=folder/'8c10cd44_instructions.tsv';assert not listing.exists()
with listing.open('w',newline='') as stream:
 writer=csv.DictWriter(stream,fieldnames=('address','length','bytes','instruction','flows'),delimiter='\t');writer.writeheader();writer.writerows(rows)
table=struct.unpack_from('<I',binary,0x10cd50-0x10000)[0];assert table==0x8c10cd98
targets=[struct.unpack_from('<I',binary,table-0x8c010000+4*i)[0] for i in range(17)]
assert targets==[0x8c10cd94]+[0x8c10cd96-length for length in range(4,68,4)]
COPY_JUMPS=set(targets)
chosen=chosen.replace('main-dispatch-model-traversal/','model-camera-record-copy/')
marker='if w&0xF0FF==0x402B:calls.add(a);continue';assert chosen.count(marker)==1
chosen=chosen.replace(marker,'if w&0xF0FF==0x402B:todo.extend(COPY_JUMPS);continue')
old="code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'"
new="code=f'target=r[{w>>8&15}];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{v:x}u:goto L{v:x};' for v in sorted(COPY_JUMPS))+'default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);}'"
assert chosen.count(old)==1;chosen=chosen.replace(old,new)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original computed record copy, all seventeen internal table entries; names inferred.')
function,record=recover(0x8c10cd44,0x8c10cd98,'sa_camera_record_copy_8c10cd44',39)
function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;')
record.update(internal_jump_targets=list(map(hex,targets)),table_address=hex(table),table_bytes_sha256=hashlib.sha256(binary[table-0x8c010000:table-0x8c010000+68]).hexdigest(),supported_sizes=list(range(0,68,4)),zero_size_original_preload_retained=True)
source=PROJECT/'source/sa_model_camera_copy.c';assert not source.exists();source.write_text(header+function)
parent=PROJECT/'source/sa_model_camera_final_connected.c';connected=parent.read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);','DECL(sa_camera_record_copy_8c10cd44);\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
connected=connected.replace(marker,marker+'\n if(target==0x8c10cd44u)return sa_camera_record_copy_8c10cd44(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);')
main=PROJECT/'source/sa_model_camera_copy_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_camera_final.ps1';build=PROJECT/'build_model_camera_copy.ps1';assert not build.exists()
text=recipe.read_text().replace('sa_model_camera_final_connected.c','sa_model_camera_copy_connected.c').replace('model_camera_final.dll','model_camera_copy.dll')
text=text.replace("'sa_model_camera_final.c'","'sa_model_camera_final.c','sa_model_camera_copy.c'");build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,raw,listing,source,parent,main,recipe,build]
report=dict(status='complete_internal_copy_CFG_recovered_behavior_proof_separate',routines=[record],instruction_rows=39,new_original_routine_credit=0,prior_sources_preserved=True,scope='All 17 original internal computed targets, original register/stack/delay effects and guarded reads/writes. No synthetic external service at a copy-table label. Invalid selectors retain actual table/preload effects and external-target fallback.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_copy_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
