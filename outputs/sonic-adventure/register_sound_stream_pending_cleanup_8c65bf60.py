import hashlib
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
REPORT = 'analysis/sound_stream_pending_cleanup_8c65bf60_verification.json'
SOURCE = 'source/sa_sound_stream_pending_cleanup_8c65bf60.c'
report = json.loads((PROJECT / REPORT).read_text(encoding='utf-8'))
assert (report['cases_passed'], report['service_entries_compared'],
        report['timer_reads_compared'], report['host_rejections_passed']) == (96, 160, 80, 4)

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

binary = ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN'
assert digest(binary) == report['binary_sha256']
assert digest(ROOT / 'work/sonic-adventure/build/sound_stream_pending_cleanup_8c65bf60.dll') == report['dll_sha256']
assert digest(PROJECT / 'verify_startup.py') == report['interpreter_sha256']
assert digest(PROJECT / 'verify_sound_stream_pending_cleanup_8c65bf60.py') == report['verifier_sha256']
for source, expected in report['sources'].items():
    assert digest(PROJECT / source) == expected, source
assert digest(PROJECT / SOURCE) == report['source_sha256']
retail_span = binary.read_bytes()[0x64BF60:0x64BFA4]
assert hashlib.sha256(retail_span).hexdigest() == report['retail_span_sha256']

registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert registry['binary_sha256'] == report['binary_sha256']
name = 'sa_sound_stream_pending_cleanup_8c65bf60'
routine = {
    'module': '1ST_READ.BIN', 'load_address': report['load_address'],
    'name': name, 'address': report['address'], 'end_exclusive': report['end_exclusive'],
    'indexed_instruction_bytes': 68,
    'instruction_body_ranges': [{'start': report['address'], 'end_exclusive': report['end_exclusive']}],
    'retail_span_sha256': report['retail_span_sha256'], 'source': SOURCE,
    'source_sha256': report['source_sha256'], 'verification': REPORT,
    'native_build_script': 'build_sound_stream_pending_cleanup_8c65bf60.ps1',
    'verifier_script': 'verify_sound_stream_pending_cleanup_8c65bf60.py',
    'status': 'Pending-I/O cleanup caller matched 96 cases with native timer/subtraction and explicit external abort service',
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
}
prior = next((item for item in registry['routines']
              if item.get('module') == '1ST_READ.BIN' and int(item['address'], 16) == 0x8C65BF60), None)
if prior is None:
    registry['routines'].append(routine)
else:
    prior.update(routine)
for item in registry['routines']:
    if item.get('module') == '1ST_READ.BIN' and int(item['address'], 16) in (0x8C60260E, 0x8C602618):
        evidence = item.setdefault('additional_verification', [])
        if REPORT not in evidence:
            evidence.append(REPORT)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[name] = {'verification': REPORT, 'cases': 96, 'service_entries_compared': 160,
                  'timer_reads_compared': 80, 'comparison_level': report['comparison_level'],
                  'limitations': report['limitations']}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered {name}; {len(registry["routines"])} routine entries')
