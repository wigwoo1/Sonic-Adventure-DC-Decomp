"""Register the differentially verified object mode/table updater."""
import hashlib
import json

from verify_startup import ROOT

ADDRESS, LENGTH, BASE = 0x8c01b3e0, 38, 0x8c010000
PROJECT = ROOT / 'outputs/sonic-adventure'
report_path = PROJECT / 'analysis/sa_update_object_mode_and_table_8c01b3e0_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['cases_passed'] == 512
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
assert hashlib.sha256(binary[ADDRESS - BASE:ADDRESS - BASE + LENGTH]).hexdigest() == report['retail_span_sha256']
source = PROJECT / 'source/sa_update_object_mode_and_table_8c01b3e0.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': '1ST_READ.BIN', 'load_address': hex(BASE),
    'name': 'sa_update_object_mode_and_table_8c01b3e0', 'address': hex(ADDRESS),
    'end_exclusive': hex(ADDRESS + LENGTH),
    'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/sa_update_object_mode_and_table_8c01b3e0.c',
    'verification': 'analysis/sa_update_object_mode_and_table_8c01b3e0_verification.json',
    'status': 'Object mode/table updater matched retail RAM and CPU state in all fixtures',
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
}
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
existing = next((entry for entry in registry['routines']
                 if entry.get('module') == routine['module'] and int(entry['address'], 16) == ADDRESS), None)
if existing is None:
    registry['routines'].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[routine['name']] = {
    'verification': routine['verification'], 'cases': report['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'], 'scope': report['scope'],
    'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered object mode/table helper at {hex(ADDRESS)}; {len(registry['routines'])} routines total")
