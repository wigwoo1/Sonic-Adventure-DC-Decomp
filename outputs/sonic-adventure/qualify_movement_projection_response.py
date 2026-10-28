"""Qualify the original complete movement projection/math group; canonical reuse."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','edges_verification','rounding_verification','predicate_proof','retained_verification','unbounded'):
        path=P/f'analysis/movement_projection_response_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_projection_response.dll';sha=digest(dll)
    for name in ('audit','direct_verification','edges_verification','rounding_verification'):assert reports[name]['dll_sha256']==sha
    retained=reports['retained_verification']
    for suite in retained['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct,edges,rounding=(reports[n+'_verification'] for n in ('direct','edges','rounding'));t=reports['translation'];proof=reports['predicate_proof']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'],direct['unchanged_standalone_CPU_comparisons'])==(796,5766,608,192)
    assert direct['status_counts']=={'0':692,'-1':90,'-3':14}
    assert (edges['original_native_comparisons'],edges['independent_checks'],edges['independent_oracle_cases'])==(100,1148,100) and edges['status_counts']=={'0':100}
    assert (rounding['original_native_comparisons'],rounding['independent_checks'],rounding['independent_oracle_cases'])==(12,168,12) and rounding['status_counts']=={'0':12}
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    coverage=[]
    for row in direct['coverage_by_routine']:
        same=[next(x for x in r['coverage_by_routine'] if x['address']==row['address']) for r in (direct,edges,rounding)]
        missing=set(same[0]['missing_rows']);outcomes=set(map(tuple,same[0]['missing_outcomes']))
        for r in same[1:]:missing&=set(r['missing_rows']);outcomes&=set(map(tuple,r['missing_outcomes']))
        proved=proof['scalar_profile_infeasible_outcomes'] if row['address']=='0x8c64016c' else []
        assert not missing and outcomes==set(map(tuple,proved)),(row['address'],missing,outcomes)
        coverage.append(dict(address=row['address'],static_rows=row['static_rows'],executed_rows=row['static_rows'],static_outcomes=row['static_outcomes'],executed_outcomes=row['static_outcomes']-len(outcomes),scalar_profile_infeasible_outcomes=proved,unaccounted_software_profile_outcomes=[]))
    assert sum(x['executed_rows'] for x in coverage)==1005 and sum(x['executed_outcomes'] for x in coverage)==63
    assert not proof['original_code_removed'] and not proof['domain_excluded_rows'] and not proof['domain_excluded_outcomes']
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==258 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    retpath=P/'analysis/movement_projection_response_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2458 and len(ret['new_call_returns'])==30 and len(ret['allowed_call_returns'])==2488
    actual={b['pr'] for report in (direct,edges,rounding) for case in report['cases'] for b in case['boundaries']};assert set(ret['original_call_returns'])<=actual
    previous=P/'analysis/movement_model_response_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==76622
    routines=[]
    for body in t['routines']:
        if body['canonical_registered_reuse']:continue
        r={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};r.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(x for x in coverage if x['address']==body['address']));routines.append(r)
    assert len(routines)==5 and sum(r['instruction_rows'] for r in routines)==994
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1605,1610);baseline=registry[:1605]
    assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==654;files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_original_movement_projection_response_group',compiled_source_count=258,newly_registered_entries=5,new_original_routine_credit=5,newly_qualified_original_rows=994,canonical_reused_entries=['0x8c055d5a'],canonical_duplicate_credit=0,canonical_reuse_original_rows=11,executed_original_rows=1005,executed_conditional_outcomes=63,scalar_profile_proved_infeasible_outcomes=1,scalar_predicate_profile=proof['scalar_profile'],unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes']+proof['unqualified_broader_precision_or_physical_outcomes'],unaccounted_software_profile_outcomes=[],domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=routines,prior_registry_entries=1605,result_registry_entries=1610,result_unique_module_address_pairs=1607,linked_routine_inventory=linked,fresh_original_native_comparisons=1151,fresh_independent_checks=8765,independent_oracle_cases=720,unchanged_standalone_CPU_comparisons_included=192,direct_status_counts=direct['status_counts'],actual_original_call_returns_executed=30,supplied_game_callee_outputs=0,supplied_execution_frames=0,flat_return_guard_sites=2488,explicit_nonfunction_resume_routes_total=596,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=525,new_nonlocal_resume_exports=30,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,retained_previous_evidence_original_native_comparisons=76622,total_evidence_original_native_comparisons=77773,world_owner_still_unregistered='0x8c028bfe',world_owner_unaccounted_outcomes=old['world_owner_unaccounted_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=old['collision_caller_unaccounted_outcomes'],fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,next_task='Revisit actual0730d8remaining dependency/control-flow closure, including its777e0mode-dispatch tail; preserve older world/collision/math and broaderprecision obligations.',limitations=[
      'Complete original075a40(612rows),64016c(251),650614(40),640080(81),63a9dc(10),plus canonical055d5a(11). All1005rows execute across actual input suites;63of64conditionaloutcomes execute. The remaining6402c0->308 outcome is proved impossible ONLY in the explicit stable mappedRAM PR0/SZ0 softwareprofile. Original edge stays emitted;broaderprecision/physicalbehavior is NOT universallyproved. Prior074626broaderprecision remains open.',
      '796direct comparisons/5766checks:692complete,90existingunsupportednumericstops,14mappedguards.100additionalmovement/clamp/angle/plane cases/1148checkscomplete.12finite nearlyparallelrounding/bank/roundingmode/RAMview cases/168checkscomplete and exercise075eb8negative-dot reversal. Full CPU/FPU/RAM/device/queue state and orderedentries compare;720included independent scalar geometry/predicate/heading/write-footprint oracle cases. All8bank/PR/SZcombinations additionally compared; scalar geometric interpretation is not asserted for raw double-register transfers.192included canonicalcases separatelyagreewith preservedstandaloneCPUimplementation;notadditionalcomparisoncredit.',
      'Original075a40 canreturnearly0/1/2 orleaveangularvalues inR0 afterheadingcommit. Tests independentlycheck the circularshortdifference,6000threshold,8000halfturn andactor/velocityheadingstores. Original mixed-normal6402c8zero-determinant path is preserved: authoredplanesproduce(0,-infinity,-infinity),notanidealintersection. Actual math/gamecallees execute;no suppliedresults/frames. No upstreamSARecompimplementationcopied.',
      '243fresh retainedIRQ/context/taskcomparisons/1683checks preserveoldnumericassertionsbyAST.1151freshcomparisons/8765checks;77773cumulativeevidence isnotunique/codecoverage. Priorfullstartup andfull-devicewrite remainhistorical,notrerun. Physicalalignment/FPUexceptions/MMU/bootROM/GPU/gameplay/frametiming remainunqualified.',
      '258-sourceDLL/library andstrictunboundedWall/Wextra/Werror pass. Exact257bridge+6entries/30actualreturns/30continuationexports;canonical055d5a is reusedwithzero routinecredit.2749oldbuilds/1605oldregistryrows preserved.596explicitresumes,71qualifiednonlocal/525remaining;new30exportsnotindependentlyqualifiednonlocalroutes. No newguestwaitbounds orCPU/guard/observerchanges. Hostimportfloor4096retained.',
      'Initialtests foundactualmissing63a9dc,nowrecoveredwithoutfurthercalls. NewtestassumptionsaboutR0status,rawSZ1geometryandtheoriginalzero-determinant casewerecorrectedfromoriginalinstructions;nativeCneverneededbehaviorfixes. Onlyfinalsuccessfulwhole-suitereportsarecounted. Bounded original-only witnesssearch iterations and diagnostic/initial runs awardno credit.',
      'Older028bfenull-readoutcomes,029b00A48cskip,threeoldmathwrapperprecisionpaths andbroader074626/6402c0precision remainopen;0730d8parent staysunregistered. Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. TextureOffdisablessampling/usesvertexcolours.10%weightedmilestones;researchDLLnonplayable;goalactive,keepSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_projection_response_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('PROJECTION QUALIFIED5software-profile routines/994newrows;1151comparisons/8765checks;654linked')
if __name__=='__main__':main()
