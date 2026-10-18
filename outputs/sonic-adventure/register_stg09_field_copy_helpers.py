"""Register retail-verified STG09 field-copy loops and their direct thunks."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/STG09.BIN/field_copy_helpers_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 10 and report['total_full_state_cases'] == 640
binary = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
source = ROOT / 'source/stg09_field_copy_helpers.c'
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
        'source': 'source/stg09_field_copy_helpers.c',
        'verification': 'analysis/common-overlays/STG09.BIN/field_copy_helpers_verification.json',
        'status': f"STG09 retail execution matched in {item['cases_passed']} complete CPU-state and RAM cases",
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
status.setdefault('verified_progress', {})['stg09_field_copy_helpers_0c90bcba_family'] = {
    'verification': 'analysis/common-overlays/STG09.BIN/field_copy_helpers_verification.json',
    'routines': len(report['routines']), 'cases': report['total_full_state_cases'],
    'comparison_level': 'complete SH-4 CPU state and all 16 MiB RAM',
    'retail_span_sha256': [item['retail_span_sha256'] for item in report['routines']],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered 10 STG09 field-copy routines; {len(registry['routines'])} routines total")
