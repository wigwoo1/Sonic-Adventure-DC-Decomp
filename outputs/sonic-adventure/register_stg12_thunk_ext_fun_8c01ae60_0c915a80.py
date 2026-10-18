import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis' / 'stg12_thunk_ext_fun_8c01ae60_0c915a80_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['cases_passed'] == 128
registry_path = ROOT / 'analysis' / 'recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
primary = (ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
assert hashlib.sha256(primary).hexdigest() == registry['binary_sha256']
module_path = ROOT.parent.parent / 'work' / 'sonic-adventure' / 'decoded' / 'SONICADV' / 'STG12.BIN'
module = module_path.read_bytes()
assert hashlib.sha256(module).hexdigest() == report['decoded_module_sha256']
scope = json.loads((ROOT / 'analysis' / 'module_scope.json').read_text(encoding='utf-8'))
entry = next(item for item in scope['unclassified_binaries'] if item['source'] == 'SONICADV/STG12.PRS')
assert hashlib.sha256(module).hexdigest() == entry['sha256']
start, end = int(report['address'], 16), int(report['end_exclusive'], 16)
offset = start - int(report['load_address'], 16)
assert hashlib.sha256(module[offset:offset + end - start]).hexdigest() == report['retail_span_sha256']
source = ROOT / 'source' / 'sa_stg12_thunk_ext_fun_8c01ae60_0c915a80.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
routine = {
    'module': report['module'], 'load_address': report['load_address'],
    'name': 'sa_stg12_thunk_ext_fun_8c01ae60_0c915a80',
    'address': report['address'], 'end_exclusive': report['end_exclusive'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/sa_stg12_thunk_ext_fun_8c01ae60_0c915a80.c',
    'verification': 'analysis/stg12_thunk_ext_fun_8c01ae60_0c915a80_verification.json',
    'status': 'STG12 literal-backed external-function thunk matched in 128 complete RAM/CPU-state cases',
    'limitations': report['limitations'],
}
existing = next((item for item in registry['routines'] if item.get('module') == routine['module'] and item.get('address', '').lower().removeprefix('0x') == report['address'].lower().removeprefix('0x')), None)
if existing:
    assert existing == routine, f'Conflicting registration: {existing}'
else:
    registry['routines'].append(routine)
    registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
print(f"registered {routine['name']}")
