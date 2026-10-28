"""Accept complete original surface bodies after union coverage and preserved controls."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__),P/'audit_movie_linked_routines.py']
    for suffix in ('translation','connection','audit','direct_verification','edges_verification','retained_verification','unbounded','preservation'):
        path=P/f'analysis/movement_surface_resolution_{suffix}.json';r=json.loads(path.read_text());reports[suffix]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    t=reports['translation'];d=reports['direct_verification'];e=reports['edges_verification'];retained=reports['retained_verification']
    assert len(t['routines'])==3 and t['instruction_rows']==647 and len(t['resume_routes'])==29
    assert (d['original_native_comparisons'],d['independent_checks'],d['independent_oracle_cases'])==(409,2624,178)
    assert d['status_counts']=={'0':376,'-1':18,'-3':15}
    assert (e['original_native_comparisons'],e['independent_checks'],e['zero_direction_branch_cases'])==(28,92,20)
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    coverage=[]
    for row in d['coverage_by_routine']:
        other=next(x for x in e['coverage_by_routine'] if x['address']==row['address'])
        missing=set(row['missing_rows'])&set(other['missing_rows']);outcomes=set(map(tuple,row['missing_outcomes']))&set(map(tuple,other['missing_outcomes']))
        assert not missing and not outcomes
        coverage.append(dict(address=row['address'],static_rows=row['static_rows'],executed_rows=row['static_rows'],static_outcomes=row['static_outcomes'],executed_outcomes=row['static_outcomes'],missing_rows=[],missing_outcomes=[]))
    assert sum(x['executed_rows'] for x in coverage)==647 and sum(x['executed_outcomes'] for x in coverage)==64
    dll=ROOT/'work/sonic-adventure/build/movement_surface_resolution.dll';sha=digest(dll)
    for key in ('audit','direct_verification','edges_verification'):assert reports[key]['dll_sha256']==sha
    for suite in retained['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    u=reports['unbounded'];assert u['source_count']==reports['connection']['compiled_source_count']==261 and u['exit_code']==0
    for name,h in u['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    source=P/t['source'];code=source.read_text();assert all('L'+x+':' in code for x in ('8c074860','8c07498e','8c0763e4','8c0764d0')) and 'SA_BOUND_RETAIL_STALLS' not in code
    retpath=P/'analysis/movement_surface_resolution_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2503 and len(ret['allowed_call_returns'])==2532 and len(ret['new_call_returns'])==29
    observed={b['pr'] for report in (d,e) for case in report['cases'] for b in case['boundaries']};assert set(ret['original_call_returns'])<=observed
    preserved=reports['preservation'];assert preserved['preserved_previous_selected_files_outside_four_mutables']==16278 and preserved['preserved_prior_registry_rows']==1615 and preserved['preserved_archived_builds']==2755
    previous=P/'analysis/movement_record_adjustment_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==79430
    routines=[]
    for body in t['routines']:
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(x for x in coverage if x['address']==body['address']));routines.append(row)
    baseline=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1615];assert len(baseline)==1615
    assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==662
    limitations=[
      'Original0747d2(339rows/40outcomes),0763a0(289/20),07256a(19/4) are complete emitted instruction bodies. All647rows and64outcomes execute across direct+targeted comparisons. No branch removed, infeasibility assumed or original Ghidra-dropped comparison discarded.',
      '409direct comparisons/2624checks include178semantic oracle cases and376normal returns,18reachednumericstops,15mappedguards. Tests cover grounded/airborne selectors,rotated movement,normal/cap/character policies,nullable clears,normal copies,actor anchor/offset positions,response velocity,heading/character side effects,bank/rounding/raw precision and nonfinite geometry. Actual original callees execute;no supplied game results or repaired frames.',
      '28targeted comparisons/92checks cover grounded cap rejection and20actual finite-underflow witnesses. Small normal-valued direction and large finite plane normal leave a nonzero cross product while original direction length rounds/flushes to zero. This is ordinary function input geometry,not a claim of a discovered retail gameplay occurrence. Both missed outcomes are executed instead of declared impossible.',
      '243retainedIRQ/context/task comparisons/1683checks keep old numeric assertions unchangedbyAST.680fresh comparisons/4399checks,182includedsemantic oracle cases;80110cumulativecomparison evidence is not unique routines or measured code coverage. Other historical fullstartup/physical-device evidence was not rerun.',
      '261-sourceDLL/build and strict unboundedWall/Wextra/Werror pass. Exact260bridge plus3entries/29actualreturns/29continuationexports.2532flatreturns,640explicitresumes,71qualifiednonlocal/569remaining. New29exports are not independently qualified nonlocal resumptions. OldCPU/guards/observer/ABI/complete-state comparison unchanged;no new guest limits.2755oldbuilds and1615oldregistryrows preserved.',
      'The new clear-oracle initially assumed status preservation;the original finalTST setsT fromquery-nullness. Only that new expectation was corrected;native/original behavior always matched. Source/observer/numeric backends were never changed to fit tests.',
      'Older0730d8/028bfe/029b00 owners remain unregistered;old callback-assignment/unknown-target and074ee4original-mathscalar/broaderprecision obligations remain. PhysicalFPU/MMU/exceptions/devices/fullgame/PC60/120/240FPS8K/Android/stereo3DS/N64/PS1/SUPIR/settings unfinished. TextureOff disables sampling/usesvertex-materialcolours. NoSARecompimplementation imported;goalactive/SolHigh;10%weightedmilestones.'
    ]
    result=dict(status='complete_original_surface_resolution_bodies_software_qualified',compiled_source_count=261,
      routines=routines,new_original_routine_credit=3,newly_qualified_original_rows=647,coverage_by_routine=coverage,
      executed_original_rows=647,executed_conditional_outcomes=64,infeasible_outcomes_claimed=[],canonical_duplicate_credit=0,
      result_registry_entries=1618,result_unique_module_address_pairs=1615,linked_routine_inventory=linked,
      fresh_original_native_comparisons=680,fresh_independent_checks=4399,independent_oracle_cases=182,
      finite_underflow_zero_direction_witnesses=20,retained_previous_evidence_original_native_comparisons=79430,total_evidence_original_native_comparisons=80110,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,flat_return_guard_sites=2532,
      explicit_nonfunction_resume_routes_total=640,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=569,
      new_nonlocal_resume_exports=29,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,
      unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',
      world_owner_still_unregistered='0x8c028bfe',collision_caller_still_unregistered='0x8c029b00',full_gameplay_qualified=False,
      physical_exception_delivery_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Recover remaining ordinary0730d8dependency06cd22;retain specialtail0777e0 and world/collision/precision/callback/physical obligations.',limitations=limitations,
      sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_surface_resolution_qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('SURFACE QUALIFIED3routines/647rows/64outcomes;680comparisons/4399checks;662linked',flush=True)
if __name__=='__main__':main()
