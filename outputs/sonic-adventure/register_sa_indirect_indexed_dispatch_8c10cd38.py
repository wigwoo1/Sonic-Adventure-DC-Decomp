"""Register the two verified indexed indirect dispatchers."""
import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
REPORT_PATH = 'analysis/sa_indirect_indexed_dispatch_8c10cd38_verification.json'
SOURCE_PATH = 'source/sa_indirect_indexed_dispatch_8c10cd38.c'
BASE = 0x8c010000
report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 256
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
source_hash = hashlib.sha256((PROJECT / SOURCE_PATH).read_bytes()).hexdigest()
assert source_hash == report['source_sha256']

registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
for record in report['routines']:
    start, end = int(record['address'], 16), int(record['end_exclusive'], 16)
    assert hashlib.sha256(binary[start - BASE:end - BASE]).hexdigest() == record['retail_span_sha256']
    routine = {key: record[key] for key in (
        'module', 'load_address', 'name', 'address', 'end_exclusive',
        'retail_span_sha256', 'comparison_level', 'limitations')}
    routine.update({
        'source': SOURCE_PATH, 'verification': REPORT_PATH,
        'status': f"Indexed dispatcher matched retail in {record['cases_passed']} complete RAM/CPU comparisons",
    })
    existing = next((item for item in registry['routines']
                     if item.get('module') == routine['module'] and int(item['address'], 16) == start), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)
    progress[routine['name']] = {
        'verification': REPORT_PATH, 'cases': record['cases_passed'],
        'comparison_level': record['comparison_level'],
        'retail_span_sha256': record['retail_span_sha256'],
        'source_sha256': source_hash, 'scope': report['scope'],
        'limitations': record['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered two indexed dispatchers; {len(registry['routines'])} routines total")
