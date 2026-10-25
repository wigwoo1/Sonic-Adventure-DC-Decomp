"""Retain the 179-source pipeline and connect the original bulk-transfer family."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_bulk_family_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_driver_control_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 assert f'target=={r["address"]}u' not in text
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
for address in ('8c10d460','8c10d770'):
 old_symbol=('sa_movie_rtc_' if address=='8c10d460' else 'sa_movie_hardware_')+address;new_symbol='sa_movie_bulk_transfer_'+address
 assert text.count('VDECL('+old_symbol+');')==1;text=text.replace('VDECL('+old_symbol+');','VDECL('+new_symbol+');')
 assert text.count('return '+old_symbol+'(')==1;text=text.replace('return '+old_symbol+'(','return '+new_symbol+'(')
text=text.replace(marker,routes+marker).replace('sa_movie_driver_control_connected','sa_movie_bulk_family_connected').replace('sa_movie_driver_control_initialized_connected','sa_movie_bulk_family_initialized_connected')
bridge=P/'source/sa_movie_bulk_family_connected.c';bridge.write_text(text)
old_build=P/'build_movie_driver_control.ps1';build=old_build.read_text().replace("'sa_movie_driver_control_connected.c'","'sa_movie_bulk_family_connected.c','sa_movie_bulk_family.c','sa_movie_bulk_transfer_read.c','sa_movie_bulk_transfer_write.c','sound_ram_bus.c'").replace('movie_driver_control_probe.c','movie_bulk_family_probe.c').replace('movie_driver_control.dll','movie_bulk_family.dll')
probe=P/'source/movie_bulk_family_probe.c';probe.write_text((P/'source/movie_driver_control_probe.c').read_text().replace('sa_movie_driver_control_connected','sa_movie_bulk_family_connected').replace('sa_movie_driver_control_observed','sa_movie_bulk_family_observed'))
bp=P/'build_movie_bulk_family.ps1';bp.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==183
files=[Path(__file__),old,old_build,bridge,bp,mp,P/'source/driver_signal_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,
 scope='Original driver-control body with all prior services retained. Borrowed raw normal-status observations are finite software-platform inputs; missing original services remain stops.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_bulk_family_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated driver pipeline:',len(names),'sources')
