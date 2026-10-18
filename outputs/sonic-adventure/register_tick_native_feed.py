import json
from pathlib import Path

root = Path(__file__).resolve().parent
report = 'analysis/tick_native_feed_verification.json'
evidence = json.loads((root / report).read_text())
assert evidence['cases_passed'] == 256
names = {'sa_sound_stream_work_feed','sa_sound_source_capacity','sa_sound_stream_format_get','sa_sound_stream_unit_count_get','sa_sound_stream_total_samples_get','sa_sound_work_unit_count_get','sa_sound_work_total_samples_get','sa_sound_stream_source_set','sa_sound_stream_start','sa_sound_source_query','sa_sound_source_return','sa_sound_source_lock_enter','sa_sound_source_lock_exit','sa_integer_remainder','sa_sound_stream_phase_get','sa_sound_stream_work_tick'}
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
status['verified_progress']['tick_native_feed'] = evidence
path.write_text(json.dumps(status, indent=2) + '\n')
path = root / 'SOURCE_HANDOFF.txt'
text = path.read_text()
marker = 'Native feeder in dispatcher:'
if marker not in text:
    path.write_text(text + '\n' + marker + ' verify_tick_native_feed.py. '
                    + evidence['scope'] + ' Overall project completion remains 10%.\n')
path = root / 'build_and_verify.ps1'
text = path.read_text()
if 'verify_tick_native_feed.py' not in text:
    path.write_text(text + "\n& $python (Join-Path $PSScriptRoot 'verify_tick_native_feed.py')\n"
                    + "if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }\n")
