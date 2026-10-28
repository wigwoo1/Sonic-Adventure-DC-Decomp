"""Qualify two original initializers; five existing helpers receive no duplicate credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    reports={};files=[Path(__file__)]
    for name in ('translation','connection','audit','direct_verification','sequence_verification',
                 'contact_alias_verification','retained_verification','unbounded'):
        path=P/f'analysis/collision_setup_{name}.json';report=json.loads(path.read_text());reports[name]=report;files.append(path)
        for name,sha in report.get('sha256',{}).items():assert digest(name)==sha,name;files.append(Path(name))
    dll=ROOT/'work/sonic-adventure/build/collision_setup.dll';sha=digest(dll)
    for name in ('audit','direct_verification','sequence_verification','contact_alias_verification'):
        assert reports[name]['dll_sha256']==sha
    retained=reports['retained_verification']
    for suite in retained['suites']:
        path=P/suite['proof'];report=json.loads(path.read_text());files.append(path)
        assert report['dll_sha256']==sha
        for name,h in report['sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    direct=reports['direct_verification'];t=reports['translation'];cv=direct['coverage_by_routine']
    assert (direct['original_native_comparisons'],direct['independent_checks'],direct['independent_oracle_cases'])==(94,518,74)
    assert direct['status_counts']=={'0':86,'-1':4,'-3':4}
    assert len(cv)==7 and sum(x['executed_rows'] for x in cv)==165 and sum(x['executed_outcomes'] for x in cv)==18
    assert all(not x['missing_rows'] and not x['missing_outcomes'] for x in cv)
    assert len(t['routines'])==7 and len(t['canonical_reused_entries'])==5 and len(t['resume_routes'])==7
    for name,count,checks in (('sequence_verification',12,52),('contact_alias_verification',12,186),('retained_verification',243,1683)):
        assert (reports[name]['original_native_comparisons'],reports[name]['independent_checks'])==(count,checks)
    assert reports['connection']['compiled_source_count']==reports['unbounded']['source_count']==250
    assert reports['unbounded']['exit_code']==0
    for name,h in reports['unbounded']['source_sha256'].items():assert digest(name)==h,name;files.append(Path(name))
    previous=P/'analysis/collision_contact_counter_alias_qualification.json';old=json.loads(previous.read_text());files.append(previous)
    assert old['total_evidence_original_native_comparisons']==71466
    routines=[]
    for body in t['routines']:
        if body['canonical_registered_reuse']:continue
        row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')}
        row.update(module='1ST_READ.BIN',source=t['source'],instruction_rows=body['instruction_rows'],
                   coverage=next(x for x in cv if x['address']==body['address']))
        routines.append(row)
    assert len(routines)==2 and sum(x['instruction_rows'] for x in routines)==102
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(registry) in (1589,1591)
    baseline=registry[:1589];assert not any(x['address'] in t['unregistered_proposals'] and x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' for x in baseline)
    from audit_movie_linked_routines import inventory
    linked=inventory(dll,baseline+routines);assert linked['confirmed_linked_unique_routines_by_name']==633
    files.append(P/'audit_movie_linked_routines.py')
    q=dict(status='software_qualified_original_collision_world_and_debug_initializers',compiled_source_count=250,
      newly_registered_entries=2,new_original_routine_credit=2,newly_qualified_original_rows=102,
      canonical_reused_entries=t['canonical_reused_entries'],canonical_duplicate_credit=0,
      executed_rows_including_canonical_adapters=165,executed_conditional_outcomes_including_canonical=18,
      new_routine_conditional_outcomes=10,unaccounted_outcomes=[],domain_exclusion_arguments=[],
      coverage_by_routine=cv,routines=routines,prior_registry_entries=1589,result_registry_entries=1591,
      result_unique_module_address_pairs=1588,linked_routine_inventory=linked,
      fresh_original_native_comparisons=361,fresh_independent_checks=2439,
      direct_status_counts=direct['status_counts'],independent_initialization_oracle_cases=74,
      real_initialized_empty_world_sequence_comparisons=12,prior_contact_alias_comparisons_replayed=12,
      flat_return_guard_sites=2333,explicit_nonfunction_resume_routes_total=441,
      qualified_nonlocal_resume_routes_total=71,remaining_unqualified_resume_routes=370,
      new_nonlocal_resume_exports=7,new_nonlocal_resume_routes_qualified=0,
      retained_previous_evidence_original_native_comparisons=71466,total_evidence_original_native_comparisons=71827,
      supplied_game_callee_outputs=0,supplied_execution_frames=0,
      collision_caller_still_unregistered='0x8c029b00',collision_caller_unaccounted_outcomes=[['0x8c02a48c','0x8c02a4a8']],
      fresh_full_game_startup_comparisons=0,full_gameplay_qualified=False,physical_exception_delivery_qualified=False,
      overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',
      dll_sha256=sha,next_task='Recover actual0730d8movement/query-owner initialization from byte-checked listing, '
      'then establish actual query/world/render-state contracts. Continue029b00A48c R10-preservation proof '
      'through seven renderer/text bodies and actual memory aliases. Keep029b00 and3mathwrappers unregistered.',
      limitations=[
        'Two new complete original bodies:028e60 world/pool reset45rows/4outcomes and04e3de debug/gameflag '
        'initialization57rows/6outcomes. Five canonical helpers63rows/8outcomes are adapted to the current '
        'ABI with zero duplicate routine credit;all165rows/18outcomes execute. Existing10d39c route retained.',
        '94direct comparisons:86complete,4existingPR1numericstops,4mapped-memoryguards.74independent cases '
        'check entire reset/pool/inputflag RAM footprints or debug control fields. World resets192records '
        'at6bb27c and8192records at6be27c,64bytes each;links terminate in zero. Fixed original counts '
        'executebothloop outcomes. PR1stopsatFLDI0 after integer resets. SZ1 stores both original FP words; '
        'the extra FR5 word is preserved in the independent expected RAM. Nonzero statuses are stops,not completions.',
        'Direct canonical pool calls use counts2/3/16. The original DT loop requires count>=2;zero/singleton '
        'is not redefined or given a new source guard/bound. Initializer uses original constants192/8192. '
        'P0/P1/P2 mapped memory views andbothphysicalFPbanks covered;physicalFPU/exception/MMU/address-error '
        'delivery remainsunqualified;SRFDclearfixedcode/softwareABI.',
        'Four same-RAM worldinit->debuginit->emptyworld029b00 sequences pass12comparisons/52checks. '
        'Worldhead/contactcount/debugFF flags are produced by real originals/nativeinitializers;no '
        'callee results or execution frames supplied. Host starts ordinary calls with arguments. '
        'This is not complete cold startup, world population, movement or gameplay qualification.',
        'Twelve prior aliased/disjoint collisioncaller cases pass186unchangedchecks against250DLL. '
        '243fresh retained IRQ/context/task comparisons pass1683checks with old numeric assertions '
        'preserved by AST. Total361freshcomparisons/2439checks;cumulative71827notunique/codecoverage. '
        'Historical216warm/211coldstartup and242full-devicewrite evidence are not freshly rerun.',
        'Exact249bridge plus7entries/7actualreturns/7continuationexports;250sources compile and strict '
        'unboundedWallWextraWerrorpass. OldC/CPU/flatinterpreter/guards/observer/rawsession/ABIand2733builds '
        'remain unchanged;newDLL/libadditional.441explicitresumeexports,71qualified/370unqualified; '
        'new continuation exports are not independently qualified nonlocal resumes.',
        'Original annotated call-graph audit identifies41bodies/5981rows and fixed88bytecopyR10preservation. '
        'Seven renderer/text save-word preservation/aliasing obligations remain;A48c skip staysOPEN. '
        'Ghidra reference export locates primary0730d8 caller and2other movement owners;draft warnings '
        'do not remove any original instruction rows. These references add no routine credit.',
        'Full game/PC60/120/240FPS8K/Android/3DSstereo/N64/PS1/SUPIR/settings unfinished. '
        'TextureOff disables sampling/usesvertexcolours.10%milestones,notcoverage;nonplayable researchDLL. '
        'KeepgoalactiveandSolHigh for remaining reverseengineering;no upstream implementation imported.'
      ],sha256={str(p):digest(p) for p in files})
    (P/'analysis/collision_setup_qualification.json').write_text(json.dumps(q,indent=2)+'\n')
    print('SETUP QUALIFIED2newroutines/102rows;361comparisons/2439checks;633linked;callerbranchstillOPEN')


if __name__=='__main__':main()
