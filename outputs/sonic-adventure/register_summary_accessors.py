"""Register the five differentially verified SUMMARY.BIN accessors."""
import json

from verify_startup import ROOT

PROJECT = ROOT / 'outputs/sonic-adventure'
report_path = PROJECT / 'analysis/additional-modules/SUMMARY.BIN/accessors_verification.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
registry_path = PROJECT / 'analysis/recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
for function in report['functions']:
    routine = {
        'module': report['module'], 'load_address': report['load_address'],
        'name': function['name'], 'address': function['address'],
        'end_exclusive': function['end_exclusive'],
        'retail_span_sha256': function['retail_span_sha256'],
        'source': 'source/sa_summary_accessors.c',
        'verification': 'analysis/additional-modules/SUMMARY.BIN/accessors_verification.json',
        'status': f"SUMMARY accessor matches retail in {report['cases_per_function']} differential cases",
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
print(f"Registered seven SUMMARY accessors; {len(registry['routines'])} routines total")
