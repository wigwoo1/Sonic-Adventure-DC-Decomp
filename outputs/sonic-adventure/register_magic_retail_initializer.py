import json
from pathlib import Path

root = Path(__file__).resolve().parent
report = 'analysis/magic_retail_initializer_verification.json'
evidence = json.loads((root / report).read_text())
assert evidence['cases_passed'] == 436 and evidence['original_files'] == 109
names = {
    'sa_sound_stream_format_initialize', 'sa_sound_magic_state_initialize',
    'sa_sound_magic_header_metadata', 'sa_sound_magic_extension_word',
    'sa_sound_magic_stream_parameters',
}
path = root / 'analysis/recovered_routines.json'
registry = json.loads(path.read_text())
found = set()
for routine in registry['routines']:
    if routine['name'] in names:
        found.add(routine['name'])
        extra = routine.setdefault('additional_verification', [])
        if report not in extra:
            extra.append(report)
assert found == names, names - found
path.write_text(json.dumps(registry, indent=2) + '\n')
path = root / 'project_status.json'
status = json.loads(path.read_text())
status['verified_progress']['magic_retail_initializer'] = evidence
path.write_text(json.dumps(status, indent=2) + '\n')
path = root / 'SOURCE_HANDOFF.txt'
text = path.read_text()
marker = 'Original magic8000 initializer verification:'
if marker not in text:
    path.write_text(text + '\n' + marker + ' verify_magic_retail_initializer.py reproduces 436 retail comparisons across 109 original headers, both FPU banks, and configuration success/error results. Native format selector, initializer, metadata, extension, parameters, signed division and fill are connected. Independent full 256-byte state and configuration forwarding checks supplement full RAM/CPU/FPU comparisons. Only configuration at 0x8c644be6 is modeled. Original 64-byte headers and actual file sizes are used; full payload decoding, physical audio and full-game behavior remain unverified. Overall project remains 10%.\n')
path = root / 'build_and_verify.ps1'
text = path.read_text()
if 'verify_magic_retail_initializer.py' not in text:
    path.write_text(text + "\n& $python (Join-Path $PSScriptRoot 'verify_magic_retail_initializer.py')\nif ($LASTEXITCODE -ne 0) { throw 'Original magic initializer verification failed' }\n")
