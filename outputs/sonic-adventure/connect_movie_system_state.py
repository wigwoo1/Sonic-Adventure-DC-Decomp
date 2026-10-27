"""Extend the207-source stage with original queries and the corrected SYSINFO ABI."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;mp=P/'analysis/movie_system_state_translation.json';manifest=json.loads(mp.read_text());assert len(manifest['routines'])==5
old=P/'source/sa_movie_float_initialize_connected.c';text=old.read_text()
marker='#include "sh4_exception_state.h"';assert text.count(marker)==1;text=text.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in manifest['routines'])+marker)
marker=' return -2;';assert text.count(marker)==1
text=text.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in manifest['routines'])+marker)
marker='#include "flash_storage.h"';assert text.count(marker)==1;text=text.replace(marker,marker+'\n#include "console_sysinfo_status.h"')
marker='status=sa_console_service(c->console';assert text.count(marker)==1;text=text.replace(marker,'status=sa_console_status_service(c->console')
for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):text=text.replace('sa_movie_float_initialize_'+suffix,'sa_movie_system_state_'+suffix)
bridge=P/'source/sa_movie_system_state_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_float_initialize_probe.c';pp=P/'source/movie_system_state_probe.c';pp.write_text(old_probe.read_text().replace('sa_movie_float_initialize_','sa_movie_system_state_'))
old_build=P/'build_movie_float_initialize.ps1';build=old_build.read_text().replace("'sa_movie_float_initialize_connected.c'","'sa_movie_system_state_connected.c','sa_movie_system_state.c','console_sysinfo_status.c'").replace('movie_float_initialize_probe.c','movie_system_state_probe.c').replace('movie_float_initialize.dll','movie_system_state.dll');bp=P/'build_movie_system_state.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==209
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,mp,P/'source/console_sysinfo_status.h']+[P/'source'/n for n in sources]
report=dict(compiled_source_count=209,newly_registered_entries=0,source=bridge.relative_to(P).as_posix(),scope='Five original integrationroutes (twoearliercanonicaladapters),correctedSYSINFO_INIT guestR0=0 firmwareboundaryadapter,andexportprefixnames extend207-sourceintegration. OldconsoleABI/source/state layout/proofs retained. Actualnativeflash,IRQ/context/RTE and17/18/19/20argumentAPIs retained. No gamehelperreturns injected;separatequalification required.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_system_state_connection.json').write_text(json.dumps(report,indent=2)+'\n');print('Generated209-source settings/info integration')
