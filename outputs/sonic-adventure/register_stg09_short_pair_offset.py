"""Register the full-state-verified STG09 short-pair offset routine."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/STG09.BIN/short_pair_offset_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 1 and report['total_abi_cases'] == 160
item = report['routines'][0]
binary = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
assert hashlib.sha256(binary).hexdigest() == item['module_sha256']
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - int(item['load_address'], 16)
assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source_path = ROOT / 'source/stg09_short_pair_offset.c'
assert hashlib.sha256(source_path.read_bytes()).hexdigest() == report['source_sha256']
routine = {
    'module': 'STG09.BIN', 'load_address': item['load_address'],
    'name': 'sa_stg09_offset_short_pairs_0c90bc18',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/stg09_short_pair_offset.c',
    'verification': 'analysis/common-overlays/STG09.BIN/short_pair_offset_verification.json',
    'status': f"STG09 retail execution matched in {item['cases_passed']} complete CPU-state and RAM cases",
    'comparison_level': report['comparison_level'],
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
status.setdefault('verified_progress', {})['stg09_short_pair_offset_0c90bc18'] = {
    'verification': routine['verification'], 'cases': item['cases_passed'],
    'comparison_level': routine['comparison_level'],
    'module_sha256': item['module_sha256'], 'retail_span_sha256': item['retail_span_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered STG09 short-pair offset; {len(registry['routines'])} routines total")
