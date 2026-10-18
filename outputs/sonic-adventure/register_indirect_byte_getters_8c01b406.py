"""Register two retail-equivalent indirect signed-byte getters."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/indirect_byte_getters_8c01b406_verification.json').read_text(encoding='utf-8'))
assert report['cases_passed'] == 192
assert len(report['routines']) == 2
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
source = ROOT / 'source/sa_indirect_byte_getters_8c01b406.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})

for item in report['routines']:
    assert item['cases_passed'] == 96
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
        'source': 'source/sa_indirect_byte_getters_8c01b406.c',
        'verification': 'analysis/indirect_byte_getters_8c01b406_verification.json',
        'status': f"Indirect signed-byte getter at offset {item['field_offset']} matched in 96 full RAM/CPU cases",
        'comparison_level': report['comparison_level'],
        'limitations': report['limitations'],
    }
    existing = next((entry for entry in registry['routines']
                     if entry.get('module') == item['module'] and int(entry['address'], 16) == start), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)
    progress[routine['name']] = {
        'verification': routine['verification'],
        'cases': item['cases_passed'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': item['retail_span_sha256'],
        'source_sha256': report['source_sha256'],
        'scope': report['scope'],
        'limitations': report['limitations'],
    }

registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {len(report['routines'])} indirect byte getters; {len(registry['routines'])} routines total")
