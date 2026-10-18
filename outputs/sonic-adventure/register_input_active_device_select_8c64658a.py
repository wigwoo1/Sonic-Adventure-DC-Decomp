"""Register the verified active-device selection routine."""
import hashlib
import json

from verify_startup import ROOT


PROJECT = ROOT / 'outputs/sonic-adventure'
NAME = 'sa_input_active_device_select_8c64658a'
ADDRESS = 0x8C64658A
END = 0x8C646654
SOURCE_PATH = f'source/{NAME}.c'
REPORT_PATH = 'analysis/input_active_device_select_8c64658a_verification.json'

report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 256
assert report['verified_entry'] == hex(ADDRESS)
assert report['end_exclusive'] == hex(END)
assert report['indexed_instruction_bytes'] == 168

binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
span = binary[ADDRESS - 0x8C010000:END - 0x8C010000]
assert len(span) == report['retail_span_bytes']
assert hashlib.sha256(span).hexdigest() == report['retail_span_sha256']
source = PROJECT / SOURCE_PATH
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': '1ST_READ.BIN',
    'load_address': hex(0x8C010000),
    'name': NAME,
    'address': hex(ADDRESS),
    'end_exclusive': hex(END),
    'indexed_instruction_bytes': report['indexed_instruction_bytes'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source': SOURCE_PATH,
    'verification': REPORT_PATH,
    'status': 'Active-device selection matched retail execution in 256 complete state comparisons',
    'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/PC/FPU/FPUL/MACL state and callback-mediated helper behavior against retail SH-4 execution.',
    'limitations': [
        'The external information callback is a matching synthetic fixture in native and retail-interpreter runs.',
        'This verifies the 168-byte indexed routine and its 202-byte address span, including embedded literal data; it does not verify the 334-byte service cleanup routine or full game.',
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
