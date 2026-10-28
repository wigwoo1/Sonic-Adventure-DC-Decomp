"""Qualify three original movement/math bodies; retain canonical and caller limits."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','aliases_verification',
                 'sequence_verification','predicate_proof','retained_verification','unbounded'):
        path=P/f'analysis/movement_vectors_{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
        for name,sha in report.get('sha256',{}).items():assert digest(name)==sha,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_vectors.dll';sha=digest(dll)
    for name in ('audit','direct_verification','aliases_verification','sequence_verification'):assert reports[name]['dll_sha256']==sha
    retained=reports['retained_verification']
    for suite in retained['suites']:
        path=P/suite['proof'];report=json.loads(path.read_text());files.append(path);assert report['dll_sha256']==sha
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct=reports['direct_verification'];t=reports['translation'];proof=reports['predicate_proof']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(333,1811,138)
    assert direct['status_counts']=={'0':304,'-1':14,'-3':15}
    cv=[]
    for row in direct['coverage_by_routine']:
        r=dict(row);proved=proof['infeasible_outcomes'] if r['address']=='0x8c074cf0' else []
        assert not r['missing_rows'] and r['missing_outcomes']==proved
        r.update(proven_infeasible_outcomes=proved,unaccounted_outcomes=[]);cv.append(r)
    assert len(cv)==6 and sum(r['executed_rows'] for r in cv)==449 and sum(r['executed_outcomes'] for r in cv)==56
    assert proof['static_conditional_outcomes']==40 and len(proof['infeasible_outcomes'])==2
    assert not proof['domain_excluded_rows'] and not proof['domain_excluded_outcomes'] and not proof['original_code_removed']
    assert len(t['routines'])==6 and len(t['canonical_reused_entries'])==3 and len(t['resume_routes'])==17
    for name,count,checks in (('aliases_verification',8,32),('sequence_verification',12,40),('retained_verification',243,1683)):
        assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==(count,checks)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==251 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    returns=P/'analysis/movement_vectors_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert ret['prior_return_sites']==2333 and len(ret['new_call_returns'])==17 and len(ret['allowed_call_returns'])==2350
    actual_returns={b['pr'] for r in (direct,reports['aliases_verification'],reports['sequence_verification']) for c in r['cases'] for b in c['boundaries']}
    assert set(ret['original_call_returns'])<=actual_returns
    previous=P/'analysis/collision_setup_qualification.json';old=json.loads(previous.read_text());files.append(previous)
    assert old['total_evidence_original_native_comparisons']==71827
    routines=[]
    for body in t['routines']:
        if body['canonical_registered_reuse']:continue
        r={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        r.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(c for c in cv if c['address']==body['address']))
        routines.append(r)
    assert len(routines)==3 and sum(r['instruction_rows'] for r in routines)==294
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1591,1594)
    baseline=registry[:1591];assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==636
    files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_original_movement_rotation_response_and_dot_product',compiled_source_count=251,
      newly_registered_entries=3,new_original_routine_credit=3,newly_qualified_original_rows=294,
      canonical_reused_entries=t['canonical_reused_entries'],canonical_duplicate_credit=0,
      executed_rows_including_canonical_adapters=449,static_conditional_outcomes_including_canonical=58,
      executed_conditional_outcomes_including_canonical=56,proven_infeasible_conditional_outcomes=2,
      infeasible_outcomes=proof['infeasible_outcomes'],unaccounted_outcomes=[],domain_exclusion_arguments=[],
      new_routine_static_conditional_outcomes=46,new_routine_executed_conditional_outcomes=44,
      coverage_by_routine=cv,routines=routines,prior_registry_entries=1591,result_registry_entries=1594,
      result_unique_module_address_pairs=1591,linked_routine_inventory=linked,
      fresh_original_native_comparisons=596,fresh_independent_checks=3566,direct_status_counts=direct['status_counts'],
      independent_finite_oracle_cases=138,actual_original_call_sites_executed=17,
      overlapping_input_output_comparisons=8,real_helper_sequence_comparisons=12,
      flat_return_guard_sites=2350,explicit_nonfunction_resume_routes_total=458,
      qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=387,
      new_nonlocal_resume_exports=17,new_nonlocal_resume_routes_qualified=0,
      retained_previous_evidence_original_native_comparisons=71827,total_evidence_original_native_comparisons=72423,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,
      parent_movement_caller_still_unregistered='0x8c0730d8',collision_caller_still_unregistered='0x8c029b00',
      collision_caller_unaccounted_outcomes=[['0x8c02a48c','0x8c02a4a8']],fresh_full_game_startup_comparisons=0,
      full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      dll_sha256=sha,next_task='Recover full original0730d8movement owner and its next real missing dependency028bfe. '
      'The original normal entry obtains query throughR6+64;keepactual preparation/inputs/callees. '
      'Connect/recoverremainingoriginaltargets ratherthan providingcalleeoutputs/frames. '
      'Keep0730d8/029b00 and3mathwrappers unregistered until qualified;A48c skip remainsopen.',
      limitations=[
        'Three new complete original bodies:07900075rows/6outcomes;074cf0206rows/40syntacticoutcomes '
        '(38executed+2provedinfeasible);63909013rowdotproduct. Three canonical adapters078eb0/057b40/638f28 '
        'add155rows/12outcomes withzero duplicate credit. All449rows/56feasibleoutcomes execute;two '
        'explicit normal-entry repeated-comparison proofs,not input exclusions or removed code.',
        '333direct comparisons:304complete,14existingPR1numericstops,15mappedguards. FullCPU/FPU/RAM/device/queue/ '
        'ordered boundaries match andall17actualcallsitesexecute.138included independentfinitecases '
        'check exact small dyadic dot products, authored response coefficient examples with separately rounded '
        'binary32 operations,identity/diagonal direction transforms,zero-anglevectorpreservation andmatrixlimits. '
        'Other rotations and normalization use original execution comparison,not a complete independent physics oracle.',
        'The original079000 atan wrapper takes FR4=X,FR5=Y, so the identity orientation is(0,1,0), '
        'not(1,0,0). Actual atan/asin/matrix/axis/direction/pop callees execute. The three old scalar math '
        'wrappers remain unregistered;no new claim closes their previously unqualified precision paths. '
        'Successful vector-owner scope is ordinaryPR0/SZ0;selectedPR1numericstops andPR0/SZ1memoryguardprefixes '
        'are checked. Paired-FMOV complete owner paths remain unqualified;all raw instructions retained.',
        '074cf0 uses authored external reference vector at754bdc andnormalization flag890070;test inputs '
        'declare these RAM values. D46skip is impossible because reaching the second flag/comparison requires '
        'the first identical comparison true and stable mask/input RAM reloads. E3eskip repeats a true compare '
        'with unchanged operands. Cause/sticky changes cannot alter completed T under the existing comparison '
        'controls. Fixed code/stable mapped RAM/SRFDclear;no asynchronous memory/physical exception delivery claim.',
        'Eight output/input alias cases pass32checks,including point/state/querynormal overlaps anddisjoint '
        'controls,bothFPbanks.12comparisons/40checks coveractualconverter->response->converter sequences '
        'on sameRAM/FPstate atidentity orientation. Host starts ordinarycalls;no calleeoutputs/frames. '
        'This is not complete0730d8execution,naturalplayer/queryinitialization,wholemovement/gameplay orframe timing.',
        '243freshretainedIRQ/context/taskcomparisons/1683checks keepoldnumericassertions byAST. '
        'Total596freshcomparisons/3566checks;cumulative72423is evidencevolume,notunique/codecoverage. '
        'Earlier216warm/211coldstartup and242full-devicewrite evidence remains historical,not rerun.',
        'Exact250bridge plus6entries/17actualreturns/17continuationexports.251sourcesbuild/strictunbounded '
        'WallWextraWerrorpass. Old C/CPU/flatinterpreter/guards/observer/rawsession/ABI/2735buildspreserved; '
        'newDLL/libadditional.458explicitresumes,71qualifiednonlocal/387remaining;newexports notindependentlyqualified.',
        '0730d8movement owner remains unregistered;its2041originalrows/76calls are research input only. '
        '029b00 remains unregistered at1121rows/73of74outcomes;A48cskip isopen. Three old mathwrappers remain '
        'unregistered. No upstream implementation code iscopied. Fullgame/PC60/120/240FPS8K/Android/3DSstereo/ '
        'N64/PS1/SUPIR/settingsunfinished. TextureOff disables sampling/usesvertexcolours.10%milestones,notcoverage; '
        'researchDLLnonplayable;keepgoalactiveandSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_vectors_qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    print('MOVEMENT QUALIFIED3routines/294newrows;449rows/56executed+2provedoutcomes;596comparisons/3566checks;636linked')


if __name__=='__main__':main()
