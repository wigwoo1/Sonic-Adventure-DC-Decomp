"""Extend the retained controller build with original RTC code and raw inputs."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_rtc_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_maple_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 if r['address']=='0x8c10d460':
  old_name='sa_movie_hardware_8c10d460';old_route=f'if(target==0x8c10d460u)return {old_name}(';assert text.count(old_route)==1
  text=text.replace(old_route,f'if(target==0x8c10d460u)return {r["name"]}(')
 else:
  assert f'target=={r["address"]}u' not in text
  routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_movie_maple_connected','sa_movie_rtc_connected').replace('sa_movie_maple_initialized_connected','sa_movie_rtc_initialized_connected')
bridge=P/'source/sa_movie_rtc_connected.c';bridge.write_text(text)
old_build=P/'build_movie_maple.ps1';build=old_build.read_text().replace("'sa_movie_maple_connected.c'","'sa_movie_rtc_connected.c','sa_movie_rtc.c','rtc_sample_bus.c'").replace('movie_maple_probe.c','movie_rtc_probe.c').replace('movie_maple.dll','movie_rtc.dll')
probe=P/'source/movie_rtc_probe.c';probe.write_text((P/'source/movie_maple_probe.c').read_text().replace('sa_movie_maple_connected','sa_movie_rtc_connected').replace('sa_movie_maple_observed','sa_movie_rtc_observed'))
bp=P/'build_movie_rtc.ps1';bp.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==169
files=[Path(__file__),old,old_build,bridge,bp,mp,P/'source/rtc_sample_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,reused_existing_transfer='0x8c10d460',
 scope='Original RTC read and diagnostic sink execute with a new finite raw-word backend. The existing synchronized transfer gets a separate RTC-capable access adapter with zero duplicate recovery credit. Earlier source/build bytes and defaults remain retained.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_rtc_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated RTC pipeline:',len(names),'sources')
