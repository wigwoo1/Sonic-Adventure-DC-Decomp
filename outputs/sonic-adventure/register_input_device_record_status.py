"""Register three verified input-device record status routines."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/input_device_record_status_verification.json').read_text(encoding='utf-8'))
assert report['functions_verified'] == 3 and report['total_cases'] == 384
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
source = ROOT / 'source/sa_input_device_record_status.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})

for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    binary_offset = start - int(item['load_address'], 16)
    span = binary[binary_offset:binary_offset + end - start]
    assert hashlib.sha256(span).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': item['module'], 'load_address': item['load_address'],
        'name': item['name'], 'address': item['address'],
        'end_exclusive': item['end_exclusive'],
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/sa_input_device_record_status.c',
        'verification': 'analysis/input_device_record_status_verification.json',
        'status': f"Input-device record routine matched in {item['cases_passed']} full-state cases",
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
        'verification': routine['verification'], 'cases': item['cases_passed'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': item['retail_span_sha256'],
        'source_sha256': report['source_sha256'], 'scope': report['scope'],
        'limitations': report['limitations'],
    }

progress['recovered_buildable_routines'] = len(registry['routines'])
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered input-device status routines; {len(registry['routines'])} routines total")
