"""Byte-checked original stage resource/control paths and bounded BRAF table."""
import csv,hashlib,json,struct
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PARENT=PROJECT/'recover_draw_matrix_registers.py'
exec(compile(PARENT.read_text().split('functions=[];routines=[]')[0],str(PARENT),'exec'),globals())
pc,table,count=0x8c09cf08,0x8c09cf28,43
assert binary[pc-0x8c010000:pc-0x8c010000+4]==bytes.fromhex('23000900')
table_raw=binary[table-0x8c010000:table-0x8c010000+count*2]
targets=[pc+4+struct.unpack_from('<h',table_raw,i*2)[0] for i in range(count)]
BRAF_TARGETS={pc:set(targets)}
listing=PROJECT/'analysis/model-stage-resources/8c09ce90_instructions.tsv'
flow_row=next(row for row in csv.DictReader(listing.open(),delimiter='\t') if int(row['address'],16)==pc)
assert {int(a,16) for a in flow_row['flows'].split()}==set(targets) and len(set(targets))==37
chosen=chosen.replace('main-dispatch-model-traversal/','model-stage-resources/')
cfg='        if w==0xB or w>>12 in (0xA,0xB) or w&0xF0FF in (0x400B,0x402B):'
assert chosen.count(cfg)==1
chosen=chosen.replace(cfg,"""        if w==0x0023:
            assert a in BRAF_TARGETS and a+2 in rows
            seen.add(a+2);todo.extend(BRAF_TARGETS[a]);continue
"""+cfg)
marker='        elif w>>12==0xA:';assert chosen.count(marker)==1
chosen=chosen.replace(marker,"""        elif w==0x0023:
            code=f'target=0x{a+4:x}u+r[0];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{t:x}u:goto L{t:x};' for t in sorted(BRAF_TARGETS[a]))+f'default:*stop=0x{a:x}u;return -3;}}'
            slots.add(a+2);terminal=True
"""+marker)
exec(compile(chosen,str(TEMPLATE),'exec'),globals())
functions=[];records=[]
parts=[(0x8c09ce90,'files'),(0x8c052ba0,'bounds'),(0x8c09cce4,'localized_file'),(0x8c09cd6e,'regional_file'),(0x8c09da44,'prs_file'),(0x8c04e490,'load'),(0x8c10d3dc,'string_copy')]
for start,label in parts:
 listing=PROJECT/f'analysis/model-stage-resources/{start:08x}_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 function,record=recover(start,end,f'sa_stage_resource_{label}_{start:08x}',len(rows))
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;')
 functions.append(function);records.append(record)
assert len(records)==7 and sum(r['instruction_rows'] for r in records)==1357
header=header.replace('Complete register-input matrix scale and translation; names inferred.','Original USA stage resource/control entries; names inferred from original behavior.')
source=PROJECT/'source/sa_model_stage_resources.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_model_world_land_dependencies_connected.c';connected=parent.read_text()
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert connected.count(marker)==1
connected=connected.replace('DECL(sa_draw_float_state_wrapper_8c043a20);','\n'.join('DECL('+r['name']+');' for r in records)+'\nDECL(sa_draw_float_state_wrapper_8c043a20);',1)
connected=connected.replace(marker,marker+''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);' for r in records))
main=PROJECT/'source/sa_model_stage_resources_connected.c';assert not main.exists();main.write_text(connected)
recipe=PROJECT/'build_model_world_land_dependencies_v2.ps1';build=PROJECT/'build_model_stage_resources.ps1'
text=recipe.read_text().replace('sa_model_world_land_dependencies_connected.c','sa_model_stage_resources_connected.c')
text=text.replace('model_world_land_dependencies_v2.dll','model_stage_resources.dll').replace('model_world_land_dependencies_v2.rsp','model_stage_resources.rsp')
text=text.replace("'sa_model_world_land_dependencies_v2.c'","'sa_model_world_land_dependencies_v2.c','sa_model_stage_resources.c'")
assert not build.exists();build.write_text(text)
files=[Path(__file__),PARENT,PROJECT/'recover_overlay_model_blend.py',PROJECT/'recover_main_dispatch_particle_callback.py',PROJECT/'recover_model_view_producer.py',TEMPLATE,TRANSLATOR,BINARY,source,parent,main,recipe,build]+[Path(r['listing']) for r in records]
report=dict(status='original_USA_stage_resource_rows_emitted_behavior_proof_separate',routines=records,instruction_rows=1357,computed_switch=dict(pc=hex(pc),table=hex(table),entries=[hex(a) for a in targets],unique_targets=37,table_bytes_sha256=hashlib.sha256(table_raw).hexdigest(),Ghidra_flow_set_matches_original_table=True,invalid_internal_target_guard_preserved=True),new_original_routine_credit=0,prior_sources_preserved=True,upstream_game_C_imported=False,positive_resource_load_qualified=False,all_paths_qualified=False,scope='Original bytes/CFG statically emitted. 43 signed BRAF table entries select 37 checked internal targets; no runtime opcode decoder or new wait/return relaxation. Resource and code-bank behavior require separate verification.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
target=PROJECT/'analysis/model_stage_resources_translation.json';assert not target.exists();target.write_text(json.dumps(report,indent=2)+'\n')
print('Recovered seven stage-resource entries / 1,357 rows; 43-entry/37-target BRAF checked. Behavior not yet qualified.')
