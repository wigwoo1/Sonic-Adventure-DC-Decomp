import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/audio_initialized_allocation_verification.json'
evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==36
names={'sa_sound_work_create','sa_sound_work_reset','sa_sound_source_buffer_get','sa_sound_stream_input_callback_set','sa_sound_source_ring_size_get','sa_sound_source_mirror_size_get','sa_sound_stream_create','sa_sound_stream_destroy','sa_file_decode_handle_allocate','sa_file_decode_handle_release','sa_sound_work_pool_initialize','sa_sound_stream_pool_initialize','sa_file_decode_pool_initialize'}
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=set()
for routine in registry['routines']:
 if routine['name'] in names:
  found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['audio_initialized_allocation']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Native audio pool initialization followed by allocation:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_audio_initialized_allocation.py. '+evidence['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_audio_initialized_allocation.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_audio_initialized_allocation.py')\nif ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }\n")
