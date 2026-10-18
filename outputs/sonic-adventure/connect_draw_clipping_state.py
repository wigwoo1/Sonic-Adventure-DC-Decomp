"""Separate extended-state draw pipeline; old sources/APIs/DLLs stay intact."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
manifest_path=PROJECT/'analysis/draw_clipping_state_translation.json';manifest=json.loads(manifest_path.read_text())
old=PROJECT/'source/sa_draw_angle_blend_connected.c';text=old.read_text()
text=text.replace('#include "fpu_cleanup_transfer.h"','#include "fpu_cleanup_transfer.h"\n#include "sh4_integer_state.h"',1)
marker='struct context {';assert text.count(marker)==1
decl='typedef int (*clip_fn)(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*,const struct sa_vertex_bus*,struct sa_sh4_integer_extra*,uint32_t,uint32_t);\n'
decl+='#define CDECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*,const struct sa_vertex_bus*,struct sa_sh4_integer_extra*,uint32_t,uint32_t)\n'
decl+=''.join(f'CDECL({r["name"]});\n' for r in manifest['routines'])
text=text.replace(marker,decl+marker)
text=text.replace('uint32_t *gbr;};','uint32_t *gbr;struct sa_sh4_integer_extra *integer;uint32_t checkpoint;};')
marker=' return -2;';assert text.count(marker)==1
routes=''
for r in manifest['routines']:
 start=int(r['address'],16);interiors=sorted(p['entry'] for p in manifest['test_only_probe_entries'] if p['owner']==start)
 condition=f'target==0x{start:x}u || (c->checkpoint&&('+ '||'.join(f'target==0x{x:x}u' for x in interiors)+'))'
 routes+=f' if({condition})return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,target,c->checkpoint);\n'
text=text.replace(marker,routes+marker)
old_public=text.index('SA_STAGE_EXPORT int sa_draw_angle_blend_connected(')
text=text[:old_public]+'''static int connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer,uint32_t checkpoint){
 if(!integer){s->pc=entry;return -3;}
 struct context c={&c.stop,fpul,macl,entry,user,observe,bus,gbr,integer,checkpoint};int status=dispatch(&c,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?c.stop:s->pr;return status;
}
SA_STAGE_EXPORT int sa_draw_clipping_state_connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer){
 return connected(m,s,f,fpul,macl,entry,user,observe,bus,gbr,integer,0);
}
SA_STAGE_EXPORT int sa_draw_clipping_state_probe(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer,uint32_t checkpoint){
'''
condition='||'.join(f'(entry==0x{p["entry"]:x}u&&checkpoint==0x{p["checkpoint"]:x}u)' for p in manifest['test_only_probe_entries'])
text+=f' if(!({condition})){{s->pc=entry;return -3;}}\n return connected(m,s,f,fpul,macl,entry,user,observe,bus,gbr,integer,checkpoint);\n}}\n'
bridge=PROJECT/'source/sa_draw_clipping_state_connected.c';bridge.write_text(text)
old_build=PROJECT/'build_draw_angle_blend.ps1';build=old_build.read_text().replace("'sa_draw_angle_blend_connected.c'","'sa_draw_clipping_state_connected.c','sa_draw_clipping_state.c','sh4_integer_state.c','sh4_signed_product.c'").replace('draw_angle_blend.dll','draw_clipping_state.dll')
build_path=PROJECT/'build_draw_clipping_state.ps1';build_path.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==122
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,PROJECT/'analysis/sh4_integer_state_verification.json',PROJECT/'analysis/draw_angle_blend_connection.json']+[PROJECT/'source'/n for n in names]
report=dict(source=bridge.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),public_extra_state_bytes=36,old_public_api_preserved_in_old_dll=True,production_interface='sa_draw_clipping_state_connected',scoped_probe_interface='sa_draw_clipping_state_probe',probe_status=2,newly_recovered_entries=0,reused_routine_credit=0,scope='Separate extended-state ABI carries MACH and inactive integer R0-R7 through all reached dispatches. Four clipper proposals are routed, including cross-body conditional tail transfers. Only the explicitly mapped 24 interior sequence probes accept checkpoint exits; production dispatch has no interior probe entry. Complete behavior/registration and exception-vector delivery remain separate.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(PROJECT/'analysis/draw_clipping_state_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Clipping state connection generated:',len(names),'sources.')
