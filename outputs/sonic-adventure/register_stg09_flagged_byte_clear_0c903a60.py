import hashlib
import json
from pathlib import Path

PROJECT = Path(__file__).resolve().parent
ROOT = PROJECT.parent.parent
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
binary = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
names = ('sa_stg09_flagged_byte_clear_0c903a60', 'sa_stg09_flagged_byte_clear_0c903a6e')
for name in names:
    report_path = PROJECT / f'analysis/common-overlays/STG09.BIN/{name}_verification.json'
    report = json.loads(report_path.read_text(encoding='utf-8'))
    start, end = int(report['address'], 16), int(report['end_exclusive'], 16)
    offset = start - int(report['load_address'], 16)
    assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == report['retail_span_sha256']
    source_path = PROJECT / 'source/sa_stg09_flagged_byte_clear_0c903a60.c'
    assert hashlib.sha256(source_path.read_bytes()).hexdigest() == report['source_sha256']
    routine = {
        'module': report['module'], 'load_address': report['load_address'],
        'name': name, 'address': report['address'], 'end_exclusive': report['end_exclusive'],
        'retail_span_sha256': report['retail_span_sha256'],
        'source': 'source/sa_stg09_flagged_byte_clear_0c903a60.c',
        'verification': f'analysis/common-overlays/STG09.BIN/{name}_verification.json',
        'status': f"STG09 byte-clear leaf matches retail in {report['cases_passed']} exhaustive full RAM/CPU comparisons",
        'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
    }
    existing = next((item for item in registry['routines']
                     if item.get('module') == routine['module'] and
                     int(item['address'], 16) == start), None)
    if existing is None:
        registry['routines'].append(routine)
    else:
        existing.update(routine)
    progress[name] = {
        'verification': routine['verification'], 'cases': report['cases_passed'],
        'comparison_level': report['comparison_level'],
        'retail_span_sha256': report['retail_span_sha256'],
        'source_sha256': report['source_sha256'], 'scope': report['scope'],
        'limitations': report['limitations'],
    }
progress['recovered_buildable_routines'] = len(registry['routines'])
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f'Registered two STG09 byte-clear leaves; {len(registry["routines"])} routines total')
