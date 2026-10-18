import json
from pathlib import Path

root = Path(__file__).resolve().parent
report = 'analysis/work_complete_native_ranges_verification.json'
evidence = json.loads((root / report).read_text())
assert evidence['cases_passed'] == 432
names = {'sa_sound_stream_work_complete','sa_sound_driver_split_range'}
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
status['verified_progress']['work_complete_native_ranges'] = evidence
path.write_text(json.dumps(status, indent=2) + '\n')
path = root / 'SOURCE_HANDOFF.txt'
text = path.read_text()
marker = 'Native ranges in stream work completion:'
if marker not in text:
    path.write_text(text + '\n' + marker + ' verify_work_complete_native_ranges.py. '
                    + evidence['scope'] + ' Overall project completion remains 10%.\n')
path = root / 'build_and_verify.ps1'
text = path.read_text()
if 'verify_work_complete_native_ranges.py' not in text:
    path.write_text(text + "\n& $python (Join-Path $PSScriptRoot 'verify_work_complete_native_ranges.py')\n"
                    + "if ($LASTEXITCODE -ne 0) { throw 'Native input provider source query integration verification failed' }\n")
