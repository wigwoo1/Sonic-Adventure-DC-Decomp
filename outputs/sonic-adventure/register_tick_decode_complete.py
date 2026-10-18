import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/tick_decode_complete_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==218
names={'sa_sound_stream_default_input','sa_sound_stream_default_output','sa_sound_stream_magic_update','sa_sound_stream_decode_complete','sa_sound_stream_update_dispatch','sa_sound_stream_source_set','sa_sound_stream_work_tick','sa_sound_stream_phase_get','sa_sound_stream_work_complete','sa_sound_source_commit','sa_sound_source_return','sa_sound_driver_split_range'};found=set()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['tick_decode_complete']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native decoding/completion in work dispatcher:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_tick_decode_complete.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_tick_decode_complete.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_tick_decode_complete.py')\nif ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }\n")
