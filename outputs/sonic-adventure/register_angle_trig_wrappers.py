import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis' / 'angle_trig_wrappers_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8-sig'))
binary = (ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
source_path = ROOT / 'source' / 'angle_trig_wrappers.c'
registry_path = ROOT / 'analysis' / 'recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
assert hashlib.sha256(source_path.read_bytes()).hexdigest() == report['source_sha256']

for recovered in report['routines']:
    start = int(recovered['address'], 16)
    end = int(recovered['end_exclusive'], 16)
    offset = start - int(recovered['load_address'], 16)
    span = binary[offset:offset + end - start]
    assert len(span) == recovered['span_bytes']
    assert hashlib.sha256(span).hexdigest() == recovered['retail_span_sha256']
    routine = {
        'module': recovered['module'],
        'load_address': recovered['load_address'],
        'name': recovered['name'],
        'address': recovered['address'],
        'end_exclusive': recovered['end_exclusive'],
        'retail_span_sha256': recovered['retail_span_sha256'],
        'source': 'source/angle_trig_wrappers.c',
        'verification': 'analysis/angle_trig_wrappers_verification.json',
        'status': f'Angle trig entry compared in {report["cases_passed"]} states with FSCA output injected',
        'comparison_level': report['comparison_level'],
        'limitations': report['limitations'],
    }
    existing = next((item for item in registry['routines']
                     if item.get('module') == routine['module']
                     and int(item['address'], 16) == start), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)

registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path = ROOT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
for recovered in report['routines']:
    progress[recovered['name']] = {
        'verification': 'analysis/angle_trig_wrappers_verification.json',
        'cases': report['cases_passed'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': recovered['retail_span_sha256'],
        'source_sha256': report['source_sha256'],
        'scope': report['scope'],
        'limitations': report['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered {len(report["routines"])} angle trig spans; {len(registry["routines"])} registry rows total')
