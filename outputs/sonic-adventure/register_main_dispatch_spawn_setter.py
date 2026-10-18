"""Register two verified original helpers and their connected native evidence."""
import hashlib
import json
from pathlib import Path
from verify_startup import ROOT

PROJECT = Path(__file__).resolve().parent
REPORT = 'analysis/main_dispatch_spawn_setter_verification.json'
TRANSLATION = 'analysis/main_dispatch_spawn_setter_translation.json'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    report = json.loads((PROJECT / REPORT).read_text())
    translation = json.loads((PROJECT / TRANSLATION).read_text())
    assert report['cases_passed'] == 2517
    assert report['counts'] == dict(notify=678, create=168, ready=72, gate=583, spawn=512, set=504)
    for key, expected in dict(rows_covered=37, conditional_edges_covered=6, call_sites_covered=1,
                              boundary_comparisons=19094, partial_guard_error_cases=268,
                              independent_invariants=2185, completed_without_external_calls=2221,
                              service_error_cases=1).items():
        assert report[key] == expected, key
    assert report['routines'] == translation['routines']
    for record in (report, translation):
        for path, expected in record['sha256'].items():
            assert digest(Path(path)) == expected, path
    assert digest(PROJECT / report['source']) == report['source_sha256']
    assert digest(ROOT / 'work/sonic-adventure/build/main_dispatch_spawn_setter.dll') == report['dll_sha256']
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    assert hashlib.sha256(binary).hexdigest() == report['binary_sha256']
    registry_path = PROJECT / 'analysis/recovered_routines.json'
    registry = json.loads(registry_path.read_text(encoding='utf-8-sig'))
    added = 0
    for routine in translation['routines']:
        start, end = int(routine['address'], 16), int(routine['end_exclusive'], 16)
        assert hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest() == routine['retail_span_sha256']
        item = dict(module='1ST_READ.BIN', load_address='0x8c010000', name=routine['name'],
                    address=routine['address'], end_exclusive=routine['end_exclusive'],
                    indexed_instruction_bytes=routine['instruction_bytes'],
                    retail_span_sha256=routine['retail_span_sha256'], source=report['source'], verification=REPORT,
                    native_build_script='build_main_dispatch_spawn_setter.ps1',
                    verifier_script='verify_main_dispatch_spawn_setter.py',
                    instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),
                    descriptive_name_is_inferred=True,
                    status='All original rows, conditional outcomes and call sites covered by bounded full-state native comparisons with connected task/heap dependencies.',
                    comparison_level=report['scope'], limitations=report['limitations'])
        old = next((r for r in registry['routines'] if r.get('module', '1ST_READ.BIN') == '1ST_READ.BIN'
                    and int(r['address'], 16) == start), None)
        if old is None:
            registry['routines'].append(item)
            added += 1
        else:
            old.update(item)
    integration = dict(verification=REPORT, verification_sha256=digest(PROJECT / REPORT),
                       translation=TRANSLATION, translation_sha256=digest(PROJECT / TRANSLATION),
                       cases_passed=report['cases_passed'],
                       completed_without_external_calls=report['completed_without_external_calls'],
                       native_dependencies=['0x8c07b320', '0x8c07b2b8'],
                       remaining_prefix_service='0x8c01ae60',
                       deferred_stored_callbacks=['0x8c044680', 'dynamic values from the 0x8c19c32c table'],
                       scope=report['scope'], limitations=report['limitations'])
    for address in (0x8C0AFB10, 0x8C109FDE, 0x8C109E50, 0x8C64BED8, 0x8C09852E,
                    0x8C0985EC, 0x8C09865C, 0x8C098A80, 0x8C098B0C, 0x8C098B42):
        parent = next(r for r in registry['routines'] if r.get('module', '1ST_READ.BIN') == '1ST_READ.BIN'
                      and int(r['address'], 16) == address)
        parent.setdefault('integration_evidence', {})['native_spawn_and_callback_setter'] = integration
    registry_path.write_text(json.dumps(registry, indent=2) + '\n')
    status_path = PROJECT / 'project_status.json'
    status = json.loads(status_path.read_text(encoding='utf-8-sig'))
    status.setdefault('verified_progress', {})['native_spawn_and_callback_setter'] = integration
    status['verified_progress']['recovered_buildable_routines'] = len(registry['routines'])
    status_path.write_text(json.dumps(status, indent=2) + '\n')
    print(f'Registered spawn/setter: {added} new original routines; {len(registry["routines"])} entries. Cancel and eventual stored callback execution remain unfinished.')


if __name__ == '__main__':
    main()
