"""Register the verified B_CHAOS4 seven-record flag-table callback."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/B_CHAOS4.BIN/flag_table_callback_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 1 and report['total_abi_cases'] == 128
item = report['routines'][0]
overlay = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
dependencies = report['dependencies'][0]
assert hashlib.sha256(overlay).hexdigest() == dependencies['sha256']
fixture = bytes.fromhex(dependencies['fixture_hex'])
assert len(fixture) == dependencies['fixture_bytes']
assert hashlib.sha256(fixture).hexdigest() == dependencies['fixture_sha256']
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - int(item['load_address'], 16)
assert hashlib.sha256(overlay[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source = ROOT / 'source/b_chaos4_flag_table_callback.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routine = {
    'module': item['module'], 'load_address': item['load_address'],
    'name': 'sa_b_chaos4_flag_table_callback_0c900280',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/b_chaos4_flag_table_callback.c',
    'verification': 'analysis/common-overlays/B_CHAOS4.BIN/flag_table_callback_verification.json',
    'status': f"B_CHAOS4 flag-table callback matched in {item['cases_passed']} RAM and ABI cases",
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
    'verified_dependencies': [{'module': 'B_CHAOS4.BIN',
                               'address': dependencies['table_pointer_literal'],
                               'target': dependencies['table_pointer'],
                               'fixture_sha256': dependencies['fixture_sha256']}],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
existing = next((entry for entry in registry['routines']
                 if entry.get('module') == item['module'] and
                 int(entry['address'], 16) == start), None)
if existing is None:
    registry['routines'].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress['b_chaos4_flag_table_callback_0c900280'] = {
    'verification': routine['verification'], 'cases': item['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': item['retail_span_sha256'],
    'fixture_sha256': dependencies['fixture_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered B_CHAOS4 flag-table callback; {len(registry['routines'])} routines total")
