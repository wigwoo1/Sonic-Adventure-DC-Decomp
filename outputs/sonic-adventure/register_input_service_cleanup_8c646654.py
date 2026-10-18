"""Register the compared input-service cleanup routine and dispatch wrapper."""
import hashlib
import json

from verify_startup import ROOT


PROJECT = ROOT / 'outputs/sonic-adventure'
NAME = 'sa_input_service_cleanup_8c646654'
ADDRESS = 0x8C646654
END = 0x8C6467F0
SOURCE_PATH = f'source/{NAME}.c'
REPORT_PATH = 'analysis/input_service_cleanup_8c646654_verification.json'

report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 456
assert report['negative_exit_cases'] == 56
assert report['active_publication_cases'] == 8
assert report['callback_table_cases'] == 384
assert report['verified_entry'] == hex(ADDRESS)
assert report['end_exclusive'] == hex(END)
assert report['indexed_instruction_bytes'] == 334

binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
span = binary[ADDRESS - 0x8C010000:END - 0x8C010000]
assert len(span) == report['retail_span_bytes'] == END - ADDRESS
assert hashlib.sha256(span).hexdigest() == report['retail_span_sha256']
source = PROJECT / SOURCE_PATH
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
dispatcher = PROJECT / 'source/input_service_cleanup_native.c'
assert hashlib.sha256(dispatcher.read_bytes()).hexdigest() == report['dispatcher_source_sha256']
native_dll = ROOT / 'work/sonic-adventure/build/input_service_cleanup_native.dll'
assert hashlib.sha256(native_dll.read_bytes()).hexdigest() == report['native_dispatcher_sha256']

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
    'native_dispatcher_source': 'source/input_service_cleanup_native.c',
    'native_build_script': 'build_input_service_cleanup_native.ps1',
    'verifier_script': 'verify_input_service_cleanup_8c646654.py',
    'status': 'Cleanup matched retail SH-4 execution in 456 full-state comparisons across error, success, publication, and all callback-table slots',
    'comparison_level': 'Complete 16 MiB RAM, all GPR/SR/PR/PC/FPU/FPUL/MACL state and callback trace compared against retail execution.',
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
    'comparison_level': routine['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'],
    'scope': report['scope'],
    'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered {NAME} at {hex(ADDRESS)}; {len(registry["routines"])} routines total')
