"""Extend the partial draw bridge with existing native state/matrix bodies."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
OLD=PROJECT/'source/sa_overlay_draw_gate_connected.c'
source=OLD.read_text();marker='struct context {';assert source.count(marker)==1
declarations='''
#include "sh4_vector_numeric.h"
struct sa_cpu_state {uint32_t r[16],sr,pr,pc;};
typedef void (*cpu_service)(uint8_t*,struct sa_cpu_state*);
void sa_global_word_clear_mask_8c056990(uint8_t*,struct sa_cpu_state*);
void sa_global_word_set_mask_8c056986(uint8_t*,struct sa_cpu_state*);
void sa_copy_global_pair_8c056926(uint8_t*,struct sa_cpu_state*);
void sa_restore_global_pair_8c056938(uint8_t*,struct sa_cpu_state*);
void sa_global_pair_flags_or_8c05694a(uint8_t*,struct sa_cpu_state*);
void sa_store_nonzero_third_global_pointer_8c608ccc(uint8_t*,struct sa_cpu_state*);
int sa_model_matrix_stack_push_8c639c70(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*);
int sa_model_matrix_stack_pop_8c639b98(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*);
typedef int (*vector_op)(void*,uint32_t,struct sa_fpu_banks*);
typedef int (*axis_op)(void*,uint32_t,struct sa_fpu_banks*,uint32_t);
int sa_model_matrix_translate(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,void*,vector_op);
int sa_matrix_rotate_x(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
int sa_matrix_rotate_y(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
int sa_matrix_rotate_z(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,void*,axis_op);
DECL(sa_draw_matrix_scale_registers_8c63a69c);
DECL(sa_draw_matrix_translate_registers_8c63a878);
static int axis(void *user,uint32_t w,struct sa_fpu_banks *f,uint32_t fpul){(void)user;return sa_sh4_vector_numeric(w,f,fpul);}
static int cpu_call(cpu_service call,uint8_t *m,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t target){
 struct sa_cpu_state s;for(unsigned i=0;i<16;i++)s.r[i]=r[i];s.sr=*sr;s.pr=*pr;s.pc=target;
 call(m,&s);for(unsigned i=0;i<16;i++)r[i]=s.r[i];*sr=s.sr;*pr=s.pr;return 0;
}
'''
source=source.replace(marker,declarations+marker)
marker='static int dispatch(';assert source.count(marker)==1
source=source.replace(marker,'static int vector(void *user,uint32_t w,struct sa_fpu_banks *f){struct context *c=(struct context*)user;return sa_sh4_vector_numeric(w,f,*c->fpul);}\n'+marker)
routes=''
for address,name in ((0x8c056990,'sa_global_word_clear_mask_8c056990'),(0x8c056986,'sa_global_word_set_mask_8c056986'),(0x8c056926,'sa_copy_global_pair_8c056926'),(0x8c056938,'sa_restore_global_pair_8c056938'),(0x8c05694a,'sa_global_pair_flags_or_8c05694a'),(0x8c608ccc,'sa_store_nonzero_third_global_pointer_8c608ccc')):
 routes+=f' if(target==0x{address:x}u)return cpu_call({name},m,sr,r,pr,target);\n'
for address,name in ((0x8c639c70,'sa_model_matrix_stack_push_8c639c70'),(0x8c639b98,'sa_model_matrix_stack_pop_8c639b98')):routes+=f' if(target==0x{address:x}u)return {name}(m,f,sr,r);\n'
routes+=' if(target==0x8c63a804u)return sa_model_matrix_translate(m,f,sr,r,c,vector);\n'
for address,axis_name in ((0x8c639ec8,'x'),(0x8c639f5c,'y'),(0x8c63a1cc,'z')):routes+=f' if(target==0x{address:x}u)return sa_matrix_rotate_{axis_name}(m,f,sr,r,c->fpul,c,axis);\n'
for address,label in ((0x8c63a69c,'scale'),(0x8c63a878,'translate')):routes+=f' if(target==0x{address:x}u)return sa_draw_matrix_{label}_registers_{address:x}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n'
assert source.count(' return -2;')==1;source=source.replace(' return -2;',routes+' return -2;').replace('sa_overlay_draw_gate_connected','sa_overlay_draw_state_connected')
bridge=PROJECT/'source/sa_overlay_draw_state_connected.c';bridge.write_text(source)
old_build=PROJECT/'build_overlay_draw_gate.ps1';build=old_build.read_text().replace("'sa_overlay_draw_gate_connected.c'","'sa_overlay_draw_state_connected.c'")
extra=('sa_global_word_bit_updates_8c056986.c','sa_store_nonzero_third_global_pointer_8c608ccc.c','sa_copy_global_pair_8c056926.c','sa_global_pair_flags_or_8c05694a.c','sa_restore_global_pair_8c056938.c','model_matrix_stack_push.c','model_matrix_stack_pop.c','model_matrix_translate.c','sa_matrix_rotate_x.c','sa_matrix_rotate_y.c','sa_matrix_rotate_z.c','sa_draw_matrix_registers.c')
build=build.replace("'sa_overlay_object_draw_candidate.c'",','.join("'"+s+"'" for s in extra)+",'sa_overlay_object_draw_candidate.c'").replace('overlay_draw_gate.dll','overlay_draw_state.dll')
build_path=PROJECT/'build_overlay_draw_state.ps1';build_path.write_text(build)
names=re.findall(r"'([^']+\.c)'",build)
files=[Path(__file__),OLD,old_build,bridge,build_path,PROJECT/'ghidra/export_overlay_draw_state_services.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']+[PROJECT/'source'/n for n in names]
proofs=('global_word_bit_updates_8c056986_verification.json','store_nonzero_third_global_pointer_8c608ccc_verification.json','copy_global_pair_8c056926_verification.json','global_pair_flags_or_8c05694a_verification.json','restore_global_pair_8c056938_verification.json','model_matrix_stack_push_verification.json','model_matrix_stack_pop_verification.json','model_matrix_translate_verification.json','axis_rotation_native_verification.json','draw_matrix_registers_verification.json')
files.extend(PROJECT/'analysis'/p for p in proofs)
binary_path=PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes();files.append(binary_path);listings=sorted((PROJECT/'analysis/overlay-draw-state-services').glob('*_instructions.tsv'))
import csv
assert len(listings)==12;row_count=0
for listing in listings:
 for row in csv.DictReader(listing.open(),delimiter='\t'):
  a=int(row['address'],16);assert int(row['length'])==2 and binary[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes']);row_count+=1
 files.append(listing)
report=dict(source=bridge.relative_to(PROJECT).as_posix(),newly_recovered_entries=0,reused_routine_credit=0,compiled_source_count=len(names),reused_service_rows=row_count,reused_service_targets=[hex(a) for a in (0x8c056990,0x8c056986,0x8c056926,0x8c056938,0x8c05694a,0x8c608ccc,0x8c639c70,0x8c639b98,0x8c63a804,0x8c639ec8,0x8c639f5c,0x8c63a1cc,0x8c63a69c,0x8c63a878)],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Separate partial draw integration reuses native global-state and matrix bodies. Every old source and proof is preserved; no new routine/candidate recovery credit. Legacy service guards/ABIs retain their previously qualified valid-RAM and narrow-FPU limits.')
(PROJECT/'analysis/overlay_draw_state_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Partial draw state/matrix connection generated:',len(names),'sources,',row_count,'byte-checked reused service rows; zero recovery credit.')
