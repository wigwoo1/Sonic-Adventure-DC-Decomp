import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
BASE = 0x8C010000
SPECS = [
    ('sa_record_callback_arg_tail_dispatch_8c647bb8', 0x8C647BB8, 38,
     'source/sa_record_callback_tail_dispatchers_8c647bb8.c',
     'Indexed record callback dispatcher passes the stored argument in R7 and tail-transfers or returns.'),
    ('sa_record_callback_tail_dispatch_8c647bde', 0x8C647BDE, 34,
     'source/sa_record_callback_tail_dispatchers_8c647bb8.c',
     'Indexed record callback dispatcher tail-transfers or returns when its callback is empty.'),
]
binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})

for name, address, length, source_path, description in SPECS:
    report_path = f'analysis/{name}_verification.json'
    report = json.loads((PROJECT / report_path).read_text(encoding='utf-8'))
    assert report['cases_passed'] == 158
    assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
    assert hashlib.sha256(binary[address - BASE:address - BASE + length]).hexdigest() == report['retail_span_sha256']
    source = PROJECT / source_path
    assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
    routine = {
        'module': '1ST_READ.BIN', 'load_address': hex(BASE), 'name': name,
        'address': hex(address), 'end_exclusive': hex(address + length),
        'retail_span_sha256': report['retail_span_sha256'], 'source': source_path,
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
print(f"Registered both record callback dispatchers; {len(registry['routines'])} routines total")