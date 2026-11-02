"""Recover both original copy heads and all seventeen shared table paths."""
import csv,hashlib,json,struct
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
folder=PROJECT/'analysis/model-camera-record-copy-v2';raw=folder/'copy_range.tsv'
allowed=set(range(0x8c10cd38,0x8c10cd50,2))|set(range(0x8c10cd56,0x8c10cd98,2))
rows=[dict(address=r['address'],length='2',bytes=int(r['opcode'],16).to_bytes(2,'little').hex(),instruction=r['instruction'],flows=r['references']) for r in csv.DictReader(raw.open(),delimiter='\t') if int(r['address'],16) in allowed]
assert {int(r['address'],16) for r in rows}==allowed and len(rows)==45
listing=folder/'8c10cd38_instructions.tsv';assert not listing.exists()
with listing.open('w',newline='') as stream:
 writer=csv.DictWriter(stream,fieldnames=('address','length','bytes','instruction','flows'),delimiter='\t');writer.writeheader();writer.writerows(rows)
table=struct.unpack_from('<I',binary,0x10cd50-0x10000)[0];assert table==0x8c10cd98
targets=[struct.unpack_from('<I',binary,table-0x8c010000+4*i)[0] for i in range(17)]
assert targets==[0x8c10cd94]+[0x8c10cd96-length for length in range(4,68,4)]
COPY_JUMPS=set(targets)
chosen=chosen.replace('main-dispatch-model-traversal/','model-camera-record-copy-v2/')
assert chosen.count('seen=set();todo=[start]')==1;chosen=chosen.replace('seen=set();todo=[start]','seen=set();todo=[start,0x8c10cd44]')
marker='if w&0xF0FF==0x402B:calls.add(a);continue';assert chosen.count(marker)==1
chosen=chosen.replace(marker,'if w&0xF0FF==0x402B:todo.extend(COPY_JUMPS);continue')
old="code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'"
new="code=f'target=r[{w>>8&15}];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{v:x}u:goto L{v:x};' for v in sorted(COPY_JUMPS))+'default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);}'"
assert chosen.count(old)==1;chosen=chosen.replace(old,new)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original dual-head computed record copy and shared internal table paths; names inferred.')
function,record=recover(0x8c10cd38,0x8c10cd98,'sa_camera_copy_engine',45)
function=function.replace('SA_STAGE_EXPORT int sa_camera_copy_engine','static int camera_copy_engine').replace('uint32_t *stop){','uint32_t *stop,uint32_t entry){')
function=function.replace('(void)entry_pr;', '(void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;')
function=function.replace(' goto L8c10cd38;',' if(entry==0x8c10cd44u)goto L8c10cd44;goto L8c10cd38;',1)
entries=[]
for a in (0x8c10cd38,0x8c10cd44):
 name=f'sa_camera_record_copy_{a:08x}';entries.append(dict(address=hex(a),name=name))
 function+=f'\nSA_STAGE_EXPORT int {name}(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_shared_service call,uint32_t *stop){{return camera_copy_engine(m,f,sr,r,pr,fpul,macl,ctx,call,stop,0x{a:x}u);}}\n'
record.update(name=entries[0]['name'],translation_unit_has_two_entry_heads=True,internal_jump_targets=list(map(hex,targets)),table_address=hex(table),table_selector_bytes=list(range(0,68,4)),entry_for_plain_copy={str(n):hex(0x8c10cd44 if n&4 else 0x8c10cd38) for n in range(0,68,4)},zero_size_original_preload_retained=True)
source=PROJECT/'source/sa_model_camera_copy_v2.c';assert not source.exists();source.write_text(header+function)
shim=PROJECT/'source/sa_model_camera_copy_lower.c';assert not shim.exists()
shim.write_text('''/* Bridge the existing lower dispatcher to the complete original copy body.
 * Its documented internal context begins with the stop-publication pointer.
 * Public/main entry points pass that pointer explicitly instead. */
#include "fpu_cleanup_transfer.h"
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
int sa_camera_record_copy_8c10cd38(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*);
SA_STAGE_EXPORT int sa_engine_record_copy_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call){
 uint32_t local_stop=0,*stop=ctx?*(uint32_t**)ctx:0;
 return sa_camera_record_copy_8c10cd38(m,f,sr,r,pr,fpul,macl,ctx,call,stop?stop:&local_stop);
}
''')
parent=PROJECT/'source/sa_model_camera_final_connected.c';connected=parent.read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
decl=''.join('DECL('+e['name']+');\n' for e in entries)
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);',decl+'DECL(sa_draw_float_state_wrapper_8c043a20);',1)
routes=''.join(f'\n if(target=={e["address"]}u)return {e["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for e in entries)
connected=connected.replace(marker,marker+routes)
main=PROJECT/'source/sa_model_camera_copy_v2_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_camera_final.ps1';build=PROJECT/'build_model_camera_copy_v2.ps1';assert not build.exists()
text=recipe.read_text().replace('sa_model_camera_final_connected.c','sa_model_camera_copy_v2_connected.c').replace('model_camera_final.dll','model_camera_copy_v2.dll')
text=text.replace("'sa_model_camera_final.c'","'sa_model_camera_final.c','sa_model_camera_copy_v2.c'").replace('sa_motion_reuse_sa_engine_record_copy_dispatch.c','sa_model_camera_copy_lower.c');build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',TEMPLATE,TRANSLATOR,BINARY,raw,listing,source,shim,PROJECT/'source/sa_draw_motion_stop.h',parent,main,recipe,build]
report=dict(status='dual_head_internal_copy_CFG_recovered_behavior_proof_separate',routines=[record],public_entries=entries,instruction_rows=45,internal_entry_predecessors={hex(a):list(map(hex,sorted({0x8c10cd42,0x8c10cd4e,a-2}))) for a in targets},new_original_routine_credit=0,prior_sources_preserved=True,scope='Both original preload heads, all 17 table paths and lower/main dispatch integration. Original R0/R1/R2 ABI, register/stack/delay effects, preloads and guarded memory access retained. Internal edges stay internal; no fake service or result.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
out=PROJECT/'analysis/model_camera_copy_v2_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
