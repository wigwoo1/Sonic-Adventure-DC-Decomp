import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/record_initialize_8c600dd0_verification.json').read_text(encoding='utf-8-sig'))
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == json.loads((ROOT / 'analysis/recovered_routines.json').read_text(encoding='utf-8-sig'))['binary_sha256']
start, end = int(report['address'], 16), int(report['end_exclusive'], 16)
offset = start - int(report['load_address'], 16)
assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == report['retail_span_sha256']
source_path = ROOT / 'source/sa_record_initialize_8c600dd0.c'
assert hashlib.sha256(source_path.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': report['module'], 'load_address': report['load_address'],
    'name': 'sa_record_initialize_8c600dd0', 'address': report['address'],
    'end_exclusive': report['end_exclusive'], 'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/sa_record_initialize_8c600dd0.c',
    'verification': 'analysis/record_initialize_8c600dd0_verification.json',
    'status': 'Record initializer matched retail RAM and CPU state in 128 cases',
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
existing = next((entry for entry in registry['routines'] if entry.get('module') == routine['module'] and int(entry['address'], 16) == start), None)
if existing is None:
    registry['routines'].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[routine['name']] = {
    'verification': routine['verification'], 'cases': report['cases_passed'],
    'comparison_level': report['comparison_level'], 'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'], 'scope': report['scope'], 'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered record initializer at {report['address']}; {len(registry['routines'])} routines total")
