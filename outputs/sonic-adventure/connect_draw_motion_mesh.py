"""Join original animated-object drawing to the existing native mesh/queue chain."""
import hashlib,json,re
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
old_top=PROJECT/'source/sa_draw_motion_formats_connected.c';top=old_top.read_text()
declarations='''
#include "vertex_bus.h"
DECL(sa_motion_mesh_prepare_8c617c34);
DECL(sa_motion_mesh_wrapper_8c621574);
typedef void (*lower_observer)(void*,uint32_t,const uint8_t*,const struct sa_fpu_banks*,const uint32_t*,const uint32_t*,const uint32_t*);
int sa_motion_mesh_lower_entry(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t,void*,lower_observer,const struct sa_vertex_bus*,uint32_t*);
'''
marker='struct context {';assert top.count(marker)==1;top=top.replace(marker,declarations+marker)
top=top.replace('void *user;observer observe;};','void *user;observer observe;const struct sa_vertex_bus *bus;};')
top=top.replace('struct context {uint32_t *fpul','struct context {uint32_t *published_stop;uint32_t *fpul')
marker='static int vector(';assert top.count(marker)==1
top=top.replace(marker,'''static void lower_observe(void *user,uint32_t target,const uint8_t *m,const struct sa_fpu_banks *f,const uint32_t *sr,const uint32_t *r,const uint32_t *pr){
 struct context *c=(struct context*)user;c->stop=target;
 if(c->observe)c->observe(c->user,target,(uint8_t*)m,(struct sa_fpu_banks*)f,(uint32_t*)sr,(uint32_t*)r,(uint32_t*)pr);
}
'''+marker)
marker=' struct context *c=(struct context*)opaque;c->stop=target;';assert top.count(marker)==1
top=top.replace(marker,marker+'''
 if(target==0x8c6215bau||target==0x8c6215d6u)return sa_motion_mesh_lower_entry(m,f,sr,r,pr,c->fpul,c->macl,target,c,lower_observe,c->bus,&c->stop);''')
marker=' return -2;';assert top.count(marker)==1
routes=''.join(f' if(target==0x{a:x}u)return sa_motion_mesh_{label}_{a:08x}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n' for a,label in ((0x8c617c34,'prepare'),(0x8c621574,'wrapper')))
top=top.replace(marker,routes+marker).replace('sa_draw_motion_formats_connected','sa_draw_motion_mesh_connected')
old='uint32_t entry,void *user,observer observe){';assert top.count(old)==1;top=top.replace(old,'uint32_t entry,void *user,observer observe,const struct sa_vertex_bus *bus){').replace('struct context c={fpul,macl,entry,user,observe};','struct context c={&c.stop,fpul,macl,entry,user,observe,bus};')
top_path=PROJECT/'source/sa_draw_motion_mesh_connected.c';top_path.write_text(top)
old_lower=PROJECT/'source/sa_draw_matrix_renderer_connected.c';lower=old_lower.read_text()
# Contexts share an initial publication pointer; guest signatures/logic stay
# unchanged. Private copies publish precise existing guard sites, including
# folded delay instructions, without changing frozen legacy sources.
binary_path=PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes()
def publish_site(match):
 address=int(match[2],16);word=int.from_bytes(binary[address-0x8c010000:address-0x8c010000+2],'little')
 # Emitted delayed-control labels contain the delay instruction inline. The
 # branch itself cannot raise these numerical/bus statuses; report its delay.
 delay=word>>12 in (0xa,0xb) or word&0xf0ff in (0x400b,0x402b) or word==0xb or word>>8 in (0x8d,0x8f)
 return f'{match[1]}: sa_motion_publish_stop(ctx,0x{address+(2 if delay else 0):x}u); '
