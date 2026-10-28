"""Qualify complete original bodies only after actual full-state execution."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__),P/'audit_movie_linked_routines.py']
    for suffix in ('translation','connection','audit','direct_verification','callback_alias_verification','retained_verification','unbounded','preservation'):
        path=P/f'analysis/movement_record_adjustment_{suffix}.json';r=json.loads(path.read_text());reports[suffix]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    t=reports['translation'];direct=reports['direct_verification'];alias=reports['callback_alias_verification'];retained=reports['retained_verification']
    assert len(t['routines'])==2 and t['instruction_rows']==164 and len(t['resume_routes'])==2
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(225,1979,136)
    assert direct['status_counts']=={'0':188,'-2':8,'-1':18,'-3':11}
    assert (alias['original_native_comparisons'],alias['independent_checks'],alias['independent_oracle_cases'])==(16,128,16)
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    assert all(not r['missing_rows'] and not r['missing_outcomes'] for r in direct['coverage_by_routine'])
    assert sum(x['executed_rows'] for x in direct['coverage_by_routine'])==164 and sum(x['executed_outcomes'] for x in direct['coverage_by_routine'])==8
    dll=ROOT/'work/sonic-adventure/build/movement_record_adjustment.dll';sha=digest(dll)
    for key in ('audit','direct_verification','callback_alias_verification'):assert reports[key]['dll_sha256']==sha
    for suite in retained['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    u=reports['unbounded'];assert u['source_count']==reports['connection']['compiled_source_count']==260 and u['exit_code']==0
    for name,h in u['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    source=P/t['source'];assert 'L8c0750a0:' in source.read_text() and 'SA_BOUND_RETAIL_STALLS' not in source.read_text()
    retpath=P/'analysis/movement_record_adjustment_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2501 and len(ret['allowed_call_returns'])==2503 and len(ret['new_call_returns'])==2
    observed={b['pr'] for report in (direct,alias) for case in report['cases'] for b in case['boundaries']}
    assert set(ret['original_call_returns'])<=observed
    preserved=reports['preservation'];assert preserved['preserved_previous_selected_files_outside_four_mutables']==16233 and preserved['preserved_prior_registry_rows']==1613 and preserved['preserved_archived_builds']==2753
    previous=P/'analysis/movement_orientation_fold_qualification.json';old=json.loads(previous.read_text());files.append(previous)
    assert old['total_evidence_original_native_comparisons']==78946
    routines=[]
    for body in t['routines']:
        r={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        r.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(x for x in direct['coverage_by_routine'] if x['address']==body['address']))
        routines.append(r)
    baseline=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1613];assert len(baseline)==1613
    assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==659
    limitations=[
      'Complete original07506e122rows/8conditionaloutcomes and0751c042rows/0outcomes execute. All164rows are emitted and byte-checked, including0750a0 omitted by the Ghidra draft. No dropped rows, assumed infeasible edges, replaced math, injected game results or repaired execution frames.',
      '225direct comparisons/1979checks include136semantic oracle cases: selected28byte records,character indices0/1/255,null pointer,no/zero motion,modulo16bitangles,position restoration,actual original0751c0 callback side effects,raw arithmetic/bank/rounding controls,aliases and guards.188complete,18numericstops,8unknowncallbackstops,11mappedguards. Unknown callbacks remain reached unresolved targets with original frame/return intact;this is not recovery of every possible callback body.',
      '16additionalcomparisons/128checks author a pointer alias that makes the real callback mutate the record translation;the caller must reload the changed offsets. This verifies live callback effects without a supplied output.0751c0 is an argument-compatible original test callback;no actual retail callback assignment is claimed.',
      '243retainedIRQ/context/task comparisons/1683checks preserve the old numeric assertions byAST.484freshcomparisons/3790checks;152includedsemantic oracle cases.79430cumulativecomparison evidence is not unique routines or measured code coverage. Other historical startup/fullgame evidence was not rerun.',
      '260-sourceDLL and strict unboundedWall/Wextra/Werror check pass. Exact previous259bridge plus2entries/2actualreturns/2continuationexports.2503flatreturns,611explicitresumes,71qualifiednonlocal/540remaining. New2continuationexports are not independently qualified nonlocal resumptions. Old CPU/guards/ABI/complete-state comparison unchanged;new observer captures callback inputs read-only. No new guest wait limits.',
      'Older0730d8/028bfe/029b00 owners remain unregistered. Original-math scalar074ee4qualification and broader074fd4/074626/6402c0precision obligations remain intact. PhysicalFPU/MMU/exceptions/devices,fullgame,PC60/120/240FPS8K,Android,stereoscopic3DS,N64/PS1,SUPIR/settings unfinished. TextureOff disables sampling and uses vertex/material colours. NoSARecompimplementation copied;goalactive/SolHigh;10% weighted milestones.'
    ]
    result=dict(status='complete_original_movement_record_adjustment_bodies_software_qualified',
      compiled_source_count=260,routines=routines,new_original_routine_credit=2,newly_qualified_original_rows=164,
      executed_original_rows=164,executed_conditional_outcomes=8,infeasible_outcomes_claimed=[],canonical_duplicate_credit=0,
      result_registry_entries=1615,result_unique_module_address_pairs=1612,linked_routine_inventory=linked,
      fresh_original_native_comparisons=484,fresh_independent_checks=3790,independent_oracle_cases=152,
      retained_previous_evidence_original_native_comparisons=78946,total_evidence_original_native_comparisons=79430,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,retail_callback_assignment_discovered=False,
      unresolved_callback_test_targets=['0x8cfef000'],flat_return_guard_sites=2503,
      explicit_nonfunction_resume_routes_total=611,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=540,
      new_nonlocal_resume_exports=2,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,
      unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes'],
      parent_movement_caller_still_unregistered='0x8c0730d8',world_owner_still_unregistered='0x8c028bfe',collision_caller_still_unregistered='0x8c029b00',
      full_gameplay_qualified=False,physical_exception_delivery_qualified=False,overall_completion_percent=10,
      estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Recover ordinary0730d8 dependency0747d2 and its actual callees;preserve callback-assignment/unknown target,world/collision and physical obligations.',
      limitations=limitations,sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_record_adjustment_qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('RECORD ADJUSTMENT QUALIFIED2routines/164rows/8outcomes;484comparisons/3790checks;659linked',flush=True)
if __name__=='__main__':main()
