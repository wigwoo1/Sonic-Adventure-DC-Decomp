import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/source_error_callback_connected_verification.json';evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==2100
names={'sa_sound_source_error_callback_set','sa_sound_source_identity_get','sa_sound_source_contiguous_check','sa_sound_source_lock_enter','sa_sound_source_lock_exit'}
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=set()
for routine in registry['routines']:
 if routine['name'] in names:
  found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
assert found==names;path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['source_error_callback_connected']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native source callback installation and error-path invocation:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_source_error_callback_connected.py. '+evidence['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_source_error_callback_connected.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_source_error_callback_connected.py')\nif ($LASTEXITCODE -ne 0) { throw 'Source pool connected lifecycle verification failed' }\n")
