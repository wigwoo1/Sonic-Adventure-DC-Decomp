"""Register the differentially verified AL_MAIN secondary callback-field setter."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/AL_MAIN.BIN/set_secondary_callback_fields_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 1 and report['total_abi_cases'] == 128
item = report['routines'][0]
overlay = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes()
main_binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dependencies = {entry['module']: entry for entry in report['dependencies']}
assert hashlib.sha256(overlay).hexdigest() == dependencies['AL_MAIN.BIN']['sha256']
assert hashlib.sha256(main_binary).hexdigest() == dependencies['1ST_READ.BIN']['sha256']
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - int(item['load_address'], 16)
assert hashlib.sha256(overlay[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source = ROOT / 'source/al_main_set_secondary_callback_fields.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': item['module'], 'load_address': item['load_address'],
    'name': 'sa_al_main_set_secondary_callback_fields_0c908530',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/al_main_set_secondary_callback_fields.c',
    'verification': 'analysis/common-overlays/AL_MAIN.BIN/set_secondary_callback_fields_verification.json',
    'status': f"AL_MAIN secondary callback-field setter matched in {item['cases_passed']} full CPU/RAM cases",
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
existing = next((entry for entry in registry['routines']
                 if entry.get('module') == item['module'] and int(entry['address'], 16) == start), None)
if existing is None:
    registry['routines'].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[routine['name']] = {
    'verification': routine['verification'], 'cases': item['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': item['retail_span_sha256'],
    'source_sha256': report['source_sha256'], 'scope': report['scope'],
    'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered AL_MAIN secondary callback-field setter; {len(registry['routines'])} routines total")
