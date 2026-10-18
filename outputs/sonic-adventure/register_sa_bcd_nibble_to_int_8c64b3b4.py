"""Register the validated packed-decimal conversion helper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/sa_bcd_nibble_to_int_8c64b3b4_verification.json').read_text())
assert report['cases_passed'] == 128
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
address, end = int(report['address'], 16), int(report['end_exclusive'], 16)
assert hashlib.sha256(binary[address - 0x8c010000:end - 0x8c010000]).hexdigest() == report['retail_span_sha256']
source = ROOT / 'source/sa_bcd_nibble_to_int_8c64b3b4.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
routine = {
    'module': '1ST_READ.BIN', 'name': report['name'], 'address': hex(address),
    'end_exclusive': hex(end), 'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/sa_bcd_nibble_to_int_8c64b3b4.c',
    'verification': 'analysis/sa_bcd_nibble_to_int_8c64b3b4_verification.json',
    'status': 'All twelve retail instructions matched in 128 full RAM/CPU cases',
    'limitations': report['limitations'],
}
found = next((entry for entry in registry['routines']
              if entry.get('module') == routine['module'] and
              int(entry['address'], 16) == address), None)
if found is None:
    registry['routines'].append(routine)
else:
    found.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress', {})['packed_decimal_helper_8c64b3b4'] = {
    'verification': 'analysis/sa_bcd_nibble_to_int_8c64b3b4_verification.json',
    'cases': report['cases_passed'], 'retail_span_sha256': report['retail_span_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {routine['name']}; {len(registry['routines'])} routines total")
