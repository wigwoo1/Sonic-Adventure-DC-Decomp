"""Qualify complete nearby-object selection and identity loading; retain world-owner gaps."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','aliases_verification','sequence_verification','predicate_proof','retained_verification','unbounded'):
        path=P/f'analysis/movement_world_{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
        for name,sha in report.get('sha256',{}).items():assert digest(name)==sha,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_world.dll';sha=digest(dll)
    for name in ('audit','direct_verification','aliases_verification','sequence_verification'):assert reports[name]['dll_sha256']==sha
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];report=json.loads(path.read_text());files.append(path);assert report['dll_sha256']==sha
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct=reports['direct_verification'];t=reports['translation'];proof=reports['predicate_proof']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(218,1162,92)
    assert direct['status_counts']=={'0':196,'-1':16,'-3':6}
    coverage=direct['coverage_by_routine'];eligible={'0x8c0522c0','0x8c63a8e0'}
    for row in coverage:
        if row['address'] in eligible:assert not row['missing_rows'] and not row['missing_outcomes']
        else:
            assert row['address']=='0x8c028bfe' and row['executed_rows']==245 and row['executed_outcomes']==53
            assert row['missing_rows']==proof['unexecuted_rows']
            assert row['missing_outcomes']==sorted(proof['infeasible_outcomes']+proof['unaccounted_outcomes'])
    assert len(coverage)==3 and sum(r['executed_rows'] for r in coverage)==540 and sum(r['executed_outcomes'] for r in coverage)==83
    assert not proof['owner_eligible_for_registration'] and len(proof['unaccounted_outcomes'])==2
    for name,count,checks in (('aliases_verification',40,160),('sequence_verification',16,68),('retained_verification',243,1683)):
        assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==(count,checks)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==252 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    returns=P/'analysis/movement_world_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert ret['prior_return_sites']==2350 and len(ret['new_call_returns'])==13 and len(ret['allowed_call_returns'])==2363
    actual={b['pr'] for r in (direct,reports['aliases_verification'],reports['sequence_verification']) for case in r['cases'] for b in case['boundaries']}
    assert set(ret['original_call_returns'])<=actual
    previous=P/'analysis/movement_vectors_qualification.json';old=json.loads(previous.read_text());files.append(previous)
    assert old['total_evidence_original_native_comparisons']==72423
    routines=[]
    for body in t['routines']:
        if body['address'] not in eligible:continue
        row={key:body[key] for key in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(c for c in coverage if c['address']==body['address']))
        routines.append(row)
    assert len(routines)==2 and sum(r['instruction_rows'] for r in routines)==295
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1594,1596)
    baseline=registry[:1594];assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==638
    files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_nearby_object_selection_and_identity_loader_world_owner_partial',compiled_source_count=252,
      newly_registered_entries=2,new_original_routine_credit=2,newly_qualified_original_rows=295,canonical_duplicate_credit=0,
      qualified_original_routine_conditional_outcomes=30,proposed_original_rows=542,executed_original_rows=540,
      proposed_static_conditional_outcomes=86,executed_conditional_outcomes=83,proven_infeasible_conditional_outcomes=1,
      unaccounted_outcomes=proof['unaccounted_outcomes'],unexecuted_rows=proof['unexecuted_rows'],
      infeasible_outcomes=proof['infeasible_outcomes'],coverage_by_routine=coverage,routines=routines,
      prior_registry_entries=1594,result_registry_entries=1596,result_unique_module_address_pairs=1593,linked_routine_inventory=linked,
      fresh_original_native_comparisons=517,fresh_independent_checks=3073,direct_status_counts=direct['status_counts'],
      independent_finite_oracle_cases=92,actual_original_call_sites_executed=13,identity_overlap_comparisons=40,
      real_world_collision_sequence_comparisons=16,flat_return_guard_sites=2363,explicit_nonfunction_resume_routes_total=471,
      qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=400,
      new_nonlocal_resume_exports=13,new_nonlocal_resume_routes_qualified=0,
      retained_previous_evidence_original_native_comparisons=72423,total_evidence_original_native_comparisons=72940,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,partial_unregistered_proposals=['0x8c028bfe'],
      parent_movement_caller_still_unregistered='0x8c0730d8',collision_caller_still_unregistered='0x8c029b00',
      collision_caller_unaccounted_outcomes=[['0x8c02a48c','0x8c02a4a8']],fresh_full_game_startup_comparisons=0,
      full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Trace original scene/list construction for028bfe null-head/null-next readfrontiers;do not inventBIOSbytes orassume list/sentinel invariants. '
      'Continueactual0730d8movementdependencies withoriginalcontracts. Keep028bfe/029b00/0730d8unregistered untiltheircoverageobligationsclose.',
      limitations=[
        'Two complete software-qualified original routines:0522c0 nearby-object selection246rows/28outcomes and63a8e0 identityloader49rows/2outcomes. '
        '295rows/30outcomes execute. Worldowner028bfe iscompiled/testedbutUNREGISTERED:245of247rows/53of56outcomes execute; '
        'CA6taken isprovedinfeasible;Cba/E0cfallthroughandCbc/Cberowsremainopen. No creditforpartialbody.',
        '218directcomparisons:196complete,16reachedexistingnumericstops,6mappedmemoryguards.92includedfiniteoracles check '
        'identityloading,zero-centre staticselection,dynamicgates,separatedspherecases,empty/reverseorderedmixedselection, '
        '1024objectand96polygoncaps. Othergeometry/rotationusesoriginalinstructioncomparison,notcompleteindependentphysicsproof.',
        '63a8e0 nullR4loadsidentityintoinactiveFPbankatPR0/SZ0;explicitR4copiesidentitytoRAM throughactiveFPscratchregisters. '
        'NullR4 PR1stopsatFSCHG. Rawexplicit/all8PR/SZ/FRprofilesand40overlapcasesmatchoriginal;pairedmodeisnotassertedtobe '
        'anordinaryidentityAPI. Independenttwo-half-blockmemoryoraclechecksactualoverlapordering. Successfulgeometricowners '
        'remainPR0/SZ0;pairedownerpathsnotqualified. SRFDclear/softwaremappedRAMABI;physicalFPU/exception/MMUdeliveryunqualified.',
        '0522c0takesFR4/5/6=XYZ,FR7radius,produces16bitcount754ef0and12byteentries754ef4. Staticrowsarewalkedreverse; '
        '10000000flagusesactualidentity/rotation/pointcallees. Dynamicgateand36byterecordsareactualexternalRAMinput. '
        'A sufficientlylarge dynamicinputtable overlapsits fixedpresenceword(record128.x);fixtureexplicitlyretains '
        'thisRAMoverlap. Actualcallersproduceallselectionarrays/counts;nosuppliedcalleeoutputs/frames.',
        'Eight actual028bfe->029b00sequences(16comparisons/68checks)retainRAM/FPstate:lower/upperlists,hit/miss,bothFPbanks. '
        'Originalworldproducerselectsmeshrecordsconsumedbyoriginalcollisioncaller. Inputscene/query/mesh/sentinelsareauthored; '
        'thisisnotnaturalworldinitialization,complete0730d8movement/gameplay orqualificationofthelargecallers.',
        'Oneantisymmetrypredicateproofrequiresoriginalnormalentry/fixedcode/stableRAMandunchangedFPUcontrols. '
        'Twootherworldoutcomesfollowearliernull+10reads,whichcurrentRAM/busguardsstop. ThatdoesnotprovephysicalDreamcast '
        'unreachabilityorjustifyinventedBIOSinput. Bothoutcomesandtwoinstructionrowsremainexplicitlyunqualified.',
        '243freshretainedIRQ/context/taskcomparisons/1683checkskeepoldnumericassertionsbyAST. Total517freshcomparisons/3073checks; '
        'cumulative72940is evidencevolume,notunique/codecoverage. Earlier216warm/211coldstartupand242full-devicewrite '
        'remainhistorical,notrerun.252sourceDLL/libandstrictunboundedWallWextraWerrorpass.2737oldbuildsretained;newpairadditional.',
        'Exact251bridge plus3entries/13actualreturns/13continuationexports. OldCPU/flatinterpreter/guards/observer/rawsession/ABI '
        'unchanged;nonewsourcewaitbounds.471explicitresumes,71qualifiednonlocal/400remaining;newexportsnotindependentlyqualified. '
        '029b00A48cskipandthreeoldmathwrapperprecisionpathsremainunqualified. Noupsreamimplementationcodecopied. '
        'Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. TextureOffdisablessampling/usesvertexcolours. '
        '10%weightedmilestones;researchDLLnonplayable;keepgoalactiveandSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_world_qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    print('WORLD QUALIFIED2routines/295rows/30outcomes;028bfepartial;517comparisons/3073checks;638linked')

if __name__=='__main__':main()
