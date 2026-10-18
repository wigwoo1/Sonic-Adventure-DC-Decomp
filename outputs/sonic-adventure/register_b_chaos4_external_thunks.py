"""Register verified B_CHAOS4 external tail thunks."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/B_CHAOS4.BIN/external_thunks_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 3 and report['total_abi_cases'] == 384
overlay = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
assert hashlib.sha256(overlay).hexdigest() == report['dependencies'][0]['sha256']
source = ROOT / 'source/b_chaos4_external_thunks.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']

registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for item in report['routines']:
    start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
    offset = start - int(item['load_address'], 16)
    assert hashlib.sha256(overlay[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
    routine = {
        'module': item['module'], 'load_address': item['load_address'],
        'name': item['native_symbol'],
        'address': hex(start), 'end_exclusive': hex(end),
        'retail_span_sha256': item['retail_span_sha256'],
        'source': 'source/b_chaos4_external_thunks.c',
        'verification': 'analysis/common-overlays/B_CHAOS4.BIN/external_thunks_verification.json',
        'status': f"B_CHAOS4 tail-jump handoff matched in {item['cases_passed']} full-RAM and CPU-state cases",
        'comparison_level': report['comparison_level'],
        'limitations': report['limitations'],
    }
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
for item in report['routines']:
    key = f"b_chaos4_{item['name']}_{int(item['address'], 16):08x}"
    progress[key] = {
        'verification': 'analysis/common-overlays/B_CHAOS4.BIN/external_thunks_verification.json',
        'cases': item['cases_passed'], 'comparison_level': report['comparison_level'],
        'retail_span_sha256': item['retail_span_sha256'],
        'scope': report['scope'], 'limitations': report['limitations'],
        'external_target': item['external_target'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {len(report['routines'])} B_CHAOS4 external thunks; {len(registry['routines'])} routines total")
