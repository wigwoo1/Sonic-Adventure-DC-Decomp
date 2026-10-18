"""Register the verified B_CHAOS4 list callback updater."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/B_CHAOS4.BIN/list_callback_update_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 1 and report['total_abi_cases'] == 128
item = report['routines'][0]
overlay = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
main_binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dependencies = {entry['module']: entry for entry in report['dependencies']}
assert hashlib.sha256(overlay).hexdigest() == dependencies['B_CHAOS4.BIN']['sha256']
assert hashlib.sha256(main_binary).hexdigest() == dependencies['1ST_READ.BIN']['sha256']
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - int(item['load_address'], 16)
assert hashlib.sha256(overlay[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source = ROOT / 'source/b_chaos4_list_callback_update.c'
helper_source = ROOT / 'source/sa_signed_division.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
assert hashlib.sha256(helper_source.read_bytes()).hexdigest() == report['helper_source_sha256']

routine = {
    'module': item['module'], 'load_address': item['load_address'],
    'name': 'sa_b_chaos4_list_callback_update_0c900aa4',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/b_chaos4_list_callback_update.c',
    'verification': 'analysis/common-overlays/B_CHAOS4.BIN/list_callback_update_verification.json',
    'status': f"B_CHAOS4 list updater matched in {item['cases_passed']} RAM and ABI cases",
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
    'verified_dependencies': [{'module': '1ST_READ.BIN',
                               'address': dependencies['B_CHAOS4.BIN']['callback_target'],
                               'source': dependencies['1ST_READ.BIN']['callback_source'],
                               'source_sha256': report['helper_source_sha256']}],
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
progress['b_chaos4_list_callback_update_0c900aa4'] = {
    'verification': routine['verification'], 'cases': item['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': item['retail_span_sha256'],
    'helper_source_sha256': report['helper_source_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered B_CHAOS4 list updater; {len(registry['routines'])} routines total")
