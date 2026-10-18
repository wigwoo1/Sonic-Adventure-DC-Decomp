"""Register the retail-compared input response loop at 0x8c65f202."""
import hashlib
import json

from verify_startup import ROOT


PROJECT = ROOT / 'outputs/sonic-adventure'
NAME = 'sa_input_device_responses_process'
ADDRESS = 0x8C65F202
LENGTH = 130
SOURCE_PATH = f'source/{NAME}.c'
REPORT_PATH = 'analysis/input_device_dispatcher_discovery_verification.json'

report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 48
assert report['verified_entry'] == hex(ADDRESS)
assert report['retail_span_bytes'] == LENGTH

binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
span = binary[ADDRESS - 0x8C010000:ADDRESS - 0x8C010000 + LENGTH]
assert hashlib.sha256(span).hexdigest() == report['retail_span_sha256']
source = PROJECT / SOURCE_PATH
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': '1ST_READ.BIN',
    'load_address': hex(0x8C010000),
    'name': NAME,
    'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': report['retail_span_sha256'],
    'source': SOURCE_PATH,
    'verification': REPORT_PATH,
    'status': 'Response processing routine compared with retail execution in 48 connected discovery cases',
    'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/FPU/MACL and final register comparisons at the routine entry.',
    'limitations': [
        'This verifies the 130-byte response loop in connected synthetic-host discovery scenarios, not every callback branch or all input-service behavior.',
        'This does not verify the 334-byte input cleanup routine or full game.',
    ],
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
    'comparison_level': routine['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'],
    'scope': report['scope'],
    'limitations': routine['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered {NAME} at {hex(ADDRESS)}; {len(registry["routines"])} routines total')
