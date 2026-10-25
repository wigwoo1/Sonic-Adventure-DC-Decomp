"""Retain the renderer pipeline and connect its next original bodies."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_render_setup_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_render_config_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 assert f'target=={r["address"]}u' not in text
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_movie_render_config_connected','sa_movie_render_setup_connected').replace('sa_movie_render_config_initialized_connected','sa_movie_render_setup_initialized_connected')
bridge=P/'source/sa_movie_render_setup_connected.c';bridge.write_text(text)
old_build=P/'build_movie_render_config.ps1'
build=old_build.read_text().replace("'sa_movie_render_config_connected.c'","'sa_movie_render_setup_connected.c','sa_movie_render_setup.c'").replace('movie_render_config.dll','movie_render_setup.dll')
bp=P/'build_movie_render_setup.ps1';bp.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==159
files=[Path(__file__),old,old_build,bridge,bp,mp]+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,
 scope='Original renderer setup with previous context initialization, configuration, timing constructors and software peripherals retained. Missing services remain real stops.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_render_setup_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated renderer setup pipeline:',len(names),'sources')
