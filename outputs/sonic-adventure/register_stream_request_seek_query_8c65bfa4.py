import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
REPORT = 'analysis/stream_request_seek_query_8c65bfa4_verification.json'
SOURCE = 'source/sa_stream_request_seek_query_8c65bfa4.c'
report = json.loads((PROJECT / REPORT).read_text(encoding='utf-8'))
assert (report['cases_passed'], report['callee_entries_compared'],
        report['uninstrumented_cases_passed'], report['unsupported_target_cases_passed']) == (160, 320, 160, 2)

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

assert digest(ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN') == report['binary_sha256']
assert digest(ROOT / 'work/sonic-adventure/build/stream_request_seek_query_8c65bfa4.dll') == report['dll_sha256']
assert digest(PROJECT / 'verify_startup.py') == report['interpreter_sha256']
assert digest(PROJECT / 'verify_stream_request_seek_query_8c65bfa4.py') == report['verifier_sha256']
for source, expected in report['sources'].items():
    assert digest(PROJECT / source) == expected, source

registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256'] == report['binary_sha256']
name = 'sa_stream_request_seek_query_8c65bfa4'
routine = {
    'module': '1ST_READ.BIN', 'load_address': report['load_address'],
    'name': name, 'address': report['address'], 'end_exclusive': report['end_exclusive'],
    'indexed_instruction_bytes': 24,
    'instruction_body_ranges': [{'start': report['address'], 'end_exclusive': report['end_exclusive']}],
    'retail_span_sha256': report['retail_span_sha256'],
    'source': SOURCE, 'source_sha256': report['source_sha256'], 'verification': REPORT,
    'native_build_script': 'build_stream_request_seek_query_8c65bfa4.ps1',
    'verifier_script': 'verify_stream_request_seek_query_8c65bfa4.py',
    'status': 'Original seek/query caller matched 160 cases and 320 native callee-entry comparisons',
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
}
prior = next((item for item in registry['routines']
              if item.get('module') == '1ST_READ.BIN' and int(item['address'], 16) == 0x8C65BFA4), None)
if prior is None:
    registry['routines'].append(routine)
else:
    prior.update(routine)
for item in registry['routines']:
    if item.get('module') == '1ST_READ.BIN' and int(item['address'], 16) in (0x8C600E7A, 0x8C600EC6):
        reports = item.setdefault('additional_verification', [])
        if REPORT not in reports:
            reports.append(REPORT)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[name] = {'verification': REPORT, 'cases': 160, 'callee_entries_compared': 320,
                  'comparison_level': report['comparison_level'], 'limitations': report['limitations']}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered {name}; {len(registry["routines"])} routine entries')
