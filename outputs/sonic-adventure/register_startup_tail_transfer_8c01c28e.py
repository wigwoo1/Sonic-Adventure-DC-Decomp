import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis' / 'startup_tail_transfer_8c01c28e_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['cases_passed'] == 128
binary = (ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
registry_path = ROOT / 'analysis' / 'recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
start, end = int(report['address'], 16), int(report['end_exclusive'], 16)
offset = start - int(report['load_address'], 16)
assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == report['retail_span_sha256']
source = ROOT / 'source' / 'sa_startup_tail_transfer_8c01c28e.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
routine = {
    'module': report['module'], 'load_address': report['load_address'],
    'name': 'sa_startup_tail_transfer_8c01c28e', 'address': report['address'],
    'end_exclusive': report['end_exclusive'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/sa_startup_tail_transfer_8c01c28e.c',
    'verification': 'analysis/startup_tail_transfer_8c01c28e_verification.json',
    'status': 'Startup tail-transfer wrapper matched in 128 complete RAM/CPU-state cases',
    'limitations': report['limitations'],
}
existing = next((item for item in registry['routines'] if item.get('address', '').lower().removeprefix('0x') == report['address'].lower().removeprefix('0x')), None)
if existing:
    assert existing == routine, f'Conflicting existing registration: {existing}'
else:
    registry['routines'].append(routine)
    registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
print(f"registered {routine['name']}")
