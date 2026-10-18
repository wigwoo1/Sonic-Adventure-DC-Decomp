import json
from pathlib import Path
root=Path(__file__).resolve().parent;report='analysis/pool_audio_slot_verification.json'
evidence=json.loads((root/report).read_text());assert evidence['cases_passed']==16
names={'sa_sound_stream_default_input','sa_sound_stream_default_output','sa_sound_stream_magic_update','sa_sound_stream_decode_complete','sa_sound_stream_update_dispatch','sa_sound_stream_source_set','sa_sound_stream_work_tick','sa_sound_stream_phase_get','sa_sound_stream_work_complete','sa_sound_source_commit','sa_sound_source_return','sa_sound_driver_split_range','sa_sound_stream_work_feed','sa_sound_source_capacity','sa_sound_work_unit_count_get','sa_sound_work_total_samples_get','sa_sound_stream_input_provider','sa_sound_source_query','sa_sound_stream_work_update','sa_sound_work_pool_update'};found=set()
path=root/'analysis/recovered_routines.json';registry=json.loads(path.read_text())
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
        if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=root/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['pool_audio_slot']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=root/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Every work slot in native pool decoding:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_pool_audio_slot.py. '+evidence['scope']+' Overall project10%.\n')
path=root/'build_and_verify.ps1';text=path.read_text()
if 'verify_pool_audio_slot.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_pool_audio_slot.py')\nif ($LASTEXITCODE -ne 0) { throw 'Native input audio lifecycle verification failed' }\n")
