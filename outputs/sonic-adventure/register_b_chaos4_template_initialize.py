"""Register the verified B_CHAOS4 cross-module template initializer."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis/common-overlays/B_CHAOS4.BIN/template_initialize_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
assert report['functions_verified'] == 1 and report['total_abi_cases'] == 128
item = report['routines'][0]
overlay = (ROOT.parent.parent / 'work/sonic-adventure/decoded/SONICADV/B_CHAOS4.BIN').read_bytes()
main_binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
deps = {entry['module']: entry for entry in report['dependencies']}
assert hashlib.sha256(overlay).hexdigest() == deps['B_CHAOS4.BIN']['sha256']
assert hashlib.sha256(main_binary).hexdigest() == deps['1ST_READ.BIN']['sha256']
template = bytes.fromhex(deps['1ST_READ.BIN']['template_fixture_hex'])
assert len(template) == deps['1ST_READ.BIN']['template_bytes']
assert hashlib.sha256(template).hexdigest() == deps['1ST_READ.BIN']['template_sha256']
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - int(item['load_address'], 16)
assert hashlib.sha256(overlay[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source = ROOT / 'source/b_chaos4_template_initialize.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
routine = {
    'module': 'B_CHAOS4.BIN', 'load_address': item['load_address'],
    'name': 'sa_b_chaos4_template_initialize_0c900348',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/b_chaos4_template_initialize.c',
    'verification': 'analysis/common-overlays/B_CHAOS4.BIN/template_initialize_verification.json',
    'status': f"B_CHAOS4 retail initialization matched in {item['cases_passed']} full-RAM and ABI cases",
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
    'verified_dependencies': [{'module': '1ST_READ.BIN', 'address': deps['1ST_READ.BIN']['template_address'],
                               'bytes': deps['1ST_READ.BIN']['template_bytes'],
                               'sha256': deps['1ST_READ.BIN']['template_sha256']}],
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
status.setdefault('verified_progress', {})['b_chaos4_template_initialize_0c900348'] = {
    'verification': routine['verification'], 'cases': item['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': item['retail_span_sha256'],
    'template_sha256': deps['1ST_READ.BIN']['template_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered B_CHAOS4 template initializer; {len(registry['routines'])} routines total")
