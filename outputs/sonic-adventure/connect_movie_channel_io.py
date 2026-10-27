"""Original I/O routes and explicit nested coroutine continuation integration."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_channel_io_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==12
old=P/'source/sa_movie_channel_decoder_connected.c';text=old.read_text()
text=text.replace('#include "sh4_exception_state.h"',''.join(f'VDECL({r["name"]});\n' for r in manifest['resume_routes'])+'#include "sh4_exception_state.h"')
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1;text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_channel_decoder_'+suffix,'sa_movie_channel_io_'+suffix)
marker='struct sa_flash_storage *flash;};';assert text.count(marker)==1;text=text.replace(marker,'struct sa_flash_storage *flash;uint32_t root_pr;};')
for ending in ('console,0,0,0','console,exception,0,0','console,exception,&runtime_memory,0','console,exception,&runtime_memory,storage'):
 marker=ending+'};int status=dispatch';assert text.count(marker)==1
 text=text.replace(marker,ending+',s->pr};int status=dispatch')
allowed=json.loads((P/manifest['return_manifest']).read_text())['allowed_call_returns']
helper='\nint sa_movie_channel_io_return_allowed(void *opaque,uint32_t target){\n struct context *c=(struct context*)opaque;if(target==c->root_pr)return 1;\n static const uint32_t allowed[]={'+','.join(x+'u' for x in allowed)+'};\n unsigned low=0,high=(unsigned)(sizeof(allowed)/sizeof(allowed[0]));\n while(low<high){unsigned mid=low+(high-low)/2;if(allowed[mid]<target)low=mid+1;else high=mid;}\n return low<(unsigned)(sizeof(allowed)/sizeof(allowed[0]))&&allowed[low]==target;\n}\n'
marker='static void lower_observe(';assert text.count(marker)==1;text=text.replace(marker,helper+marker)
marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["pc"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['resume_routes'])+marker)
bridge=P/'source/sa_movie_channel_io_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_channel_decoder_probe.c';probe=P/'source/movie_channel_io_probe.c';probe.write_text(old_probe.read_text().replace('sa_movie_channel_decoder_','sa_movie_channel_io_'))
old_build=P/'build_movie_channel_decoder.ps1';build=old_build.read_text().replace("'sa_movie_channel_decoder_connected.c'","'sa_movie_channel_io_connected.c','sa_movie_channel_io.c'").replace('movie_channel_decoder_probe.c','movie_channel_io_probe.c').replace("'sa_movie_channel_decoder.c'","'sa_movie_channel_io_decoder.c'").replace("'sa_movie_channel_worker.c'","'sa_movie_channel_io_worker.c'").replace('movie_channel_decoder.dll','movie_channel_io.dll');bp=P/'build_movie_channel_io.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==221
files=[Path(__file__),old,old_probe,old_build,bridge,probe,bp,mp]+[P/'source'/n for n in sources]
report=dict(resume_routes=manifest['resume_routes'],resumed_return_guard='Exact prior/new flat-model call-return set plus captured root PR;ordinary entry guards unchanged',compiled_source_count=221,proposed_integrated_entries=12,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Old220routes retained plus12real routes,82nonfunction callcontinuations,internalcapturedrootPR hostmetadata andexactflatreturnset check forresumedRTS. ExternalABI/SRFD/device/runtime blocks preserved. Olderbodies adaptedforresumption withzero extra originalcredit. Allresumegraphs remainproposals untiltested;no fakeparser/callbackresults/context/counterrepair.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_channel_io_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated221-source I/O continuation integration')
