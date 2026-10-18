import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs' / 'sonic-adventure'
REPORT_PATH = 'analysis/sound_work_callback_thunks_verification.json'
SOURCE_PATH = 'source/sa_sound_work_callback_thunks.c'
report = json.loads((PROJECT / REPORT_PATH).read_text(encoding='utf-8'))
assert report['cases_passed'] == 512

binary = (ROOT / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
registry_path = PROJECT / 'analysis' / 'recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
source_hash = hashlib.sha256((PROJECT / SOURCE_PATH).read_bytes()).hexdigest()
assert source_hash == report['source_sha256']

for evidence in report['routines']:
    address = int(evidence['address'], 16)
    end = int(evidence['end_exclusive'], 16)
    span = binary[address - 0x8C010000:end - 0x8C010000]
    assert hashlib.sha256(span).hexdigest() == evidence['retail_body_sha256']
    routine = {
        'module': '1ST_READ.BIN', 'load_address': hex(0x8C010000),
        'name': evidence['name'], 'address': evidence['address'],
        'end_exclusive': evidence['end_exclusive'],
        'indexed_instruction_bytes': evidence['indexed_code_bytes_verified'],
        'ghidra_draft_span_bytes': 16,
        'instruction_body_ranges': [{'start': evidence['address'], 'end_exclusive': evidence['end_exclusive']}],
        'retail_span_sha256': evidence['retail_body_sha256'],
        'source': SOURCE_PATH, 'source_sha256': source_hash,
        'verification': REPORT_PATH,
        'native_build_script': 'build_sound_work_callback_thunks.ps1',
        'verifier_script': 'verify_sound_work_callback_thunks.py',
        'status': 'Sound-work callback tail thunk matched 128 complete RAM/CPU-state cases',
        'comparison_level': report['comparison_level'],
        'limitations': report['limitations'],
        'callback_literal_slot': evidence['literal_slot'],
    }
    prior = next((item for item in registry['routines']
                  if item.get('module') == routine['module']
                  and int(item['address'], 16) == address), None)
    if prior is None:
        registry['routines'].append(routine)
    else:
        prior.update(routine)

registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
for evidence in report['routines']:
    progress[evidence['name']] = {
        'verification': REPORT_PATH, 'cases': 128,
        'comparison_level': report['comparison_level'],
        'source_sha256': source_hash, 'scope': report['scope'],
        'limitations': report['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered four sound-work callback thunks; {len(registry["routines"])} routines total')
