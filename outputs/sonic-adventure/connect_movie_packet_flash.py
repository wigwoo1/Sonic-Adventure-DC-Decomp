"""Add an explicit flash-storage ABI while retaining earlier entry signatures."""
import hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
old=P/'source/sa_movie_packet_startup_connected.c';text=old.read_text()
for suffix in ('connected','initialized_connected','exception_connected','running_connected'):
 text=text.replace('sa_movie_packet_startup_'+suffix,'sa_movie_packet_flash_'+suffix)
text='#include "flash_storage.h"\n'+text
marker='struct sa_transfer_runtime_binding *runtime;};';assert text.count(marker)==1
text=text.replace(marker,'struct sa_transfer_runtime_binding *runtime;struct sa_flash_storage *flash;};')
for before,after in ((',console,0,0};',',console,0,0,0};'),(',console,exception,0};',',console,exception,0,0};'),(',console,exception,&runtime_memory};',',console,exception,&runtime_memory,0};')):
 assert text.count(before)==1;text=text.replace(before,after)
marker=' if(target==SA_NATIVE_SYSINFO_TARGET||target==SA_NATIVE_FLASH_TARGET)'
assert text.count(marker)==1
text=text.replace(marker,' if(target==SA_NATIVE_FLASH_TARGET&&c->flash){uint32_t result=r[0];int rc=sa_flash_storage_service(c->flash,m,r[7],r[4],r[5],r[6],&result);if(!rc)r[0]=result;return rc;}\n'+marker)
marker='SA_STAGE_EXPORT int sa_movie_packet_flash_running_connected('
assert text.count(marker)==1
extra=text[text.index(marker):]
assert extra.count('SA_STAGE_EXPORT')==1
extra=extra.replace('sa_movie_packet_flash_running_connected','sa_movie_packet_flash_storage_connected')
extra=extra.replace('struct sa_transfer_runtime *runtime){','struct sa_transfer_runtime *runtime,struct sa_flash_storage *storage){')
extra=extra.replace(' if(!runtime){',' if(!storage){s->pc=entry;return -6;}\n if(!runtime){')
extra=extra.replace('console,exception,&runtime_memory,0};','console,exception,&runtime_memory,storage};')
text+='\n/* Explicit mutable host storage; older ABIs retain read-only console policy. */\n'+extra
bridge=P/'source/sa_movie_packet_flash_connected.c';bridge.write_text(text)
old_probe=P/'source/movie_packet_startup_probe.c'
probe=old_probe.read_text().replace('sa_movie_packet_startup_','sa_movie_packet_flash_')
probe='#include "flash_storage.h"\n'+probe
declaration=next(line for line in probe.splitlines() if line.startswith('int sa_movie_packet_flash_running_connected('))
new_declaration=declaration.replace('sa_movie_packet_flash_running_connected','sa_movie_packet_flash_storage_connected').replace('struct sa_transfer_runtime*);','struct sa_transfer_runtime*,struct sa_flash_storage*);')
probe=probe.replace(declaration,declaration+'\n'+new_declaration)
marker='SA_STAGE_EXPORT int sa_movie_packet_flash_running_observed(';extra=probe[probe.index(marker):]
extra=extra.replace('sa_movie_packet_flash_running_observed','sa_movie_packet_flash_storage_observed').replace('sa_movie_packet_flash_running_connected','sa_movie_packet_flash_storage_connected')
extra=extra.replace('struct sa_transfer_runtime *runtime){','struct sa_transfer_runtime *runtime,struct sa_flash_storage *storage){').replace('console,exception,runtime);','console,exception,runtime,storage);')
probe+='\n'+extra;pp=P/'source/movie_packet_flash_probe.c';pp.write_text(probe)
old_build=P/'build_movie_packet_startup.ps1'
build=old_build.read_text().replace("'sa_movie_packet_startup_connected.c'","'sa_movie_packet_flash_connected.c','flash_storage.c'").replace('movie_packet_startup_probe.c','movie_packet_flash_probe.c').replace('movie_packet_startup.dll','movie_packet_flash.dll')
bp=P/'build_movie_packet_flash.ps1';bp.write_text(build)
sources=re.findall(r"'([^']+\.c)'",build);assert len(sources)==len(set(sources))==196
files=[Path(__file__),old,old_probe,old_build,bridge,pp,bp,P/'source/flash_storage.h',P/'analysis/movie_packet_startup_connection.json']+[P/'source'/n for n in sources]
report=dict(compiled_source_count=196,newly_registered_entries=0,explicit_storage_observed_arguments=20,legacy_argument_counts=[17,18,19],source=bridge.relative_to(P).as_posix(),scope='Explicit extra storage pointer,per-invocation native context,no global mutable storage and no inferred extended old struct. Original24packet bodies retained. Older entry signatures preserve their original console service. New entry dispatches actual firmware flash calls to separately authored byte/partition service; no game-helper result supplied.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
(P/'analysis/movie_packet_flash_connection.json').write_text(json.dumps(report,indent=2)+'\n')
print('Generated196-source packet/explicit-flash pipeline')
