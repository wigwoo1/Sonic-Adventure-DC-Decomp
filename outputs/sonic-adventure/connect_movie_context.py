"""Connect original control, transfer and synchronization services."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_context_translation.json';manifest=json.loads(mp.read_text());old=P/'source/sa_movie_hardware_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 assert f'target=={r["address"]}u' not in text
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_movie_hardware_connected','sa_movie_context_connected');bridge=P/'source/sa_movie_context_connected.c'
api=text[text.index('SA_STAGE_EXPORT int sa_movie_context_connected('):];signature=api[:api.index('{')].replace('sa_movie_context_connected','sa_movie_context_initialized_connected')
text+=chr(10)+'/* Declared host startup sequence; this does not assert original reset call order. */'+chr(10)+signature+'{'+chr(10)
text+=' s->pc=0x8c10d5a8u;int status=sa_movie_context_connected(m,s,f,fpul,macl,0x8c10d5a8u,user,observe,bus,gbr,integer,vbr,disc,firmware);if(status)return status;'+chr(10)
text+=' s->pc=entry;return sa_movie_context_connected(m,s,f,fpul,macl,entry,user,observe,bus,gbr,integer,vbr,disc,firmware);'+chr(10)+'}'+chr(10)
bridge.write_text(text)
old_build=P/'build_movie_hardware.ps1';build=old_build.read_text().replace("'sa_movie_hardware_connected.c'","'sa_movie_context_connected.c','sa_movie_context.c','transfer_context_bus.c'").replace("'sa_movie_hardware.c'","'sa_movie_context_hardware.c'").replace('movie_hardware.dll','movie_context.dll');bp=P/'build_movie_context.ps1';bp.write_text(build)
original=P/'source/sa_movie_hardware.c';adapted=P/'source/sa_movie_context_hardware.c';raw=original.read_text();marker=' if(valid_ram(a,n))return 1;';assert raw.count(marker)==1
raw=raw.replace(marker,marker+chr(10)+' if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f7800u && (a&0x1fffffffu)<0x005f78c0u)return 1;');adapted.write_text(raw)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==157
files=[Path(__file__),old,old_build,bridge,bp,mp,original,adapted,P/'source/transfer_context_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=157,newly_registered_entries=0,reused_hardware_source=adapted.relative_to(P).as_posix(),scope='Original context initializer and two helpers execute with original event registration and memory fill. A separate preserved-source adaptation expands only the previous hardware access guard for the declared channel-register word aperture. No callback success or ready state is supplied.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_context_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated original context initialization pipeline: 157 sources')
