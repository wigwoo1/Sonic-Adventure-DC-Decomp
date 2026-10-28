"""Qualify one existing canonical query's full-state adapter with zero new credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','sequence_verification','retained_verification','unbounded'):
        path=P/f'analysis/movement_mode_query_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_mode_query.dll';sha=digest(dll)
    for name in ('audit','direct_verification','sequence_verification'):assert reports[name]['dll_sha256']==sha
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct=reports['direct_verification'];seq=reports['sequence_verification'];t=reports['translation']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['unchanged_standalone_CPU_comparisons'])==(212,2084,194)
    assert direct['status_counts']=={'0':204,'-3':8}
    assert len(t['routines'])==1 and t['unregistered_proposals']==[] and t['canonical_reused_entries']==['0x8c049a4e']
    body=t['routines'][0];assert body['name']=='sa_global_mode_nonzero_8c049a4e' and body['address']=='0x8c049a4e' and body['instruction_rows']==6 and body['canonical_registered_reuse']
    coverage=direct['coverage_by_routine'];assert coverage==[dict(address='0x8c049a4e',static_rows=6,executed_rows=6,static_outcomes=0,executed_outcomes=0,missing_rows=[],missing_outcomes=[])]
    assert (seq['original_native_comparisons'],seq['independent_checks'],seq['actual_producer_consumer_sequences'])==(60,210,30)
    assert seq['supplied_game_callee_outputs']==seq['supplied_execution_frames']==0
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==256 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    returns=P/'analysis/movement_mode_query_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert ret['prior_return_sites']==len(ret['allowed_call_returns'])==2392 and ret['new_call_returns']==ret['original_call_returns']==[] and t['resume_routes']==[]
    previous=P/'analysis/movement_parameters_qualification.json';old=json.loads(previous.read_text());files.append(previous);assert old['total_evidence_original_native_comparisons']==74982
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry)==1602
    canonical=[r for r in registry if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']=='0x8c049a4e'];assert len(canonical)==1
    original=canonical[0];assert original['name']==body['name'] and original['retail_span_sha256']==body['retail_span_sha256']
    legacy_proof=P/original['verification'];proof=json.loads(legacy_proof.read_text());legacy=P/original['source'];files.extend([legacy,legacy_proof])
    assert proof['source_sha256']==digest(legacy) and proof['retail_span_sha256']==body['retail_span_sha256'] and proof['cases_passed']==128
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,registry);assert linked['confirmed_linked_unique_routines_by_name']==645;files.append(P/'audit_movie_linked_routines.py')
    adapter={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};adapter.update(module='1ST_READ.BIN',source=t['source'],original_canonical_source=original['source'],original_canonical_verification=original['verification'],instruction_rows=6,coverage=coverage[0])
    q=dict(status='software_qualified_existing_canonical_mode_query_full_state_adapter',compiled_source_count=256,
      newly_registered_entries=0,new_original_routine_credit=0,newly_qualified_original_rows=0,newly_qualified_canonical_adapter_rows=6,canonical_duplicate_credit=0,
      canonical_reused_entries=['0x8c049a4e'],canonical_adapters=[adapter],routines=[],executed_original_rows_including_canonical=6,executed_conditional_outcomes=0,
      unaccounted_outcomes=[],unexecuted_rows=[],domain_exclusion_arguments=[],coverage_by_routine=coverage,
      prior_registry_entries=1602,result_registry_entries=1602,result_unique_module_address_pairs=1599,linked_routine_inventory=linked,
      fresh_original_native_comparisons=515,fresh_independent_checks=3977,direct_status_counts=direct['status_counts'],unchanged_standalone_CPU_comparisons=194,
      actual_producer_consumer_sequences=30,supplied_game_callee_outputs=0,supplied_execution_frames=0,actual_original_call_sites_executed=0,
      flat_return_guard_sites=2392,explicit_nonfunction_resume_routes_total=500,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=429,
      new_nonlocal_resume_exports=0,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,
      retained_previous_evidence_original_native_comparisons=74982,total_evidence_original_native_comparisons=75497,
      world_owner_still_unregistered='0x8c028bfe',world_owner_unaccounted_outcomes=old['world_owner_unaccounted_outcomes'],parent_movement_caller_still_unregistered='0x8c0730d8',
      collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=old['collision_caller_unaccounted_outcomes'],
      fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Recover actual0730d8 dependency0742d4 at originalcall7389c (also7394a),using original arguments and actual callees. Registeredcanonical055d5a is another later reuse candidate. Retain old world/collision/precision obligations.',
      limitations=[
        'Canonical049a4e was alreadyregisteredwith128historicalfullRAM/CPUcases. Thismilestone adapts its six originalinstructions into currentfullstate/busABI;zero neworiginalroutinecredit. '
        'OldstandaloneC/proof/DLL keep theiroriginaltwoargumentABI andbytes. CombinedDLL newlyexposes thecanonicalname throughstageABI;thecurrentconnectedentrypoints dispatchit. '
        'Rawword8c7492ac!=0 ->R0,SR.T=word==0,R3=literalpointer. No FPU/RAM/stackwrites/callees.',
        '212freshdirectcomparisons/2084checks:204ordinaryreturns,8mappedguardsat49a50. All6instructionsincludingRTSdelayXORexecute. '
        '194mappedRAMcasesalsocompareagainstunchangedoldstandaloneCPUimplementation;theseareincludedinthe212,notaddedasneworiginalcomparisons. '
        'All8FPUbank/PR/SZmodes,T/FDstates,rawzero/nonzerowords,P0/P1/P2pointers,literal/self/code/lastword/unalignedsoftwareRAM,rootPR0/8c010000/fed000, '
        '10realMMIOcases(5f8000,5f6920,ff000038,ffa00020,ffd8000c)and8badpointerschecked. IndependentrawRAMpredicate/SR/register/FPU/footprintoracles;MMIOcomparedtooriginalsoftwaredevices.',
        '60comparisons/210checkscover30actual075214->049a4esequences. Originalparameterstoreproduces0,1,80000000,ffffffff,or4.0bitsinthequeriedglobal. '
        'BothFPbanks/P0/P1/P2;rawnegativezeroisnonzeroasinteger. No suppliedintermediateresults/frames. Authoredinitialoverlaplayouts,notnaturalgameplayproof.',
        '243freshretainedIRQ/context/taskcomparisons/1683checks preserveoldnumericassertionsbyAST.515freshcomparisons/3977checks;75497cumulativeevidence isnotunique/codecoverage. '
        'Priorstartup/full-devicewrite remainhistorical,notrerun. SRFDintegerprofilesworkwithoutFPUinstructions;physicalalignment/FPU/exception/MMU/bootROM/GPU/gameplay/frametimingunqualified.',
        '256-sourceDLL/library andstrictunboundedWall/Wextra/Werror pass. Exact255bridge plusonecanonicalentry;no newcalls/returnmembers/resumeexports. '
        'All1602registryrows and2745oldbuilds preserved;newDLL/libadditional.500explicitresumes,71qualifiednonlocal/429remaining. No newsourcewaitbounds.',
        '028bfe null-readoutcomes,029b00 A48cskip andthreeoldmathwrapperprecisionpaths remainunqualified;0730d8 staysunregistered. No upstreamimplementationcodecopied. '
        'Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. TextureOffdisablessampling/usesvertexcolours.10%weightedmilestones;researchDLLnonplayable;goalactive,keepSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_mode_query_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('MODE QUERY QUALIFIED canonical6rows/newcredit0;515comparisons/3977checks;645linked')

if __name__=='__main__':main()
