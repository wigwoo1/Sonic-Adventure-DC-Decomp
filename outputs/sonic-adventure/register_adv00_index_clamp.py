"""Register the retail-equivalent ADV00 signed index clamp."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/ADV00.BIN/index_clamp_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 1 and report['total_full_state_cases'] >= 100
item = report['routines'][0]
binary = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/ADV00.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == item['module_sha256']
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - int(item['load_address'], 16)
assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source = ROOT / 'source/adv00_index_clamp.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
routine = {
    'module': 'ADV00.BIN', 'load_address': item['load_address'],
    'name': 'sa_adv00_index_clamp_0c90ffbe',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/adv00_index_clamp.c',
    'verification': 'analysis/common-overlays/ADV00.BIN/index_clamp_verification.json',
    'status': f"ADV00 retail execution matched in {item['cases_passed']} complete CPU-state and RAM cases",
    'comparison_level': 'complete SH-4 CPU state and all 16 MiB RAM',
    'limitations': report['limitations'],
}
registry_path = ROOT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
found = next((entry for entry in registry['routines']
              if entry.get('module') == routine['module'] and
              int(entry['address'], 16) == start), None)
if found is None:
    registry['routines'].append(routine)
else:
    found.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress', {})['adv00_index_clamp_0c90ffbe'] = {
    'verification': routine['verification'], 'cases': item['cases_passed'],
    'comparison_level': routine['comparison_level'],
    'module_sha256': item['module_sha256'],
    'retail_span_sha256': item['retail_span_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered ADV00 index clamp; {len(registry['routines'])} routines total")
