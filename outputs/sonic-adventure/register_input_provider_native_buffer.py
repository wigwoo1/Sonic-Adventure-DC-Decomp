import json
from pathlib import Path

root = Path(__file__).resolve().parent
report = 'analysis/input_provider_native_buffer_verification.json'
evidence = json.loads((root / report).read_text())
assert evidence['cases_passed'] == 108
names = {'sa_sound_stream_input_provider','sa_sound_stream_channel_count_get','sa_sound_stream_position_get','sa_sound_source_buffer_get'}
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
status['verified_progress']['input_provider_native_buffer'] = evidence
path.write_text(json.dumps(status, indent=2) + '\n')
path = root / 'SOURCE_HANDOFF.txt'
text = path.read_text()
marker = 'Input provider native buffer integration:'
if marker not in text:
    path.write_text(text + '\n' + marker + ' verify_input_provider_native_buffer.py. '
                    + evidence['scope'] + ' Overall project completion remains 10%.\n')
path = root / 'build_and_verify.ps1'
text = path.read_text()
if 'verify_input_provider_native_buffer.py' not in text:
    path.write_text(text + "\n& $python (Join-Path $PSScriptRoot 'verify_input_provider_native_buffer.py')\n"
                    + "if ($LASTEXITCODE -ne 0) { throw 'Native file decoder flag integration verification failed' }\n")
