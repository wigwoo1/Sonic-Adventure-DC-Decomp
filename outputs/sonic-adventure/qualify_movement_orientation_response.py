"""Qualify the complete helpers while retaining the orientation owner's open gate."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','range_verification','angle_fold_obligation','retained_verification','unbounded'):
        path=P/f'analysis/movement_orientation_response_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_orientation_response.dll';sha=digest(dll)
    for name in ('audit','direct_verification','range_verification','angle_fold_obligation'):assert reports[name]['dll_sha256']==sha
    retained=reports['retained_verification']
    for suite in retained['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct,extra=reports['direct_verification'],reports['range_verification'];t=reports['translation'];obligation=reports['angle_fold_obligation']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(361,3389,342) and direct['status_counts']=={'0':342,'-1':12,'-3':7}
    assert (extra['original_native_comparisons'],extra['independent_checks'],extra['independent_oracle_cases'])==(104,910,82) and extra['status_counts']=={'0':82,'-1':14,'1':8}
    assert (retained['original_native_comparisons'],retained['independent_checks'])==(243,1683)
    assert obligation['is_unreachability_proof'] is False and obligation['unqualified_outcomes']==[['0x8c074fd4','0x8c074fd6']]
    assert not obligation['original_code_removed'] and not obligation['domain_excluded_rows'] and not obligation['domain_excluded_outcomes']
    coverage=[]
    for row in direct['coverage_by_routine']:
        other=next(x for x in extra['coverage_by_routine'] if x['address']==row['address']);missing=sorted(set(row['missing_rows'])&set(other['missing_rows']));outcomes=sorted(set(map(tuple,row['missing_outcomes']))&set(map(tuple,other['missing_outcomes'])))
        if row['address']=='0x8c074ee4':assert missing==obligation['unqualified_rows'] and outcomes==list(map(tuple,obligation['unqualified_outcomes']))
        else:assert not missing and not outcomes
        coverage.append(dict(address=row['address'],static_rows=row['static_rows'],executed_rows=row['static_rows']-len(missing),static_outcomes=row['static_outcomes'],executed_outcomes=row['static_outcomes']-len(outcomes),unqualified_rows=missing,unqualified_outcomes=[list(x) for x in outcomes]))
    assert sum(r['executed_rows'] for r in coverage)==233 and sum(r['executed_outcomes'] for r in coverage)==35
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==259 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    retpath=P/'analysis/movement_orientation_response_returns.json';ret=json.loads(retpath.read_text());files.append(retpath)
    assert ret['prior_return_sites']==2488 and len(ret['new_call_returns'])==13 and len(ret['allowed_call_returns'])==2501
    actual={b['pr'] for report in (direct,extra) for case in report['cases'] for b in case['boundaries']};assert set(ret['original_call_returns'])<=actual
    previous=P/'analysis/movement_projection_response_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==77773
    routines=[];pending=[]
    for body in t['routines']:
        r={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};r.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(x for x in coverage if x['address']==body['address']))
        (pending if body['address']=='0x8c074ee4' else routines).append(r)
    assert len(routines)==2 and sum(x['instruction_rows'] for x in routines)==79 and len(pending)==1
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1610,1612);baseline=registry[:1610]
    assert not any(x['address'] in t['unregistered_proposals'] and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in baseline)
    assert not any(x['address']=='0x8c074ee4' and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in registry)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==656;files.append(P/'audit_movie_linked_routines.py')
    owner_cases=sum(c['configuration']['entry']==0x8c074ee4 for r in (direct,extra) for c in r['cases']);assert owner_cases==232
    q=dict(status='helpers_software_qualified_orientation_owner_still_pending',compiled_source_count=259,newly_registered_entries=2,new_original_routine_credit=2,newly_qualified_original_rows=79,canonical_duplicate_credit=0,existing_connected_dependencies_reused=['0x8c63a9c4','0x8c057b40','0x8c639f5c','0x8c639ec8','0x8c63a1cc','0x8c638f28','0x8c639b98','0x8c639090','0x8c10d0f8'],all_proposed_original_rows=236,executed_original_rows=233,executed_conditional_outcomes=35,proposed_owner_new_routine_credit=0,unregistered_owner='0x8c074ee4',unregistered_proposals=pending,unqualified_original_rows=obligation['unqualified_rows'],unqualified_original_outcomes=obligation['unqualified_outcomes'],infeasible_outcomes_claimed=[],angle_fold_range_proved=False,numeric_boundary_queries_not_comparison_credit=obligation['numeric_boundary_queries'],coverage_by_routine=coverage,routines=routines,prior_registry_entries=1610,result_registry_entries=1612,result_unique_module_address_pairs=1609,linked_routine_inventory=linked,fresh_original_native_comparisons=708,fresh_independent_checks=5982,independent_oracle_cases=424,partial_owner_original_native_comparisons=owner_cases,supplied_game_callee_outputs=0,supplied_execution_frames=0,flat_return_guard_sites=2501,explicit_nonfunction_resume_routes_total=609,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=538,new_nonlocal_resume_exports=13,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,retained_previous_evidence_original_native_comparisons=77773,total_evidence_original_native_comparisons=78481,unqualified_broader_precision_outcomes=old['unqualified_broader_precision_outcomes'],world_owner_still_unregistered='0x8c028bfe',world_owner_unaccounted_outcomes=old['world_owner_unaccounted_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=old['collision_caller_unaccounted_outcomes'],fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,next_task='Resolve actual074ee4 angle-fold074fd4->074fd6 with a proved bound for the real original atan2 closure or an ordinary-input witness. Do not register074ee4 or move past its open acceptance gate based on ideal math, finite samples or injected math outputs.',limitations=[
      'Recovered original074ee4(157rows),078e40(56),055d20(23),all236rowsretained in new C. Only078e40and055d20are software-qualified/registered:79newrows,12fullyexecutedhelperoutcomes. Owner074ee4executes154of157rows/23of24outcomes andstaysUNREGISTERED. Threeanglefoldrows074fd6/8/a and074fd4->074fd6remainunqualified even in the ordinary profile;no infeasibility proof claimed, no original rows/branches deleted. Fullowner recovery/verification remains the next task, not replaced by helper completion.',
      '361direct comparisons/3389checks:342complete,12existingunsupportednumericstops,7mappedguards.104additionalrawgeometrycomparisons/910checks:82complete,14existingunsupportednumericstops,8reachednumerictraps. Actualmatrix/rotation/direction/signedsqrt/atan2/circularcallees execute;no suppliedcalleeoutputs/frames.424includedindependentcircularangle/cardinalrotation/normalization/matrixbalance/input-footprint/copy/clamporaclecases.232ownercomparisonsarepartialcoverageevidence,notownerqualification.',
      'Angle-fold audit derives exact original constants and native scalar multiplication/truncation thresholds:RM0firstpositivefoldword40491169,RM1word4049116a,398/399binary32wordsabove retailPI40490fdb. These local arithmetic queries are NOT original/native instruction comparisons and do NOT prove a bound for the actual atan2 approximation. Neither104edgecases nor the older5260nativeatan2samples prove universal unreachability. The real mathclosure bound/witness remains open.',
      '243retainedIRQ/context/taskcomparisons/1683checks preserveoldnumericassertionsbyAST.708freshcomparisons/5982checks;78481cumulativeevidence isnotunique/codecoverage. Previousfullstartup/physicaldevicewrite evidence is historical,notrerun. Physicalalignment/FPU/MMU/exceptions/GPU/gameplay/frametiming remainunqualified.',
      '259-sourceDLL/library andstrictunboundedWall/Wextra/Werror pass. Exact258bridge+3entries/13actualreturns/13continuationexports;oldCPU/observer/guards/ABI unchanged;nonewguestwaitlimits.2751oldbuilds/1610oldregistryrows preserved.609explicitresumes,71qualifiednonlocal/538remaining;new13exportsNOTindependentlyqualifiednonlocalroutes. Signedsqrt63a9c4andexistingmatrix/mathdependenciesreusedwithoutduplicatecodeorcredit.',
      'A newtest initiallyreversedtheoriginalspeedtiercomparisons;correctedfromtheactualoriginalBT/BFflow,thenfull361casespassed. NativeCbehaviorandobserverneverchangedtofitthatassumption. Onlyfinalwhole-suitescount;diagnostic/privatepartialrunsaddnocredit. Broader074626/6402c0precision,older028bfe/029b00frontiers andthreeoldmathwrapperprecisionpaths remainopen;0730d8staysunregistered.',
      'Fullgame/PC60/120/240FPS8K/Android/stereo3DS/N64/PS1/SUPIR/settingsunfinished. TextureOffdisablessampling/usesvertexcolours. No SARecompimplementationcodecopied.10%weightedmilestones;researchDLLnonplayable;goalactive,keepSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_orientation_response_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('ORIENTATION2HELPERS QUALIFIED/79rows;OWNER074ee4PENDING3rows/1outcome;708comparisons/5982checks;656linked')
if __name__=='__main__':main()
