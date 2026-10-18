import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/sound_source_pool_connected_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==36 and evidence['native_operations_compared']==9396
names={'sa_sound_source_pool_initialize','sa_sound_source_create','sa_sound_source_reset','sa_sound_source_destroy'}
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=set()
for routine in registry['routines']:
 if routine['name'] in names:
  found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
assert found==names;path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['sound_source_pool_connected']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Persistent native source-pool lifecycles:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_sound_source_pool_connected.py. '+evidence['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_sound_source_pool_connected.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_sound_source_pool_connected.py')\nif ($LASTEXITCODE -ne 0) { throw 'Source pool connected lifecycle verification failed' }\n")
