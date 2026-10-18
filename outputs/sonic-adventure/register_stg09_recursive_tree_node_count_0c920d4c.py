"""Register the differentially verified STG09 recursive tree counter."""
import json
from pathlib import Path

PROJECT = Path(__file__).resolve().parent
report_path = PROJECT / 'analysis/common-overlays/STG09.BIN/recursive_tree_node_count_0c920d4c_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
routine = {
    'module': report['module'], 'load_address': report['load_address'],
    'name': 'sa_stg09_recursive_tree_node_count_0c920d4c',
    'address': report['address'], 'end_exclusive': report['end_exclusive'],
    'retail_span_sha256': report['retail_span_sha256'],
    'source': 'source/sa_stg09_recursive_tree_node_count_0c920d4c.c',
    'verification': 'analysis/common-overlays/STG09.BIN/recursive_tree_node_count_0c920d4c_verification.json',
    'status': f"STG09 recursive tree counter matches retail in {report['cases_passed']} differential cases",
    'comparison_level': report['comparison_level'], 'limitations': report['limitations'],
}
address = int(routine['address'], 16)
existing = next((entry for entry in registry['routines']
                 if entry.get('module') == routine['module'] and
                 int(entry['address'], 16) == address), None)
if existing is None:
    registry['routines'].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
print(f"Registered STG09 recursive tree counter; {len(registry['routines'])} routines total")
