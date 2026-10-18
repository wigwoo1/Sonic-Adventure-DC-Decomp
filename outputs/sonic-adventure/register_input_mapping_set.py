"""Register two ABI-verified input-to-bitset setter wrappers."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/input_mapping_set_verification.json').read_text())
assert report['functions_verified'] == 2
assert report['total_cases'] == 176
symbols = {'index_set': 'index_bitset_set', 'mapping_set': 'mapping_bitset_set'}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    routine = {
        'module': '1ST_READ.BIN',
        'name': f"sa_input_{symbols[item['kind']]}_8c0a{start & 0xffff:04x}",
        'address': hex(start), 'end_exclusive': hex(end),
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/input_mapping_set.c',
        'verification': 'analysis/input_mapping_set_verification.json',
        'status': f"Input mapping setter matched in {item['cases_passed']} ABI-level cases",
        'comparison_level': report['comparison_level'],
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
status.setdefault('verified_progress', {})['input_mapping_setters_8c0a6a88_family'] = {
    'verification': 'analysis/input_mapping_set_verification.json',
    'cases': report['total_cases'], 'comparison_level': report['comparison_level'],
    'retail_span_sha256': [item['retail_span_sha256'] for item in report['routines']],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered 2 input mapping setters; {len(registry['routines'])} routines total")
