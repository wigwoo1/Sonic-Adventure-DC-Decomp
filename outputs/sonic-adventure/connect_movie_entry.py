"""Extend the native boot pipeline into the observed MOVIE runtime image."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_entry_translation.json';manifest=json.loads(mp.read_text());main_mp=P/'analysis/movie_main_translation.json';main_manifest=json.loads(main_mp.read_text());routines=manifest['routines']+main_manifest['routines'];old=P/'source/sa_boot_cache_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in routines)+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in routines:routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_boot_cache_connected','sa_movie_entry_connected');bridge=P/'source/sa_movie_entry_connected.c';bridge.write_text(text)
old_build=P/'build_boot_cache.ps1';build=old_build.read_text().replace("'sa_boot_cache_connected.c'","'sa_movie_entry_connected.c','sa_movie_entry.c','sa_movie_main.c'").replace('boot_cache.dll','movie_entry.dll');bp=P/'build_movie_entry.ps1';bp.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==149
files=[Path(__file__),old,old_build,bridge,bp,mp,main_mp,P/'analysis/movie_runtime_inventory.json']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=149,newly_registered_entries=0,scope='Existing boot/disc/CCR pipeline plus original raw MOVIE startup and reached main dependencies. No file or callback result is substituted to pass unknown dependencies.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_entry_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated MOVIE entry pipeline: 149 sources')
