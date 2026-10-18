"""Register both exhaustively byte-tested input flag update helpers."""
import hashlib
import json

from verify_startup import ROOT

BASE = 0x8c010000
ROOT = ROOT / 'outputs/sonic-adventure'
report_path = ROOT / 'analysis/sa_input_flag_updates_8c0509b8_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['cases_passed'] == 512
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
source = ROOT / 'source/sa_input_flag_updates_8c0509b8.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
progress = json.loads((ROOT / 'project_status.json').read_text(encoding='utf-8-sig'))
verified = progress.setdefault('verified_progress', {})
for item in report['routines']:
    address, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    assert hashlib.sha256(binary[address - BASE:end - BASE]).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': item['module'], 'load_address': hex(BASE), 'name': item['name'],
        'address': item['address'], 'end_exclusive': item['end_exclusive'],
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/sa_input_flag_updates_8c0509b8.c',
        'verification': 'analysis/sa_input_flag_updates_8c0509b8_verification.json',
        'status': f"Input flag {item['name'].split('_')[1]} helper matched retail in {item['cases_passed']} cases",
        'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
    }
    existing = next((entry for entry in registry['routines']
                     if entry.get('module') == routine['module'] and int(entry['address'], 16) == address), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)
    verified[routine['name']] = {
        'verification': routine['verification'], 'cases': item['cases_passed'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': item['retail_span_sha256'],
        'source_sha256': report['source_sha256'], 'scope': report['scope'],
        'limitations': report['limitations'],
    }

registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = ROOT / 'project_status.json'
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(progress, indent=2) + '\n', encoding='utf-8')
print(f"Registered two input flag helpers; {len(registry['routines'])} routines total")
