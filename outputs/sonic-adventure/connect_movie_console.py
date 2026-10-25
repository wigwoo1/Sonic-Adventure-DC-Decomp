"""Add a separate explicit console/flash host service to the retained bridge."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
old=P/'source/sa_movie_initialize_connected.c';text=old.read_text()
marker='#include "native_disc_queue.h"';assert text.count(marker)==1
text=text.replace(marker,marker+'\n#include "native_console_service.h"')
marker='struct sa_disc_queue *disc;uint32_t firmware;};';assert text.count(marker)==1
text=text.replace(marker,'struct sa_disc_queue *disc;uint32_t firmware;struct sa_console_service *console;};')
marker=' if(target==c->firmware)';assert text.count(marker)==1
route=' if(target==SA_NATIVE_SYSINFO_TARGET||target==SA_NATIVE_FLASH_TARGET){uint32_t result=r[0],write_result=0;int status=sa_console_service(c->console,m,target==SA_NATIVE_SYSINFO_TARGET?0u:1u,r[7],r[4],r[5],r[6],&result,&write_result);if(!status&&write_result)r[0]=result;return status;}\n'
text=text.replace(marker,route+marker)
marker='struct sa_disc_queue *disc,uint32_t firmware){';assert text.count(marker)==2
text=text.replace(marker,'struct sa_disc_queue *disc,uint32_t firmware,struct sa_console_service *console){')
marker='0,disc,firmware};';assert text.count(marker)==1;text=text.replace(marker,'0,disc,firmware,console};')
text=text.replace('disc,firmware);','disc,firmware,console);')
marker=' if(!integer||!vbr||!disc||!firmware)';assert text.count(marker)==1
text=text.replace(marker,' if(firmware==SA_NATIVE_SYSINFO_TARGET||firmware==SA_NATIVE_FLASH_TARGET){s->pc=entry;return -3;}\n'+marker)
text=text.replace('sa_movie_initialize_connected','sa_movie_console_connected').replace('sa_movie_initialize_initialized_connected','sa_movie_console_initialized_connected')
bridge=P/'source/sa_movie_console_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_initialize_probe.c';probe=old_probe.read_text()
probe=probe.replace('#include "movie_clear_probe.h"','#include "movie_clear_probe.h"\n#include "native_console_service.h"')
marker='struct sa_disc_queue*,uint32_t);';assert probe.count(marker)==1;probe=probe.replace(marker,'struct sa_disc_queue*,uint32_t,struct sa_console_service*);')
marker='uint32_t firmware,struct sa_clear_stats';assert probe.count(marker)==1;probe=probe.replace(marker,'uint32_t firmware,struct sa_console_service *console,struct sa_clear_stats')
probe=probe.replace('disc,firmware);','disc,firmware,console);').replace('sa_movie_initialize_connected','sa_movie_console_connected').replace('sa_movie_initialize_observed','sa_movie_console_observed')
pp=P/'source/movie_console_probe.c';pp.write_text(probe)
old_build=P/'build_movie_initialize.ps1';build=old_build.read_text().replace("'sa_movie_initialize_connected.c'","'sa_movie_console_connected.c','native_console_service.c'").replace('movie_initialize_probe.c','movie_console_probe.c').replace('movie_initialize.dll','movie_console.dll')
bp=P/'build_movie_console.ps1';bp.write_text(build);sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==173
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,P/'source/native_console_service.h',P/'console_service_reference.py']+[P/'source'/name for name in sources]
report=dict(compiled_source_count=173,source='source/sa_movie_console_connected.c',new_registry_credit=0,sysinfo_target='0x8c00f110',flash_target='0x8c00f120',scope='Separate declared native console/flash services using borrowed explicit immutable input bytes. Old disc policies and all original game adapters unchanged. Unsupported/unconfigured services remain stops; no game callback can replace a missing original dependency.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_console_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated console service pipeline:',len(sources),'sources')
