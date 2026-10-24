"""Per-call disc platform context; preserve the previous boot-file API."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;manifest_path=P/'analysis/boot_disc_translation.json';manifest=json.loads(manifest_path.read_text());old=P/'source/sa_boot_file_connected.c';text=old.read_text()
marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,'#include "native_disc_queue.h"\n'+''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker='uint32_t checkpoint;};';assert text.count(marker)==1;text=text.replace(marker,'uint32_t checkpoint;struct sa_disc_queue *disc;uint32_t firmware;};')
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
routes+=' if(target==c->firmware){uint32_t result=0;if(r[6]!=0u)return -8;int status=sa_disc_service(c->disc,m,r[7],r[4],r[5],&result);if(!status)r[0]=result;return status;}\n'
text=text.replace(marker,routes+marker)
text=text[:text.index('static int connected(')]+'''/* The disc queue is an explicit host platform, not recovered firmware.
 * Single-threaded, borrowed stable RAM; preserve queue ownership across calls.
 * No guest game-service callback can override a missing game dependency. */
SA_STAGE_EXPORT int sa_boot_disc_connected(uint8_t *m,struct cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe,const struct sa_vertex_bus *bus,uint32_t *gbr,struct sa_sh4_integer_extra *integer,uint32_t *vbr,struct sa_disc_queue *disc,uint32_t firmware){
 if(!integer||!vbr||!disc||!firmware){s->pc=entry;return -3;}
 struct context c={&c.stop,fpul,macl,entry,user,observe,bus,gbr,integer,vbr,0,disc,firmware};int status=dispatch(&c,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?c.stop:s->pr;return status;
}
'''
bridge=P/'source/sa_boot_disc_connected.c';bridge.write_text(text)
old_vertex=P/'source/vertex_bus_lighting.c';vertex=old_vertex.read_text();marker=' if(op!=1u)return -3;';assert vertex.count(marker)==1
vertex=vertex.replace(marker,' /* Explicit coherent-RAM OCBP policy; event 18 distinguishes purge. */\n if(op==3u){uint32_t region=a&0xe0000000u;if((region!=0u&&region!=0x80000000u&&region!=0xa0000000u)||(a&0x1f000000u)!=0x0c000000u)return -3;status=event(m,18u,a,0,0);if(!status)m->invalidates++;return status;}\n'+marker)
vertex_path=P/'source/vertex_bus_disc.c';vertex_path.write_text(vertex)
old_build=P/'build_boot_file.ps1';build=old_build.read_text().replace("'sa_boot_file_connected.c'","'sa_boot_disc_connected.c','sa_boot_disc.c','native_disc_queue.c','native_disc_sector_reader.c','native_disc_read_command.c'").replace('boot_file.dll','boot_disc.dll').replace('vertex_bus_lighting.c','vertex_bus_disc.c');build_path=P/'build_boot_disc.ps1';build_path.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==145
files=[Path(__file__),old,old_build,bridge,build_path,manifest_path,P/'source/native_disc_queue.h',old_vertex]+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,scope='Original game dispatch plus explicit per-call native disc platform. Existing host queue policies are reused; unknown game/cache targets still stop. Prior source and API remain unchanged.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/boot_disc_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated disc pipeline:',len(names),'sources')
