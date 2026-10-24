"""Connect the original display wait and explicit raw timing observations."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_transition_translation.json';manifest=json.loads(mp.read_text());old=P/'source/sa_movie_status_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 assert f'target=={r["address"]}u' not in text
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_movie_status_connected','sa_movie_transition_connected');bridge=P/'source/sa_movie_transition_connected.c';bridge.write_text(text)
old_build=P/'build_movie_status.ps1';build=old_build.read_text().replace("'sa_movie_status_connected.c'","'sa_movie_transition_connected.c','sa_movie_transition.c','display_timing_bus.c'").replace('movie_status.dll','movie_transition.dll');bp=P/'build_movie_transition.ps1';bp.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==153
files=[Path(__file__),old,old_build,bridge,bp,mp,P/'source/display_timing_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=153,newly_registered_entries=0,scope='Original ten-row display transition wait integrated with borrowed raw scan-status observations. Sample exhaustion remains an explicit host stop, never a supplied game return. Prior code and artifacts are preserved.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_transition_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated MOVIE display transition pipeline: 153 sources')
