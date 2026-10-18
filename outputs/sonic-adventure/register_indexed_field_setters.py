"""Register seven differential-verified indexed field setters."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report_path = ROOT / 'analysis/indexed_field_setters_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 7
assert report['total_full_state_cases'] == 112
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    span = binary[start - 0x8c010000:end - 0x8c010000]
    assert hashlib.sha256(span).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': '1ST_READ.BIN',
        'name': f"sa_indexed_field_setter_{start:08x}",
        'address': hex(start), 'end_exclusive': hex(end),
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/indexed_field_setters.c',
        'verification': 'analysis/indexed_field_setters_verification.json',
        'status': f"Indexed field setter matched in {item['cases_passed']} full-state cases",
        'limitations': report['limitations'],
    }
    found = next((entry for entry in registry['routines']
                  if entry.get('module') == routine['module'] and
                  int(entry['address'], 16) == start), None)
    if found is None:
        registry['routines'].append(routine)
    else:
        found.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress', {})['indexed_field_setters_8c65f462_family'] = {
    'verification': 'analysis/indexed_field_setters_verification.json',
    'cases': report['total_full_state_cases'],
    'retail_span_sha256': [item['retail_span_sha256'] for item in report['routines']],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered 7 indexed field setters; {len(registry['routines'])} routines total")
