import hashlib
import json
from pathlib import Path

PROJECT = Path(__file__).resolve().parent
ROOT = PROJECT.parent.parent
report = json.loads((PROJECT / 'analysis/external_overlay_tail_thunks_verification.json').read_text(encoding='utf-8'))
source_path = PROJECT / 'source/external_overlay_tail_thunks.c'
source_hash = hashlib.sha256(source_path.read_bytes()).hexdigest()
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))

for record in report['routines']:
    record_source_hash = record['source_sha256']
    assert record_source_hash == source_hash
    binary_path = ROOT / f"work/sonic-adventure/decoded/SONICADV/{record['module']}"
    binary = binary_path.read_bytes()
    start, end = int(record['address'], 16), int(record['end_exclusive'], 16)
    base = int(record['load_address'], 16)
    assert hashlib.sha256(binary[start - base:end - base]).hexdigest() == record['retail_span_sha256']
    routine = {
        key: record[key] for key in ('module', 'load_address', 'name', 'address', 'end_exclusive',
                                     'retail_span_sha256', 'comparison_level', 'limitations')
    }
    routine.update({
        'source': 'source/external_overlay_tail_thunks.c',
        'verification': 'analysis/external_overlay_tail_thunks_verification.json',
        'status': f"External overlay tail thunk matches retail in {record['cases_passed']} complete RAM/CPU comparisons",
    })
    prior = next((entry for entry in registry['routines']
                  if entry.get('module') == routine['module'] and
                  int(entry['address'], 16) == start), None)
    if prior is None:
        registry['routines'].append(routine)
    else:
        prior.update(routine)

registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
for record in report['routines']:
    progress[record['name']] = {
        'verification': 'analysis/external_overlay_tail_thunks_verification.json',
        'cases': record['cases_passed'], 'comparison_level': record['comparison_level'],
        'retail_span_sha256': record['retail_span_sha256'], 'source_sha256': source_hash,
        'scope': record['scope'], 'limitations': record['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered {len(report['routines'])} overlay thunks; {len(registry['routines'])} routine records total")
