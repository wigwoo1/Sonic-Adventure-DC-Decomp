"""Preserve the203-source integration and add original buffer initialization."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_buffer_initialize_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_render_buffer_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1
text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
routes=''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'] if r['address']!='0x8c65253e')
text=text.replace(marker,routes+marker)
copy=next(r for r in manifest['routines'] if r['address']=='0x8c65253e')
old_route=re.search(r' if\(target==0x8c65253eu\)[^\n]+',text).group(0)
new_route=f" if(target==0x8c65253eu)return {copy['name']}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);"
text=text.replace(old_route,new_route)
# Retain the existing interrupt/context/RTE implementation at the additional
# real SDK-poll return boundary. No game queue record is retired by the host.
begin=text.index('static int runtime_poll(');end=text.index('static int dispatch(void *opaque',begin)
poll=text[begin:end].replace('runtime_poll(','runtime_poll_buffer(').replace('*pr!=0x8c65a6e8u','*pr!=0x8c65100au||!(c->runtime->state->pending&(1u<<19))')
poll=poll.replace('status=sa_transfer_runtime_irq_ready(c->runtime,&ready);if(status||!ready)return status;','if(!c->runtime->parent.read)return -2;status=c->runtime->parent.read(c->runtime->parent.context,0xa05f6930u,4u,&ready);if(status||!(ready&c->runtime->state->pending))return status;').replace('0x360u,6u','0x320u,6u')
text=text[:end]+poll+text[end:]
route=re.search(r' if\(target==0x8c650f78u\)[^\n]+',text).group(0)
call=route.split('return ',1)[1].rstrip(';')
text=text.replace(route,' if(target==0x8c650f78u){int status='+call+';if(status)return status;return runtime_poll_buffer(c,m,f,sr,r,pr);}')
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):
 text=text.replace('sa_movie_render_buffer_'+suffix,'sa_movie_buffer_initialize_'+suffix)
bridge=P/'source/sa_movie_buffer_initialize_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_render_buffer_probe.c';probe=old_probe.read_text().replace('sa_movie_render_buffer_','sa_movie_buffer_initialize_');pp=P/'source/movie_buffer_initialize_probe.c';pp.write_text(probe)
old_build=P/'build_movie_render_buffer.ps1'
build=old_build.read_text().replace("'sa_movie_render_buffer_connected.c'","'sa_movie_buffer_initialize_connected.c','sa_movie_buffer_initialize.c','vram_aperture_bus.c','pvr_dma_bus.c'").replace('movie_render_buffer_probe.c','movie_buffer_initialize_probe.c').replace('movie_render_buffer.dll','movie_buffer_initialize.dll')
bp=P/'build_movie_buffer_initialize.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==206
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,mp]+[P/'source'/n for n in sources]
report=dict(compiled_source_count=206,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Original buffer initialization and upload use the actual native providers and explicit complete state; partial callers remain unqualified. Persistent transfer/interrupt,flash storage and17/18/19/20argument APIs retained. Accounting results come from original list traversal,not substituted totals.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_buffer_initialize_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated206-source graphics accounting pipeline')
