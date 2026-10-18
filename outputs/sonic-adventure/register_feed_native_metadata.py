import json
from pathlib import Path

root = Path(__file__).resolve().parent
report = 'analysis/feed_native_metadata_verification.json'
evidence = json.loads((root / report).read_text())
assert evidence['cases_passed'] == 256
names = {'sa_sound_stream_work_feed','sa_sound_source_capacity','sa_sound_stream_format_get','sa_sound_stream_unit_count_get','sa_sound_stream_total_samples_get','sa_sound_work_unit_count_get','sa_sound_work_total_samples_get'}
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
status['verified_progress']['feed_native_metadata'] = evidence
path.write_text(json.dumps(status, indent=2) + '\n')
path = root / 'SOURCE_HANDOFF.txt'
text = path.read_text()
marker = 'Native metadata in source feeding:'
if marker not in text:
    path.write_text(text + '\n' + marker + ' verify_feed_native_metadata.py. '
                    + evidence['scope'] + ' Overall project completion remains 10%.\n')
path = root / 'build_and_verify.ps1'
text = path.read_text()
if 'verify_feed_native_metadata.py' not in text:
    path.write_text(text + "\n& $python (Join-Path $PSScriptRoot 'verify_feed_native_metadata.py')\n"
                    + "if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }\n")
