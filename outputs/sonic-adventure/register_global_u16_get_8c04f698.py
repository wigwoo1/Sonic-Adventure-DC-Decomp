"""Register the retail-equivalent unsigned global-word getter."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/global_u16_get_8c04f698_verification.json').read_text(encoding='utf-8'))
assert report['cases_passed'] == 128
item = report['routines'][0]
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - int(item['load_address'], 16)
assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source = ROOT / 'source/sa_global_u16_get_8c04f698.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': item['module'],
    'load_address': item['load_address'],
    'name': 'sa_global_u16_get_8c04f698',
    'address': item['address'],
    'end_exclusive': item['end_exclusive'],
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/sa_global_u16_get_8c04f698.c',
    'verification': 'analysis/global_u16_get_8c04f698_verification.json',
    'status': 'Global unsigned-word getter matched retail RAM and CPU state in 128 cases',
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
existing = next((entry for entry in registry['routines']
                 if entry.get('module') == item['module'] and int(entry['address'], 16) == start), None)
if existing is None:
    registry['routines'].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[routine['name']] = {
    'verification': routine['verification'],
    'cases': report['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'],
    'scope': report['scope'],
    'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered unsigned global-word getter at {item['address']}; {len(registry['routines'])} routines total")
