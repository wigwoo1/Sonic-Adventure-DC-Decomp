"""Attach connected-sequence evidence only when its inputs match current files."""
import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
REPORT = 'analysis/sound_stream_record_lifecycle_verification.json'
report = json.loads((PROJECT / REPORT).read_text(encoding='utf-8'))
assert report['cases_passed'] == 2
assert report['steps_passed'] == 328
assert report['callback_entries_compared'] == 66

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

assert digest(ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN') == report['binary_sha256']
assert digest(ROOT / 'work/sonic-adventure/build/sound_stream_record_lifecycle.dll') == report['dll_sha256']
assert digest(PROJECT / 'verify_startup.py') == report['interpreter_sha256']
assert digest(PROJECT / 'verify_sound_stream_record_lifecycle.py') == report['verifier_sha256']
for source, expected_hash in report['sources'].items():
    assert digest(PROJECT / source) == expected_hash, f'Stale source evidence: {source}'

registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
attached = 0
for routine in registry['routines']:
    if routine.get('module') == '1ST_READ.BIN' and routine.get('source') in report['sources']:
        reports = routine.setdefault('additional_verification', [])
        if REPORT not in reports:
            reports.append(REPORT)
        attached += 1
assert attached == 7, attached
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress', {})['sound_stream_record_lifecycle'] = {
    'verification': REPORT, 'scenarios': 2, 'steps_passed': 328,
    'callback_entries_compared': 66, 'sources': report['sources'],
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
}
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Attached connected stream evidence to {attached} existing routines; no new routines counted')
