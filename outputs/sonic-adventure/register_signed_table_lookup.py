"""Register the full-state-verified signed-byte table lookup."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report = json.loads((ROOT / 'analysis/signed_table_lookup_verification.json').read_text())
assert report['functions_verified'] == 1 and report['total_full_state_cases'] == 64
binary = (ROOT.parent.parent / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
item = report['routines'][0]
start, end = int(item['address'], 16), int(item['end_exclusive'], 16)
offset = start - 0x8c010000
assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == item['retail_span_sha256']
source = ROOT / 'source/signed_table_lookup.c'
assert hashlib.sha256(source.read_bytes()).hexdigest() == report['source_sha256']
routine = {
    'module': '1ST_READ.BIN', 'name': 'sa_signed_table_byte_8c049a5a',
    'address': hex(start), 'end_exclusive': hex(end),
    'retail_span_sha256': item['retail_span_sha256'],
    'source': 'source/signed_table_lookup.c',
    'verification': 'analysis/signed_table_lookup_verification.json',
    'status': f"Retail execution matched in {item['cases_passed']} complete CPU-state and RAM cases",
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
status.setdefault('verified_progress', {})['signed_table_lookup_8c049a5a'] = {
    'verification': 'analysis/signed_table_lookup_verification.json',
    'cases': item['cases_passed'], 'comparison_level': routine['comparison_level'],
    'retail_span_sha256': item['retail_span_sha256'],
    'scope': report['scope'], 'limitations': report['limitations'],
}
status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered signed table lookup; {len(registry['routines'])} routines total")
