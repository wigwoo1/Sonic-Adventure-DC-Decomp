"""Register the verified initializer-call wrapper."""
import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
ADDRESS, LENGTH, BASE = 0x8c093b1e, 12, 0x8c010000
REPORT_PATH = 'analysis/sa_initializer_service_wrapper_8c093b1e_verification.json'
SOURCE_PATH = 'source/sa_initializer_service_wrapper_8c093b1e.c'
report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 128
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
assert hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest() == report['retail_span_sha256']
source = PROJECT / SOURCE_PATH
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'name': 'sa_initializer_service_wrapper_8c093b1e', 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': report['retail_span_sha256'],
    'source': SOURCE_PATH, 'verification': REPORT_PATH,
    'status': 'Initializer service wrapper matched retail RAM and CPU state through its call and return',
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
}
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
old = next((entry for entry in registry['routines']
            if entry.get('module') == routine['module'] and int(entry['address'], 16) == ADDRESS), None)
if old is None:
    registry['routines'].append(routine)
else:
    old.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[routine['name']] = {
    'verification': REPORT_PATH, 'cases': report['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'], 'scope': report['scope'],
    'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered initializer service wrapper; {len(registry['routines'])} routines total")
