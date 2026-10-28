"""Qualify the original call-free parameter owner with explicit branch accounting."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','aliases_verification','predicate_proof','retained_verification','unbounded'):
        path=P/f'analysis/movement_parameters_{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
        for name,h in r.get('sha256',{}).items():assert digest(name)==h,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/movement_parameters.dll';sha=digest(dll)
    for name in ('audit','direct_verification','aliases_verification'):assert reports[name]['dll_sha256']==sha
    for suite in reports['retained_verification']['suites']:
        path=P/suite['proof'];r=json.loads(path.read_text());files.append(path);assert r['dll_sha256']==sha
        for name,h in r['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct=reports['direct_verification'];aliases=reports['aliases_verification'];t=reports['translation'];proof=reports['predicate_proof']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(388,3348,246)
    assert direct['status_counts']=={'0':352,'-1':20,'1':12,'-3':4}
    body=t['routines'][0];assert len(t['routines'])==1 and body['address']=='0x8c075214' and body['instruction_rows']==216
    c=direct['coverage_by_routine'][0];assert c['executed_rows']==216 and c['missing_rows']==[] and c['executed_outcomes']==15
    assert c['missing_outcomes']==proof['infeasible_outcomes']==[['0x8c07532c','0x8c075382']]
    assert proof['original_rows_byte_checked']==216 and proof['static_conditional_outcomes']==16
    assert not proof['domain_excluded_rows'] and not proof['domain_excluded_outcomes'] and not proof['original_code_removed']
    coverage=[dict(address=body['address'],static_rows=216,executed_rows=216,static_outcomes=16,executed_outcomes=15,
      proved_infeasible_outcomes=proof['infeasible_outcomes'],missing_rows=[],unaccounted_outcomes=[])]
    assert (aliases['original_native_comparisons'],aliases['independent_checks'],aliases['overlap_cases'],aliases['actual_produced_mode_sequences'])==(36,180,28,4)
    assert aliases['supplied_game_callee_outputs']==aliases['supplied_execution_frames']==0
    assert (reports['retained_verification']['original_native_comparisons'],reports['retained_verification']['independent_checks'])==(243,1683)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==255 and reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    returns=P/'analysis/movement_parameters_returns.json';ret=json.loads(returns.read_text());files.append(returns)
    assert ret['prior_return_sites']==len(ret['allowed_call_returns'])==2392 and ret['new_call_returns']==ret['original_call_returns']==[] and t['resume_routes']==[]
    previous=P/'analysis/movement_alternate_qualification.json';old=json.loads(previous.read_text());files.append(previous)
    assert old['total_evidence_original_native_comparisons']==74315
    routine={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};routine.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=216,coverage=coverage[0])
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1601,1602);baseline=registry[:1601]
    assert not any(r['address']==body['address'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for r in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+[routine]);assert linked['confirmed_linked_unique_routines_by_name']==644;files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_original_movement_parameter_update',compiled_source_count=255,newly_registered_entries=1,new_original_routine_credit=1,
      newly_qualified_original_rows=216,canonical_duplicate_credit=0,executed_original_rows=216,executed_conditional_outcomes=15,proved_infeasible_conditional_outcomes=1,
      infeasible_outcomes=proof['infeasible_outcomes'],unaccounted_outcomes=[],unexecuted_rows=[],domain_exclusion_arguments=[],coverage_by_routine=coverage,routines=[routine],
      prior_registry_entries=1601,result_registry_entries=1602,result_unique_module_address_pairs=1599,linked_routine_inventory=linked,
      fresh_original_native_comparisons=667,fresh_independent_checks=5211,direct_status_counts=direct['status_counts'],independent_finite_oracle_cases=246,
      overlap_comparisons=28,actual_produced_mode_sequences=4,actual_original_call_sites_executed=0,supplied_game_callee_outputs=0,supplied_execution_frames=0,
      flat_return_guard_sites=2392,explicit_nonfunction_resume_routes_total=500,qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=429,
      new_nonlocal_resume_exports=0,new_nonlocal_resume_routes_qualified=0,host_import_recursion_floor=4096,guest_execution_limits_unchanged=True,
      retained_previous_evidence_original_native_comparisons=74315,total_evidence_original_native_comparisons=74982,
      world_owner_still_unregistered='0x8c028bfe',world_owner_unaccounted_outcomes=old['world_owner_unaccounted_outcomes'],
      parent_movement_caller_still_unregistered='0x8c0730d8',collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=old['collision_caller_unaccounted_outcomes'],
      fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',dll_sha256=sha,
      next_task='Audit and connect canonical049a4e at original0730d8 call73490. Its six-instruction global7492ac nonzero query is already registered as standalone source but absent from this bridge;reuse with zero duplicate routine credit and verify current full-state ABI. Retain older world/collision/precision obligations.',
      limitations=[
        'Original075214 is a call-free216instruction leaf. R4+9 is the unsigned selector;R5+3c receives scalar;R6 is state;R7 is flagmask. No fifth stackargument. '
        'It ORs state5c,reads mode1e4,and writes114/f4/d8 from original table8c1bdfe4 stride84 fields78/58/3c. Repeated selector/table reads and store order are preserved.',
        '388direct comparisons/3348checks:352complete,20existingunsupportednumericstops,12reachednumerictrap stops,4mappedguards.246finitecases independently check exactdyadic rounding, '
        'state-mode priority,flagOR,scalar precedence,finalscratch/T and RAM footprint.18untouched retail-table cases plus selectedrawmodes/specialvalues/guards included. '
        'Not complete independentphysics/physicalFPU proof. SuccessfulfiniteprofilesPR0/SZ0;selectedotherrawmodes compared through original instructions.',
        'All216rows/15of16outcomes execute. Byte-backedCFG proof shows532c->5382 impossible on original normalentry with fixedcode/stablemappedRAM: '
        'only5236taken reaches52b8 requiringbit2clear;5328 rereads sameword atR6+1e4 without interveningstores/calls. No inputdisjointness assumption or removedcode/edge.',
        '28overlap comparisons cover selector changes from priorfirst/secondfieldstores andinitialflagOR,scalaroverwritingparameter,storeschanginglater table reads,and sharedrecords. '
        'Four realtwo-call sequences consume originalscalar1.5bits asnextstate-mode400000. BothFPbanks/P1/P2views;no providedintermediateresults orframes. '
        'AuthoredinitialRAM layouts,not naturalgameplayinitialization evidence.',
        '243freshretainedIRQ/context/taskcomparisons/1683checks preserveoldnumericassertionsbyAST.667freshcomparisons/5211checks;74982cumulativeevidence isnot unique/codecoverage. '
        'Prior216warm/211coldstartup and242full-devicewrite remainhistorical,notrerun. PhysicalFPU/exception/MMU/bootROM/GPU/gameplay/frametimingunqualified.',
        '255-sourceDLL/library andstrictunboundedWall/Wextra/Werror pass. Exact254bridge plusoneentry;no newcalls/returnmembers/resumeexports. '
        'OldC/guestCPU/referenceinterpreter/guards/observer/ABI and2743oldbuilds preserved;newDLL/libadditional.500explicitresumes,71qualifiednonlocal/429remaining. No newsourcewaitbounds.',
        '028bfe null-readoutcomes,029b00 A48cskip andthreeoldmathwrapperprecisionpaths remainunqualified;0730d8 staysunregistered. No upstreamimplementationcodecopied. '
        'Fullgame/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settingsunfinished. TextureOffdisablessampling/usesvertexcolours.10%weightedmilestones;researchDLLnonplayable;goalactive,keepSolHigh.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/movement_parameters_qualification.json').write_text(json.dumps(q,indent=2)+'\n');print('PARAMETERS QUALIFIED1routine/216rows/15executed+1provedoutcome;667comparisons/5211checks;644linked')

if __name__=='__main__':main()
