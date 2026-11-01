"""Extend pinned gate evidence; leave the unproven caller contract open."""
import hashlib
import json
from pathlib import Path

P=Path(__file__).resolve().parent


def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    paths=[P/'analysis/movement_special_tail_gate_progress.json',
           P/'analysis/movement_special_tail_live_record_verification.json',
           P/'analysis/movement_special_tail_sort_contract.json']
    old,live,contract=[json.loads(p.read_text()) for p in paths]
    for report in (old,live,contract):
        for p,h in report['sha256'].items():assert sha(p)==h,p
    assert old['new_original_routine_credit']==live['new_original_routine_credit']==contract['new_original_routine_credit']==0
    dead_rows=set(old['source_bound_infeasible_rows'])
    dead_edges={tuple(x) for x in old['source_bound_infeasible_outcomes']}
    old_rows=set(old['unresolved_rows'])|dead_rows
    old_edges={tuple(x) for x in old['unresolved_conditional_outcomes']}|dead_edges
    coverage=live['coverage_by_routine'][0]
    missing_rows=old_rows&set(coverage['missing_rows'])
    missing_edges=old_edges&{tuple(x) for x in coverage['missing_outcomes']}
    assert dead_rows<=missing_rows and dead_edges<=missing_edges
    result=dict(
        status='partial_whole_routine_live_callback_verification;not_registered',
        original_entry='0x8c0777e0',static_rows=1273,observed_rows=1273-len(missing_rows),
        static_conditional_outcomes=138,observed_conditional_outcomes=138-len(missing_edges),
        source_bound_infeasible_rows=sorted(dead_rows),
        source_bound_infeasible_outcomes=[list(x) for x in sorted(dead_edges)],
        reachability_scope=old['reachability_scope'],
        unresolved_rows=sorted(missing_rows-dead_rows),
        unresolved_conditional_outcomes=[list(x) for x in sorted(missing_edges-dead_edges)],
        conditional_sort_analysis=str(paths[2]),
        sort_contract_globally_closed_edges=contract['globally_closed_branch_outcomes'],
        caller_allocation_alias_contract_proven=False,retail_callback_assignment_proven=False,
        new_original_native_comparisons=live['original_native_comparisons'],
        new_independent_checks=live['independent_checks'],
        new_normal_completions=live['normal_completions'],
        new_software_numeric_frontier_cases=live['declared_numeric_frontier_cases'],
        combined_original_native_comparisons=old['combined_original_native_comparisons']+live['original_native_comparisons'],
        combined_independent_checks=old['combined_independent_checks']+live['independent_checks'],
        compiled_source_count=265,runtime_code_changed=False,new_original_routine_credit=0,
        registry_entries_unchanged=1623,overall_completion_percent=10,
        scope='Actual original0751c0 executes through whole0777e0/07506e and covers repeated positive response. Authored callback pointer/table inputs and live aliasing are not retail callback assignment evidence. One matched nonfinite FMAC software stop is not a physical-hardware trap or a completed gameplay return. Sort guards remain globally unresolved;conditional arithmetic proof does not establish caller allocation/frame alias contracts. Full game and ports remain incomplete.',
        sha256={str(p):sha(p) for p in [Path(__file__),*paths]})
    assert result['combined_original_native_comparisons']==1454
    assert result['combined_independent_checks']==10756
    target=P/'analysis/movement_special_tail_live_record_progress.json'
    assert not target.exists(),'Preserve earlier summaries;write a new revision.'
    target.write_text(json.dumps(result,indent=2)+'\n')
    print('LIVE RECORD PROGRESS',result['observed_rows'],'rows',result['observed_conditional_outcomes'],'outcomes;',len(result['unresolved_rows']),'rows/',len(result['unresolved_conditional_outcomes']),'outcomes globally unresolved',flush=True)


if __name__=='__main__':main()
