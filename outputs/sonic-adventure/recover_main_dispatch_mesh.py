"""Recover the complete material loop, internal jumps and filter-call wrapper."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
GENERATOR=PROJECT/'recover_main_dispatch_particle_callback.py'
prefix=GENERATOR.read_text().split('functions=[];records=[]')[0]
assert prefix.count("'main-dispatch-particle-callback/'")==1
prefix=prefix.replace("'main-dispatch-particle-callback/'","'main-dispatch-mesh/'")
exec(compile(prefix,str(GENERATOR),'exec'),globals())
JUMPS={0x8C6209CA:0x8C620F0A,0x8C6209DC:0x8C620F0A,0x8C6209E4:0x8C620EFA,0x8C620F06:0x8C620A30}
old='            if w&0xF0FF==0x402B:calls.add(a);continue'
assert code.count(old)==1
code=code.replace(old,'            if w&0xF0FF==0x402B:\n                assert a in JUMPS;todo.append(JUMPS[a]);continue')
old="code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'"
assert code.count(old)==1
code=code.replace(old,"code=f'target=r[{w>>8&15}];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{v:x}u:goto L{v:x};' for v in sorted(set(JUMPS.values())))+'default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);}'")
exec(compile(code,str(TEMPLATE),'exec'),globals())
header=header.replace('Particle callback/update/draw/state wrappers; names inferred.','Mesh material loop and filter-call wrapper; names inferred.')
header=header.replace('SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);\n','')
functions=[];records=[]
for args in ((0x8C6209A8,0x8C620F1E,'sa_mesh_material_loop_8c6209a8',542),
             (0x8C62159A,0x8C6215BA,'sa_mesh_filter_wrapper_8c62159a',16)):
 function,record=recover(*args)
 if args[0]==0x8C6209A8:
  record['internal_indirect_jumps']={hex(a):hex(v) for a,v in JUMPS.items()}
  # Validate the literal producers rather than trusting Ghidra flow hints.
  for a,literal_address in ((0x8C6209CA,0x8C620A24),(0x8C6209DC,0x8C620A24),(0x8C6209E4,0x8C620A2C),(0x8C620F06,0x8C620F28)):
   assert int.from_bytes(binary[literal_address-0x8C010000:literal_address-0x8C010000+4],'little')==JUMPS[a]
  record['internal_jump_literals']=['0x8c620a24','0x8c620a2c','0x8c620f28']
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_mesh.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),GENERATOR,BINARY,TRANSLATOR,TEMPLATE,source]+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,
            sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
            scope='558 original rows statically translated. Four indirect jumps retain internal control flow; filter, material services and primitive renderers remain explicit callees. Behavior verification separate.')
(PROJECT/'analysis/main_dispatch_mesh_translation.json').write_text(json.dumps(report,indent=2)+'\n')
