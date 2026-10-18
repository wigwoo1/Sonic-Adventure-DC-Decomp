import json
from pathlib import Path
p=Path(__file__).resolve().parent;report='analysis/native_audio_full_file_verification.json';evidence=json.loads((p/report).read_text())
assert evidence['cases_passed']==2 and evidence['original_files']==1
assert {r['bank'] for r in evidence['assets']}=={0,0x200000}
assert all(r['total_samples']==156653 and r['full_file_published'] and r['streamed_updates']>=600 for r in evidence['assets'])
names={'sa_sound_source_pool_initialize','sa_sound_source_create','sa_sound_source_reset','sa_sound_source_query','sa_sound_source_commit','sa_sound_source_return','sa_sound_source_lock_enter','sa_sound_source_lock_exit','sa_byte_move','sa_sound_work_pool_initialize','sa_sound_stream_pool_initialize','sa_file_decode_pool_initialize','sa_sound_work_create','sa_sound_work_start','sa_sound_work_sample_limit_set','sa_sound_work_remaining_limit_set','sa_sound_stream_work_initialize','sa_sound_stream_work_feed','sa_sound_stream_work_update','sa_sound_stream_work_tick','sa_sound_stream_work_complete','sa_sound_stream_decode_schedule','sa_sound_stream_decode_complete','sa_file_decode_request_process','sa_sound_decode_block_stereo'}
path=p/'analysis/recovered_routines.json';registry=json.loads(path.read_text());found=set()
for routine in registry['routines']:
 if routine['name'] in names:
  found.add(routine['name']);extra=routine.setdefault('additional_verification',[])
  if report not in extra:extra.append(report)
assert found==names,(names-found)
path.write_text(json.dumps(registry,indent=2)+'\n')
path=p/'project_status.json';status=json.loads(path.read_text());status['verified_progress']['native_audio_full_file']=evidence;path.write_text(json.dumps(status,indent=2)+'\n')
path=p/'SOURCE_HANDOFF.txt';text=path.read_text();marker='Complete original FISHMISS audio streaming:'
if marker not in text:path.write_text(text+'\n'+marker+' verify_native_audio_full_file.py. '+evidence['scope']+' See AUDIO_SOURCE_ABI.txt for verified buffer ABI and caller scheduling constraints. Overall project10%.\n')
path=p/'build_and_verify.ps1';text=path.read_text()
if 'verify_native_audio_full_file.py' not in text:path.write_text(text+"\n& $python (Join-Path $PSScriptRoot 'verify_native_audio_full_file.py')\nif ($LASTEXITCODE -ne 0) { throw 'Complete native audio file verification failed' }\n")
