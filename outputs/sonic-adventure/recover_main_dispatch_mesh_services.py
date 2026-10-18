"""Recover mesh filter and four material/header services from retail bytes."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;GENERATOR=PROJECT/'recover_main_dispatch_particle_callback.py'
prefix=GENERATOR.read_text().split('functions=[];records=[]')[0]
assert prefix.count("'main-dispatch-particle-callback/'")==1
prefix=prefix.replace("'main-dispatch-particle-callback/'","'main-dispatch-mesh-services/'")
marker="        if text.startswith('fmov'):"
assert prefix.count(marker)==1
prefix=prefix.replace(marker,"""        if text=='fschg':return 'if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;'
        if text.startswith(('ftrv ','fipr ','fsrra ')):return f'status=sa_sh4_vector_numeric(0x{w:04x}u,f,*fpul);if(status)return status;'
        if text.startswith('fmac '):return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[{m}],&fr[{n}],&f->fpscr,&fr[{n}]);if(status)return status;}}'
        if text.startswith('fdiv '):return f'{{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[{n}],&fr[{m}],&f->fpscr,&fr[{n}]);if(status)return status;}}'
"""+marker)
exec(compile(prefix,str(GENERATOR),'exec'),globals())
JUMPS={0x8C61149A:0x8C6115A2,**{a:0x8C6116C8 for a in (0x8C61150A,0x8C61151C,0x8C611574,0x8C611582,0x8C611590,0x8C61159E)}}
old='            if w&0xF0FF==0x402B:calls.add(a);continue';assert code.count(old)==1
code=code.replace(old,'            if w&0xF0FF==0x402B:\n                assert a in JUMPS;todo.append(JUMPS[a]);continue')
old="code=f'target=r[{w>>8&15}];'+simple(a+2)+'if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);'";assert code.count(old)==1
code=code.replace(old,"code=f'target=r[{w>>8&15}];'+simple(a+2)+'switch(target){'+''.join(f'case 0x{v:x}u:goto L{v:x};' for v in sorted(set(JUMPS.values())))+'default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);}'")
exec(compile(code,str(TEMPLATE),'exec'),globals())
header=header.replace('#include "sh4_add_sub.h"','#include "sh4_add_sub.h"\n#include "sh4_divide.h"\n#include "sh4_fmac.h"\n#include "sh4_vector_numeric.h"')
header=header.replace('Particle callback/update/draw/state wrappers; names inferred.','Mesh filter and material/header services; names inferred.')
header=header.replace('SA_STAGE_EXPORT int sa_particle_work_update_8c0448ba(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_shared_service);\n','')
functions=[];records=[]
for args in ((0x8C611444,0x8C6116E8,'sa_mesh_filter_8c611444',292),
             (0x8C606C56,0x8C606C68,'sa_material_flags_8c606c56',9),
             (0x8C606A08,0x8C606A5A,'sa_material_shader_state_8c606a08',41),
             (0x8C606A88,0x8C606AE0,'sa_material_texture_state_8c606a88',44),
             (0x8C6069A4,0x8C6069B6,'sa_material_command_finish_8c6069a4',9)):
 function,record=recover(*args)
 function=function.replace('(void)fpul;(void)macl;','(void)fpul;(void)macl;(void)sr;')
 if args[0]==0x8C611444:
  function=function.replace(' (void)fpul;', ' uint32_t entry_pr=*pr;\n (void)fpul;',1)
  function=function.replace('L8c6116e4: ','L8c6116e4: if(*pr!=entry_pr)return -3;',1)
  for pc,address in ((0x8C6114F8,'r[0]+r[2]'),(0x8C6115A6,'r[2]+8u'),(0x8C61162E,'r[2]+8u')):
   function=function.replace(f'L{pc:x}: ',f'L{pc:x}: if((({address})&0x1f000000u)!=0x0c000000u)return -3;',1)
  record['native_return_guard']={'pc':'0x8c6116e4','status':-3,'scope':'Before RTS delay slot when guest PR differs from filter entry; unsupported return transfer, not a hardware-exception model.'}
  record['native_saved_pointer_guards']={'pcs':['0x8c6114f8','0x8c6115a6','0x8c61162e'],'status':-3,'scope':'Before dereferencing the reloaded saved mesh pointer outside the modeled RAM range; no hardware-exception dispatch.'}
  record['internal_indirect_jumps']={hex(a):hex(t) for a,t in JUMPS.items()}
  assert int.from_bytes(binary[0x8C6114DC-0x8C010000:0x8C6114DC-0x8C010000+4],'little')==0x8C6115A2
  assert int.from_bytes(binary[0x8C6115C0-0x8C010000:0x8C6115C0-0x8C010000+4],'little')==0x8C6116C8
 functions.append(function);records.append(record)
source=PROJECT/'source/sa_main_dispatch_mesh_services.c';source.write_text(header+'\n'.join(functions))
files=[Path(__file__),GENERATOR,BINARY,TRANSLATOR,TEMPLATE,source]+[Path(r['listing']) for r in records]
report=dict(source=source.relative_to(PROJECT).as_posix(),routines=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
            scope='395 original rows statically translated; seven indirect filter jumps retain internal flow. Four lower vertex/filter workers remain services; native behavior verification separate.')
(PROJECT/'analysis/main_dispatch_mesh_services_translation.json').write_text(json.dumps(report,indent=2)+'\n')
