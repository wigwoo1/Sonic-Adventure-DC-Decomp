import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/magic_update_retail_pipeline_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==218
names={'sa_sound_stream_update_dispatch','sa_sound_stream_magic_update','sa_sound_stream_decode_schedule','sa_sound_stream_stereo_request','sa_file_decode_stereo_request','sa_file_decode_request_activate','sa_file_decode_request_process','sa_sound_decode_block_stereo','sa_sound_stream_decode_complete','sa_file_decode_result_get','sa_file_decode_phase_get','sa_file_decode_completion_acknowledge'};found=set()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['magic_update_retail_pipeline']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native retail audio update pipeline:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_magic_update_retail_pipeline.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_magic_update_retail_pipeline.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_magic_update_retail_pipeline.py')\nif ($LASTEXITCODE -ne 0) { throw 'File-dispatch stereo pipeline verification failed' }\n")
