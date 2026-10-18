"""Register the recovered input-device wait/reset routine and its evidence."""
import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
NAME = 'sa_input_device_wait_reset_8c65f2a4'
ADDRESS = 0x8C65F2A4
END = 0x8C65F348
SOURCE_PATH = f'source/{NAME}.c'
REPORT_PATH = 'analysis/input_device_wait_reset_8c65f2a4_verification.json'

report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 4
assert report['address'] == hex(ADDRESS)
assert report['end_exclusive'] == hex(END)
assert report['indexed_instruction_bytes'] == END - ADDRESS == 164

binary_path = ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN'
binary = binary_path.read_bytes()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
span = binary[ADDRESS - 0x8C010000:END - 0x8C010000]
assert len(span) == END - ADDRESS
assert hashlib.sha256(span).hexdigest() == report['retail_span_sha256']
source = PROJECT / SOURCE_PATH
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': '1ST_READ.BIN',
    'load_address': hex(0x8C010000),
    'name': NAME,
    'address': hex(ADDRESS),
    'end_exclusive': hex(END),
    'indexed_instruction_bytes': END - ADDRESS,
    'retail_span_sha256': report['retail_span_sha256'],
    'source': SOURCE_PATH,
    'verification': REPORT_PATH,
    'native_build_script': 'build_input_device_wait_reset_8c65f2a4.ps1',
    'verifier_script': 'verify_input_device_wait_reset_8c65f2a4.py',
    'status': 'Wait/reset routine matched retail SH-4 execution in four full-state status and timeout scenarios',
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
