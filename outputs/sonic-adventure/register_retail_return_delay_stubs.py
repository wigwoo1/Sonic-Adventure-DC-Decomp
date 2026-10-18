"""Register nineteen verified return-delay routines from retail modules."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/retail_return_delay_stubs_verification.json').read_text())
assert report['functions_verified'] == 19
assert report['total_full_state_cases'] == 304
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    module = item['module']
    if module == '1ST_READ.BIN':
        binary_path, base = ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN', 0x8c010000
    else:
        binary_path, base = ROOT.parent.parent / f'work/sonic-adventure/decoded/SONICADV/{module}', 0x0c900000
    binary = binary_path.read_bytes()
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    assert hashlib.sha256(binary[start - base:end - base]).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': module,
        'name': f"sa_return_{item['kind']}_{start:08x}",
        'address': hex(start), 'end_exclusive': hex(end),
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/retail_return_delay_stubs.c',
        'verification': 'analysis/retail_return_delay_stubs_verification.json',
        'status': f"RTS delay-slot behavior matched in {item['cases_passed']} full-state cases",
        'limitations': report['limitations'],
    }
    found = next((entry for entry in registry['routines']
                  if entry.get('module') == module and int(entry['address'], 16) == start), None)
    if found is None:
        registry['routines'].append(routine)
    else:
        found.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress', {})['retail_return_delay_stubs'] = {
    'verification': 'analysis/retail_return_delay_stubs_verification.json',
    'cases': report['total_full_state_cases'],
    'retail_span_sha256': [item['retail_span_sha256'] for item in report['routines']],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered 19 return-delay routines; {len(registry['routines'])} routines total")
