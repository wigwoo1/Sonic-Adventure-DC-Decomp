"""Preserve the190-source pipeline; add original contexts and separate exception ABI."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_interrupt_context_translation.json';manifest=json.loads(mp.read_text())
old=P/'source/sa_movie_transfer_dispatch_connected.c';text=old.read_text()
common='uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*,uint32_t*,const struct sa_vertex_bus*,struct sa_sh4_integer_extra*,uint32_t*,struct sa_sh4_exception_state*'
declarations='#include "sh4_exception_state.h"\n'
for r in manifest['routines']:declarations+=f'int {r["name"]}({common}'+(',uint32_t' if r['kind']=='copied_vector_fragment' else '')+');\n'
marker='struct context {';assert text.count(marker)==1;text=text.replace(marker,declarations+marker)
marker='struct sa_console_service *console;};';assert text.count(marker)==1;text=text.replace(marker,'struct sa_console_service *console;struct sa_sh4_exception_state *exception;};')
routes=''
arguments='m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr,c->exception'
for r in manifest['routines']:
 routes+=f' if(target=={r["address"]}u)return {r["name"]}({arguments}'+(',target' if r['kind']=='copied_vector_fragment' else '')+');\n'
for offset,address in ((0x100,'0x8c65e7a4'),(0x400,'0x8c65e7a4'),(0x600,'0x8c65e7d8')):
 routine=next(r for r in manifest['routines'] if r['address']==address)
 routes+=f' if(c->exception && target==*c->vbr+0x{offset:x}u)return {routine["name"]}({arguments},target);\n'
marker=' return -2;';assert text.count(marker)==1;text=text.replace(marker,routes+marker)
text=text.replace('firmware,console};','firmware,console,0};')
text=text.replace('sa_movie_transfer_dispatch_connected','sa_movie_interrupt_context_connected').replace('sa_movie_transfer_dispatch_initialized_connected','sa_movie_interrupt_context_initialized_connected')
a=text.index('SA_STAGE_EXPORT int sa_movie_interrupt_context_connected(');b=text.index('\n}\n',a)+3
engine=text[a:b];engine=engine.replace('sa_movie_interrupt_context_connected(','sa_movie_interrupt_context_exception_connected(',1)
engine=engine.replace('struct sa_console_service *console){','struct sa_console_service *console,struct sa_sh4_exception_state *exception){')
marker=' struct context c=';assert engine.count(marker)==1
engine=engine.replace(marker,' if(!exception){s->pc=entry;return -3;}\n struct sa_exception_bus memory;struct sa_vertex_bus decorated;\n int initialized=sa_exception_bus_init(&memory,bus,exception,&decorated);if(initialized){s->pc=entry;return initialized;}\n exception->returned=0;\n'+marker)
engine=engine.replace('observe,bus,gbr,integer,vbr,0,disc,firmware,console,0};','observe,&decorated,gbr,integer,vbr,0,disc,firmware,console,exception};')
marker=' s->pc=status?c.stop:s->pr;return status;';assert engine.count(marker)==1
engine=engine.replace(marker,' if(status==2 && exception->returned){s->pc=exception->spc;return 0;}\n'+marker)
text+='\n/* RTE return token unwinds native calls; guest R0/PR are never fabricated. */\n'+engine
bridge=P/'source/sa_movie_interrupt_context_connected.c';bridge.write_text(text)
old_build=P/'build_movie_transfer_dispatch.ps1';build=old_build.read_text().replace("'sa_movie_transfer_dispatch_connected.c'","'sa_movie_interrupt_context_connected.c','sa_movie_interrupt_context.c','sh4_exception_state.c'").replace('movie_transfer_dispatch_probe.c','movie_interrupt_context_probe.c').replace('movie_transfer_dispatch.dll','movie_interrupt_context.dll')
probe_text=(P/'source/movie_transfer_dispatch_probe.c').read_text().replace('sa_movie_transfer_dispatch_connected','sa_movie_interrupt_context_connected').replace('sa_movie_transfer_dispatch_observed','sa_movie_interrupt_context_observed')
line=next(line for line in probe_text.splitlines() if line.startswith('int sa_movie_interrupt_context_connected('))
declaration=line.replace('sa_movie_interrupt_context_connected','sa_movie_interrupt_context_exception_connected').replace('struct sa_console_service*);','struct sa_console_service*,struct sa_sh4_exception_state*);')
probe_text='#include "sh4_exception_state.h"\n'+probe_text.replace(line,line+'\n'+declaration)
a=probe_text.index('SA_STAGE_EXPORT int sa_movie_interrupt_context_observed(');b=probe_text.index('\n}\n',a)+3
observed=probe_text[a:b].replace('sa_movie_interrupt_context_observed','sa_movie_interrupt_context_exception_observed').replace('uint32_t failure){','uint32_t failure,struct sa_sh4_exception_state *exception){').replace('return sa_movie_interrupt_context_connected(','return sa_movie_interrupt_context_exception_connected(').replace('vbr,disc,firmware,console);','vbr,disc,firmware,console,exception);')
probe_text+='\n'+observed
probe=P/'source/movie_interrupt_context_probe.c';probe.write_text(probe_text)
bp=P/'build_movie_interrupt_context.ps1';bp.write_text(build);names=re.findall(r"'([^']+\.c)'",build);assert len(names)==len(set(names))==192
files=[Path(__file__),old,old_build,bridge,bp,mp,probe]+[P/'source'/name for name in names]
report=dict(source=bridge.relative_to(P).as_posix(),compiled_source_count=192,newly_registered_entries=0,scope='Original copied vectors/context handlers and existing dispatch pipeline. Separate explicit exception-state API; old ABI retained. RTE control token propagates through native calls, then produces actual SPC/SR return. No helper result or physical interrupt delivery supplied.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_interrupt_context_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated interrupt-context pipeline:',len(names),'sources')
