"""Retain the188-source pipeline and connect original registered-event/ASIC dispatch plus existing authored G2 support."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_transfer_dispatch_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_transfer_interrupt_connected.c';text=old.read_text();marker='struct context {';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;routes=''
for r in manifest['routines']:
 assert f'target=={r["address"]}u' not in text
 routes+=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
text=text.replace(marker,routes+marker).replace('sa_movie_transfer_interrupt_connected','sa_movie_transfer_dispatch_connected').replace('sa_movie_transfer_interrupt_initialized_connected','sa_movie_transfer_dispatch_initialized_connected')
bridge=P/'source/sa_movie_transfer_dispatch_connected.c';bridge.write_text(text)
old_build=P/'build_movie_transfer_interrupt.ps1';build=old_build.read_text().replace("'sa_movie_transfer_interrupt_connected.c'","'sa_movie_transfer_dispatch_connected.c','sa_movie_transfer_dispatch.c','g2_transfer_host.c'").replace('movie_transfer_interrupt_probe.c','movie_transfer_dispatch_probe.c').replace('movie_transfer_interrupt.dll','movie_transfer_dispatch.dll')
probe=P/'source/movie_transfer_dispatch_probe.c';probe.write_text((P/'source/movie_transfer_interrupt_probe.c').read_text().replace('sa_movie_transfer_interrupt_connected','sa_movie_transfer_dispatch_connected').replace('sa_movie_transfer_interrupt_observed','sa_movie_transfer_dispatch_observed'))
bp=P/'build_movie_transfer_dispatch.ps1';bp.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==190
files=[Path(__file__),old,old_build,bridge,bp,mp,P/'source/driver_signal_bus.h']+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=len(names),newly_registered_entries=0,
 scope='Original installed transfer interrupt handler connected with all prior services retained. Explicit handler entry; existing software acknowledge policy unchanged. No scheduled hardware delivery or DMA copy is supplied.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_transfer_dispatch_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated driver pipeline:',len(names),'sources')
