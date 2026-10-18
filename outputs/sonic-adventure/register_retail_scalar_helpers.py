"""Register the state-and-RAM verified retail scalar helpers."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/retail_scalar_helpers_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 3
assert report['total_full_state_cases'] == 192
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
source_path = ROOT / 'source/retail_scalar_helpers.c'
assert hashlib.sha256(source_path.read_bytes()).hexdigest() == report['source_sha256']

registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    offset = start - 0x8c010000
    assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': '1ST_READ.BIN',
        'name': f"sa_{item['name']}_8c{start & 0xffffff:06x}",
        'address': hex(start), 'end_exclusive': hex(end),
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/retail_scalar_helpers.c',
        'verification': 'analysis/retail_scalar_helpers_verification.json',
        'status': f"Retail execution matched in {item['cases_passed']} full-state and RAM cases",
        'comparison_level': 'complete SH-4 CPU state and all 16 MiB RAM',
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
status.setdefault('verified_progress', {})['retail_scalar_helpers_8c5ff396_8c602618_8c09de34'] = {
    'verification': 'analysis/retail_scalar_helpers_verification.json',
    'cases': report['total_full_state_cases'],
    'comparison_level': 'complete SH-4 CPU state and all 16 MiB RAM',
    'retail_span_sha256': [item['retail_span_sha256'] for item in report['routines']],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered 3 retail scalar helpers; {len(registry['routines'])} routines total")
