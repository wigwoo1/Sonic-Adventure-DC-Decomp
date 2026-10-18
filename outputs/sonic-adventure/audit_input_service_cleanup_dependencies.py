"""Record verified source/index coverage for cleanup helper 8c646654."""
import csv
import json
from pathlib import Path


PROJECT = Path(__file__).resolve().parent
ROOT = PROJECT.parents[2]
REGISTRY = json.loads((PROJECT / 'analysis/recovered_routines.json').read_text(encoding='utf-8-sig'))
ENTRY_MAP = json.loads((PROJECT / 'analysis/existing_source_entries.json').read_text(encoding='utf-8-sig'))

# Direct targets/data-derived call paths visible in Ghidra's pseudocode and
# function-pointer literals. The 48-record callback table is represented below
# as a dynamic edge because its function pointers are supplied at runtime.
CALLEES = [
    ('0x8c65f1aa', 'initial device service function pointer', 'indirect call'),
    ('0x8c65f202', 'response processing function pointer', 'indirect call'),
    ('0x8c6461ae', 'object table compaction helper', 'direct call'),
    ('0x8c65f1ec', 'device submit rearm function pointer', 'indirect call'),
    ('0x8c65f50c', 'device record getter function pointer', 'indirect call'),
    ('0x8c64658a', 'active-device selection helper', 'direct call'),
    ('0x8c65f3f4', 'record-match test used by callback loop', 'thunked call'),
    ('0x8c646868', 'callback argument/pointer helper', 'direct call'),
    ('0x8c646856', 'record-address helper', 'direct call'),
]


def main():
    index_path = PROJECT / 'analysis/ghidra/function_index.tsv'
    with index_path.open(encoding='utf-8-sig', newline='') as handle:
        index_rows = list(csv.DictReader(handle, delimiter='\t'))
    index = {row['address'].lower(): row for row in index_rows}
    index_addresses = [int(row['address'], 16) for row in index_rows]
    cleanup_address = 0x8C646654
    cleanup_next = next(address for address in index_addresses if address > cleanup_address)
    cleanup_indexed_body_bytes = int(index[f'{cleanup_address:08x}']['body_bytes'])
    recovered = {
        (entry.get('module', '1ST_READ.BIN'), entry['address'].lower()): entry
        for entry in REGISTRY['routines']
    }
    cleanup_routine = recovered.get(('1ST_READ.BIN', '0x8c646654'))
    results = []
    for address, description, relation in CALLEES:
        key = address[2:]
        candidates = index.get(key)
        routine = recovered.get(('1ST_READ.BIN', address.lower()))
        possible_sources = ENTRY_MAP.get('entries', {}).get(key, [])
        results.append({
            'address': address,
            'description': description,
            'relation': relation,
            'indexed_body_bytes': int(candidates['body_bytes']) if candidates else None,
            'index_status': candidates.get('decompilation_status') if candidates else 'not indexed',
            'verified_registry_entry': bool(routine),
            'registered_source': routine.get('source') if routine else None,
            'verification_report': routine.get('verification') if routine else None,
            'source_entry_candidates_address_only': possible_sources,
        })

    report = {
        'module': '1ST_READ.BIN',
        'routine': {
            'address': '0x8c646654',
            'end_exclusive': hex(cleanup_next),
            'indexed_body_bytes': cleanup_indexed_body_bytes,
            'address_span_bytes': cleanup_next - cleanup_address,
            'non_instruction_span_bytes': cleanup_next - cleanup_address - cleanup_indexed_body_bytes,
        },
        'analysis_basis': [
            'Ghidra draft pseudocode at analysis/ghidra-all/initial_pseudocode.c',
            '1ST_READ.BIN function index and current recovered-routines registry',
            'Current source-entry inventory',
        ],
        'source_candidate_warning': 'The source-entry inventory is address-only and can mix module origins; these candidates are context only. Only a matching recovered-routines registry entry proves module-specific verification.',
        'direct_dependencies': results,
        'dynamic_callback_table': {
            'records': 48,
            'record_stride_words': 3,
            'callback_pointer_word': 2,
            'runtime_callbacks': 'Each non-null callback is invoked after a record-match check; targets are runtime data and cannot be exhaustively named from this static listing.',
        },
        'data_dependencies': [
            {'literal': '0x8c64672c', 'value': '0x8c8a83b4', 'role': 'base pointer cell used for status fields'},
            {'literal': '0x8c646730', 'value': '0x8c893e10', 'role': 'device object base'},
            {'literal': '0x8c646734', 'value': '0x8c894098', 'role': 'cleanup/status flag'},
            {'literal': '0x8c646770', 'value': '0x8c893e50', 'role': '48-entry callback record table'},
            {'literal': '0x8c646898', 'value': '0x8c893e48', 'role': 'optional error callback pointer cell'},
        ],
        'current_recovery_boundary': (
            'The 0x8c646654 routine is registered and passed 456 full-state retail comparisons across negative exits, successful cleanup, active-device publication, and every callback-table slot. Its external callback implementations remain explicit stubs; this audit does not claim full input-service or game completion.'
            if cleanup_routine else
            'The caller 0x8c647198 is registered and verified with explicit callee stubs. This audit does not claim 0x8c646654 or its dependencies are recovered.'
        ),
        'recommended_next_work': [
            'Replace external cleanup callback stubs with verified game-owned callback implementations as their runtime targets are identified.',
            'Verify the cleanup caller and native input-service integration using the registered cleanup routine rather than an opaque callee stub.',
        ],
    }
    path = PROJECT / 'analysis/input_service_cleanup_dependency_audit.json'
    path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    unverified = sum(not item['verified_registry_entry'] for item in results)
    print(f"Mapped {len(results)} dependency targets; {unverified} lack a verified registry entry")
    print(f"Dynamic callback records: {report['dynamic_callback_table']['records']}")
    print(f"Wrote {path}")


if __name__ == '__main__':
    main()
