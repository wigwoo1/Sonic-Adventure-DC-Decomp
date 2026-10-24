"""Connect the original status functions and explicit sampled-port interface."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_status_translation.json';manifest=json.loads(mp.read_text());old=P/'source/sa_movie_entry_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 assert f'target=={r["address"]}u' not in text
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_movie_entry_connected','sa_movie_status_connected');bridge=P/'source/sa_movie_status_connected.c';bridge.write_text(text)
old_build=P/'build_movie_entry.ps1';build=old_build.read_text().replace("'sa_movie_entry_connected.c'","'sa_movie_status_connected.c','sa_movie_status.c','video_signal_bus.c'").replace('movie_entry.dll','movie_status.dll');bp=P/'build_movie_status.ps1';bp.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==151
files=[Path(__file__),old,old_build,bridge,bp,mp,P/'source/video_signal_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=151,newly_registered_entries=0,scope='Original wrapper/nested port read integrated into MOVIE startup with a caller-supplied sampled PDTRA word. Device data is an explicit bus input, not a substituted game return. Prior APIs/source/DLLs remain unchanged.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_status_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated MOVIE status pipeline: 151 sources')
