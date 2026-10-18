"""Register the verified 8c65f1ec helper with the module-specific inventory."""
import hashlib
import json

from verify_startup import ROOT


PROJECT = ROOT / 'outputs/sonic-adventure'
NAME = 'sa_input_device_submit_rearm'
ADDRESS = 0x8C65F1EC
LENGTH = 22
BASE = 0x8C010000
REPORT_PATH = 'analysis/input_device_submit_rearm_verification.json'
SOURCE_PATH = f'source/{NAME}.c'

report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 64
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
span = binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]
assert hashlib.sha256(span).hexdigest() == report['retail_span_sha256']
source = PROJECT / SOURCE_PATH
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': '1ST_READ.BIN',
    'load_address': hex(BASE),
    'name': NAME,
    'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': report['retail_span_sha256'],
    'source': SOURCE_PATH,
    'verification': REPORT_PATH,
    'status': 'Input device submit-rearm helper matched retail RAM and CPU/FPU state in 64 cases',
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
}
old = next((entry for entry in registry['routines']
            if entry.get('module') == routine['module']
            and int(entry['address'], 16) == ADDRESS), None)
if old is None:
    registry['routines'].append(routine)
else:
    old.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[NAME] = {
    'verification': REPORT_PATH,
    'cases': report['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'],
    'scope': report['scope'],
    'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered {NAME} at {hex(ADDRESS)}; {len(registry["routines"])} routines total')
