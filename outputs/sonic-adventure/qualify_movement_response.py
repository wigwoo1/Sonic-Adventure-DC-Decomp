"""Qualify complete original contact response and math with executed alias-sensitive paths."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','aliases_verification','sequence_verification','retained_verification','unbounded'):
        path=P/f'analysis/movement_response_{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
        for name,h in report.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_response.dll';sha=digest(dll)
    for name in ('audit','direct_verification','aliases_verification','sequence_verification'):assert reports[name]['dll_sha256']==sha
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];report=json.loads(path.read_text());files.append(path);assert report['dll_sha256']==sha
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct=reports['direct_verification'];aliases=reports['aliases_verification'];t=reports['translation']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(436,2352,242)
    assert direct['status_counts']=={'0':376,'-1':44,'-3':10,'1':6}
    coverage=[]
    for body in t['routines']:
        rows=[next(r for r in report['coverage_by_routine'] if r['address']==body['address']) for report in (direct,aliases)]
        missing_rows=set(rows[0]['missing_rows'])&set(rows[1]['missing_rows']);missing_outcomes=set(map(tuple,rows[0]['missing_outcomes']))&set(map(tuple,rows[1]['missing_outcomes']))
        assert not missing_rows and not missing_outcomes,body['address']
        coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],static_outcomes=len(body['conditional_edges']),executed_outcomes=len(body['conditional_edges']),missing_rows=[],missing_outcomes=[]))
    assert len(coverage)==5 and sum(r['executed_rows'] for r in coverage)==378 and sum(r['executed_outcomes'] for r in coverage)==32
    assert aliases['original_zero_length_branch_executed'] and aliases['supplied_game_callee_outputs']==aliases['supplied_execution_frames']==0
    for name,count,checks in (('aliases_verification',50,198),('sequence_verification',48,240),('retained_verification',243,1683)):
        assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==(count,checks)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==253 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    returns=P/'analysis/movement_response_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert ret['prior_return_sites']==2363 and len(ret['new_call_returns'])==19 and len(ret['allowed_call_returns'])==2382
    actual={b['pr'] for report in (direct,aliases,reports['sequence_verification']) for case in report['cases'] for b in case['boundaries']}
    assert set(ret['original_call_returns'])<=actual
    previous=P/'analysis/movement_world_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==72940
    routines=[]
    for body in t['routines']:
        if body['canonical_registered_reuse']:continue
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],coverage=next(r for r in coverage if r['address']==body['address']));routines.append(row)
    assert len(routines)==4 and sum(r['instruction_rows'] for r in routines)==373 and t['canonical_reused_entries']==['0x8c63a970']
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1596,1600)
    baseline=registry[:1596];assert not any(r['address'] in t['unregistered_proposals'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==642;files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_original_contact_response_signed_angle_cross_length_and_vector_add',compiled_source_count=253,
      newly_registered_entries=4,new_original_routine_credit=4,newly_qualified_original_rows=373,canonical_reused_entries=t['canonical_reused_entries'],canonical_duplicate_credit=0,
      executed_original_rows_including_canonical=378,executed_conditional_outcomes=32,proven_infeasible_conditional_outcomes=0,
      unaccounted_outcomes=[],unexecuted_rows=[],domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=routines,
      prior_registry_entries=1596,result_registry_entries=1600,result_unique_module_address_pairs=1597,linked_routine_inventory=linked,
      fresh_original_native_comparisons=777,fresh_independent_checks=4473,direct_status_counts=direct['status_counts'],independent_finite_oracle_cases=242,
      actual_original_call_sites_executed=19,alias_comparisons=50,world_collision_response_sequence_comparisons=48,
      original_zero_length_branch_executed=True,supplied_game_callee_outputs=0,supplied_execution_frames=0,
      flat_return_guard_sites=2382,explicit_nonfunction_resume_routes_total=490,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=419,
      new_nonlocal_resume_exports=19,new_nonlocal_resume_routes_qualified=0,
      retained_previous_evidence_original_native_comparisons=72940,total_evidence_original_native_comparisons=73717,
      world_owner_still_unregistered='0x8c028bfe',world_owner_unaccounted_outcomes=[['0x8c028cba','0x8c028cbc'],['0x8c028e0c','0x8c028e0e']],
      parent_movement_caller_still_unregistered='0x8c0730d8',collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=[['0x8c02a48c','0x8c02a4a8']],
      fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Recover next actual0730d8 dependency075860 at originalcall733e8, with its real state/query contracts. '
      'Retain original world/collision null-read and alias/predicate obligations;keep0730d8/028bfe/029b00 unregistered until qualified.',
      limitations=[
        'Four complete original routines:075416 response280rows/28outcomes,055dac signedangle44rows/4outcomes,638eac vectoradd13rows,639b50 cross/length36rows. '
        'All373newrows/32outcomes execute. Canonicalcosine63a970 adds5rows with zero duplicatecredit. All378rows execute;no excluded rows/outcomes or unreachable-branch assumptions.',
        '436direct comparisons:376complete,44existing unsupported numeric stops,10mapped guards,6numeric traps.242included finiteoracles checkexactsmall-dyadiccross/add, '
        'independent rational-root rounding,cardinal angles/signs,quarter-phase cosine/16bitwrap andselectedaxisresponse/velocitylimits. Otheracos/normalization/geometry uses '
        'originalinstructioncomparison,not a full independent physics or physicalFPU proof. Successfulgeometricowner scope PR0/SZ0;rawleafprofiles/selectedspecialnumeric cases checked.',
        '075416 reads actual88byte contactrecords at6bacf0,querycount+28,point+4,motion+10,normal+1c;R5velocity+4 andR6state+C4 limit;initialstackword is callerflag. '
        'Originaldistance,acos,signedangle,cross/length,dot,cosine,normalization andvectoradd callees execute. No provided callee results/frames.',
        'The zero-length branch75626 is REACHABLE,despite the Ghidra C draft elision.16real638eac->075416 sequences use oldRAMword-1/0,velocity/querymotion overlap/disjoint, '
        'bothFPbanks andRM0/1. Originalvectoradd produces prior stack RAM. Contactdistance1001 leaves minindex-1,so originalresponse reads frame+64 beforeits distancearray. '
        'Aliasedhead-on velocityclear zeroesquerymotion;0>negative priorword permits secondlength0 branch. Bothpositive/zero controls match. '
        'No rootframe,returnaddress orresultis fabricated;normalentry allocates itsownframe. This tests raw validRAM alias/history behavior,not naturalgame objectlayout.',
        '50aliascomparisons/198checks include vector input/output overlaps,signedangle source/computed-cross aliases and32phases of16realproducer/responsesequences. '
        '48sequencecomparisons/240checks cover16original028bfe->029b00->075416pipelines. Originalcollision producesquerycontactcount andrecordsconsumed byresponse; '
        'initialscene/query/mesh/velocity/state/sentinels only. BothFPbanks,lower/upperlists,hit/miss,stationary/downwardmotion. '
        'This is not naturalworldconstruction,complete0730d8 movement,gameplay orframe timing;olderownercoveragegapsremain.',
        '243freshretained IRQ/context/taskcomparisons/1683checks preserveoldernumeric assertions byAST.777freshcomparisons/4473checks; '
        'cumulative73717 is evidencevolume,notunique/codecoverage. Earlier216warm/211coldstartup and242full-devicewrite remainhistorical,notrerun. '
        'SRFDclear/softwaremappedRAMABI;physical FPU/exception/MMU/bootROM/GPU delivery unqualified.',
        '253-source DLL/library andstrictunbounded Wall/Wextra/Werror checks pass. Exact252bridge plus5entries/19actualreturns/19continuationexports. '
        'OldC/CPU/referenceflatinterpreter/guards/observer/comparisonlogic/rawsession/ABI and2739oldbuilds preserved;newDLL/libadditional. '
        '490explicitresumes,71qualifiednonlocal/419remaining;new19exports not independentlyqualifiednonlocalresumes. No newsourcewaitbounds.',
        '028bfe remainsunregistered withCba/E0c null-readoutcomesopen;029b00 A48cskipopen;0730d8unregistered. Threeoldmathwrapperprecisionpaths remainunqualified. '
        'No upstream implementation codecopied. Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings unfinished. '
        'TextureOff disables sampling andusesvertexcolours.10% weightedmilestones,notcodecoverage;researchDLLnonplayable;goalactive,keepSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_response_qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    print('RESPONSE QUALIFIED4routines/373newrows;378rows/32outcomesexecute;777comparisons/4473checks;642linked')

if __name__=='__main__':main()
