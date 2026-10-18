import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
REPORT_PATH = 'analysis/stream_request_open_prepare_8c65bf42_verification.json'
SOURCE_PATH = 'source/sa_stream_request_open_prepare_8c65bf42.c'
BUILD_SCRIPT = 'build_stream_request_open_prepare_8c65bf42.ps1'
VERIFY_SCRIPT = 'verify_stream_request_open_prepare_8c65bf42.py'
report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 192

binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis' / 'recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
source_hash = hashlib.sha256((PROJECT / SOURCE_PATH).read_bytes()).hexdigest()
routines = [
    {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'name': 'sa_stream_request_open_prepare_8c65bf42',
        'address': report['address'], 'end_exclusive': report['end_exclusive'],
        'indexed_instruction_bytes': report['indexed_instruction_bytes'],
        'instruction_body_ranges': [{'start': report['address'], 'end_exclusive': report['end_exclusive']}],
        'retail_span_sha256': report['retail_span_sha256'],
        'source': SOURCE_PATH, 'source_sha256': source_hash,
        'verification': REPORT_PATH, 'native_build_script': BUILD_SCRIPT,
        'verifier_script': VERIFY_SCRIPT,
        'status': 'Stream-open preparation matched 128 complete RAM/CPU-state cases',
        'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
    },
    {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'name': 'sa_stream_request_query_8c600ec6',
        'address': report['related_callback']['address'],
        'end_exclusive': report['related_callback']['end_exclusive'],
        'indexed_instruction_bytes': report['related_callback']['indexed_instruction_bytes'],
        'instruction_body_ranges': [{'start': report['related_callback']['address'],
                                     'end_exclusive': report['related_callback']['end_exclusive']}],
        'retail_span_sha256': report['callback_span_sha256'],
        'source': SOURCE_PATH, 'source_sha256': source_hash,
        'verification': REPORT_PATH, 'native_build_script': BUILD_SCRIPT,
        'verifier_script': VERIFY_SCRIPT,
        'status': 'Request query callback matched 64 complete RAM/CPU-state cases',
        'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
    },
]
for routine in routines:
    start = int(routine['address'], 16) - 0x8C010000
    end = int(routine['end_exclusive'], 16) - 0x8C010000
    assert hashlib.sha256(binary[start:end]).hexdigest() == routine['retail_span_sha256']
    prior = next((item for item in registry['routines']
                  if item.get('module') == routine['module']
                  and int(item['address'], 16) == int(routine['address'], 16)), None)
    if prior is None:
        registry['routines'].append(routine)
    else:
        prior.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
for name in ('sa_stream_request_open_prepare_8c65bf42', 'sa_stream_request_query_8c600ec6'):
    progress[name] = {
        'verification': REPORT_PATH, 'cases': report['cases_passed'],
        'comparison_level': report['comparison_level'],
        'source_sha256': source_hash, 'scope': report['scope'],
        'limitations': report['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered stream request wrapper and callback; {len(registry["routines"])} routines total')
