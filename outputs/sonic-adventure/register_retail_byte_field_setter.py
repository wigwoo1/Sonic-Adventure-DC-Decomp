"""Register the verified 0x8c6446d2 byte field setter."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
report = json.loads((ROOT / 'analysis/retail_byte_field_setter_verification.json').read_text())
start, end = int(report['routine_start'], 16), int(report['routine_end_exclusive'], 16)
assert hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest() == report['retail_span_sha256']
routine = {'module': '1ST_READ.BIN', 'name': 'sa_retail_byte_field_setter_8c6446d2',
           'address': hex(start), 'end_exclusive': hex(end),
           'retail_span_sha256': report['retail_span_sha256'],
           'source': 'source/retail_byte_field_setter.c',
           'verification': 'analysis/retail_byte_field_setter_verification.json',
           'status': f"Indexed byte setter matched in {report['cases_passed']} full-state cases",
           'limitations': report['limitations']}
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
status.setdefault('verified_progress', {})['retail_byte_field_setter_8c6446d2'] = {
    'verification': 'analysis/retail_byte_field_setter_verification.json',
    'cases': report['cases_passed'], 'retail_span_sha256': report['retail_span_sha256'],
    'scope': report['scope'], 'limitations': report['limitations']}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered byte field setter; {len(registry['routines'])} routines total")
