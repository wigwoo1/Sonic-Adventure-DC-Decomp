import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/file_decode_ack_sequence_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==218
names={'sa_file_decode_request_process','sa_sound_decode_block_stereo','sa_file_decode_stereo_request','sa_file_decode_request_activate','sa_file_decode_completion_acknowledge'};found=set()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['file_decode_ack_sequence']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native audio completion sequence:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_file_decode_ack_sequence.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_file_decode_ack_sequence.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_file_decode_ack_sequence.py')\nif ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }\n")
