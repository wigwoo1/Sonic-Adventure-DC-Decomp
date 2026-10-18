"""Register the ABI-verified STG09 object-to-record refresh loops."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/common-overlays/STG09.BIN/field_refresh_helpers_verification.json').read_text(encoding='utf-8'))
assert report['functions_verified'] == 2 and report['total_abi_cases'] == 192
binary = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
source = ROOT / 'source/stg09_field_refresh_helpers.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    offset = start - int(item['load_address'], 16)
    assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': 'STG09.BIN', 'load_address': item['load_address'],
        'name': item['name'], 'address': hex(start), 'end_exclusive': hex(end),
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/stg09_field_refresh_helpers.c',
        'verification': 'analysis/common-overlays/STG09.BIN/field_refresh_helpers_verification.json',
        'status': f"STG09 retail RAM and ABI behavior matched in {item['cases_passed']} cases",
        'comparison_level': item.get('comparison_level', report['comparison_level']),
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
status.setdefault('verified_progress', {})['stg09_field_refresh_helpers_0c90bd42_family'] = {
    'verification': 'analysis/common-overlays/STG09.BIN/field_refresh_helpers_verification.json',
    'routines': len(report['routines']), 'cases': report['total_abi_cases'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': [item['retail_span_sha256'] for item in report['routines']],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered 2 STG09 field-refresh routines; {len(registry['routines'])} routines total")
