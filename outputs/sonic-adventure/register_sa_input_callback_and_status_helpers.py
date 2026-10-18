import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE = 0x8C010000
SOURCE_PATH = 'source/sa_input_callback_and_status_helpers.c'
SPECS = [
    ('sa_input_request_completion_thunk_8c646884', 0x8C646884, 6, 'Request-completion callback tail thunk matched retail CPU state.'),
    ('sa_input_device_busy_thunk_8c646b36', 0x8C646B36, 6, 'Input-device busy callback tail thunk matched retail CPU state.'),
    ('sa_input_device_request_thunk_8c646b3c', 0x8C646B3C, 6, 'Input-device request callback tail thunk matched retail CPU state.'),
    ('sa_input_device_state_get_8c646eda', 0x8C646EDA, 6, 'Input-device state getter matched retail RAM and CPU state.'),
    ('sa_input_status_table_store_8c646eee', 0x8C646EEE, 16, 'Bounds-checked input status table writer matched retail RAM and CPU state.'),
]
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
source = PROJECT / SOURCE_PATH

for name, address, length, description in SPECS:
    report_path = f'analysis/{name}_verification.json'
    report = json.loads((PROJECT / report_path).read_text(encoding='utf-8'))
    assert report['cases_passed'] == 128
    assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
    assert hashlib.sha256(binary[address - BASE:address - BASE + length]).hexdigest() == report['retail_span_sha256']
    assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
    routine = {
        'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'name': name,
        'address': hex(address), 'end_exclusive': hex(address + length),
        'retail_span_sha256': report['retail_span_sha256'], 'source': SOURCE_PATH,
        'verification': report_path, 'status': description,
        'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
    }
    existing = next((entry for entry in registry['routines']
                     if entry.get('module') == routine['module'] and int(entry['address'], 16) == address), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)
    progress[name] = {
        'verification': report_path, 'cases': report['cases_passed'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': report['retail_span_sha256'],
        'source_sha256': report['source_sha256'], 'scope': report['scope'],
        'limitations': report['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered five input helpers; {len(registry['routines'])} routines total")