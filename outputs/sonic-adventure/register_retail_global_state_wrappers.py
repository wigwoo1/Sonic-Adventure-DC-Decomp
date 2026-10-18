"""Register verified main-executable global-state wrappers."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report = json.loads((ROOT / 'analysis/retail_global_state_wrappers_verification.json').read_text())
names = {0x8c64401c: 'sa_global_setter_8c64401c',
         0x8c606f14: 'sa_dual_global_setter_8c606f14',
         0x8c60ede6: 'sa_dual_argument_global_setter_8c60ede6',
         0x8c107ba0: 'sa_conditional_global_setter_8c107ba0'}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    assert hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest() == item['retail_span_sha256']
    routine = {'module': '1ST_READ.BIN', 'name': names[start],
               'address': hex(start), 'end_exclusive': hex(end),
               'retail_span_sha256': item['retail_span_sha256'],
               'source': 'source/retail_global_state_wrappers.c',
               'verification': 'analysis/retail_global_state_wrappers_verification.json',
               'status': f"Global state wrapper matched in {item['cases_passed']} full-state cases",
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
status.setdefault('verified_progress', {})['retail_global_state_wrappers'] = {
    'verification': 'analysis/retail_global_state_wrappers_verification.json',
    'functions': report['functions_verified'], 'cases': report['total_full_state_cases'],
    'scope': report['scope'], 'limitations': report['limitations']}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {report['functions_verified']} global wrappers; {len(registry['routines'])} routines total")
