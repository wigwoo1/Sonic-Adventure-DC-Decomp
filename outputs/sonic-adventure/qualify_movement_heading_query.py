"""Qualify the full original heading query after coverage and preservation checks."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__),P/'audit_movie_linked_routines.py']
    for suffix in ('translation','connection','audit','direct_verification','retained_verification','unbounded','preservation'):
        path=P/f'analysis/movement_heading_query_{suffix}.json';r=json.loads(path.read_text());reports[suffix]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    t=reports['translation'];d=reports['direct_verification'];retained=reports['retained_verification']
    assert len(t['routines'])==1 and t['instruction_rows']==106 and len(t['resume_routes'])==2
    assert (d['original_native_comparisons'],d['independent_checks'],d['independent_oracle_cases'])==(343,3772,305)
    coverage=d['coverage_by_routine'];assert len(coverage)==1 and coverage[0]['address']=='0x8c06cd22'
    assert (coverage[0]['static_rows'],coverage[0]['executed_rows'],coverage[0]['static_outcomes'],coverage[0]['executed_outcomes'])==(106,106,26,26)
    assert not coverage[0]['missing_rows'] and not coverage[0]['missing_outcomes']
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    dll=ROOT/'work/sonic-adventure/build/movement_heading_query.dll';sha=digest(dll)
    assert sha==reports['audit']['dll_sha256']==d['dll_sha256']
    for suite in retained['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    u=reports['unbounded'];assert u['source_count']==reports['connection']['compiled_source_count']==262 and u['exit_code']==0
    for name,h in u['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    retpath=P/'analysis/movement_heading_query_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2532 and len(ret['allowed_call_returns'])==2534 and len(ret['new_call_returns'])==2
    observed={b['pr'] for case in d['cases'] for b in case['boundaries']};assert set(ret['original_call_returns'])<=observed
    preserved=reports['preservation'];assert preserved['preserved_previous_selected_files_outside_four_mutables']==16325 and preserved['preserved_prior_registry_rows']==1618 and preserved['preserved_archived_builds']==2757
    previous=P/'analysis/movement_surface_resolution_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==80110
    body=t['routines'][0];routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=106,coverage=coverage[0])
    baseline=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1618];assert len(baseline)==1618
    assert not any(x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']=='0x8c06cd22' for x in baseline)
    linked=inventory(dll,baseline+[routine]);assert linked['confirmed_linked_unique_routines_by_name']==663
    limitations=[
      'Original06cd22 full106rows/26conditionaloutcomes execute. Actorflag override,nonzero table-value eligibility,optional outputs,character lock andactual0902/0903 stage fallback remain original. No discarded rows,assumed infeasible edges,new wait bounds,supplied game results or repaired execution frames.',
      '343direct original/native comparisons/3772checks include305semantic oracle cases. Tests cover raw heading words,signedzero andnumeric table values,optional pointers,indices0/1/255,actor/character flags,signed stage globals,actual stage query short-circuit calls,bank/rounding/rawPR/SZ/FD states and mapped guards. The4000override preserves fullFPUstate andignores output pointers as original.',
      'Live aliases preserve original store order. Heading output may overwrite the table float field oractor index;the later value load observes that original write. Shared outputs and writesbacktoactorheading are also tested. No observer/reference/native changes were needed after initial generation;representative andfullsuites passed.',
      '243retainedIRQ/context/task comparisons/1683checks preserveoldnumeric expectationsbyAST.586fresh comparisons/5455checks;305includedsemantic oracle cases;80696cumulativecomparisons are evidence totals,notunique routines ormeasured codecoverage. Otherhistorical fullstartup/physicaldevice evidence notrerun.',
      '262-sourceDLL/build andstrict unboundedWall/Wextra/Werror pass. Exact261bridge plus1entry/2actualreturns/2continuationexports.2534flatreturns,642explicitresumes,71qualifiednonlocal/571remaining. New2exports are notindependentlyqualifiednonlocalroutes. OldCPU/guards/observer/ABI/comparisonlogic unchanged.2757oldbuilds/1618oldregistryrows preserved.',
      'Older0730d8/028bfe/029b00 owners,specialtail0777e0,callbackassignments/missingcallbacks,074ee4scalar/broaderprecision andphysicalFPU/MMU/exceptions/devices/fullgame remainunfinished. PC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR/settings remainplanned. TextureOff disables sampling/usesvertex-materialcolours. NoSARecompimplementation imported;goalactive/SolHigh;10%weightedmilestones.'
    ]
    result=dict(status='complete_original_heading_query_software_qualified',compiled_source_count=262,routines=[routine],
      new_original_routine_credit=1,newly_qualified_original_rows=106,coverage_by_routine=coverage,executed_original_rows=106,
      executed_conditional_outcomes=26,infeasible_outcomes_claimed=[],canonical_duplicate_credit=0,
      existing_stage_query_reused='0x8c04f7e0',result_registry_entries=1619,result_unique_module_address_pairs=1616,
      linked_routine_inventory=linked,fresh_original_native_comparisons=586,fresh_independent_checks=5455,independent_oracle_cases=305,
      retained_previous_evidence_original_native_comparisons=80110,total_evidence_original_native_comparisons=80696,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,flat_return_guard_sites=2534,
      explicit_nonfunction_resume_routes_total=642,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=571,
      new_nonlocal_resume_exports=2,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,
      unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',
      world_owner_still_unregistered='0x8c028bfe',collision_caller_still_unregistered='0x8c029b00',special_tail_still_unregistered='0x8c0777e0',
      full_gameplay_qualified=False,physical_exception_delivery_qualified=False,overall_completion_percent=10,
      estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Recover remaining specialtail0777e0dependencies075686/075f8c/074afc and074afc child076640 coherently,thenverifyparents. Preserveolderworld/collision/precision/callback/physical obligations.',
      limitations=limitations,sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_heading_query_qualification.json').write_text(json.dumps(result,indent=2)+'\n')
    print('HEADING QUERY QUALIFIED106rows/26outcomes;586comparisons/5455checks;663linked',flush=True)
if __name__=='__main__':main()
