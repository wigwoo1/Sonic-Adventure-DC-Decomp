"""Recover recursive model traversal and its R8-preserving mesh wrapper."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
GENERATOR=PROJECT/'recover_main_dispatch_particle_callback.py'
prefix=GENERATOR.read_text().split('functions=[];records=[]')[0]
assert prefix.count("'main-dispatch-particle-callback/'")==1
prefix=prefix.replace("'main-dispatch-particle-callback/'","'main-dispatch-model-traversal/'")
marker="        if text.startswith('fmov'):"
assert prefix.count(marker)==1
prefix=prefix.replace(marker,"        if text=='frchg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;'\n"+marker)
exec(compile(prefix,str(GENERATOR),'exec'),globals())
header=header.replace('Particle callback/update/draw/state wrappers; names inferred.','Recursive model traversal and R8-preserving mesh wrapper; names inferred.')
header=header.replace('SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);\n','')
functions=[];records=[]
for args in ((0x8C62113C,0x8C621574,'sa_model_traversal_8c62113c',514),
             (0x8C6215BA,0x8C6215D6,'sa_model_mesh_wrapper_8c6215ba',14)):
 function,record=recover(*args)
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sx;')
 if args[0]==0x8C62113C:
  record['literal_pool_gaps']=[['0x8c621234','0x8c62125c'],['0x8c6214ac','0x8c6214b8']]
  record['structurally_unreachable_rows']=['0x8c621194','0x8c621196']
  record['structurally_unreachable_edges']=[['0x8c62118e','0x8c621194']]
  # The selector is constant between masking and the eighth comparison:
  # no store, external call or register-writing instruction can change R0.
  assert binary[0x8C621152-0x8C010000:0x8C621154-0x8C010000]==bytes.fromhex('4920')
  record['unreachable_proof']={str(i):next(t for t in range(8) if (i&7)==t) for i in range(256)}
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_model_traversal.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),GENERATOR,BINARY,TRANSLATOR,TEMPLATE,source]+[Path(r['listing']) for r in records]
dependencies=[('model_matrix_scale_registers.c','model_matrix_scale_registers_verification.json'),
 ('model_matrix_rotate.c','model_matrix_rotate_verification.json'),('model_matrix_rotate_implicit.c','model_matrix_rotate_implicit_verification.json'),
 ('sa_matrix_rotate_yxz.c','axis_rotation_native_verification.json'),('model_hierarchy_position_native.c','model_hierarchy_position_native_verification.json'),
 ('model_hierarchy_position_interpolate.c','model_hierarchy_position_interpolate_verification.json')]
for name,report in dependencies:
 p=PROJECT/'source'/name;q=PROJECT/'analysis'/report;d=json.loads(q.read_text())
 expected=d.get('source_sha256') or d['dependency_source_sha256'][name]
 assert hashlib.sha256(p.read_bytes()).hexdigest()==expected,name
 files.extend((p,q))
files.extend((PROJECT/'analysis/main-dispatch-model-traversal'/f'{a}_instructions.tsv') for a in ('8c63a878','8c63a69c','8c639cf4','8c639ff8'))
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,
            reused_transform_sources=[dict(source='source/'+s,verification='analysis/'+v) for s,v in dependencies],
            sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
            scope='528 original rows statically translated; two rows and one conditional outcome proven structurally unreachable by the 3-bit selector. Existing transform source reused without new recovery credit. Behavior proof separate; mesh 8c6209a8 remains a service.')
(PROJECT/'analysis/main_dispatch_model_traversal_translation.json').write_text(json.dumps(report,indent=2)+'\n')
