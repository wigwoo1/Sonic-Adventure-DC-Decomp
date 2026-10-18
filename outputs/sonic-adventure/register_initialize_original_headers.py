import json
from pathlib import Path

root = Path(__file__).resolve().parent
report = 'analysis/initialize_original_headers_verification.json'
evidence = json.loads((root / report).read_text())
assert evidence['cases_passed'] == 218
names = {'sa_sound_stream_work_initialize','sa_sound_source_query','sa_sound_source_commit','sa_sound_source_return','sa_sound_source_lock_enter','sa_sound_source_lock_exit','sa_integer_remainder','sa_sound_driver_split_range'}
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
status['verified_progress']['initialize_original_headers'] = evidence
path.write_text(json.dumps(status, indent=2) + '\n')
path = root / 'SOURCE_HANDOFF.txt'
text = path.read_text()
marker = 'Original headers in native work initialization:'
if marker not in text:
    path.write_text(text + '\n' + marker + ' verify_initialize_original_headers.py. '
                    + evidence['scope'] + ' Overall project completion remains 10%.\n')
path = root / 'build_and_verify.ps1'
text = path.read_text()
if 'verify_initialize_original_headers.py' not in text:
    path.write_text(text + "\n& $python (Join-Path $PSScriptRoot 'verify_initialize_original_headers.py')\n"
                    + "if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }\n")
