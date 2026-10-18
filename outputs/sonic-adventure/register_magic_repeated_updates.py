import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/magic_repeated_updates_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==436
names={'sa_sound_stream_default_input','sa_sound_stream_default_output','sa_sound_stream_magic_update','sa_sound_stream_decode_complete','sa_sound_stream_update_dispatch','sa_sound_stream_source_set','sa_sound_stream_decode_acknowledge'};found=set()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['magic_repeated_updates']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Repeated native original-audio updates:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_magic_repeated_updates.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_magic_repeated_updates.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_magic_repeated_updates.py')\nif ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }\n")
