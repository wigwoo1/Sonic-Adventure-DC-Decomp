"""Register the ABI-verified input bit setter and callback dispatch."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/input_bitset_set_verification.json').read_text())
assert report['cases_passed'] == 48
routine = {
    'module': '1ST_READ.BIN', 'name': 'sa_input_bitset_set_8c0a6918',
    'address': report['routine_start'], 'end_exclusive': report['routine_end_exclusive'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/input_bitset_set.c',
    'verification': 'analysis/input_bitset_set_verification.json',
    'status': f"Input bit setter matched in {report['cases_passed']} ABI-level cases",
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
start = int(routine['address'], 16)
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
status.setdefault('verified_progress', {})['input_bitset_set_8c0a6918'] = {
    'verification': 'analysis/input_bitset_set_verification.json',
    'cases': report['cases_passed'], 'comparison_level': report['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered input bit setter; {len(registry['routines'])} routines total")
