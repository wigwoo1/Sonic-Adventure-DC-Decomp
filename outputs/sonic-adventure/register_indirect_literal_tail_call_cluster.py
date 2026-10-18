import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / 'analysis' / 'indirect_literal_tail_call_cluster_verification.json'
reports = json.loads(report_path.read_text(encoding='utf-8'))['routines']
assert len(reports) == 4 and all(item['cases_passed'] == 64 for item in reports)
binary = (ROOT.parent.parent / 'work' / 'sonic-adventure' / 'extracted' / '1ST_READ.BIN').read_bytes()
registry_path = ROOT / 'analysis' / 'recovered_routines.json'
registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
assert hashlib.sha256(binary).hexdigest() == registry['binary_sha256']
source = ROOT / 'source' / 'sa_indirect_literal_tail_call_cluster.c'
source_hash = hashlib.sha256(source.read_bytes()).hexdigest()
for report in reports:
    assert report['source_sha256'] == source_hash
    start, end = int(report['address'], 16), int(report['end_exclusive'], 16)
    offset = start - int(report['load_address'], 16)
    assert hashlib.sha256(binary[offset:offset + end - start]).hexdigest() == report['retail_span_sha256']
    routine = {
        'module': report['module'], 'load_address': report['load_address'],
        'name': f"sa_indirect_literal_tail_call_{report['address'].lower().removeprefix('0x')}",
        'address': report['address'], 'end_exclusive': report['end_exclusive'],
        'retail_span_sha256': report['retail_span_sha256'],
        'source': 'source/sa_indirect_literal_tail_call_cluster.c',
        'verification': 'analysis/indirect_literal_tail_call_cluster_verification.json',
        'status': f"Indirect literal tail call matched in {report['cases_passed']} complete RAM/CPU-state cases",
        'limitations': report['limitations'],
    }
    existing = next((item for item in registry['routines'] if item.get('address', '').lower().removeprefix('0x') == report['address'].lower().removeprefix('0x')), None)
    if existing:
        assert existing == routine, f'Conflicting registration at {report["address"]}: {existing}'
    else:
        registry['routines'].append(routine)
registry_path.write_text(json.dumps(registry, indent=2) + '\n', encoding='utf-8')
print(f'registered {len(reports)} indirect literal tail-call wrappers')
