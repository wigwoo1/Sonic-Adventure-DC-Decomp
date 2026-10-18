"""Register the verified B_CHAOS4 callback wrapper and main-image target."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/B_CHAOS4.BIN/task_callback_candidate_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 2 and report['total_abi_cases'] == 128
overlay = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
main_binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dependencies = {entry['module']: entry for entry in report['dependencies']}
assert hashlib.sha256(overlay).hexdigest() == dependencies['B_CHAOS4.BIN']['sha256']
assert hashlib.sha256(main_binary).hexdigest() == dependencies['1ST_READ.BIN']['sha256']
source = ROOT / 'source/b_chaos4_callback_candidate.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

routines = []
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    if item['module'] == 'B_CHAOS4.BIN':
        binary, load_address = overlay, int(item['load_address'], 16)
    else:
        binary, load_address = main_binary, int(item['load_address'], 16)
    offset = start - load_address
    assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
    routines.append({
        'module': item['module'], 'load_address': item['load_address'],
        'name': ('sa_b_chaos4_callback_candidate_c90701a'
                 if item['module'] == 'B_CHAOS4.BIN'
                 else 'sa_callback_candidate_8c052a5e'),
        'address': hex(start), 'end_exclusive': hex(end),
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/b_chaos4_callback_candidate.c',
        'verification': 'analysis/common-overlays/B_CHAOS4.BIN/task_callback_candidate_verification.json',
        'status': f"{item['name']} matched in {item['cases_passed']} direct/composed cases",
        'comparison_level': item['comparison_level'],
        'limitations': report['limitations'],
        'verified_dependencies': ([{'module': '1ST_READ.BIN',
                                     'address': dependencies['B_CHAOS4.BIN']['callback_target'],
                                     'source_sha256': dependencies['1ST_READ.BIN']['sha256']}]
                                  if item['module'] == 'B_CHAOS4.BIN' else []),
    })

registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for routine in routines:
    existing = next((entry for entry in registry['routines']
                     if entry.get('module') == routine['module'] and
                     int(entry['address'], 16) == int(routine['address'], 16)), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
for routine in routines:
    progress[routine['name']] = {
        'verification': routine['verification'], 'cases': report['total_abi_cases'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': routine['retail_span_sha256'],
        'source_sha256': report['source_sha256'],
        'scope': report['scope'], 'limitations': report['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered callback wrapper and main callback; {len(registry['routines'])} routines total")