lower=lower.replace('struct context {void *user;','struct context {uint32_t *published_stop;void *user;').replace('{user,external,observe,fpul,macl,bus}','{0,user,external,observe,fpul,macl,bus}')
lower+='''
SA_STAGE_EXPORT int sa_motion_mesh_lower_entry(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t target,void *user,observer observe,const struct sa_vertex_bus *bus,uint32_t *stop){
 struct context s={stop,user,0,observe,fpul,macl,bus};return dispatch(&s,target,ram,f,sr,r,pr);
}
'''
lower_path=PROJECT/'source/sa_draw_motion_mesh_lower_connected.c';lower_path.write_text(lower)
old_build=PROJECT/'build_draw_motion_formats.ps1';old_lower_build=PROJECT/'build_draw_matrix_renderer.ps1'
names=[]
reused_originals=[];reused_copies=[]
publication_header=PROJECT/'source/sa_draw_motion_stop.h'
publication_header.write_text('''#ifndef SA_DRAW_MOTION_STOP_H
#define SA_DRAW_MOTION_STOP_H
#include <stdint.h>
/* Internal connected contexts begin with a uint32_t * publication field. */
static inline void sa_motion_publish_stop(void *ctx,uint32_t pc){
 if(ctx){uint32_t *stop=*(uint32_t **)ctx;if(stop)*stop=pc;}
}
#endif
''')
for p in (old_build,old_lower_build):
 for n in re.findall(r"'([^']+\.c)'",p.read_text()):
  n={'sa_draw_motion_formats_connected.c':'sa_draw_motion_mesh_connected.c','sa_draw_matrix_renderer_connected.c':'sa_draw_motion_mesh_lower_connected.c'}.get(n,n)
  original=PROJECT/'source'/n;body=original.read_text()
  if re.search(r'^L[0-9a-f]+: ',body,re.M) and 'void *ctx' in body and 'uint32_t *stop' not in body:
   for chunk in body.split('SA_STAGE_EXPORT')[1:]:
    if re.search(r'^L[0-9a-f]+: ',chunk,re.M):assert 'void *ctx' in chunk.split('goto L')[0],n
   copy=PROJECT/'source'/('sa_motion_reuse_'+n)
   copy.write_text('#include "sa_draw_motion_stop.h"\n'+re.sub(r'^(L([0-9a-f]+)): ',publish_site,body,flags=re.M))
   reused_originals.append(original);reused_copies.append(copy);n=copy.name
  if n not in names:names.append(n)
names.append('sa_draw_motion_mesh.c')
build=old_build.read_text();start=build.index('$taskSources=@(');end=build.index(" | ForEach-Object",start)
build=build[:start]+'$taskSources=@('+','.join(repr(n) for n in names)+')'+build[end:];build=build.replace('draw_motion_formats.dll','draw_motion_mesh.dll').replace('Partial object-draw gate build failed','Motion mesh native pipeline build failed')
build_path=PROJECT/'build_draw_motion_mesh.ps1';build_path.write_text(build)
files=[Path(__file__),binary_path,old_top,old_lower,old_build,old_lower_build,top_path,lower_path,publication_header,build_path,PROJECT/'source/vertex_bus.h',PROJECT/'analysis/draw_motion_mesh_translation.json',PROJECT/'analysis/draw_motion_formats_connection.json',PROJECT/'analysis/draw_matrix_renderer_connection.json',PROJECT/'analysis/main_dispatch_mesh_families_verification.json']+[PROJECT/'source'/n for n in names]+reused_originals
report=dict(source=top_path.relative_to(PROJECT).as_posix(),lower_source=lower_path.relative_to(PROJECT).as_posix(),compiled_source_count=len(names),reused_stop_publication_copies=len(set(reused_copies)),newly_recovered_entries=0,reused_routine_credit=0,candidate_recovery_credit=0,external_game_service_fixtures=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Separate native animated-object draw build joins original motion preprocessing to existing complete material/primitive pipeline and coherent-RAM queue bus. Nine-argument entry takes caller-owned bus. Private reused source copies publish existing emitted guard sites through a common context prefix, including folded delays; guest logic and old files are preserved. No external service substitutes. Legacy valid-RAM/narrow-FPU and some internal guard limitations remain.')
(PROJECT/'analysis/draw_motion_mesh_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Native motion mesh/pipeline connection generated:',len(names),'sources.')
