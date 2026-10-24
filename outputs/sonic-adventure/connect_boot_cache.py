"""Add byte-checked cache leaf aliases and CCR storage without changing old APIs."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/boot_cache_translation.json';manifest=json.loads(mp.read_text());old=P/'source/sa_boot_disc_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_boot_disc_connected','sa_boot_cache_connected')
bridge=P/'source/sa_boot_cache_connected.c';bridge.write_text(text)
old_build=P/'build_boot_disc.ps1';build=old_build.read_text().replace("'sa_boot_disc_connected.c'","'sa_boot_cache_connected.c','sa_boot_cache.c','cache_register_bus.c'").replace('boot_disc.dll','boot_cache.dll');bp=P/'build_boot_cache.ps1';bp.write_text(build)
names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==147
files=[Path(__file__),old,old_build,bridge,bp,mp,P/'source/cache_register_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=147,newly_registered_entries=0,scope='Same explicit native disc context plus original cached/uncached CCR leaf aliases and separate coherent-RAM cache-control storage. Prior APIs and backend sources remain unchanged.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/boot_cache_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated cache pipeline: 147 sources')
