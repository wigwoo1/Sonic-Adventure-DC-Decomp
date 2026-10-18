import json
from pathlib import Path

root = Path(__file__).resolve().parent
report = 'analysis/public_audio_terminal_verification.json'
evidence = json.loads((root / report).read_text())
assert evidence['cases_passed'] == 216
names = {'sa_sound_stream_work_feed','sa_sound_source_capacity','sa_sound_stream_format_get','sa_sound_stream_unit_count_get','sa_sound_stream_total_samples_get','sa_sound_work_unit_count_get','sa_sound_work_total_samples_get','sa_sound_stream_source_set','sa_sound_stream_start','sa_sound_source_query','sa_sound_source_return','sa_sound_source_lock_enter','sa_sound_source_lock_exit','sa_integer_remainder','sa_sound_stream_work_update','sa_sound_stream_work_tick','sa_sound_stream_phase_get','sa_sound_stream_update_dispatch','sa_sound_stream_magic_update','sa_file_decode_phase_get'}
path = root / 'analysis/recovered_routines.json'
registry = json.loads(path.read_text())
found = set()
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name'])
        extra = routine.setdefault('additional_verification', [])
        if report not in extra:
            extra.append(report)
assert found == names
path.write_text(json.dumps(registry, indent=2) + '\n')
path = root / 'project_status.json'
status = json.loads(path.read_text())
status['verified_progress']['public_audio_terminal'] = evidence
path.write_text(json.dumps(status, indent=2) + '\n')
path = root / 'SOURCE_HANDOFF.txt'
text = path.read_text()
marker = 'Original audio markers through public update:'
if marker not in text:
    path.write_text(text + '\n' + marker + ' verify_public_audio_terminal.py. '
                    + evidence['scope'] + ' Overall project completion remains 10%.\n')
path = root / 'build_and_verify.ps1'
text = path.read_text()
if 'verify_public_audio_terminal.py' not in text:
    path.write_text(text + "\n& $python (Join-Path $PSScriptRoot 'verify_public_audio_terminal.py')\n"
                    + "if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }\n")
