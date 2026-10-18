"""Register the ABI-verified input mapping query wrapper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report = json.loads((ROOT / 'analysis/input_mapping_query_verification.json').read_text())
assert report['cases_passed'] == 118
start, end = int(report['routine_start'], 16), int(report['routine_end_exclusive'], 16)
assert hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest() == report['retail_span_sha256']
routine = {
    'module': '1ST_READ.BIN', 'name': 'sa_input_mapping_query_8c0a6ab4',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/input_mapping_query.c',
    'verification': 'analysis/input_mapping_query_verification.json',
    'status': f"Input mapping query matched in {report['cases_passed']} ABI-level cases",
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
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
status.setdefault('verified_progress', {})['input_mapping_query_8c0a6ab4'] = {
    'verification': 'analysis/input_mapping_query_verification.json',
    'cases': report['cases_passed'], 'comparison_level': report['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered input mapping query; {len(registry['routines'])} routines total")
