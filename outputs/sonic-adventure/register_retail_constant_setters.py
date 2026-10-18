"""Register verified eight-byte main-executable constant setters."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report = json.loads((ROOT / 'analysis/retail_constant_setters_verification.json').read_text())
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    assert hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest() == item['retail_span_sha256']
    routine = {'module': '1ST_READ.BIN',
               'name': f"sa_retail_constant_setter_{start:08x}",
               'address': hex(start), 'end_exclusive': hex(end),
               'retail_span_sha256': item['retail_span_sha256'],
               'source': 'source/retail_constant_setters.c',
               'verification': 'analysis/retail_constant_setters_verification.json',
               'status': f"Constant setter matched in {item['cases_passed']} full-state cases",
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
status.setdefault('verified_progress', {})['retail_constant_setters'] = {
    'verification': 'analysis/retail_constant_setters_verification.json',
    'functions': report['functions_verified'], 'cases': report['total_full_state_cases'],
    'scope': report['scope'], 'limitations': report['limitations']}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {report['functions_verified']} constant setters; {len(registry['routines'])} routines total")
