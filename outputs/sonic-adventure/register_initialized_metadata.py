import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/initialized_metadata_verification.json'
evidence=json.loads((p/report).read_text());assert evidence['cases_passed']==218
names={'sa_sound_work_sample_rate_get', 'sa_sound_stream_format_get', 'sa_sound_work_channel_count_get', 'sa_sound_stream_work_initialize', 'sa_sound_work_format_get', 'sa_sound_work_sample_bits_get', 'sa_sound_stream_channel_count_get', 'sa_sound_stream_sample_rate_get', 'sa_sound_stream_sample_bits_get', 'sa_sound_stream_work_update'}
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=set()
for routine in registry['routines']:
 if routine['name'] in names:
  found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
assert found==names
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['initialized_metadata']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Original-header initialization and native metadata queries:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_initialized_metadata.py. '+evidence['scope']+' Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_initialized_metadata.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_initialized_metadata.py')\nif ($LASTEXITCODE -ne 0) { throw 'Native constructor callback configuration verification failed' }\n")
