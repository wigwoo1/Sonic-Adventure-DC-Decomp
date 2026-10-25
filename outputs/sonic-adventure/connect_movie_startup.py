"""Retain the heap pipeline and connect original early initialization."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_startup_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_heap_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 assert f'target=={r["address"]}u' not in text
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_movie_heap_connected','sa_movie_startup_connected').replace('sa_movie_heap_initialized_connected','sa_movie_startup_initialized_connected')
bridge=P/'source/sa_movie_startup_connected.c';bridge.write_text(text)
old_build=P/'build_movie_heap.ps1';build=old_build.read_text().replace("'sa_movie_heap_connected.c'","'sa_movie_startup_connected.c','sa_movie_startup.c','startup_platform_bus.c'").replace('movie_heap.dll','movie_startup.dll')
bp=P/'build_movie_startup.ps1';bp.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==162
files=[Path(__file__),old,old_build,bridge,bp,mp,P/'source/startup_platform_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,
 scope='Original initialization caller/platform services and cache aliases with previous heap/rendering code retained. Startup register/cache-array storage is an explicit software backend; original missing calls remain stops.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_startup_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated startup pipeline:',len(names),'sources')
