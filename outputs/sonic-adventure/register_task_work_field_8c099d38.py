"""Register the verified task-work-field setter/getter pair."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/task_work_field_8c099d38_verification.json').read_text(encoding='utf-8'))
assert report['functions_verified'] == 4
assert report['total_full_state_cases'] == 512
assert all(item['cases_passed'] == 128 for item in report['routines'])
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
source = ROOT / 'source/sa_task_work_field_8c099d38.c'
source_sha256 = hashlib.sha256(source.read_bytes()).hexdigest()

registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})

for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    offset = start - int(item['load_address'], 16)
    assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': item['module'],
        'load_address': item['load_address'],
        'name': item['name'],
        'address': item['address'],
        'end_exclusive': item['end_exclusive'],
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/sa_task_work_field_8c099d38.c',
        'verification': 'analysis/task_work_field_8c099d38_verification.json',
        'status': f"Task work-field {'setter' if 'set_' in item['name'] else 'getter'} matched retail RAM and CPU state in {item['cases_passed']} cases",
        'comparison_level': report['comparison_level'],
        'limitations': report['limitations'],
    }
    existing = next((entry for entry in registry['routines']
                     if entry.get('module') == item['module'] and int(entry['address'], 16) == start), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)
    progress[item['name']] = {
        'verification': routine['verification'],
        'cases': item['cases_passed'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': item['retail_span_sha256'],
        'source_sha256': source_sha256,
        'scope': report['scope'],
        'limitations': report['limitations'],
    }

registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {len(report['routines'])} task work-field routines; {len(registry['routines'])} routines total")
