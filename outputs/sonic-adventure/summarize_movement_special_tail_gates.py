"""Combine pinned whole-entry evidence without awarding unfinished recovery credit."""
import hashlib
import json
from pathlib import Path

P = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    missing_rows = missing_edges = None
    comparisons = checks = 0
    files = [Path(__file__)]
    suites = []
    for name in ('direct', 'context', 'phases', 'gates', 'object_order'):
        path = P / f'analysis/movement_special_tail_{name}_verification.json'
        report = json.loads(path.read_text())
        for source, expected in report['sha256'].items():
            assert sha(source) == expected, source
        assert report['dll_sha256'] == 'a470e4abe0e3c2e53f779bc7feedbcf5fe9f74d1607c12c463a24636f103c0b0'
        assert report['new_original_routine_credit'] == 0
        assert report['supplied_game_callee_outputs'] == report['supplied_execution_frames'] == 0
        c = report['coverage_by_routine'][0]
        assert c['address'] == '0x8c0777e0' and c['static_rows'] == 1273 and c['static_outcomes'] == 138
        rows, edges = set(c['missing_rows']), {tuple(x) for x in c['missing_outcomes']}
        missing_rows = rows if missing_rows is None else missing_rows & rows
        missing_edges = edges if missing_edges is None else missing_edges & edges
        comparisons += report['original_native_comparisons']
        checks += report['independent_checks']
        suites.append(dict(name=name, comparisons=report['original_native_comparisons'], checks=report['independent_checks']))
        files.append(path)
    path = P / 'analysis/movement_special_tail_retained_verification.json'
    retained = json.loads(path.read_text())
    for source, expected in retained['sha256'].items():
        assert sha(source) == expected, source
    comparisons += retained['original_native_comparisons']
    checks += retained['independent_checks']
    files.append(path)
    path = P / 'analysis/movement_special_tail_gate_reachability.json'
    proof = json.loads(path.read_text())
    for source, expected in proof['sha256'].items():
        assert sha(source) == expected, source
    dead_rows = set(proof['corresponding_unreachable_rows'])
    dead_edges = {tuple(x) for x in proof['proven_infeasible_edges']}
    assert dead_rows <= missing_rows and dead_edges <= missing_edges
    files.append(path)
    report = dict(
        status='partial_whole_routine_gate_verification;not_registered_or_fully_qualified',
        original_entry='0x8c0777e0', static_rows=1273, observed_rows=1273-len(missing_rows),
        static_conditional_outcomes=138, observed_conditional_outcomes=138-len(missing_edges),
        source_bound_infeasible_rows=sorted(dead_rows),
        source_bound_infeasible_outcomes=[list(x) for x in sorted(dead_edges)],
        reachability_scope=proof['scope'],
        unresolved_rows=sorted(missing_rows-dead_rows),
        unresolved_conditional_outcomes=[list(x) for x in sorted(missing_edges-dead_edges)],
        new_original_native_comparisons=284, new_independent_checks=2256,
        combined_original_native_comparisons=comparisons, combined_independent_checks=checks,
        suites=suites, retained_comparisons=retained['original_native_comparisons'],
        compiled_source_count=265, runtime_code_changed=False, new_original_routine_credit=0,
        registry_entries_unchanged=1623, overall_completion_percent=10,
        remaining='Positive-count sort entry/loop guards and repeated nonzero response path remain unresolved. Natural caller allocation/alias contracts, live callbacks, nonlocal resumptions, hardware and full gameplay remain open.',
        scope='Full routine retained. New original-entry comparisons and limited source-bound reachability evidence extend prior software tests. Counts are comparison/check totals, not distinct routines or full-game coverage. No acceptance or routine-registration credit.',
        sha256={str(f): sha(f) for f in files})
    assert comparisons == 1437 and checks == 10573
    target = P / 'analysis/movement_special_tail_gate_progress.json'
    assert not target.exists(), 'Preserve an existing summary; add a new revision.'
    target.write_text(json.dumps(report, indent=2)+'\n')
    print('SPECIAL TAIL GATE PROGRESS', report['observed_rows'], 'rows',
          report['observed_conditional_outcomes'], 'outcomes;',
          len(report['unresolved_rows']), 'rows/',
          len(report['unresolved_conditional_outcomes']), 'outcomes unresolved', flush=True)


if __name__ == '__main__':
    main()
