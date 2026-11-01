"""Accept original09844a using execution plus instruction-bound infeasibility."""
import hashlib,json
from pathlib import Path
from audit_movie_linked_routines import inventory
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    paths=[P/'analysis'/name for name in ('player_task_pool_verification.json','player_task_pool_invariants.json','player_task_pool_compile_checks.json','player_work_allocation_translation.json','player_work_lifecycle_qualification.json')]
    direct,proof,compiler,translation,prior=[json.loads(p.read_text()) for p in paths]
    files=[Path(__file__),P/'audit_movie_linked_routines.py',*paths]
    for report in (direct,proof,compiler,translation):
        for path,expected in report.get('sha256',{}).items():assert sha(path)==expected,path;files.append(Path(path))
    for path,expected in compiler['source_sha256'].items():assert sha(path)==expected,path;files.append(Path(path))
    assert (direct['comparisons'],direct['checks'],direct['natural_original_allocator_exhaustion_cases'])==(33,263,8)
    coverage=direct['coverage'];assert (coverage['static_rows'],coverage['executed_rows'],coverage['static_outcomes'],coverage['executed_outcomes'])==(114,111,16,12)
    assert set(coverage['missing_rows'])==set(proof['source_bound_infeasible_rows'])
    assert set(map(tuple,coverage['missing_outcomes']))==set(map(tuple,proof['source_bound_infeasible_outcomes']))
    assert proof['original_rows_byte_checked']==114 and proof['loop_iterations']==1024 and proof['task_count']==4096
    assert not proof['private_memory_counter_alias_premise_required'] and not proof['runtime_code_changed'] and proof['guards_removed']==0
    assert compiler['source_count']==268 and compiler['compiler_exit_code']==0 and not compiler['runtime_code_changed'] and not compiler['binary_rebuilt']
    dll=R/'work/sonic-adventure/build/player_work_allocation.dll';assert sha(dll)==prior['dll_sha256']==compiler['dll_sha256']
    body=next(x for x in translation['routines'] if x['address']=='0x8c09844a')
    row={k:body[k] for k in ('name','address','end_exclusive','retail_span_sha256')};row.update(module='1ST_READ.BIN',source=translation['source'],instruction_rows=114,coverage=coverage)
    registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1626];assert len(registry)==1626
    assert not any(x['address']==row['address'] and x.get('module','1ST_READ.BIN')==row['module'] for x in registry)
    linked=inventory(dll,registry+[row]);assert linked['confirmed_linked_unique_routines_by_name']==671
    limitations=[
        'Original09844a is a complete114-row/16-outcome body.111rows and12outcomes execute;three rows/four outcomes are source-bound infeasible in normal original-entry immutable synchronous execution. All original branches and labels remain in source. No arbitrary nonlocal resume qualification is added.',
        'The fixed scheduler-head interval79f04c..79f06c cannot initially be skipped. R7 starts0,R12=4095,R11=4096;four phases add1 each with no loads,calls,or other protected-register writes inside. First three phases see4j,4j+1,4j+2 forj0..1023,all below4095. The fourth phase supplies the actual final null link;the loop stopsat4096 without wrap. Counter induction does not depend on a private RAM-counter alias premise.',
        '33fresh complete-software-state/ordered-boundary comparisons and263checks pass. Eight raw FPSCR/SR profiles execute original pool creation,create a real parent task,consume the remaining original heap block via098a80,and exercise actual pool allocation failure. All4096records/links/zero fields are independently checked. Failure clears the two pool globals but retains the existing nonempty scheduler heads and task records,matching original partial side effects.',
        'No allocation return,free-head value,execution frame or forged callback output is supplied. Host chooses calls/request size anddeclares caller stack. This is not the normal full boot/caller chain or physical device/exception/interrupt proof. Nonlocal/asynchronous execution,self-modifying code,and unsupported mappings are outside the accepted completion domain.',
        'All268compiled source files and the existingDLL retain their previous hashes. Strict unboundedWall/Wextra/Werror compiler checks pass. No native code change or binary rebuild was needed. Existing317constructor/helper/regression comparisons remain pinned evidence;they were not rerun solely for this proof extension. Canonical allocators receive zero duplicate credit.',
        'The two globally unresolved0777e0sort outcomes still require actual caller stack/object/callback alias evidence. Stored child callback0b3a2e is not executed here. Full game decompilation andPC60/120/240FPS8K,Android,stereo3DS,N64/PS1,SUPIR/settings remain incomplete. TextureOff must disable sampling/use vertex-material colours. NoSARecompimplementation imported. Goalactive/SolHigh;10%weightedmilestones,not measured code coverage.'
    ]
    result=dict(status='complete_original_task_pool_initializer_software_qualified',routines=[row],new_original_routine_credit=1,newly_qualified_original_rows=114,
                executed_original_rows=111,source_bound_infeasible_rows=proof['source_bound_infeasible_rows'],executed_conditional_outcomes=12,source_bound_infeasible_outcomes=proof['source_bound_infeasible_outcomes'],
                fresh_original_native_comparisons=33,fresh_checks=263,natural_original_allocator_exhaustion_cases=8,
                retained_pinned_allocation_comparisons=317,retained_pinned_allocation_checks=12456,combined_allocation_evidence_comparisons=350,combined_allocation_evidence_checks=12719,
                compiled_source_count=268,runtime_code_changed=False,binary_rebuilt=False,dll_sha256=sha(dll),result_registry_entries=1627,result_unique_module_address_pairs=1624,linked_routine_inventory=linked,
                canonical_duplicate_credit=0,new_nonlocal_resume_exports=0,new_nonlocal_resume_routes_qualified=0,caller_allocation_alias_contract_globally_proven=False,full_game_qualified=False,
                overall_completion_percent=10,estimate_method='weighted milestones;not measured code coverage',next_task='Trace actual callers of06c350/0730d8 and real stack/object lifecycles before closing0777e0sort premises. Treat Ghidra references as leads,not recovered code.',limitations=limitations,
                sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_task_pool_qualification.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('PLAYER TASK POOL QUALIFIED1original/114rows:111observed+3infeasible;12observed+4infeasibleoutcomes;671linked',flush=True)
if __name__=='__main__':main()
