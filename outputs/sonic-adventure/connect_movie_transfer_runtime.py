"""Keep recovered instructions intact; schedule native devices after an actual RTS."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
old=P/'source/sa_movie_interrupt_context_connected.c';text=old.read_text()
text=text.replace('#include "fpu_cleanup_transfer.h"','#include "fpu_cleanup_transfer.h"\n#include "transfer_runtime.h"',1)
marker='struct sa_sh4_exception_state *exception;};';assert text.count(marker)==1
text=text.replace(marker,'struct sa_sh4_exception_state *exception;struct sa_transfer_runtime_binding *runtime;};')
marker='static int dispatch(void *opaque,';assert text.count(marker)==1
a=text.index(marker)
prefix='''static int dispatch(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
/* The recovered idle helper has returned through its actual RTS/delay slot.
 * This is an authored untimed platform boundary, not a replacement game call. */
static int runtime_poll(struct context *c,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 if(!c->runtime||!c->runtime->state->enabled||c->runtime->state->in_irq||*pr!=0x8c65a6e8u)return 0;
 int status=sa_transfer_runtime_tick(c->runtime);if(status)return status;
 uint32_t ready;status=sa_transfer_runtime_irq_ready(c->runtime,&ready);if(status||!ready)return status;
 uint32_t resume=*pr,pc=resume,old_stop=c->stop;
 status=sa_sh4_interrupt_enter(c->exception,sr,r,c->integer,&pc,*c->vbr,0x360u,6u,0);
 if(status==1){c->runtime->state->blocked++;return 0;}if(status)return status;
 c->runtime->state->in_irq=1;
 status=dispatch(c,pc,m,f,sr,r,pr);c->runtime->state->in_irq=0;
 if(status!=2||!c->exception->returned)return status?status:-7;
 if(c->exception->spc!=resume||c->stop!=resume)return -7;
 c->runtime->state->deliveries++;c->stop=old_stop;return 0;
}
'''
text=text[:a]+prefix+text[a:]
route=' if(target==0x8c666e02u)return sa_movie_queue_family_8c666e02(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);'
assert text.count(route)==1
text=text.replace(route,' if(target==0x8c666e02u){int status=sa_movie_queue_family_8c666e02(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);return status?status:runtime_poll(c,m,f,sr,r,pr);}')
text=text.replace('console,0};','console,0,0};').replace('console,exception};','console,exception,0};')
text=text.replace('sa_movie_interrupt_context','sa_movie_transfer_runtime')
a=text.index('SA_STAGE_EXPORT int sa_movie_transfer_runtime_exception_connected(');b=text.index('\n}\n',a)+3
engine=text[a:b].replace('sa_movie_transfer_runtime_exception_connected','sa_movie_transfer_runtime_running_connected',1)
engine=engine.replace('struct sa_sh4_exception_state *exception){','struct sa_sh4_exception_state *exception,struct sa_transfer_runtime *runtime){')
marker=' exception->returned=0;';assert engine.count(marker)==1
engine=engine.replace(marker,''' struct sa_transfer_runtime_binding runtime_memory;struct sa_vertex_bus runtime_bus;
 if(!runtime){s->pc=entry;return -6;}
 int attached=sa_transfer_runtime_bind(&runtime_memory,runtime,m,&decorated,&runtime_bus);if(attached){s->pc=entry;return attached;}
'''+marker)
engine=engine.replace('observe,&decorated,gbr,integer,vbr,0,disc,firmware,console,exception,0};','observe,&runtime_bus,gbr,integer,vbr,0,disc,firmware,console,exception,&runtime_memory};')
text+='\n/* Persistent platform state is supplied explicitly, never a guest checkpoint. */\n'+engine
# Renaming entry exports must not rename the recovered context-body symbols.
text=text.replace('sa_movie_transfer_runtime_8c','sa_movie_interrupt_context_8c')
bridge=P/'source/sa_movie_transfer_runtime_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_interrupt_context_probe.c';probe=old_probe.read_text().replace('sa_movie_interrupt_context','sa_movie_transfer_runtime')
probe='#include "transfer_runtime.h"\n'+probe
line=next(l for l in probe.splitlines() if l.startswith('int sa_movie_transfer_runtime_exception_connected('))
declaration=line.replace('exception_connected','running_connected').replace('struct sa_sh4_exception_state*);','struct sa_sh4_exception_state*,struct sa_transfer_runtime*);')
probe=probe.replace(line,line+'\n'+declaration)
a=probe.index('SA_STAGE_EXPORT int sa_movie_transfer_runtime_exception_observed(');b=probe.index('\n}\n',a)+3
observed=probe[a:b].replace('exception_observed','running_observed').replace('struct sa_sh4_exception_state *exception){','struct sa_sh4_exception_state *exception,struct sa_transfer_runtime *runtime){').replace('exception_connected','running_connected').replace('console,exception);','console,exception,runtime);')
probe+='\n'+observed;pp=P/'source/movie_transfer_runtime_probe.c';pp.write_text(probe)
old_build=P/'build_movie_interrupt_context.ps1';build=old_build.read_text().replace("'sa_movie_interrupt_context_connected.c'","'sa_movie_transfer_runtime_connected.c','transfer_runtime.c'").replace('movie_interrupt_context_probe.c','movie_transfer_runtime_probe.c').replace('movie_interrupt_context.dll','movie_transfer_runtime.dll')
bp=P/'build_movie_transfer_runtime.ps1';bp.write_text(build);sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==193
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,P/'source/transfer_runtime.h']+[P/'source'/n for n in sources]
report=dict(compiled_source_count=193,newly_registered_entries=0,original_sources_changed=False,service_boundary='Actual return of original666e02 to its caller; original waiting instructions unchanged',scope='Persistent authored untimed transfer/event platform; real copied-vector/context interrupt executes inside original native caller, then original caller resumes. No game flags/helper results or guest checkpoint supplied. Hardware timing is not claimed.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_transfer_runtime_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated193-source persistent runtime integration')
