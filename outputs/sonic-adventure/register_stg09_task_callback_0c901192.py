import hashlib
import json
from pathlib import Path

PROJECT = Path(__file__).resolve().parent
ROOT = PROJECT.parent.parent
report_path = PROJECT / 'analysis/common-overlays/STG09.BIN/task_callback_0c901192_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
binary = (ROOT / 'work/sonic-adventure/decoded/SONICADV/STG09.BIN').read_bytes()
start, end = int(report['address'], 16), int(report['end_exclusive'], 16)
offset = start - int(report['load_address'], 16)
assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == report['retail_span_sha256']
source_path = PROJECT / 'source/sa_stg09_task_callback_0c901192.c'
assert hashlib.sha256(source_path.read_bytes()).hexdigest() == report['source_sha256']

registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
routine = {
    'module': report['module'], 'load_address': report['load_address'],
    'name': 'sa_stg09_task_callback_0c901192', 'address': report['address'],
    'end_exclusive': report['end_exclusive'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/sa_stg09_task_callback_0c901192.c',
    'verification': 'analysis/common-overlays/STG09.BIN/task_callback_0c901192_verification.json',
    'status': f"STG09 0x1234 task callback matches retail in {report['cases_passed']} full RAM/CPU/service-boundary comparisons",
    'comparison_level': report['comparison_level'],
    'limitations': report['limitations'],
}
found = next((item for item in registry['routines']
              if item.get('module') == routine['module'] and
              int(item['address'], 16) == start), None)
if found is None:
    registry['routines'].append(routine)
else:
    found.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')

status_path = PROJECT / 'project_status.json'
status = json.loads(status_path.read_text(encoding='utf-8-sig'))
progress = status.setdefault('verified_progress', {})
progress[routine['name']] = {
    'verification': routine['verification'], 'cases': report['cases_passed'],
    'comparison_level': report['comparison_level'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source_sha256': report['source_sha256'], 'scope': report['scope'],
    'limitations': report['limitations'],
}
progress['recovered_buildable_routines'] = len(registry['routines'])
status_path.write_text(json.dumps(status, indent=2) + '\n', encoding='utf-8')
print(f"Registered STG09 task callback; {len(registry['routines'])} routines total")
