"""Original USA IP.BIN bootstrap with explicit integer banks, VBR and FPSCR."""
import csv,hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
PARENT=PROJECT/'recover_boot_event.py'
exec(compile(PARENT.read_text().split('notes=[];diagnostics=[]')[0],str(PARENT),'exec'),globals())
BINARY=ROOT/'work/sonic-adventure/extracted/IP.BIN';binary=BINARY.read_bytes()
assert hashlib.sha256(binary).hexdigest()=='5933bbf838b2c3416ab5198293895e5d1ff7f116c9101f58f0e03a4618c7efac'
marker=' if(valid_ram(a,n))return 1;';assert header.count(marker)==1
header=header.replace(marker,marker+'\n if(n==4u && a==0xff00001cu)return 1;')
header='/* Original IP.BIN boot handoff; native behavior qualification is separate. */\n'+header
marker="        assert not text.startswith('f'),text";assert BASE_CHOSEN.count(marker)==1
BASE_CHOSEN=BASE_CHOSEN.replace(marker,"""        if w&0xf0ff==0x402e:return f'{{uint32_t unused;status=sa_sh4_stc_sr(*sr,&unused,{delay}u);if(status)return status;*vbr=r[{n}];}}'
        if w&0xf0ff==0x406a:return f'if(*sr&0x8000u)return -1;f->fpscr=r[{n}]&0x3fffffu;'
"""+marker)
functions=[];records=[]
for start in (0xac00e000,0xac00e020,0xac00e0d0,0xac00e0dc,0xac00e140):
 listing=PROJECT/f'analysis/ip-boot-handoff/{start:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));end=max(int(r['address'],16)+2 for r in rows)
 chosen=BASE_CHOSEN.replace('main-dispatch-model-traversal/','ip-boot-handoff/').replace('0x8c010000','0xac008000')
 exec(compile(chosen,str(TEMPLATE),'exec'),globals())
 function,record=recover(start,end,f'sa_ip_boot_{start:08x}',len(rows))
 marker='sa_shared_service call,uint32_t *stop){';assert function.count(marker)==1
 function=function.replace(marker,'sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t *vbr){\n int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;')
 function=function.replace('(void)entry_pr;','(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;')
 for name in ('rd','sx','wr','fload','fstore'):function=function.replace(name+'(m,',name+'(m,bus,&io_status,')
 function=re.sub(r'((?:rd|sx|wr|fload|fstore)\(m,[^;]*\);)',r'\1if(io_status)return io_status;',function)
 function=re.sub(r'(r\[\d+\])=(rd|sx)\(([^;]+)\);if\(io_status\)return io_status;',r'{uint32_t loaded=\2(\3);if(io_status)return io_status;\1=loaded;}',function)
 functions.append(function);records.append(record)
assert len(records)==5 and sum(r['instruction_rows'] for r in records)==91
source=PROJECT/'source/sa_ip_boot_handoff.c';assert not source.exists();source.write_text(header+'\n'.join(functions))
parent=PROJECT/'source/sa_stage_render_end_connected.c';text=parent.read_text()
macro=next(s for s in text.splitlines() if s.startswith('#define VDECL('))
bootmacro=macro.replace('VDECL(','BOOTDECL(').replace('const uint32_t*)','uint32_t*)')
assert bootmacro!=macro and bootmacro.endswith('uint32_t*)')
text=text.replace(macro,macro+'\n'+bootmacro+'\n'+'\n'.join('BOOTDECL('+r['name']+');' for r in records),1)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace(marker,marker+''.join(f'\n if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);' for r in records))
connected=PROJECT/'source/sa_ip_boot_handoff_connected.c';assert not connected.exists();connected.write_text(text)
prior=PROJECT/'build_stage_render_end.ps1';build=PROJECT/'build_ip_boot_handoff.ps1'
recipe=prior.read_text().replace('sa_stage_render_end_connected.c','sa_ip_boot_handoff_connected.c').replace("'sa_stage_render_end.c'","'sa_stage_render_end.c','sa_ip_boot_handoff.c'").replace('stage_render_end.dll','ip_boot_handoff.dll').replace('stage_render_end.rsp','ip_boot_handoff.rsp')
assert not build.exists();build.write_text(recipe)
files=[Path(__file__),PARENT,SUBMIT_PARENT,WAIT_PARENT,TEMPLATE,TRANSLATOR,BINARY,source,parent,connected,prior,build]+[Path(r['listing']) for r in records]
report=dict(status='original_disc_bootstrap_statically_translated_behavior_proof_separate',routines=records,instruction_rows=91,explicit_mutable_VBR=True,explicit_integer_register_banks=True,FPSCR_write_mask='0x3fffff',FPSCR_reached_FD_guard=True,privileged_boot_entry_required=True,original_fatal_return_spins_preserved=True,uncached_P2_code_addresses_preserved=True,new_wait_or_return_guard_relaxations=False,new_original_routine_credit=0,prior_sources_preserved=True,upstream_game_C_imported=False,all_paths_qualified=False,scope='Five original IP.BIN entries, including the combined seven-row entry/thunk. Reached privileged SR/VBR transfers and masked FPSCR assignment retain real integer-bank effects. No fabricated main return or missing service. VBR/FPSCR support is qualified only in the explicit boot API domain; full hardware exception timing is separate.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
out=PROJECT/'analysis/ip_boot_handoff_translation.json';assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
print('Recovered five original IP boot entries / 91 rows; native behavior not yet qualified.',flush=True)
