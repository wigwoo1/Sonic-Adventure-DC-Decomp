import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/work_destroy_native_release_verification.json'
evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==756
names={'sa_sound_work_destroy','sa_sound_critical_enter','sa_sound_critical_exit','sa_sound_stream_destroy','sa_file_decode_handle_release'}
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=set()
for routine in registry['routines']:
 if routine['name'] in names:
  found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['work_destroy_native_release']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Fully native work, stream and handle release:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_work_destroy_native_release.py. '+evidence['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_work_destroy_native_release.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_work_destroy_native_release.py')\nif ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }\n")
