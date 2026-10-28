"""Extend the frozen partial caller proof; do not register an unfinished routine."""
import hashlib
import json
from pathlib import Path

P=Path(__file__).resolve().parent
ROOT=P.parents[1]


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    prior_path=P/'analysis/collision_contact_qualification.json'
    alias_path=P/'analysis/collision_contact_counter_alias_verification.json'
    prior=json.loads(prior_path.read_text())
    alias=json.loads(alias_path.read_text())
    files=[Path(__file__),prior_path,alias_path]
    for report in (prior,alias):
        for name,sha in report['sha256'].items():
            assert digest(name)==sha,name
            files.append(Path(name))
    assert prior['fresh_original_native_comparisons']==444
    assert prior['total_evidence_original_native_comparisons']==71454
    assert (alias['original_native_comparisons'],alias['independent_checks'])==(12,186)
    assert alias['status_counts']=={'0':12}
    assert prior['dll_sha256']==alias['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/collision_contact.dll')
    coverage=alias['coverage_by_routine'][0]
    remaining=sorted(set(map(tuple,prior['unaccounted_outcomes'])) & set(map(tuple,coverage['missing_outcomes'])))
    assert remaining==[('0x8c02a48c','0x8c02a4a8')]
    assert prior['executed_rows']==1121 and not prior['coverage_by_routine'][0]['missing_rows']
    assert alias['newly_executed_outcomes']==[['0x8c02a592','0x8c02a5f4']]
    registry_path=P/'analysis/recovered_routines.json'
    registry=json.loads(registry_path.read_text())['routines']
    assert len(registry)==1589 and len({(x.get('module','1ST_READ.BIN'),x['address']) for x in registry})==1586
    assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']=='0x8c029b00' for x in registry)
    from audit_movie_linked_routines import inventory
    linked=inventory(ROOT/'work/sonic-adventure/build/collision_contact.dll',registry)
    assert linked['confirmed_linked_unique_routines_by_name']==631
    files.extend([registry_path,P/'audit_movie_linked_routines.py'])
    report=dict(status='partially_verified_collision_caller_one_remaining_outcome',
      prior_qualification='analysis/collision_contact_qualification.json',
      additive_verification='analysis/collision_contact_counter_alias_verification.json',
      compiled_source_count=249,executed_rows=1121,static_conditional_outcomes=74,
      executed_conditional_outcomes=73,unaccounted_outcomes=remaining,
      proven_infeasible_conditional_outcomes=[],domain_exclusions=[],
      new_original_routine_credit=0,newly_registered_entries=0,
      registry_entries=1589,registry_unique_module_address_pairs=1586,
      confirmed_linked_unique_routines_by_name=631,
      fresh_original_native_comparisons=12,fresh_independent_checks=186,
      retained_previous_evidence_original_native_comparisons=71454,
      total_evidence_original_native_comparisons=71466,
      combined_caller_stage_comparisons_including_retained=456,
      combined_caller_stage_checks_including_retained=2837,
      combined_direct_caller_comparisons=213,combined_direct_caller_status_counts={'0':180,'-1':29,'-3':4},
      supplied_game_callee_outputs=0,supplied_execution_frames=0,
      dll_sha256=alias['dll_sha256'],linked_routine_inventory=linked,
      native_implementation_changed=False,old_proofs_rewritten=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      next_task='Resolve normal-entry A48c->A4a8: establish actual R10 preservation across all callees '
      'and stack/input/global aliasing requirements. A41a requires COUNT>=R10; A48c skips if COUNT<=0. '
      'Do not assume callee-saved registers or disjoint memory without byte-backed evidence. '
      'Keep029b00 and three math wrappers unregistered until qualified.',
      limitations=[
        'A592 skip now executes from original normal entry. A query INPUT at COUNT-54 overlaps '
        'the original output store with the counter. Two contacts are generated; A56c overwrites '
        'COUNT with zero; A58c reloads zero; decrement yields ffffffff and A592 takes its skip. '
        'Subsequent division/output stores produce canonical NaN bits in the aliased counter. '
        'This is original behavior, not a valid final contact count or a native-code fix.',
        '12fresh complete original/native comparisons and186checks cover aliased/disjoint controls, '
        'P0/P1/P2 mapped MMU-off views and both physical FP banks at PR0/SZ0. Full state, RAM, '
        'devices, queues and ordered boundaries match. Existing CPU trace is composed read-only '
        'with its prior path observer; CPU instructions and comparison assertions are unchanged.',
        'Historical caller proof remains byte-preserved at1121rows/72outcomes and444comparisons/2651checks. '
        'The union is1121rows/73outcomes,456comparisons/2837checks including243retained comparisons. '
        'No retained suite or complete game startup was rerun; the C/DLL is unchanged. '
        'Cumulative71466 is historical-plus-fresh evidence, not unique cases or code coverage.',
        'The remaining A48c skip is open, not proved unreachable or excluded. No new recovered '
        'routine credit. The mapped input alias is not natural gameplay initialization evidence. '
        'Physical FPU/exception/MMU/address-error delivery, GPU pixels and complete gameplay remain unqualified.',
        'All game/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings work remains unfinished. '
        'TextureOff disables texture sampling and uses vertex colours.10%milestones,notcoverage; '
        'incomplete nonplayable research builds;keepgoalactive and SolHigh for remaining RE.'
      ],sha256={str(p):digest(p) for p in files})
    path=P/'analysis/collision_contact_counter_alias_qualification.json'
    path.write_text(json.dumps(report,indent=2)+'\n')
    print('COUNTER ALIAS PARTIAL:1121rows/73of74outcomes;12newcomparisons/186checks;0routinecredit')


if __name__=='__main__':
    main()
