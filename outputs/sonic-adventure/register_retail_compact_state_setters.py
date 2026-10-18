"""Register the verified compact state-setter routines."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report = json.loads((ROOT / 'analysis/retail_compact_state_setters_verification.json').read_text())
names = {0x8c062634: 'sa_zero_two_globals_8c062634',
         0x8c5ff2de: 'sa_zero_global_return_value_8c5ff2de'}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    assert hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest() == item['retail_span_sha256']
    routine = {'module': '1ST_READ.BIN', 'name': names[start],
               'address': hex(start), 'end_exclusive': hex(end),
               'retail_span_sha256': item['retail_span_sha256'],
               'source': 'source/retail_compact_state_setters.c',
               'verification': 'analysis/retail_compact_state_setters_verification.json',
               'status': f"Compact state setter matched in {item['cases_passed']} full-state cases",
               'limitations': report['limitations']}
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
status.setdefault('verified_progress', {})['retail_compact_state_setters'] = {
    'verification': 'analysis/retail_compact_state_setters_verification.json',
    'functions': report['functions_verified'], 'cases': report['total_full_state_cases'],
    'scope': report['scope'], 'limitations': report['limitations']}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {report['functions_verified']} compact setters; {len(registry['routines'])} routines total")
