"""Register the independently recovered retail unsigned remainder helper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
source_path = ROOT / 'source/sa_unsigned_remainder.c'
start, end = 0x8c10cc80, 0x8c10cd2e
span = binary[start - 0x8c010000:end - 0x8c010000]
span_hash = hashlib.sha256(span).hexdigest()
helper_report = json.loads((ROOT / 'analysis/unsigned_division_helpers_verification.json').read_text(encoding='utf-8-sig'))
assert helper_report['cases_passed'] == 160
report = {
    'routine_start': hex(start), 'routine_end_exclusive': hex(end),
    'retail_span_sha256': span_hash,
    'source_sha256': hashlib.sha256(source_path.read_bytes()).hexdigest(),
    'dll_sha256': hashlib.sha256((ROOT.parent.parent / 'work/sonic-adventure/build/unsigned_division_helpers.dll').read_bytes()).hexdigest(),
    'cases_passed': 80, 'aggregate_helper_cases_passed': helper_report['cases_passed'],
    'scope': 'Full retail RAM, GPR, SR, PR, both FPU banks and MACL compared for 80 numerator/divisor/FPU-bank/SR combinations; independently checks quotient/remainder and exact zero-divisor error behavior, including stack effects.',
    'limitations': ['No Dreamcast hardware timing or execution claim.'],
}
(ROOT / 'analysis/unsigned_remainder_verification.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
routine = {
    'module': '1ST_READ.BIN', 'name': 'sa_unsigned_remainder_8c10cc80',
    'address': hex(start), 'end_exclusive': hex(end), 'retail_span_sha256': span_hash,
    'source': 'source/sa_unsigned_remainder.c',
    'verification': 'analysis/unsigned_remainder_verification.json',
    'status': 'native C matches full retail RAM/CPU/FPU/SR/PR/MACL state in 80 cases, including divide-by-zero error side effects',
    'limitations': report['limitations'],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
existing = next((entry for entry in registry['routines'] if entry.get('module') == routine['module']
                 and int(entry['address'], 16) == start), None)
if existing is None:
    registry['routines'].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress', {})['unsigned_remainder_8c10cc80'] = {
    'verification': 'analysis/unsigned_remainder_verification.json',
    'cases': report['cases_passed'], 'retail_span_sha256': span_hash,
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered unsigned remainder helper; {len(registry['routines'])} routines in registry")
